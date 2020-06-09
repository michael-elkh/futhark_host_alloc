#define CL_TARGET_OPENCL_VERSION 120
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <stdio.h>
#include "../futhark_host_alloc.h"

extern cl_command_queue futhark_context_get_command_queue(struct futhark_context *ctx);

FHA_RESULT flush_and_finish(cl_command_queue cq)
{
    cl_int res = clFlush(cq);
    if (res != CL_SUCCESS)
    {
        return FHA_FAIL;
    }
    res = clFinish(cq);
    if (res != CL_SUCCESS)
    {
        return FHA_FAIL;
    }
    return FHA_SUCCESS;
}

FHA_RESULT futhark_host_alloc(struct futhark_context *ctx, void **mem, size_t size)
{
    cl_int res;

    cl_command_queue cq = futhark_context_get_command_queue(ctx);
    cl_context cl_ctx;
    res = clGetCommandQueueInfo(cq, CL_QUEUE_CONTEXT, sizeof(cl_context), &cl_ctx, NULL);
    if (res != CL_SUCCESS)
        return FHA_FAIL;

    size_t full_size = size + sizeof(cl_mem);

    cl_mem cl_memory = clCreateBuffer(cl_ctx, CL_MEM_ALLOC_HOST_PTR, full_size, NULL, &res);
    if (res != CL_SUCCESS)
        return FHA_FAIL;

    *mem = sizeof(cl_mem) + (void *)clEnqueueMapBuffer(cq, cl_memory, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, full_size, 0, NULL, NULL, &res);

    if (res != CL_SUCCESS)
    {
        clReleaseMemObject(cl_memory);
        return FHA_FAIL;
    }

    *(cl_mem *)(*mem - sizeof(cl_mem)) = cl_memory; // Store the cl_mem at the begining of the allocated memoryspace
    return FHA_SUCCESS;
}
FHA_RESULT futhark_host_free(struct futhark_context *ctx, void *mem)
{
    cl_command_queue cq = futhark_context_get_command_queue(ctx);
    void *tmp = (mem - sizeof(cl_mem));
    cl_mem cl_memory = *(cl_mem *)tmp;
    cl_int res = clEnqueueUnmapMemObject(cq, cl_memory, tmp, 0, NULL, NULL);
    if (res != CL_SUCCESS)
        return FHA_FAIL;

    FHA_RESULT fha_res = flush_and_finish(cq);
    if (fha_res != FHA_SUCCESS)
        return fha_res;

    clReleaseMemObject(cl_memory);
    return FHA_SUCCESS;
}