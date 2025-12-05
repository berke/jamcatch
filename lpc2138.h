/* lpc2138.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef LPC2138_H
#define LPC2138_H

#include "types.h"

#define H8(x) (*((volatile u8 *)(x)))
#define H16(x) (*((volatile u16 *)(x)))
#define H32(x) (*((volatile u32 *)(x)))
#define HR(t,x) (*((volatile t *)(x)))

#define PINSEL0 H8(0xe002c000)
#define PINSEL1 H8(0xe002c004)
#define PINSEL2 H8(0xe002c014)

typedef struct
{
  u32 PS_0:2;
  u32 PS_1:2;
  u32 PS_2:2;
  u32 PS_3:2;
  u32 PS_4:2;
  u32 PS_5:2;
  u32 PS_6:2;
  u32 PS_7:2;
  u32 PS_8:2;
  u32 PS_9:2;
  u32 PS_10:2;
  u32 PS_11:2;
  u32 PS_12:2;
  u32 PS_13:2;
  u32 PS_14:2;
  u32 PS_15:2;
} PINSEL_LOW_R;

typedef struct
{
  u32 PS_16:2;
  u32 PS_17:2;
  u32 PS_18:2;
  u32 PS_19:2;
  u32 PS_20:2;
  u32 PS_21:2;
  u32 PS_22:2;
  u32 PS_23:2;
  u32 PS_24:2;
  u32 PS_25:2;
  u32 PS_26:2;
  u32 PS_27:2;
  u32 PS_28:2;
  u32 PS_29:2;
  u32 PS_30:2;
  u32 PS_31:2;
} PINSEL_HIGH_R;

#define PINSEL0_B HR(PINSEL_LOW_R, 0xe002c000)
#define PINSEL1_B HR(PINSEL_HIGH_R, 0xe002c004)

#define IOPIN0 H32(0xe0028000)
#define IOSET0 H32(0xe0028004)
#define IODIR0 H32(0xe0028008)
#define IOCLR0 H32(0xe002800c)
#define IOPIN1 H32(0xe0028010)
#define IOSET1 H32(0xe0028014)
#define IODIR1 H32(0xe0028018)
#define IOCLR1 H32(0xe002801c)

/* Memory Accelerator Module (MAM) */

#define MAMCR H8(0xe01fc000)
#define MAMTIM H8(0xe01fc004)
#define MEMMAP H8(0xe01fc040)

typedef struct {
  u8 MAP:2;
  u8 _:6;
} MEMMAP_R;
#define MEMMAP_B HR(MEMMAP_R, 0xe01fc040)

/* Phase Locked Loop (PLL) */

#define PLLCON H8(0xe01fc080)

typedef struct
{
  u8 PLLE:1;
  u8 PLLC:1;
} PLLCON_R;
#define PLLCON_B HR(PLLCON_R, 0xe01fc080)

#define PLLCFG H8(0xe01fc084)
#define PLLCFG_B HR(PLLCFG_R, 0xe01fc084)

typedef struct
{
  u8 MSEL:5;
  u8 PSEL:2;
} PLLCFG_R;

#define PLLSTAT H16(0xe01fc088)
#define PLLSTAT_B HR(PLLSTAT_R, 0xe01fc088)

typedef struct
{
  unsigned short MSEL:5;
  unsigned short PSEL:2;
  unsigned short _:1;
  unsigned short PLLE:1;
  unsigned short PLLC:1;
  unsigned short PLOCK:1;
} PLLSTAT_R;

#define PLLFEED H8(0xe01fc08c)
#define PLLFEED_B HR(PLLFEED_R, 0xe01fc08c)
//#define PLLFEED_B (*((volatile PLLFEED_R *)(0xe01fc08c)))

typedef struct
{
  u8 FEED:8;
} PLLFEED_R;


/* Power Control */

#define PCON H8(0xe01fc0c0)
#define PCON_B HR(PCON_R, 0xe01fc0c0)

typedef struct
{
  u8 IDL:1;
  u8 PD:1;
  u8 PDBOD:2;
} PCON_R;

