#include <cuda.h>
#include <nvrtc.h>
#include <cuda_runtime_api.h>
#include "../futhark_host_alloc.h"

FHA_RESULT futhark_host_alloc(struct futhark_context *ctx, void **mem, size_t size)
{
    (void)(ctx);
    cudaError_t res = cudaMallocHost(mem, size);
    return res == CUDA_SUCCESS ? FHA_SUCCESS : FHA_FAIL;
}
FHA_RESULT futhark_host_free(struct futhark_context *ctx, void *mem)
{
    (void)(ctx);
    cudaError_t res = cudaFreeHost(mem);
    return res == CUDA_SUCCESS ? FHA_SUCCESS : FHA_FAIL;
}