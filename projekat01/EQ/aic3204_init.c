/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/*   aic3204_init.c                                                          */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Initialisation of the AIC3204 Codec.                                    */
/*                                                                           */
/*   Sets up the codec for line inputs.                                      */
/*   Uses a sampling frequency of 48000 Hz.                                  */ 
/*                                                                           */
/*   Author  : Dejan Bokan                                                   */
/*                                                                           */
/*****************************************************************************/
/*
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/  
 *
*/


#include "ezdsp5535.h"
#include "ezdsp5535_i2s.h"
#include "csl_i2s.h"
#include "aic3204.h"
#include "stdio.h"        // For printf();          

#define AIC3204_SAMPLE_SIZE 16

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  AIC3204 Initialisation.                                                 *
 *      Initialise both the registers and I2S                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 _I2S_init(I2S_WordLen word_len);
 
 /* Setup: Line input. Fs = 48000 Hz */ 
void aic3204_init(void)
{
	/* Configure AIC3204 */
    AIC3204_rset( 0,  0x00 );  // Select page 0
    AIC3204_rset( 1,  0x01 );  // Reset codec
    EZDSP5535_waitusec(1000);  // Wait 1ms after reset
    AIC3204_rset( 0,  0x01 );  // Select page 1
    AIC3204_rset( 1,  0x08 );  // Disable crude AVDD generation from DVDD
    AIC3204_rset( 2,  0x01 );  // Enable Analog Blocks, use LDO power
    AIC3204_rset( 123,0x05 );  // Force reference to power up in 40ms
    EZDSP5535_waitusec(50000); // Wait at least 40ms
    AIC3204_rset( 0,  0x00 );  // Select page 0
    
    /* PLL and Clocks config and Power Up  */
    if(AIC3204_SAMPLE_SIZE == 32)
    	AIC3204_rset( 27, 0x3d );  // BCLK and WCLK are set as o/p; AIC3204(Master); 32-bit sample
    else
    	AIC3204_rset( 27, 0x0d );  // BCLK and WCLK are set as o/p; AIC3204(Master); 16-bit sample
   
	AIC3204_rset( 28, 0x00 );  // Data ofset = 0
    AIC3204_rset( 4,  0x03 );  // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    AIC3204_rset( 6,  0x07 );  // PLL setting: J=7
    AIC3204_rset( 7,  0x06 );  // PLL setting: HI_BYTE(D=1680)
    AIC3204_rset( 8,  0x90 );  // PLL setting: LO_BYTE(D=1680)
    AIC3204_rset( 30, 0x88 );  // For 32 bit clocks per frame in Master mode ONLY
                               // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    AIC3204_rset( 5,  0x91 );  // PLL setting: Power up PLL, P=1 and R=1
    EZDSP5535_waitusec(10000); // Wait for PLL to come up    
    AIC3204_rset( 13, 0x00 );  // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    AIC3204_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    AIC3204_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    AIC3204_rset( 11, 0x82 );  // Power up NDAC and set NDAC value to 2
    AIC3204_rset( 12, 0x87 );  // Power up MDAC and set MDAC value to 7
    AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
    AIC3204_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2
    
    /* DAC ROUTING and Power Up */
    AIC3204_rset( 0,  0x01 );  // Select page 1
    AIC3204_rset( 12, 0x08 );  // LDAC AFIR routed to HPL
    AIC3204_rset( 13, 0x08 );  // RDAC AFIR routed to HPR
    AIC3204_rset( 0,  0x00 );  // Select page 0
    AIC3204_rset( 64, 0x02 );  // Left vol=right vol
    AIC3204_rset( 65, 0x00 );  // Left DAC gain to 0dB VOL; Right tracks Left
    AIC3204_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
    AIC3204_rset( 0,  0x01 );  // Select page 1
    AIC3204_rset( 16, 0x00 );  // Unmute HPL , 0dB gain
    AIC3204_rset( 17, 0x00 );  // Unmute HPR , 0dB gain
    AIC3204_rset( 9 , 0x30 );  // Power up HPL,HPR
    EZDSP5535_waitusec(100 );  // Wait
    
    /* ADC ROUTING and Power Up */
    AIC3204_rset( 0,  0x01 );  // Select page 1
    AIC3204_rset( 51, 0x48);  // power up MICBIAS with AVDD (0x40)or LDOIN (0x48)	//MM - added micbias
    AIC3204_rset( 52, 0x30 );  // STEREO 1 Jack
                               // IN2_L to LADC_P through 40 kohm
    AIC3204_rset( 55, 0x30 );  // IN2_R to RADC_P through 40 kohmm
    AIC3204_rset( 54, 0x03 );  // CM_1 (common mode) to LADC_M through 40 kohm
    AIC3204_rset( 57, 0xc0 );  // CM_1 (common mode) to RADC_M through 40 kohm
    AIC3204_rset( 59, 60 );  // MIC_PGA_L unmute
    AIC3204_rset( 60, 60 );  // MIC_PGA_R unmute
    AIC3204_rset( 0,  0x00 );  // Select page 0
    AIC3204_rset( 81, 0xc0 );  // Powerup Left and Right ADC
    AIC3204_rset( 82, 0x00 );  // Unmute Left and Right ADC
    AIC3204_rset( 0,  0x00 );  // Select page 0 
    EZDSP5535_waitusec(100 );  // Wait
    
    /* Initialize I2S */
    if(AIC3204_SAMPLE_SIZE == 32)
        _I2S_init(I2S_WORDLEN_32);
    else
       	_I2S_init(I2S_WORDLEN_16);

}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  AIC3204 Initialisation of both sampling frequency and gain in dB.       *
 *      Initialise both the registers and I2S                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */

