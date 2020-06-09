/*
 * Project : Futhark host alloc
 * Author : El Kharroubi Michaël
 * Date : 2020  
*/
#ifdef __CUDA__
#include "versions/cuda_futhark_host_alloc.c"
#elif defined __OPENCL__
#include "versions/opencl_futhark_host_alloc.c"
#else
#include "versions/c_futhark_host_alloc.c"
#endif