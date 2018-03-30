//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 2
// * Godina: 2017
// *
// * Zadatak: Ekvalizacija audio signala
// * Autor:
// *                                                                          
// *                                                                          
/////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "ezdsp5535_sar.h"
#include "print_number.h"
#include "math.h"

#include "iir.h"
#include "processing.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 16000L

#define PI 3.14159265

/* Niz za smestanje ulaznih i izlaznih odbiraka */
#pragma DATA_ALIGN(sampleBufferL,4)
Int16 sampleBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(sampleBufferR,4)
Int16 sampleBufferR[AUDIO_IO_SIZE];
float alpha;
float beta;
Int16 shCoeffL[4];
Int16 shCoeffH[4];
Int16 shPeekCoeffPL[6];
Int16 shPeekCoeffPH[6];
Int16 xHistoryL[2];
Int16 yHistoryL[2];
Int16 xHistoryH[2];
Int16 yHistoryH[2];
Int16 xHistoryPL[3];
Int16 yHistoryPL[3];
Int16 xHistoryPH[3];
Int16 yHistoryPH[3];

Int16 buffL1[AUDIO_IO_SIZE];
Int16 buffL2[AUDIO_IO_SIZE];
Int16 buffL3[AUDIO_IO_SIZE];
Int16 buffL4[AUDIO_IO_SIZE];

Int16 buffR1[AUDIO_IO_SIZE];
Int16 buffR2[AUDIO_IO_SIZE];
Int16 buffR3[AUDIO_IO_SIZE];
Int16 buffR4[AUDIO_IO_SIZE];


Int16 k[4] = { 24000, 24000, 24000, 24000 };
int k_i = 0;

float f[6] = { 2 * PI * 140/SAMPLE_RATE, 2 * PI * 390/SAMPLE_RATE, 2 * PI * 200/SAMPLE_RATE, 2 * PI * 2935/SAMPLE_RATE, 2 * PI * 1905/SAMPLE_RATE, 2 * PI * 5500/SAMPLE_RATE };

Uint16 getKey()
{
    static Uint16 old = NoKey;
    Uint16 key = EZDSP5535_SAR_getKey();
    if (key == old) {
        return NoKey;
    } else {
        old = key;
        return key;
    }
}

void printNewValue() {
	setWritePointerToFirstChar();
	/* Indeks broja k */
	printChar('0' + k_i);
	printChar(' ');
	/* Vrednost broja k */
	if (k[k_i] == 32767) {
		printChar('1');
		printChar('.');
		printChar('0');
	} else {
		printChar('0');
		printChar('.');
		printChar('1' + k[k_i] / 3277);
	}
}

Int16 dirac[AUDIO_IO_SIZE];

void main( void )
{   
    /* Inicijalizaija razvojne ploce */
    EZDSP5535_init( );

    /* Inicijalizacija kontrolera za ocitavanje vrednosti pritisnutog dugmeta*/
    EZDSP5535_SAR_init();

    /* Inicijalizacija LCD kontrolera */
    initPrintNumber();

	printf("\n Ekvalizacija audio signala \n");
		
    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();
	
    /* Inicijalizacija AIC3204 kodeka */
    aic3204_init();

    aic3204_dma_init();
    
    initPrintNumber();
    printNewValue();

    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, 0);

    int i;
    dirac[0] = 16000;
    for (i = 1; i < AUDIO_IO_SIZE; i++) {
        dirac[i] = 0;
    }

    /* Racunanje koeficijenata */
    float alpha, beta;

    alpha = (1 - sin(f[0]))/cos(f[0]);
    calculateShelvingCoeff(0.3, shCoeffL);

    alpha = (1 - sin(f[2]))/cos(f[2]);
    beta = cos(f[1]);
    calculatePeekCoeff(alpha, beta, shPeekCoeffPL);

    alpha = (1 - sin(f[4]))/cos(f[4]);
    beta = cos(f[3]);
    calculatePeekCoeff(alpha, beta, shPeekCoeffPH);

    alpha = (1 - sin(f[5]))/cos(f[5]);
    calculateShelvingCoeff(alpha, shCoeffH);

    while(1)
    {
    	aic3204_read_block(sampleBufferL, sampleBufferR);

    	Uint16 key = getKey();

    	switch (key) {
    	    case SW1:
    	        (k_i + 1 == 4) ? k_i = 0 : ++k_i;
    	        printNewValue();
    	        break;
    	    case SW2:
    	        k[k_i] -= 3277;
    	        if (k[k_i] < 0) {
    	            k[k_i] = 32767;
    	        }
    	        printNewValue();
    	        break;
    	}

    	setWritePointerToFirstChar();


    	for(i = 0; i < AUDIO_IO_SIZE; i++)
        {
			/* Levi kanal */
    	    buffL1[i] = shelvingLP(sampleBufferL[i], shCoeffL, xHistoryL, yHistoryL, k[0]);
            buffL2[i] = shelvingPeek(buffL1[i], shPeekCoeffPL, xHistoryPL, yHistoryPL, k[1]);
            buffL3[i] = shelvingPeek(buffL2[i], shPeekCoeffPH, xHistoryPH, yHistoryPH, k[2]);
            buffL4[i] = shelvingHP(buffL3[i], shCoeffH, xHistoryH, yHistoryH, k[3]);
			/* Desni kanal */
			buffR1[i] = shelvingLP(sampleBufferR[i], shCoeffL, xHistoryL, yHistoryL, k[0]);
            buffR2[i] = shelvingPeek(buffR1[i], shPeekCoeffPL, xHistoryPL, yHistoryPL, k[1]);
            buffR3[i] = shelvingPeek(buffR2[i], shPeekCoeffPH, xHistoryPH, yHistoryPH, k[2]);
            buffR4[i] = shelvingHP(buffR3[i], shCoeffH, xHistoryH, yHistoryH, k[3]);
        }

    	aic3204_write_block(buffL4, buffR4);
	}

    	
	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}
