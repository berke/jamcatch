# Makefile

BASE=jamcatch

HEADERS=\
	abort.h \
	adc.h \
	alarm.h \
	assert.h \
	command.h \
	config.h \
	dac.h \
	debug.h \
        delay.h \
	disk.h \
	display.h \
	dsp_loop.h \
	exec.h \
	fft.h \
	fft_core.h \
	fix.h \
	format.h \
	gpio.h \
	indicator.h \
	interrupts.h \
	joker.h \
	keypad.h \
	lcd.h \
	led.h \
	list.h \
	log.h \
	lpc2138.h \
	machine.h \
	math.h \
	menu.h \
	mmc.h \
	pwm.h \
	random.h \
	rtc.h \
	serial.h \
	sinus.h \
	sort.h \
	ssp.h \
	string.h \
	system.h \
	timer.h \
	types.h \
	ui.h \
	ui_menu.h \
	watchdog.h \
	xtea.h
OBJS=\
  crt0.o \
  delay.o \
  math.o \
  fiq.o \
  watchdog.o \
  xtea.o \
  random.o \
  system.o \
  schedule.o \
  machine.o \
  string.o \
  list.o \
  indicator.o \
  exec.o \
  keypad.o \
  gpio.o \
  led.o \
  timer.o \
  interrupts.o \
  format.o \
  serial.o \
  rtc.o \
  lcd.o \
  display.o \
  ui_menu.o \
  log.o \
  ui.o \
  mulbench.o \
  sinus.o \
  dac.o \
  adc.o \
  pwm.o \
  fft_core.o \
  fft.o \
  ssp.o \
  mmc.o \
  disk.o \
  settings.o \
  sort.o \
  window.o \
  classifier1.o \
  alarm.o \
  dsp_loop.o \
  joker.o \
  secret.o \
  command.o \
  main.o
LIBDEPS=detector/libdetector.a softfloat/libsoftfloat.a
LIBS=-Ldetector -ldetector -Lsoftfloat -lsoftfloat

.PHONY: all program clean

CC=arm-lpc-elf-gcc-4.2.0
AS=arm-lpc-elf-gcc-4.2.0
OC=arm-lpc-elf-objcopy
OD=arm-lpc-elf-objdump
LD=arm-lpc-elf-ld
NM=arm-lpc-elf-nm

include Makefile.config

#CFLAGS=-Wall -g -mcpu=arm7tdmi-s -mstructure-size-boundary=8 -DDEBUG_ENABLED=1
#CFLAGS=-Wall -O2 -mcpu=arm7tdmi-s -mstructure-size-boundary=8 -DDEBUG_ENABLED=0 -DASSERT_ENABLED=0
#CFLAGS=-Wall -O2 -mcpu=arm7tdmi-s -mstructure-size-boundary=8 \
#       -DDEBUG_ENABLED=1 -DASSERT_ENABLED=1 -DBETA=1 -I. -Isoftfloat -Idetector
#CFLAGS=-Wall -O2 -mcpu=arm7tdmi-s -mstructure-size-boundary=8 -DDEBUG_ENABLED=1 -DKMD=1
#CFLAGS=-Wall -O2 -mcpu=arm7tdmi-s -mstructure-size-boundary=8

MODULES=detector softfloat

.PHONY: $(MODULES)

all: $(MODULES) $(BASE).hex $(BASE).bin $(BASE).lst $(BASE).sym

detector:
	@make -C detector TARGET=arm

softfloat:
	@make -C softfloat TARGET=arm

ui_menu.c ui_menu.h: menu/menu.ml
	ocaml unix.cma menu/menu.ml generate ui_menu "" ui_menu

$(BASE).sym: $(BASE)
	@echo SYM
	@$(NM) $(BASE) | sort >$(BASE).sym

$(BASE).hex: $(BASE)
	@echo HEX
	@$(OC) -O ihex $(BASE) $(BASE).hex

$(BASE).bin: $(BASE)
	@echo BIN
	@$(OC) -O binary $(BASE) $(BASE).bin

$(BASE).lst: $(BASE)
	@echo LST
	@$(OD) -sDS $(BASE) >$(BASE).lst

$(BASE): $(OBJS) $(LIBDEPS) simple.ld
	@echo LD
	@$(LD) -T simple.ld $(OBJS) $(LIBS) -o $(BASE)

%.o: %.c $(HEADERS)
	@echo CC $<
	@$(CC) $(CFLAGS) -c $<

%.o: %.S
	@echo ASM $<
	@$(CC) -DCODE_READ_PROTECTION=$(CODE_READ_PROTECTION) -c $<

$(BASE).o: $(BASE).S
	@echo ASM
	$(AS) -c $(BASE).S

program: $(BASE).hex
	lpcsprg -cache .lpc2138-cache -baud 38400 -unlock -program $(BASE).hex

	#lpcsprg -toggle -cache .lpc2138-cache -baud 38400 -unlock -program $(BASE).hex

clean:
	@rm -f *.o *.bin *.hex $(BASE)
	@make -C detector clean
	@make -C softfloat clean