#define PCONP H32(0xe01fc0c4)
#define PCONP_B HR(PCONP_R, 0xe01fc0c4)

typedef struct
{
  u32 _1:1;
  u32 PCTIM0:1;
  u32 PCTIM1:1;
  u32 PCUART0:1;
  u32 PCUART1:1;
  u32 PCPWM0:1;
  u32 _2:1;
  u32 PCI2C0:1;
  u32 PCSPI0:1;
  u32 PCRTC:1;
  u32 PCSPI1:1;
  u32 _3:1;
  u32 PCAD0:1;
  u32 _4:6;
  u32 PCI2C1:1;
  u32 PCAD1:1;
  u32 _5:11;
} PCONP_R;

/* APB divider */

#define APBDIV H8(0xe01fc100)
#define APBDIV_B HR(APBDIV_R, 0xe01fc100)

typedef struct
{
  u8 DIV:2;
  u8 _:6;
} APBDIV_R;

/* External Interrupts */

#define EXTINT H8(0xe01fc140)
#define INTWAKE H8(0xe01fc144)
#define EXTWAKE H8(0xe01fc144)
#define EXTMODE H8(0xe01fc148)
#define EXTPOLAR H8(0xe01fc14c)

/* Reset */

#define RSIR H8(0xe01fc180)
#define RSIR_B HR(RSIR_R, 0xe01fc180)

typedef struct
{
  u8 POR:1;
  u8 EXTR:1;
  u8 WDTR:1;
  u8 BODR:1;
  u8 _:4;
} RSIR_R;

/* Code Security / Debugging */

#define CSPR H8(0xe01fc184)

/* Timer 0 */

typedef struct
{
  u8 MR0I:1;
  u8 MR1I:1;
  u8 MR2I:1;
  u8 MR3I:1;
  u8 CR0I:1;
  u8 CR1I:1;
  u8 CR2I:1;
  u8 CR3I:1;
} TXIR_R;

typedef struct
{
  u8 CE:1;
  u8 CR:1;
  u8 _:6;
} TXTCR_R;

#define T0TC H32(0xe0004008)
#define T0PR H32(0xe000400c)
#define T0PC H32(0xe0004010)

#define T0IR H8(0xe0004000)
#define T0IR_B HR(TXIR_R, 0xe0004000)

#define T1IR H8(0xe0008000)
#define T1IR_B HR(TXIR_R, 0xe0008000)

#define T0TCR H8(0xe0004004)
#define T0TCR_B HR(TXTCR_R, 0xe0004004)

#define T1TCR H8(0xe0008004)
#define T1TCR_B HR(TXTCR_R, 0xe0008004)

typedef struct
{
  u32 MR0I:1;
  u32 MR0R:1;
  u32 MR0S:1;
  u32 MR1I:1;
  u32 MR1R:1;
  u32 MR1S:1;
  u32 MR2I:1;
  u32 MR2R:1;
  u32 MR2S:1;
  u32 MR3I:1;
  u32 MR3R:1;
  u32 MR3S:1;
  u32 _:4;
} TXMCR_R;

typedef volatile struct
{
  u16 EM0:1;
  u16 EM1:1;
  u16 EM2:1;
  u16 EM3:1;
  u16 EMC0:2;
  u16 EMC1:2;
  u16 EMC2:2;
  u16 EMC3:2;
  u16 _:4;
} TXEMR_R;

#define T0MCR H32(0xe0004014)
#define T0MCR_B HR(TXMCR_R, 0xe0004014)

#define T0EMR H32(0xe000403c)
#define T0EMR_B HR(TXEMR_R, 0xe000403c)

#define T0MR0 H32(0xe0004018)
#define T0MR1 H32(0xe000401c)
#define T0MR2 H32(0xe0004020)
#define T0MR3 H32(0xe0004024)
#define T0CCR H32(0xe0004028)
#define T0CR0 H32(0xe000402c)
#define T0CR1 H32(0xe0004030)
#define T0CR2 H32(0xe0004034)
#define T0CR3 H32(0xe0004038)
#define T0CTCR H32(0xe0004070)

