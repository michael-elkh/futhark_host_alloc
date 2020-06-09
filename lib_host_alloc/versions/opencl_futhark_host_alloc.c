/*
 * Project : Futhark host alloc
 * Author : El Kharroubi Michaël
 * Date : 2020  
*/
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

// Provided by futhark generated code
extern cl_command_queue futhark_context_get_command_queue(struct futhark_context *ctx);

// Flush the command queue 
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
    //Recover the OpenCL context.
    res = clGetCommandQueueInfo(cq, CL_QUEUE_CONTEXT, sizeof(cl_context), &cl_ctx, NULL);
    if (res != CL_SUCCESS)
        return FHA_FAIL;

    // Make place to store cl_mem struct, it's needed later to avoid leaking memory.
    size_t full_size = size + sizeof(cl_mem);

    cl_mem cl_memory = clCreateBuffer(cl_ctx, CL_MEM_ALLOC_HOST_PTR, full_size, NULL, &res);
    if (res != CL_SUCCESS)
        return FHA_FAIL;

    // The result pointer is the address following the cl_mem struct space. 
    *mem = sizeof(cl_mem) + (void *)clEnqueueMapBuffer(cq, cl_memory, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, full_size, 0, NULL, NULL, &res);
    if (res != CL_SUCCESS)
    {
        clReleaseMemObject(cl_memory);
        return FHA_FAIL;
    }
    
    // Store the cl_mem at the begining of the allocated space.
    *(cl_mem *)(*mem - sizeof(cl_mem)) = cl_memory;
    return FHA_SUCCESS;
}
FHA_RESULT futhark_host_free(struct futhark_context *ctx, void *mem)
{
    cl_command_queue cq = futhark_context_get_command_queue(ctx);
    void *tmp = (mem - sizeof(cl_mem)); //Recover the allocated address
    cl_mem cl_memory = *(cl_mem *)tmp; //Get the cl_mem struct
    cl_int res = clEnqueueUnmapMemObject(cq, cl_memory, tmp, 0, NULL, NULL);
    if (res != CL_SUCCESS)
        return FHA_FAIL;

    FHA_RESULT fha_res = flush_and_finish(cq);
    if (fha_res != FHA_SUCCESS)
        return fha_res;

    clReleaseMemObject(cl_memory);
    return FHA_SUCCESS;
}