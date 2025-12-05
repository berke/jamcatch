# jamcatch

Berke DURAK <bd@exhrd.fr>

## What is this?

This is the complete source code for an Electronic Article
Surveillance (EAS) jammer detection system, that I've developed in
2007 for a client who did not want to pursue further development after
I delivered the prototype.

This is for 58 kHz "acousto-magnetic" systems (mostly sold by
Sensormatic).

The hardware is based on a pickup coil connected to a downconverting
analog front-end (a balanced mixer and some filters) and a Philips
(now NXP) LPC2138 ARM 7TDMI microcontroller.  (Unfortunately KiCAD
won't open binary Eagle schematics.)  There is an HD44780 LCD module,
four buttons and a relay output, and of course a UART console.

The firmware is written in C and little bits of ARM assembly.  The
OCaml files lying around are used for generating pieces of data or
code, for tests or for interfacting.

The firmware includes a minimal custom real-time OS, essentially a
preemptive scheduler where tasks have priority levels, can receive and
send signals and messages to ports.  There is no dynamic memory
allocation.  The scheduler is inspired by AmigaOS.

The only 3rd-party code I'm using is SoftFloat.

## Detection

An EAS jammer is a simple device that blasts whatever around 58 kHz in
magnetic mode, blinding EAS systems that send pings and listen for the
resonances induces in the theft prevention tags.

Presumably, modern EAS systems should be able to figure out that they
are being subject to jamming, however apparently in 2007 this wasn't
the case and I made a little bit of money with my first company.
(In Soviet France you had to set up a company in 2007!  Nowadays
it's much easier.)

The detector listens at 58 kHz and activates a relay if it notices
anything out of the ordinary, literally.

It is up to the store management to connect the relay to a silent
or noisy alarm.

Detection works as follows.

The signals are downconverted using an analog mixer so that they are
amenable to be digitized on a microcontroller from 2007.  The MCU
computes windowed FFTs, channelizes them and keeps and per-channel
statistics.  Low-probability excursions, such as those that could be
expected from a jamming system trigger the alarm.  It's simple but it
works and it adjusts automatically to the noise level.

## Why is this interesting?

It's one of the pieces of firmware that I'm able to share and it's
nice and clean.

## Firmware structure

This is based on the header files.

| File           | Purpose                                                        |
|----------------|----------------------------------------------------------------|
| `abort.h`      | An abort macro that places a cookie in RAM for debugging       |
| `adc.h`        | ADC acquisition (interrupt and buffer)                         |
| `alarm.h`      | Alarm output (relay handling)                                  |
| `assert.h`     | Run-time assertion checking macro                              |
| `command.h`    | Command definition for CLI                                     |
| `config.h`     | Configuration definitions                                      |
| `crash.h`      | Crash cookie (debugging words located at a fixed address)      |
| `dac.h`        | DAC output (carrier input to mixer)                            |
| `debug.h`      | Debugging printf-like macros                                   |
| `delay.h`      | Busyloop micro-delay and scheduled macro delays                |
| `disk.h`       | MMC card access                                                |
| `display.h`    | Handles the LCD menu tree                                      |
| `dsp_loop.h`   | Main DSP loop                                                  |
| `exec.h`       | The scheduler                                                  |
| `fft_core.h`   | Inner loop (single pass) of the FFT code                       |
| `fft.h`        | Custom fixed-point FFT code                                    |
| `fix.h`        | Auxiliary functions for fixed-point arithmetic                 |
| `format.h`     | Printf-like formatting functions                               |
| `gpio.h`       | Generic GPIO handler                                           |
| `indicator.h`  | Telemetry elements (atomic and string data types)              |
| `interrupts.h` | Interrupt handling                                             |
| `joker.h`      | Test code for checking inter-task communication                |
| `keypad.h`     | Handling of the four-button keypad                             |
| `lcd.h`        | Handles the HD44780 LCD module and provides bargraph functions |
| `led.h`        | Generic LED handler with blinking etc.                         |
| `list.h`       | Doubly-linked lists                                            |
| `log.h`        | Base two logarithm                                             |
| `lpc2138.h`    | Hardware-specific defines                                      |
| `machine.h`    | Code for dumping machine registers                             |
| `math.h`       | Some fixed-precision division routines                         |
| `menu.h`       | Menu structure                                                 |
| `mmc.h`        | MMC driver                                                     |
| `pwm.h`        | PWM driver                                                     |
| `random.h`     | PRNG based on a Feistel structure (knock-off XTEA)             |
| `rtc.h`        | Real-time clock                                                |
| `secret.h`     | Authentication scheme                                          |
| `serial.h`     | UART handling                                                  |
| `settings.h`   | Handles system settings, with persistence                      |
| `sinus.h`      | Ye olde sine table                                             |
| `sort.h`       | Sort routine                                                   |
| `spi_mmc.h`    | MMC protocol definitions                                       |
| `ssp.h`        | LPC213xx SSP (SPI) driver                                      |
| `string.h`     | String-handling routines                                       |
| `system.h`     | Entrypoint                                                     |
| `timer.h`      | Timer interrupt and wait request handler                       |
| `types.h`      | Because C does not have well-defined types                     |
| `ui.h`         | UI notification system                                         |
| `ui_menu.h`    | The actual menu structure                                      |
| `watchdog.h`   | Watchdog handling                                              |
| `xtea.h`       | XTEA block cipher for encrypting MMC contents                  |

The detection code is under the `detector` directory, so that it can be compiled as a library
and tested separately.

| File                | Purpose                      |
|---------------------|------------------------------|
| `detect.h`          | Top-level detector process   |
| `detector_config.h` | Parameters for the detector  |
| `test_detector.c`   | Test driver for the detector |

## Compilation

I haven't compiled this since 2007 but if you install an ARM7TDMI
toolchain, update the toolchain prefixes in the Makefile and type make
I believe it should work.

## Programming

You can use the `lpcsprg` tool https://github.com/berke/lpcsprg/