/* Timer 1 */

#define T1MCR H32(0xe0008014)
#define T1MCR_B HR(TXMCR_R, 0xe0008014)

#define T1EMR H32(0xe000803c)
#define T1EMR_B HR(TXEMR_R, 0xe000803c)

#define T1TC H32(0xe0008008)
#define T1PR H32(0xe000800c)
#define T1PC H32(0xe0008010)
#define T1MR0 H32(0xe0008018)
#define T1MR1 H32(0xe000801c)
#define T1MR2 H32(0xe0008020)
#define T1MR3 H32(0xe0008024)
#define T1CCR H32(0xe0008028)
#define T1CR0 H32(0xe000802c)
#define T1CR1 H32(0xe0008030)
#define T1CR2 H32(0xe0008034)
#define T1CR3 H32(0xe0008038)
#define T1CTCR H32(0xe0008070)


/* PWM */

#define PWMTC H32(0xe0014008)
#define PWMPR H32(0xe001400c)
#define PWMPC H32(0xe0014010)
#define PWMMR0 H32(0xe0014018)
#define PWMMR1 H32(0xe001401c)
#define PWMMR2 H32(0xe0014020)
#define PWMMR3 H32(0xe0014024)
#define PWMMR4 H32(0xe0014040)
#define PWMMR5 H32(0xe0014044)
#define PWMMR6 H32(0xe0014048)

typedef struct
{
  u8 ENA:1;
  u8 RST:1;
  u8 _1:1;
  u8 PWMENA:1;
  u8 _2:4;
} PWMTCR_R;

#define PWMTCR_B HR(PWMTCR_R, 0xe0014004)

typedef struct
{
  u16 _1:2;
  u16 PWMSEL2:1;
  u16 PWMSEL3:1;
  u16 PWMSEL4:1;
  u16 PWMSEL5:1;
  u16 PWMSEL6:1;
  u16 _2:2;
  u16 PWMENA1:1;
  u16 PWMENA2:1;
  u16 PWMENA3:1;
  u16 PWMENA4:1;
  u16 PWMENA5:1;
  u16 PWMENA6:1;
  u16 _3:1;
} PWMPCR_R;

#define PWMPCR 0xe001404c
#define PWMPCR_B HR(PWMPCR_R, 0xe001404c)

typedef struct
{
  u8 EPWMM0L:1;
  u8 EPWMM1L:1;
  u8 EPWMM2L:1;
  u8 EPWMM3L:1;
  u8 EPWMM4L:1;
  u8 EPWMM5L:1;
  u8 EPWMM6L:1;
  u8 _:1;
} PWMLER_R;

#define PWMLER 0xe0014050
#define PWMLER_B HR(PWMLER_R, 0xe0014050)

typedef struct
{
  u32 PWMMR0I:1;
  u32 PWMMR0R:1;
  u32 PWMMR0S:1;

  u32 PWMMR1I:1;
  u32 PWMMR1R:1;
  u32 PWMMR1S:1;

  u32 PWMMR2I:1;
  u32 PWMMR2R:1;
  u32 PWMMR2S:1;

  u32 PWMMR3I:1;
  u32 PWMMR3R:1;
  u32 PWMMR3S:1;

  u32 PWMMR4I:1;
  u32 PWMMR4R:1;
  u32 PWMMR4S:1;

  u32 PWMMR5I:1;
  u32 PWMMR5R:1;
  u32 PWMMR5S:1;

  u32 PWMMR6I:1;
  u32 PWMMR6R:1;
  u32 PWMMR6S:1;

  u32 _:11;
} PWMMCR_R;

#define PWMMCR 0xe0014014
#define PWMMCR_B HR(PWMMCR_R, 0xe0014014)

/* Universal Asynchronous Receiver Transmitter 0 (UART0) */

#define U0RBR H8(0xe000c000)
#define U0THR H8(0xe000c000)

#define U0IER H8(0xe000c004)

