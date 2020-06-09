# FHA (Futhark host alloc)
FHA is a pinned memory allocation lib for [futhark](https://github.com/diku-dk/futhark). Right now there is three backend C, Cuda and OpenCL. The C backend is just a simple malloc.

## Usage:

```C
#include "lib_host_alloc/futhark_host_alloc.h"
int main(){
    struct futhark_context *f_ctx = NULL;
    /*
    * Init futhark context
    */
    char* memory;
    size_t memory_size = ...;
    FHA_RESULT fres = futhark_host_alloc(f_ctx, (void **)&memory, memory_size);
    /*
    * Do something with memory
    */
    fres = futhark_host_free(f_ctx, memory);
}
```

`FHA_RESULT` is an enum with two values (FHA_SUCCESS, FHA_FAIL).

For compilation you can choose the backend with the a C macro, for exemple with gcc :

```bash
gcc -c [-D{__C__|__CUDA__|__OPENCL__}] -std=gnu11 lib_host_alloc/futhark_host_alloc.c
```

The default backend is C.