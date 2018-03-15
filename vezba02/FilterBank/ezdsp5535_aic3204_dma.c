#include "soc.h"
#include "csl_intc.h"
#include "ezdsp5535_aic3204_dma.h"

static volatile Int16 PingPongFlagInL;
static volatile Int16 PingPongFlagInR;
static volatile Int16 PingPongFlagOutL;
static volatile Int16 PingPongFlagOutR;
static volatile Int16 InputInterruptFlag;
static volatile Int16 OutputInterruptFlag;

#pragma DATA_ALIGN(DMA_InputL,4)
Int16 DMA_InputL[PING_PONG_SIZE];
#pragma DATA_ALIGN(DMA_InputR,4)
Int16 DMA_InputR[PING_PONG_SIZE];
#pragma DATA_ALIGN(DMA_OutputL,4)
Int16 DMA_OutputL[PING_PONG_SIZE];
#pragma DATA_ALIGN(DMA_OutputR,4)
Int16 DMA_OutputR[PING_PONG_SIZE];

static interrupt void DMA_ISR(void);
static Uint32 convert_address(Int16* buffer);

/* Interrupt vector start address */  
extern void VECSTART(void);


void aic3204_dma_init(void) {
 	
 	Uint16 register_value;
	Uint32 dma_address; 
	 
    // --------------------- Interrupts Setup --------------------- /  
    IRQ_globalDisable();//
	IRQ_clearAll();
	IRQ_disableAll(); //
	IRQ_setVecs((Uint32)(&VECSTART)); 
	IRQ_clear(DMA_EVENT); //
 	IRQ_plug(DMA_EVENT, &DMA_ISR);
    IRQ_enable(DMA_EVENT); 
	IRQ_globalEnable(); 
	// ------------------------------------------------------------ /
 		 
    DMA_IFR = 0xFFFF; //  Enable Interrupts for DMA 1, Channels 0-3
    IFR0 = 0x0100;    // Clear DMA CPU interrupt flag 
 
 	DMA_IER = 0x00F0; // Enable interrupts for DMA1 CH0-CH3 
    
    // Setup sync event 
    register_value 	= DMA1_CESR1;
    DMA1_CESR1 		= register_value | 0x0202; /* Set CH1, CH0 sync event to I2S2 receive */
    register_value 	= DMA1_CESR2;
    DMA1_CESR2 		= register_value | 0x0101; /* Set CH3, CH2 sync event to I2S2 transmit*/
    	
    // Channel Source Address
    DMA1_CH0_SSAL = 0x2A28; // I2S Receive Left Data 0 Register
	DMA1_CH0_SSAU = 0x0000;
	
	DMA1_CH1_SSAL = 0x2A2C; // I2S Receive Right Data 0 Register
	DMA1_CH1_SSAU = 0x0000;
    
    dma_address = convert_address(DMA_OutputL);       // convert address 
	DMA1_CH2_SSAL = (Uint16)dma_address;          // keep LSBs 
	DMA1_CH2_SSAU = 0xFFFF & (dma_address >> 16); // keep MSBs 
	
	dma_address = convert_address(DMA_OutputR);   
	DMA1_CH3_SSAL = (Uint16)dma_address;
	DMA1_CH3_SSAU = 0xFFFF & (dma_address >> 16);

    // Channel Destination Address 
    dma_address = convert_address(DMA_InputL);
	DMA1_CH0_DSAL = (Uint16)dma_address;
	DMA1_CH0_DSAU = 0xFFFF & (dma_address >> 16);
	
	dma_address = convert_address(DMA_InputR);
	DMA1_CH1_DSAL = (Uint16)dma_address;
	DMA1_CH1_DSAU = 0xFFFF & (dma_address >> 16);
	
	DMA1_CH2_DSAL = 0x2A08;	// I2S2 Transmit Left Data 0 Register
	DMA1_CH2_DSAU = 0x0000;
	
	DMA1_CH3_DSAL = 0x2A0C;	// I2S2 Transmit Right Data 0 Register
	DMA1_CH3_DSAU = 0x0000;
	
	/* DMA Transfer Length */
	/* DMA transfer length is specified in bytes.                     */
	/* DMA transfer length in ping/pong mode is half the length of    */
	/* the TCR1 register.                                             */
	DMA1_CH0_TCR1 = 2*PING_PONG_SIZE;
    DMA1_CH1_TCR1 = 2*PING_PONG_SIZE;
    DMA1_CH2_TCR1 = 2*PING_PONG_SIZE;
    DMA1_CH3_TCR1 = 2*PING_PONG_SIZE;
    
    //Configure options 
    DMA1_CH0_TCR2 = 0x3081; // source is constant, destination address increments by four bytes after each transfer.
    DMA1_CH1_TCR2 = 0x3081;
    DMA1_CH2_TCR2 = 0x3201; // destination is constant, source address increments by four bytes after each transfer.
    DMA1_CH3_TCR2 = 0x3201;  
    
    //Enable DMA Controller 0 channel 0-3
    register_value 	= DMA1_CH0_TCR2; 
    DMA1_CH0_TCR2 	= register_value | 0x8004;
    register_value 	= DMA1_CH1_TCR2; 
    DMA1_CH1_TCR2 	= register_value | 0x8004;
    register_value 	= DMA1_CH2_TCR2; 
    DMA1_CH2_TCR2 	= register_value | 0x8004;
    register_value 	= DMA1_CH3_TCR2; 
    DMA1_CH3_TCR2 	= register_value | 0x8004;
    
    InputInterruptFlag = 0;
    OutputInterruptFlag = 0;
    
}

