//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina: 2015
// *
// * Inicijalizaija i rukovanje DMA kontrolerom
// *
//////////////////////////////////////////////////////////////////////////////
#ifndef MY_DMA_PING_PONG_REGISTER_SETUP_H_
#define MY_DMA_PING_PONG_REGISTER_SETUP_H_

#define AUDIO_IO_SIZE  128		// DMA transfer size
#define PING_PONG_SIZE (2 * AUDIO_IO_SIZE)


// My_DMA
/* ==================== Some CPU Registers ==================== */
#define IER0        			 *(volatile unsigned *)0x0000
#define IFR0        			 *(volatile unsigned *)0x0001
#define IER1        			 *(volatile unsigned *)0x0045
#define IFR1        			 *(volatile unsigned *)0x0046
#define PRCR       				 *(volatile ioport Uint16*)(0x1C05)

/* ==================== General DMA Registers ==================== */
#define DMA_IFR     			 *(ioport volatile unsigned *)0x1C30    // DMA Interrupt Flag Register
#define DMA_IER     			 *(ioport volatile unsigned *)0x1C31    // DMA Interrupt Enable Register

/* ==================== DMA Controller 1 ==================== */
#define DMA1_CESR1 *(ioport volatile unsigned *)0x1C1C //DMA1 Channel Event Source Register 1
#define DMA1_CESR2 *(ioport volatile unsigned *)0x1C1D //DMA1 Channel Event Source Register 2
/* ------------------------ Channel 0 ----------------------- */
#define DMA1_CH0_SSAL *(ioport volatile unsigned *)0x0D00 //Channel 0 Source Start Address (Lower Part) Register
#define DMA1_CH0_SSAU *(ioport volatile unsigned *)0x0D01 //Channel 0 Source Start Address (Upper Part) Register
#define DMA1_CH0_DSAL *(ioport volatile unsigned *)0x0D02 //Channel 0 Destination Start Address (Lower Part) Register
#define DMA1_CH0_DSAU *(ioport volatile unsigned *)0x0D03 //Channel 0 Destination Start Address (Upper Part) Register
#define DMA1_CH0_TCR1 *(ioport volatile unsigned *)0x0D04 //Channel 0 Transfer Control Register 1
#define DMA1_CH0_TCR2 *(ioport volatile unsigned *)0x0D05 //Channel 0 Transfer Control Register 2
/* ---------------------------------------------------------- */

/* ------------------------ Channel 1 ----------------------- */
#define DMA1_CH1_SSAL *(ioport volatile unsigned *)0x0D20 //Channel 1 Source Start Address (Lower Part) Register
#define DMA1_CH1_SSAU *(ioport volatile unsigned *)0x0D21 //Channel 1 Source Start Address (Upper Part) Register
#define DMA1_CH1_DSAL *(ioport volatile unsigned *)0x0D22 //Channel 1 Destination Start Address (Lower Part) Register
#define DMA1_CH1_DSAU *(ioport volatile unsigned *)0x0D23 //Channel 1 Destination Start Address (Upper Part) Register
#define DMA1_CH1_TCR1 *(ioport volatile unsigned *)0x0D24 //Channel 1 Transfer Control Register 1
#define DMA1_CH1_TCR2 *(ioport volatile unsigned *)0x0D25 //Channel 1 Transfer Control Register 2
/* ---------------------------------------------------------- */

/* ------------------------ Channel 2 ----------------------- */
#define DMA1_CH2_SSAL *(ioport volatile unsigned *)0x0D40 //Channel 2 Source Start Address (Lower Part) Register
#define DMA1_CH2_SSAU *(ioport volatile unsigned *)0x0D41 //Channel 2 Source Start Address (Upper Part) Register
#define DMA1_CH2_DSAL *(ioport volatile unsigned *)0x0D42 //Channel 2 Destination Start Address (Lower Part) Register
#define DMA1_CH2_DSAU *(ioport volatile unsigned *)0x0D43 //Channel 2 Destination Start Address (Upper Part) Register
#define DMA1_CH2_TCR1 *(ioport volatile unsigned *)0x0D44 //Channel 2 Transfer Control Register 1
#define DMA1_CH2_TCR2 *(ioport volatile unsigned *)0x0D45 //Channel 2 Transfer Control Register 2
/* ---------------------------------------------------------- */

/* ------------------------ Channel 3 ----------------------- */
#define DMA1_CH3_SSAL *(ioport volatile unsigned *)0x0D60 //Channel 3 Source Start Address (Lower Part) Register
#define DMA1_CH3_SSAU *(ioport volatile unsigned *)0x0D61 //Channel 3 Source Start Address (Upper Part) Register
#define DMA1_CH3_DSAL *(ioport volatile unsigned *)0x0D62 //Channel 3 Destination Start Address (Lower Part) Register
#define DMA1_CH3_DSAU *(ioport volatile unsigned *)0x0D63 //Channel 3 Destination Start Address (Upper Part) Register
#define DMA1_CH3_TCR1 *(ioport volatile unsigned *)0x0D64 //Channel 3 Transfer Control Register 1
#define DMA1_CH3_TCR2 *(ioport volatile unsigned *)0x0D65 //Channel 3 Transfer Control Register 2
/* ---------------------------------------------------------- */


void aic3204_dma_init(void);
void aic3204_read_block(Int16* buffer_left, Int16* buffer_right);
void aic3204_write_block(Int16* buffer_left, Int16* buffer_right);

#endif /*MY_DMA_PING_PONG_REGISTER_SETUP_H_*/
