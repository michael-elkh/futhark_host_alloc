#include <stdio.h>
#include "bandwidth.h"
#include "time.h"
#include "lib_host_alloc/futhark_host_alloc.h"

#define SIZE (1 << 12)

int main()
{
    struct futhark_context_config *f_cfg = NULL;
    struct futhark_context *f_ctx = NULL;

    f_cfg = futhark_context_config_new();
    f_ctx = futhark_context_new(f_cfg);

    int32_t *buffer;

    FHA_RESULT fres = futhark_host_alloc(f_ctx, (void **)&buffer, SIZE * sizeof(int32_t));
    printf(fres == FHA_SUCCESS ? "Allocation : OK\n" : "Allocation : NOK\n");
    for (size_t i = 0; i < SIZE; i++)
    {
        buffer[i] = (i * i) & 0xFF;
    }

    struct futhark_i32_1d *array = NULL;
    struct futhark_i32_1d *res = NULL;

    struct timespec start, finish;
    double seconds_elapsed = 0.0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    array = futhark_new_i32_1d(f_ctx, buffer, SIZE);
    futhark_entry_main(f_ctx, &res, array);
    futhark_values_i32_1d(f_ctx, res, buffer);
    futhark_context_sync(f_ctx);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    seconds_elapsed += (double)(finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1.0e9;
    printf("Buffer size : %.2lf GB\n", (SIZE * sizeof(int32_t)) / 1e9);
    printf("Time: %.2lf s\n", seconds_elapsed);
    printf("bandwidth : %.2lf GB/s\n", (2.0 * SIZE * sizeof(int32_t)) / (seconds_elapsed * 1e9));

    fres = futhark_host_free(f_ctx, buffer);
    printf(fres == FHA_SUCCESS ? "Free : OK\n" : "Free : NOK\n");
    futhark_context_config_free(f_cfg);
    futhark_context_free(f_ctx);

    return 0;
}