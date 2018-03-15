//////////////////////////////////////////////////////////////////////////////
// *
// * VEŽBA 2 – Banke filtara
// * Godina: 2016
// *
// * Zadatak: Banke filtara
// * Autor:
// *
// *
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "ezdsp5535_sar.h"
#include "fir.h"
#include "filter_coeff.h"
#include "processing.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L
#define GAIN 0

/* Niz za smestanje ulaznih i izlaznih odbiraka */
#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(InputBufferMono,4)
Int16 InputBufferMono[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(OutputBufferMono,4)
Int16 OutputBufferMono[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(buff1,4)
Int16 buff1[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(buff2,4)
Int16 buff2[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(buff3,4)
Int16 buff3[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(buff4,4)
Int16 buff4[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(buff5,4)
Int16 buff5[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(buff6,4)
Int16 buff6[AUDIO_IO_SIZE];

Int16 history[12][N_COEFF];

void calculate(Int16* input, Int16* output, Int16* history, Int16* coefs, int n_coeffs, int size) {
    int i;
    for (i = 0; i < size; i++) {
        output[i] = fir_basic(input[i], H1, history, N_COEFF);
    }
    downsample(output, output, 2, size);
}

void stereoToMono(Int16* inputL, Int16* inputR, Int16* output, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		output[i] = (inputL[i]>>1) + (inputR[i]>>1);
	}
}

Uint16 getKey() {
    static Uint16 old = NoKey;
    Uint16 key = EZDSP5535_SAR_getKey();
    if (key == old) {
        return NoKey;
    } else {
        old = key;
        return key;
    }
}

void main( void )
{
	int i;
    int state=0;

    /* Inicijalizaija razvojne ploce */
    EZDSP5535_init( );

    /* Inicijalizacija kontrolera za ocitavanje vrednosti dugmeta*/
    EZDSP5535_SAR_init();

	printf("\n DTMF Predajnik\n");

    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();

    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();

    aic3204_dma_init();

    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, GAIN);


 	while(1)
	{
 	    int i;
 		/* Promeniti stanje na osnovu pritisnutog tastera */
 		/* Za dobavljanje vrednosti tastera koristiti funkciju EZDSP5535_SAR_getKey */
 		/* Your code here */
 	    Uint16 key = getKey();
 	    if (key != NoKey) {
 	        state = (state + 1) % 5;
 	    }

 		aic3204_read_block(InputBufferL, InputBufferR);
		stereoToMono(InputBufferL, InputBufferR, InputBufferMono, AUDIO_IO_SIZE);

 		/* Izvrsiti analizu signala InputBufferMono upotrebom filter banke 4. reda */
 		/* Your code here */
		for (i = 0; i < AUDIO_IO_SIZE; i++) {
		    buff1[i] = fir_basic(InputBufferMono[i], H0, history[0], N_COEFF);
		}
		downsample(buff1, buff1, 2, AUDIO_IO_SIZE);

        for (i = 0; i < AUDIO_IO_SIZE; i++) {
            buff2[i] = fir_basic(InputBufferMono[i], H1, history[1], N_COEFF);
        }
        downsample(buff2, buff2, 2, AUDIO_IO_SIZE);

        calculate(buff1, buff3, history[2], H0, N_COEFF, AUDIO_IO_SIZE / 2);
        calculate(buff1, buff4, history[3], H1, N_COEFF, AUDIO_IO_SIZE / 2);
        calculate(buff2, buff5, history[4], H0, N_COEFF, AUDIO_IO_SIZE / 2);
        calculate(buff2, buff6, history[5], H1, N_COEFF, AUDIO_IO_SIZE / 2);


 		/* U zavisnosti od trenutnog stanja nulirati odredjene komponente signala */
 		/* Stanje 0 - nema obrade (komponente signala na ulazu nalaze se i na izlazu) */
 		/* Stanje 1 - samo frekvencijske komponente iz prvog podopsega */
 		/* Stanje 2 - samo frekvencijske komponente iz drugog podopsega */
 		/* Stanje 3 - samo frekvencijske komponente iz treceg podopsega */
 		/* Stanje 4 - samo frekvencijske komponente iz cetvrtog podopsega */
 		/* Your code here */
		switch (state) {
		case 0:

		    break;
        case 1:
            for (i = 0; i < AUDIO_IO_SIZE; i++) {
                buff4[i] = 0;
                buff5[i] = 0;
                buff6[i] = 0;
            }
            break;
        case 2:
            for (i = 0; i < AUDIO_IO_SIZE; i++) {
                buff3[i] = 0;
                buff5[i] = 0;
                buff6[i] = 0;
            }
            break;
        case 3:
            for (i = 0; i < AUDIO_IO_SIZE; i++) {
                buff3[i] = 0;
                buff4[i] = 0;
                buff5[i] = 0;
            }
            break;
        case 4:
            for (i = 0; i < AUDIO_IO_SIZE; i++) {
                buff3[i] = 0;
                buff4[i] = 0;
                buff6[i] = 0;
            }
            break;
		}

 		/* Izvrsiti sintezu signala i rezultat smestiti u OutputBufferMono */
 		/* Your code here */
		upsample(buff3, buff1, 2, AUDIO_IO_SIZE/4);
        for (i = 0; i < AUDIO_IO_SIZE/2; i++) {
            buff3[i] = fir_basic(buff1[i], H0, history[6], N_COEFF);
        }

        upsample(buff4, buff1, 2, AUDIO_IO_SIZE/4);
        for (i = 0; i < AUDIO_IO_SIZE/2; i++) {
            buff4[i] = fir_basic(buff1[i], H1, history[7], N_COEFF);
        }

        upsample(buff5, buff1, 2, AUDIO_IO_SIZE/4);
        for (i = 0; i < AUDIO_IO_SIZE/2; i++) {
            buff5[i] = fir_basic(buff1[i], H0, history[8], N_COEFF);
        }

        upsample(buff6, buff1, 2, AUDIO_IO_SIZE/4);
        for (i = 0; i < AUDIO_IO_SIZE/2; i++) {
            buff6[i] = fir_basic(buff1[i], H1, history[9], N_COEFF);
        }

        for (i = 0; i < AUDIO_IO_SIZE/2; i++) {
            buff1[i] = buff3[i] + buff4[i];
            buff2[i] = buff5[i] + buff6[i];
        }

        upsample(buff1, buff3, 2, AUDIO_IO_SIZE/2);
        for (i = 0; i < AUDIO_IO_SIZE; i++) {
            buff1[i] = fir_basic(buff3[i], H0, history[10], N_COEFF);
        }

        upsample(buff2, buff3, 2, AUDIO_IO_SIZE/2);
        for (i = 0; i < AUDIO_IO_SIZE; i++) {
            buff2[i] = fir_basic(buff3[i], H1, history[11], N_COEFF);
        }

        for (i = 0; i < AUDIO_IO_SIZE; i++) {
            OutputBufferMono[i] = buff1[i] + buff2[i];
        }

		aic3204_write_block(OutputBufferMono, OutputBufferMono);
	}


	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}


