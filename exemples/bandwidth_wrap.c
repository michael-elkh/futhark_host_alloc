/*
<FHA is a pinned memory allocation lib for [futhark](https://github.com/diku-dk/futhark).>
    Copyright (C) 2020 El Kharroubi Michaël

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>
#include "bandwidth.h"
#include "time.h"
#include "../futhark_host_alloc.h"

#define SIZE (1 << 27)

int main()
{
    struct futhark_context_config *f_cfg = NULL;
    struct futhark_context *f_ctx = NULL;

    f_cfg = futhark_context_config_new();
    f_ctx = futhark_context_new(f_cfg);

    int32_t *buffers[2];
    size_t size_in_byte = SIZE * sizeof(int32_t);

    FHA_RESULT fres = futhark_host_alloc(f_ctx, (void **)buffers, size_in_byte);
    printf(fres == FHA_SUCCESS ? "Allocation : OK\n" : "Allocation : NOK\n");
    buffers[1] = malloc(size_in_byte);
    //Fill the buffer
    for (size_t i = 0; i < SIZE; i++)
    {
        buffers[0][i] = (i * i) & 0xFF;
    }
    memcpy(buffers[1], buffers[0], size_in_byte);

    struct futhark_i32_1d *input = NULL;
    struct futhark_i32_1d *output = NULL;

    struct timespec start, finish;
    
    //Measure transfer time back and forth.
    double seconds_elapsed[2];
    for(int i = 0; i<2; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        input = futhark_new_i32_1d(f_ctx, buffers[i], SIZE); // Host -> Device
        futhark_entry_main(f_ctx, &output, input);
        futhark_values_i32_1d(f_ctx, output, buffers[i]); // Device -> Host
        futhark_context_sync(f_ctx);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        seconds_elapsed[i] = (double)(finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1.0e9;
    }
    //FHA
    printf("FHA :\n");
    printf("    Buffer size : %.2lf GB\n", (size_in_byte) / 1e9);
    printf("    Time: %.2lf s\n", seconds_elapsed[0]);
    printf("    Bandwidth : %.2lf GB/s\n", (2.0 * size_in_byte) / (seconds_elapsed[0] * 1e9));
    //Malloc
    printf("Std malloc :\n");
    printf("    Buffer size : %.2lf GB\n", (size_in_byte) / 1e9);
    printf("    Time: %.2lf s\n", seconds_elapsed[1]);
    printf("    Bandwidth : %.2lf GB/s\n", (2.0 * size_in_byte) / (seconds_elapsed[1] * 1e9));

    fres = futhark_host_free(f_ctx, buffers[0]);
    printf(fres == FHA_SUCCESS ? "Free : OK\n" : "Free : NOK\n");
    free(buffers[1]);

    futhark_context_config_free(f_cfg);
    futhark_context_free(f_ctx);
    return EXIT_SUCCESS;
}