/*
 * Project : Futhark host alloc
 * Author : El Kharroubi Michaël
 * Date : 2020  
*/
#include <stdlib.h>
#include "../futhark_host_alloc.h"

FHA_RESULT futhark_host_alloc(struct futhark_context *ctx, void **mem, size_t size)
{
    UNUSED(ctx);
    *mem = malloc(size);
    if (*mem == NULL)
        return FHA_FAIL;
    return FHA_SUCCESS;
}
FHA_RESULT futhark_host_free(struct futhark_context *ctx, void *mem)
{
    UNUSED(ctx);
    free(mem);
    return FHA_SUCCESS;
}