void aic3204_read_block(Int16* buffer_left, Int16* buffer_right)
{
	int i;
	Int32* ping_pong_left_ptr;
	Int32* ping_pong_right_ptr;
	Int32* buffer_left_32 = (Int32*)buffer_left;
	Int32* buffer_right_32 = (Int32*)buffer_right;
	while(InputInterruptFlag == 0);
	
	InputInterruptFlag = 0;
    /* Get new input audio block */
	if (PingPongFlagInL && PingPongFlagInR)  // Last Transfer complete was Pong - Filling Ping
	{
		ping_pong_left_ptr = (Int32*)(&DMA_InputL[AUDIO_IO_SIZE]);
		ping_pong_right_ptr = (Int32*)(&DMA_InputR[AUDIO_IO_SIZE]);
	}
	else  									// Last Transfer complete was Ping - Filling Pong
	{
		ping_pong_left_ptr = (Int32*)(&DMA_InputL[0]);
		ping_pong_right_ptr = (Int32*)(&DMA_InputR[0]);
	}
	
	for (i = 0; i < AUDIO_IO_SIZE/2; i++)
	{
		*buffer_left_32++ = *ping_pong_left_ptr++;
		*buffer_right_32++ = *ping_pong_right_ptr++;
	}
}

void aic3204_write_block(Int16* buffer_left, Int16* buffer_right)
{	
	int i;
	Int32* ping_pong_left_ptr;
	Int32* ping_pong_right_ptr;
	Int32* buffer_left_32 = (Int32*)buffer_left;
	Int32* buffer_right_32 = (Int32*)buffer_right;
	
	while(OutputInterruptFlag == 0);
	
	OutputInterruptFlag = 0;
	
    if (PingPongFlagOutL && PingPongFlagOutR) // Last Transfer complete was Pong - Filling Ping
    {
     	ping_pong_left_ptr = (Int32*)(&DMA_OutputL[AUDIO_IO_SIZE]);
     	ping_pong_right_ptr = (Int32*)(&DMA_OutputR[AUDIO_IO_SIZE]);
    } 
    else // Last Transfer complete was Ping - Filling Pong
    {
    	ping_pong_left_ptr = (Int32*)(&DMA_OutputL[0]);
     	ping_pong_right_ptr = (Int32*)(&DMA_OutputR[0]);
    }

    for (i = 0; i < AUDIO_IO_SIZE/2; i++)
    {
       	*ping_pong_left_ptr++ = *buffer_left_32++;
       	*ping_pong_right_ptr++ = *buffer_right_32++;
    }

}
 
static interrupt void DMA_ISR(void)
{
	Uint16 register_value1, register_value2;
	
	/* Clear CPU DMA interrupt */
	register_value1 = IFR0;
	IFR0 = register_value1;
	   
	/* Read DMA interrupt flags */
    register_value1 = DMA_IFR;
    
    /* Channels 0-1, input */
    if (register_value1 & 0x0030) // if DMA 1 channel 0 and 1 interrupts are flagged  
    {
    	register_value2 = DMA1_CH0_TCR2;
    	if (register_value2 & 0x0002) { 
    		PingPongFlagInL = 1; // Last Transfer complete was Pong - Filling Ping
    	} else {
    		PingPongFlagInL = 0; // Last Transfer complete was Ping - Filling Pong
    	}
    	
    	register_value2 = DMA1_CH1_TCR2;
    	if (register_value2 & 0x0002) {
    		PingPongFlagInR = 1; // Last Transfer complete was Pong - Filling Ping
    	} else {
    		PingPongFlagInR = 0; // Last Transfer complete was Ping - Filling Pong
    	}
    
    	InputInterruptFlag = 1;
    	/* Clear CH0-1 interrupts */
    	DMA_IFR = 0x0030; 
    }
    
    /* Channels 2-3, output */
    if (register_value1 & 0x00C0)  // if DMA 1 channel 2-3 interrupts are flagged 
    { 
    	register_value2 = DMA1_CH2_TCR2;
    	if (register_value2 & 0x0002) {
    		PingPongFlagOutL = 1;
    	} else {
    		PingPongFlagOutL = 0;
    	}
    	
    	register_value2 = DMA1_CH3_TCR2;
    	if (register_value2 & 0x0002) {
    		PingPongFlagOutR = 1;
    	} else {
    		PingPongFlagOutR = 0;
    	}
    	  	
    	OutputInterruptFlag = 1;
    	/* Clear CH2-3 interrupts */
    	DMA_IFR = 0x00C0; 
    }

}

 
/* Change word address to byte address and add DARAM offset for DMA */
static Uint32 convert_address(Int16* buffer) {
	
	Uint32 dma_address;
	
	dma_address = (Uint32)buffer;
	dma_address = (dma_address<<1) + 0x010000;
	
	return dma_address;
}
