ifeq ($(TARGET),)
TARGET=cuda
endif

LIBS=-lm
ifeq ($(TARGET),cuda)
CC=nvcc
CFLAGS=-Xptxas -O3
LIBS+=-lcuda -lcudart -lnvrtc
VERSION=__CUDA__
else
CC=gcc
CFLAGS=-std=gnu11 -O3 #-Wall -Wextra -g -fsanitize=leak -fsanitize=undefined
VERSION=__C__
ifeq ($(TARGET),opencl)
LIBS+=-lOpenCL
VERSION=__OPENCL__
endif
endif

all: $(TARGET)_bandwidth

$(TARGET)_%: %.o bandwidth_wrap.o lib_host_alloc/futhark_host_alloc.o
	$(CC) $(CFLAGS) $(notdir $^) -o $@ $(LIBS)

%.c: %.fut
	futhark $(TARGET) --library $< -o $(@:%.c=%)

%.o: %.c
	$(CC) -c -D$(VERSION) $(CFLAGS) $^

clean:
	rm -vf *.o *_bandwidth
