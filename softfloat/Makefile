PROCESSOR_H = arm.h
SOFTFLOAT_MACROS = softfloat-macros.h

ifeq ($(TARGET),arm)
CC = arm-lpc-elf-gcc-4.2.0
#COMPILE_C = $(CC) -Wall -c -o $@ $(INCLUDES) -mcpu=arm7tdmi-s -O2
COMPILE_C = $(CC) -DDEBUG_ENABLED=1 -Wall -c -o $@ $(INCLUDES) -mcpu=arm7tdmi-s -O2
else
CC = gcc
COMPILE_C = $(CC) -Wall -c -o $@ $(INCLUDES) -O2
endif
OBJ = .o
EXE =
INCLUDES = -I. -I..
LINK = $(CC) -o $@

.PHONY: all clean

all: libsoftfloat.a

libsoftfloat.a: softfloat.o
	@echo AR
	@ar rcs $@ $<

milieu.h: $(PROCESSOR_H)
	touch milieu.h

softfloat$(OBJ): milieu.h softfloat.h softfloat-specialize $(SOFTFLOAT_MACROS) softfloat.c
	@echo CC
	@$(COMPILE_C) softfloat.c

clean:
	@rm -f *.o *.a