typedef struct
{
  u32 RBR_IE:1;
  u32 THRE_IE:1;
  u32 RXLS_IE:1;
  u32 _1:5;
  u32 ABTO_IE:1;
  u32 ABEO_IE:1;
  u32 _2:22;
} U0IER_R;

#define U0IER_B HR(U0IER_R, 0xe000c004)

#define U0DLL H8(0xe000c000)
#define U0DLM H8(0xe000c004)

#define U0IIR H8(0xe000c008)
typedef struct {
  u32 I_PEND : 1;
  u32 I_ID : 3;
  u32 _1 : 2;
  u32 FIFO_EN : 2;
  u32 ABEO_INT : 1;
  u32 ABTO_INT : 1;
  u32 _2 : 22;
} U0IIR_R;
#define U0IIR_B HR(U0IIR_R, 0xe000c008)

#define U0FCR H8(0xe000c008)
#define U0LCR H8(0xe000c00c)
#define U0MCR H8(0xe000c010)
#define U0LSR H8(0xe000c014)
#define U0MSR H8(0xe000c018)
#define U0SCR H8(0xe000c01c)

#define U0LCR_B HR(U0LCR_R, 0xe000c00c)
#define U0LSR_B HR(U0LSR_R, 0xe000c014)

typedef struct
{
  u8 WLS:2;
  u8 SBS:1;
  u8 PE:1;
  u8 PS:2;
  u8 BC:1;
  u8 DLAB:1;
} U0LCR_R;

typedef struct
{
  u8 _:7;
  u8 TXEN:1;
} U0TER_R;

typedef struct
{
  u8 RDR:1;
  u8 OE:1;
  u8 PE:1;
  u8 FE:1;
  u8 BI:1;
  u8 THRE:1;
  u8 TEMT:1;
  u8 RX_FIFO_ERROR:1;
} U0LSR_R;

/* I2C Interface 0 */

#define I2C0CONSET H8(0xe001c000)
#define I2C0CONSET_B HR(I2CONSET_R, 0xe001c000)

typedef struct
{
  u8 _1:2;
  u8 AA:1;
  u8 SI:1;
  u8 STO:1;
  u8 STA:1;
  u8 I2EN:1;
  u8 _2:1;
} I2CONSET_R;

#define I2C0STAT H8(0xe001c004)
#define I2C0STAT_B HR(I2STAT_R, 0xe001c018)

typedef struct
{
  u8 _:3;
  u8 STATUS:5;
} I2STAT_R;

#define I2C0DAT H8(0xe001c008)
#define I2C0ADR H8(0xe001c00c)
#define I2C0ADR_B HR(I2ADR_R, 0xe001c018)

typedef struct
{
  u8 GC:2;
  u8 ADDRESS:7;
} I2ADR_R;

#define I2C0SCLH H16(0xe001c010)
#define I2C0SCLL H16(0xe001c014)
#define I2C0CONCLR H8(0xe001c018)
#define I2C0CONCLR_B HR(I2CONCLR_R, 0xe001c018)

typedef struct
{
  u8 _1:2;
  u8 AAC:1;
  u8 SIC:1;
  u8 _2:1;
  u8 STAC:1;
  u8 I2ENC:1;
  u8 _3:1;
} I2CONCLR_R;

/* I2C Interface 1 */

#define I2C1CONSET H8(0xe005c000)
#define I2C1CONSET_B HR(I2CONSET_R, 0xe005c000)
#define I2C1STAT H8(0xe005c004)
#define I2C1STAT_B HR(I2STAT_R, 0xe005c004)
#define I2C1DAT H8(0xe005c008)
#define I2C1ADR H8(0xe005c00c)
#define I2C1ADR_B HR(I2ADR_R, 0xe005c00c)
#define I2C1SCLH H16(0xe005c010)
#define I2C1SCLL H16(0xe005c014)
#define I2C1CONCLR H8(0xe005c018)
#define I2C1CONCLR_B HR(I2CONCLR_R, 0xe005c018)

/* SPI0 (Serial Peripheral Interface 0) */

