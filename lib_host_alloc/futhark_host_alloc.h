/*
 * Project : Futhark host alloc
 * Author : El Kharroubi Michaël
 * Date : 2020  
*/
#ifndef __FHA__
#define __FHA__
#include <stdlib.h>
#define UNUSED(x) ((void)(x))

typedef enum result
{
    FHA_SUCCESS,
    FHA_FAIL
} FHA_RESULT;

struct futhark_context;
FHA_RESULT futhark_host_alloc(struct futhark_context *ctx, void **mem, size_t size);
FHA_RESULT futhark_host_free(struct futhark_context *ctx, void *mem);
#endif