unsigned long set_sampling_frequency_and_gain(unsigned long SamplingFrequency, unsigned int ADCgain)
{
    unsigned int PLLPR = 0x91; // Default to 48000 Hz 	
    unsigned int gain;
    unsigned long output;

    if ( ADCgain >= 48)
     {
      gain = 95;      //  Limit gain to 47.5 dB
      ADCgain = 48;   // For display using printf()
     }
    else 
    {
     gain = (ADCgain << 1); // Convert 1dB steps to 0.5dB steps
    }
     
    switch (SamplingFrequency)
    {
     case 48000:
     	PLLPR = 0x91; // 1001 0001b. PLL on. P = 1, R = 1. 
     	printf("Sampling frequency 48000 Hz Gain = %2d dB\n", ADCgain);
     	output = 48000;
     break;
     
     case 24000:
      	PLLPR = 0xA1; // 1010 0001b. PLL on. P = 2, R = 1.
      	printf("Sampling frequency 24000 Hz Gain = %2d dB\n", ADCgain);
      	output = 24000;   
     break;
     
     case 16000:
     	PLLPR = 0xB1; // 1011 0001b. PLL on. P = 3, R = 1.
      	printf("Sampling frequency 16000 Hz Gain = %2d dB\n", ADCgain); 
      	output = 16000;      	
     break;
     
     case 12000:
        PLLPR = 0xC1; //1100 0001b. PLL on. P = 4, R = 1.
        printf("Sampling frequency 12000 Hz Gain = %2d dB\n", ADCgain);
        output = 12000;  
     break;
     
     case 9600:
     	PLLPR = 0xD1; //1101 0001b. PLL on. P = 5, R = 1.
       	printf("Sampling frequency 9600 Hz Gain = %2d dB\n", ADCgain); 
       	output = 9600; 
     break;
     
     case 8000:
     	PLLPR = 0xE1; //1110 0001b. PLL on. P = 6, R = 1.
     	printf("Sampling frequency 8000 Hz Gain = %2d dB\n", ADCgain);
     	output = 8000;  
     break;   	
     
     case 6857:
     	PLLPR = 0xF1; //1111 0001b. PLL on. P = 7, R = 1.
     	printf("Sampling frequency 6857 Hz Gain = %2d dB\n", ADCgain);  
     	output = 6857;    
     break;
     
     default:
     	PLLPR = 0x91; // 1001 0001b. PLL on. P = 1, R = 1. 
     	printf("Sampling frequency not recognised. Default to 48000 Hz Gain = %2d dB\n", ADCgain);
     	output = 48000;  
     break;
    }
    
    	/* Configure AIC3204 */
    AIC3204_rset( 0,  0x00 );  // Select page 0
    AIC3204_rset( 1,  0x01 );  // Reset codec
    EZDSP5535_waitusec(1000);  // Wait 1ms after reset
    AIC3204_rset( 0,  0x01 );  // Select page 1
    AIC3204_rset( 1,  0x08 );  // Disable crude AVDD generation from DVDD
    AIC3204_rset( 2,  0x01 );  // Enable Analog Blocks, use LDO power
    AIC3204_rset( 123,0x05 );  // Force reference to power up in 40ms
    EZDSP5535_waitusec(50000); // Wait at least 40ms
    AIC3204_rset( 0,  0x00 );  // Select page 0
    
    /* PLL and Clocks config and Power Up  */
    if(AIC3204_SAMPLE_SIZE == 32)
    	AIC3204_rset( 27, 0x3d );  // BCLK and WCLK are set as o/p; AIC3204(Master); 32-bit sample
    else
    	AIC3204_rset( 27, 0x0d );  // BCLK and WCLK are set as o/p; AIC3204(Master); 16-bit sample
    AIC3204_rset( 28, 0x00 );  // Data ofset = 0
    AIC3204_rset( 4,  0x03 );  // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    AIC3204_rset( 6,  0x07 );  // PLL setting: J=7
    AIC3204_rset( 7,  0x06 );  // PLL setting: HI_BYTE(D=1680)
    AIC3204_rset( 8,  0x90 );  // PLL setting: LO_BYTE(D=1680)
    AIC3204_rset( 30, 0x88 );  // For 32 bit clocks per frame in Master mode ONLY
                               // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    AIC3204_rset( 5,  PLLPR );  // PLL setting: Power up PLL, P=1 and R=1
    EZDSP5535_waitusec(10000); // Wait for PLL to come up    
    AIC3204_rset( 13, 0x00 );  // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    AIC3204_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    AIC3204_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    AIC3204_rset( 11, 0x82 );  // Power up NDAC and set NDAC value to 2
    AIC3204_rset( 12, 0x87 );  // Power up MDAC and set MDAC value to 7
    AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
    AIC3204_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2
    
    /* DAC ROUTING and Power Up */
    AIC3204_rset( 0,  0x01 );  // Select page 1
    AIC3204_rset( 12, 0x08 );  // LDAC AFIR routed to HPL
    AIC3204_rset( 13, 0x08 );  // RDAC AFIR routed to HPR
    AIC3204_rset( 0,  0x00 );  // Select page 0
    AIC3204_rset( 64, 0x02 );  // Left vol=right vol
    AIC3204_rset( 65, 0x00 );  // Left DAC gain to 0dB VOL; Right tracks Left
    AIC3204_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
    AIC3204_rset( 0,  0x01 );  // Select page 1
    AIC3204_rset( 16, 0x00 );  // Unmute HPL , 0dB gain
    AIC3204_rset( 17, 0x00 );  // Unmute HPR , 0dB gain
    AIC3204_rset( 9 , 0x30 );  // Power up HPL,HPR
    EZDSP5535_waitusec(100 );  // Wait
    
    /* ADC ROUTING and Power Up */
    AIC3204_rset( 0,  0x01 );  // Select page 1
    AIC3204_rset( 51, 0x48);  // power up MICBIAS with AVDD (0x40)or LDOIN (0x48)	//MM - added micbias
    AIC3204_rset( 52, 0x10 );  // STEREO 1 Jack
                               // IN2_L to LADC_P through 40 kohm
    AIC3204_rset( 55, 0x10 );  // IN2_R to RADC_P through 40 kohmm
    AIC3204_rset( 54, 0x01 );  // CM_1 (common mode) to LADC_M through 40 kohm
    AIC3204_rset( 57, 0x40 );  // CM_1 (common mode) to RADC_M through 40 kohm
    AIC3204_rset( 59, gain );  // MIC_PGA_L unmute
    AIC3204_rset( 60, gain );  // MIC_PGA_R unmute
    AIC3204_rset( 0,  0x00 );  // Select page 0
    AIC3204_rset( 81, 0xc0 );  // Powerup Left and Right ADC
    AIC3204_rset( 82, 0x00 );  // Unmute Left and Right ADC
    AIC3204_rset( 0,  0x00 );  // Select page 0 
    EZDSP5535_waitusec(100 );  // Wait
    
    /* Initialize I2S */
    if(AIC3204_SAMPLE_SIZE == 32)
    	_I2S_init(I2S_WORDLEN_32);
    else
    	_I2S_init(I2S_WORDLEN_16);

    return(output);
}