#define S0SPCR H16(0xe0020000)
#define S0SPCR_B HR(S0SPCR_R, 0xe0020000)

typedef struct
{
  u16 _1:2;
  u16 BITENABLE:1;
  u16 CPHA:1;
  u16 CPOL:1;
  u16 MSTR:1;
  u16 LSBF:1;
  u16 SPIE:1;
  u16 BITS:4;
  u16 _2:4;
} S0SPCR_R;

#define S0SPSR H8(0xe0020004)
#define S0SPSR_B HR(S0SPSR_R, 0xe0020004)

typedef struct
{
  u8 _:3;
  u8 BRT:1;
  u8 MODF:1;
  u8 ROVR:1;
  u8 WCOL:1;
  u8 SPIF:1;
} S0SPSR_R;

#define S0SPDR H8(0xe0020008)
#define S0SPCCR H8(0xe002000c)
#define S0SPINT H8(0xe002001c)
#define S0SPINT_B HR(S0SPINT_R, 0xe002001c)

typedef struct
{
  u8 SPIINT:1;
} S0SPINT_B_R;

/* SSP Controller */

#define SSPCR0 H16(0xe0068000)
#define SSPCR0_B HR(SSPCR0_R, 0xe0068000)

typedef struct
{
  u16 DSS:4;
  u16 FRF:2;
  u16 CPOL:1;
  u16 CPHA:1;
  u16 SCR:8;
} SSPCR0_R;

#define SSPCR1 H8(0xe0068004)
#define SSPCR1_B HR(SSPCR1_R, 0xe0068004)

typedef struct
{
  u8 LBM:1;
  u8 SSE:1;
  u8 MS:1;
  u8 SOD:1;
  u8 _:4;
} SSPCR1_R;

#define SSPDR H16(0xe0068008)
#define SSPSR H8(0xe006800c)
#define SSPSR_B HR(SSPSR_R, 0xe006800c)

typedef struct
{
  u8 TFE:1;
  u8 TNF:1;
  u8 RNE:1;
  u8 RFF:1;
  u8 BSY:1;
  u8 _:3;
} SSPSR_R;

#define SSPCPSR H8(0xe0068010)
#define SSPIMSC H8(0xe0068014)
#define SSPIMSC_B HR(SSPIMSC_R, 0xe0068014)

typedef struct
{
  u8 RORIM:1;
  u8 RTIM:1;
  u8 RXIM:1;
  u8 TXIM:1;
  u8 _:4;
} SSPIMSC_R;

#define SSPRIS H8(0xe0068018)
#define SSPRIS_B HR(SSPRIS_R, 0xe0068018)

typedef struct
{
  u8 RORRIS:1;
  u8 RTRIS:1;
  u8 RXRIS:1;
  u8 TXRIS:1;
  u8 _:4;
} SSPRIS_R;

#define SSPMIS H8(0xe006801c)
#define SSPMIS_B HR(SSPMIS_R, 0xe006801c)

typedef struct
{
  u8 RORMIS:1;
  u8 RTMIS:1;
  u8 RXMIS:1;
  u8 TXMIS:1;
  u8 _:4;
} SSPMIS_R;

#define SSPICR H8(0xe0068020)
#define SSPICR_B HR(SSPICR_R, 0xe0068020)

typedef struct
{
  u8 RORIC:1;
  u8 RTIC:1;
  u8 _:6;
} SSPICR_R;

/* Watchdog */

typedef struct {
  u8 WDEN:1;
  u8 WDRESET:1;
  u8 WDTOF:1;
  u8 WDINT:1;
  u8 _:4;
} WDMOD_R;

#define WDMOD H8(0xe0000000)
#define WDMOD_B HR(WDMOD_R, 0xe0000000)
#define WDTC H32(0xe0000004)
#define WDFEED H8(0xe0000008)
#define WDTV H32(0xe000000c)

/* Real Time Clock */

#define ILR H8(0xe0024000)
#define CTC H16(0xe0024004)
#define CCR H8(0xe0024008)

typedef struct
{
  u8 CLKEN:1;
  u8 CTCRST:1;
  u8 CTTEST:2;
  u8 CLKSRC:1;
  u8 _1:3;
} CCR_R;

#define CCR_B HR(CCR_R, 0xe0024008)

#define CIIR H8(0xe002400c)
#define AMR H8(0xe0024010)

typedef struct
{
  u32 C_SECONDS:6;
  u32 _1:2;
  u32 C_MINUTES:6;
  u32 _2:2;
  u32 C_HOURS:5;
  u32 _3:3;
  u32 C_DAY_OF_WEEK:3;
  u32 _4:5;
} CTIME0_R;

typedef struct
{
  u32 C_DAY_OF_MONTH:5;
  u32 _1:3;
  u32 C_MONTH:4;
  u32 _2:4;
  u32 C_YEAR:12;
  u32 _3:4;
} CTIME1_R;

#define CTIME0 H32(0xe0024014)
#define CTIME1 H32(0xe0024018)
#define CTIME2 H32(0xe002401c)

#define CTIME0_B HR(CTIME0_R, 0xe0024014)
#define CTIME1_B HR(CTIME1_R, 0xe0024018)

#define SEC H8(0xe0024020)
#define MIN H8(0xe0024024)
#define HOUR H8(0xe0024028)
#define DOM H8(0xe002402c)
#define DOW H8(0xe0024030)
#define DOY H16(0xe0024034)
#define MONTH H8(0xe0024038)
#define YEAR H16(0xe002403c)
#define ALSEC H8(0xe0024060)
#define ALMIN H8(0xe0024064)
#define ALHOUR H8(0xe0024068)
#define ALDOM H8(0xe002406c)
#define ALDOW H8(0xe0024070)
#define ALDOY H16(0xe0024074)
#define ALMON H8(0xe0024078)
#define ALYEAR H16(0xe002407c)
#define PREINT H16(0xe0024080)
#define PREFRAC H16(0xe0024084)

/* A/D Converter 0 (AD0) */

#define AD0CR H32(0xe0034000)
#define AD0CR_B HR(ADCR_R, 0xe0034000)

typedef struct
{
  u32 SEL:8;
  u32 CLKDIV:8;
  u32 BURST:1;
  u32 CLKS:3;
  u32 _1:1;
  u32 PDN:1;
  u32 _2:2;
  u32 START:3;
  u32 EDGE:1;
  u32 _3:4;
} ADCR_R;

#define AD0DR H32(0xe0034004)
#define AD0DR_B HR(ADDR_R, 0xe0034004)

typedef struct
{
  u32 _1:6;
  u32 RESULT:10;
  u32 _2:8;
  u32 CHN:3;
  u32 _3:3;
  u32 OVERRUN:1;
  u32 DONE:1;
} ADDR_R;

#define AD0GDR H32(0xe0034004)
#define AD0GDR_B HR(ADDR_R, 0xe0034004)

#define ADGSR H32(0xe0034008)
#define ADGSR_B HR(ADGSR_R, 0xe0034008)

typedef struct
{
  u32 _1:16;
  u32 BURST:1;
  u32 _2:7;
  u32 START:3;
  u32 EDGE:1;
  u32 _3:4;
} ADGSR_R;

/* A/D Converter 1 (AD1) */

#define AD1CR H32(0xe0060000)
#define AD1CR_B HR(ADCR_R, 0xe0034000)
#define AD1DR H32(0xe0060004)
#define AD1DR_B HR(ADDR_R, 0xe0060004)

/* D/A Converter */

#define DACR H32(0xe006c000)
#define DACR_B HR(DACR_R, 0xe006c000)

typedef struct
{
  u32 _:6;
  u32 VALUE:10;
  u32 BIAS:1;
} DACR_R;

/* Vectored Interrupt Controller (VIC) */

#define VICIRQStatus H32(0xfffff000)
#define VICFIQStatus H32(0xfffff004)
#define VICrawIntr H32(0xfffff008)
#define VICintSelect H32(0xfffff00c)
#define VICintEnable H32(0xfffff010)
#define VICintEnClr H32(0xfffff014)
#define VICsoftInt H32(0xfffff018)
#define VICsoftIntClr H32(0xfffff01c)
#define VICprotection H32(0xfffff020)

#define VICvectAddr H32(0xfffff030)
#define VICdefVectAddr H32(0xfffff034)
#define VICvectAddr0 H32(0xfffff100)
#define VICvectAddr1 H32(0xfffff104)
#define VICvectAddr2 H32(0xfffff108)
#define VICvectAddr3 H32(0xfffff10c)
#define VICvectAddr4 H32(0xfffff110)
#define VICvectAddr5 H32(0xfffff114)
#define VICvectAddr6 H32(0xfffff118)
#define VICvectAddr7 H32(0xfffff11c)
#define VICvectAddr8 H32(0xfffff120)
#define VICvectAddr9 H32(0xfffff124)
#define VICvectAddr10 H32(0xfffff128)
#define VICvectAddr11 H32(0xfffff12c)
#define VICvectAddr12 H32(0xfffff130)
#define VICvectAddr13 H32(0xfffff134)
#define VICvectAddr14 H32(0xfffff138)
#define VICvectAddr15 H32(0xfffff13c)

#define VICvectCntl0 H32(0xfffff200)

#if 0
typedef struct
{
  u32 IRSIA:5;
  u32 IRQslot_en:1;
  u32 foo_:26;
} VICvectCntl_R;
#endif

#define IRQslot_en_ 0x20

#define VICvectCntl0_B HR(VICvectCntl_R, 0xfffff200)
#define VICvectCntl1_B HR(VICvectCntl_R, 0xfffff204)
#define VICvectCntl2_B HR(VICvectCntl_R, 0xfffff208)
#define VICvectCntl3_B HR(VICvectCntl_R, 0xfffff20c)
#define VICvectCntl4_B HR(VICvectCntl_R, 0xfffff210)
#define VICvectCntl5_B HR(VICvectCntl_R, 0xfffff214)
#define VICvectCntl6_B HR(VICvectCntl_R, 0xfffff218)
#define VICvectCntl7_B HR(VICvectCntl_R, 0xfffff21c)
#define VICvectCntl8_B HR(VICvectCntl_R, 0xfffff220)
#define VICvectCntl9_B HR(VICvectCntl_R, 0xfffff224)
#define VICvectCntl10_B HR(VICvectCntl_R, 0xfffff228)
#define VICvectCntl11_B HR(VICvectCntl_R, 0xfffff22c)
#define VICvectCntl12_B HR(VICvectCntl_R, 0xfffff230)
#define VICvectCntl13_B HR(VICvectCntl_R, 0xfffff234)
#define VICvectCntl14_B HR(VICvectCntl_R, 0xfffff238)
#define VICvectCntl15_B HR(VICvectCntl_R, 0xfffff23c)

#define VICvectCntl1 H32(0xfffff204)
#define VICvectCntl2 H32(0xfffff208)
#define VICvectCntl3 H32(0xfffff20c)
#define VICvectCntl4 H32(0xfffff210)
#define VICvectCntl5 H32(0xfffff214)
#define VICvectCntl6 H32(0xfffff218)
#define VICvectCntl7 H32(0xfffff21c)
#define VICvectCntl8 H32(0xfffff220)
#define VICvectCntl9 H32(0xfffff224)
#define VICvectCntl10 H32(0xfffff228)
#define VICvectCntl11 H32(0xfffff22c)
#define VICvectCntl12 H32(0xfffff230)
#define VICvectCntl13 H32(0xfffff234)
#define VICvectCntl14 H32(0xfffff238)
#define VICvectCntl15 H32(0xfffff23c)

