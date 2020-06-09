ifeq ($(TARGET),)
TARGET=cuda
endif


LIBS=-lm
ifeq ($(TARGET),cuda)
CC=nvcc
CFLAGS=-Xptxas -O3
LIBS+=-lcuda -lcudart -lnvrtc
else
ifeq ($(TARGET),opencl)
LIBS+=-lOpenCL
endif
CC=gcc
CFLAGS=-std=gnu11 -O3
endif

VERSION=FUTHARK_BACKEND_$(TARGET)

all: $(TARGET)_bandwidth

$(TARGET)_%: %.o bandwidth_wrap.o
	$(CC) $(CFLAGS) $(notdir $^) -o $@ $(LIBS)

%.c: %.fut
	futhark $(TARGET) --library $< -o $(@:%.c=%)

%.o: %.c
	$(CC) -c -D$(VERSION) $(CFLAGS) $^

clean:
	rm -vf *.o *_bandwidth
