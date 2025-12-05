include ../Makefile.config

WINDOW1=cos 0.2
WINDOW2=cos 0.4
DETECTOR_FRAME_LENGTH=64
DETECTOR_DOT_LENGTH=512
FOURIER_SIZES=$(DETECTOR_FRAME_LENGTH) $(DETECTOR_DOT_LENGTH)
FIX=16
HEADERS=detect.h detector_config.h fft_core.h fft.h
OBJECTS=detect.o fft.o fft_core.o window1.o class1.o window2.o

%.c: $(HEADERS)

INCLUDES=-I. -I.. -I../softfloat

ifeq ($(TARGET),arm)
CC=arm-lpc-elf-gcc-4.2.0
else
CC=gcc
ifeq ($(DEBUG_ENABLED), 1)
CFLAGS=-Wall -g $(INCLUDES) -DHOST=1 -DDEBUG_ENABLED=1
else
CFLAGS=-Wall -O3 $(INCLUDES) -DHOST=1
endif
endif

.PHONY: all clean

ifeq ($(TARGET),arm)
all: libdetector.a
else
all: libdetector.a test_detector
endif

clean:
	@rm -f libdetector.a fft_core.c fft_core.h test_detector class1.c window1.c window2.c *.o

detect.c: fft_core.h

class1.c: genclass.ml
	@echo ML
	@ocaml genclass.ml $(FIX) 18000 58000 61000 3e-5 $(DETECTOR_FRAME_LENGTH) class1 >class1.c

window1.c: window.ml
	@echo ML
	@ocaml window.ml $(WINDOW1) $(FIX) $(DETECTOR_FRAME_LENGTH) window1.c window1

window2.c: window.ml
	@echo ML
	@ocaml window.ml $(WINDOW2) $(FIX) $(DETECTOR_DOT_LENGTH) window2.c window2

fft_core.c fft_core.h: fourier.ml
	@echo ML
	@ocaml fourier.ml $(FIX) fft_core $(FOURIER_SIZES)

libdetector.a: $(OBJECTS)
	@echo AR
	@ar rcs $@ $(OBJECTS)

test_detector: test_detector.o libdetector.a
	$(CC) $< -L. -ldetector -o $@

%.o: %.c $(HEADERS)
	@echo CC $<
	@$(CC) $(CFLAGS) -c $<