Int16 _I2S_init(I2S_WordLen word_len)
{
	CSL_I2sHandle   hI2s;
	I2S_Config      hwConfig;
    Int16           result = 0;

    /* Open the device with instance 0 */
    hI2s = I2S_open(I2S_INSTANCE2, I2S_POLLED, I2S_CHAN_STEREO);

    /* Set the value for the configure structure */
    hwConfig.dataType           = I2S_STEREO_ENABLE;
    hwConfig.loopBackMode       = I2S_LOOPBACK_DISABLE;
    hwConfig.fsPol              = I2S_FSPOL_LOW;
    hwConfig.clkPol             = I2S_RISING_EDGE;
    hwConfig.datadelay          = I2S_DATADELAY_ONEBIT;
    hwConfig.datapack           = I2S_DATAPACK_ENABLE;
    hwConfig.signext            = I2S_SIGNEXT_DISABLE;
    hwConfig.wordLen            = word_len;
    hwConfig.i2sMode            = I2S_SLAVE;
    hwConfig.FError             = I2S_FSERROR_ENABLE;
    hwConfig.OuError            = I2S_OUERROR_ENABLE;

    /* Configure hardware registers */
    result += I2S_setup(hI2s, &hwConfig);
    result += I2S_transEnable(hI2s, TRUE);

    return result;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of aic3204_init.c                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */





