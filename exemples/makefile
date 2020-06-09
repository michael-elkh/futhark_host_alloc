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
ifeq ($(TARGET),opencl)
LIBS+=-lOpenCL
VERSION=__OPENCL__
else
VERSION=__C__
endif
CC=gcc
CFLAGS=-std=gnu11 -O3
endif

all: $(TARGET)_bandwidth

$(TARGET)_%: %.o bandwidth_wrap.o ../lib_host_alloc/futhark_host_alloc.o
	$(CC) $(CFLAGS) $(notdir $^) -o $@ $(LIBS)

%.c: %.fut
	futhark $(TARGET) --library $< -o $(@:%.c=%)

%.o: %.c
	$(CC) -c -D$(VERSION) $(CFLAGS) $^

clean:
	rm -vf *.o *_bandwidth