#define VIC_WDT 0
#define VIC_SOFTWARE 1
#define VIC_ARMCore0 2
#define VIC_ARMCore1 3
#define VIC_TIMER0 4
#define VIC_TIMER1 5
#define VIC_UART0 6
#define VIC_UART1 7
#define VIC_PWM0 8
#define VIC_I2C0 9
#define VIC_SPI0 10
#define VIC_SSP 11
#define VIC_PLL 12
#define VIC_RTC 13
#define VIC_EINT0 14
#define VIC_EINT1 15
#define VIC_EINT2 16
#define VIC_EINT3 17
#define VIC_AD0 18
#define VIC_I2C1 19
#define VIC_BOD 20
#define VIC_AD1 21

#define VIC_NUM_INTS 22

/* Universal Asynchronous Receiver Transmitter 1 (UART1) */

#define U1RBR H8(0xe0010000)
#define U1THR H8(0xe0010000)
#define U1IER H8(0xe0010004)
#define U1IIR H8(0xe0010008)
#define U1FCR H8(0xe0010008)
#define U1LCR H8(0xe001000c)
#define U1MCR H8(0xe0010010)
#define U1LSR H8(0xe0010014)
#define U1MSR H8(0xe0010018)
#define U1SCR H8(0xe001001c)
#define U1DLL H8(0xe0010000)
#define U1DLM H8(0xe0010004)

/* I2C Interface */

#define I2CONSET H8(0xe001c000)
#define I2STAT H8(0xe001c004)
#define I2DAT H8(0xe001c008)
#define I2ADR H8(0xe001c00c)
#define I2SCLH H16(0xe001c010)
#define I2SCLL H16(0xe001c014)
#define I2CONCLR H8(0xe001c018)

/* SPI1 (Serial Peripheral Interface 1) */

#define S1SPCR H8(0xe0030000)
#define S1SPSR H8(0xe0030004)
#define S1SPDR H8(0xe0030008)
#define S1SPCCR H8(0xe003000c)
#define S1SPTCR H8(0xe0030010)
#define S1SPTSR H8(0xe0030014)
#define S1SPTOR H8(0xe0030018)
#define S1SPINT H8(0xe003001c)

#define INTERRUPT __attribute__((naked))

#define INTERRUPT_ENTER() \
  asm volatile("sub   lr, lr,#4\n" \
               "stmfd sp!,{r0-r12,lr}\n" \
               "mrs   r1, spsr\n" \
               "stmfd sp!,{r1}")

#define INTERRUPT_EXIT() \
  asm volatile("ldmfd sp!,{r1}\n" \
               "msr   spsr,r1\n" \
               "ldmfd sp!,{r0-r12,pc}^")

#define FIQENA() \
  asm volatile("mrs r0, cpsr\n" \
               "bic r0, r0, #0x40\n" \
               "msr cpsr, r0\n" \
               : \
               : \
               :"r0")

#define INTENA() \
  asm volatile("mrs r0, cpsr\n" \
               "bic r0, r0, #0xc0\n" \
               "msr cpsr, r0\n" \
               : \
               : \
               :"r0")

#define INTDIS() \
  asm volatile("mrs r0, cpsr\n" \
               "orr r0, r0, #0xc0\n" \
               "msr cpsr, r0\n" \
               : \
               : \
               :"r0")

#define IRQDIS() \
  asm volatile("mrs r0, cpsr\n" \
               "orr r0, r0, #0x80\n" \
               "msr cpsr, r0\n" \
               : \
               : \
               :"r0")

#define IRQENA() \
  asm volatile("mrs r0, cpsr\n" \
               "bic r0, r0, #0x80\n" \
               "msr cpsr, r0\n" \
               : \
               : \
               :"r0")
               

#define GET_CPSR_AND_DISABLE(x) \
  asm volatile("mrs %0, cpsr\n" \
               "orr r1, %0, #0x80\n" \
               "msr cpsr_c, r1\n" : "=r"(x) : : "r1")

#define SET_CPSR(x) \
  asm volatile("msr cpsr_c, %0\n" : : "r"(x))

typedef u32 critical;

#define ENTER_CRITICAL(saved) GET_CPSR_AND_DISABLE(saved)
#define LEAVE_CRITICAL(saved) SET_CPSR(saved)
    
#endif
