//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina: 2015
// *
// * Inicijalizaija i rukovanje AIC3204 kodekom
// *
//////////////////////////////////////////////////////////////////////////////

#ifndef TEMPLATE_DMA_AIC3204_H_
#define TEMPLATE_DMA_AIC3204_H_

 extern void aic3204_init(void);
 extern unsigned long set_sampling_frequency_and_gain(unsigned long, unsigned int);
 extern void aic3204_hardware_init(void);
 extern void aic3204_codec_read(Int16*, Int16*);
 extern void aic3204_codec_write(Int16, Int16);
 extern void aic3204_disable(void);

 extern Int16 AIC3204_rset( Uint16 regnum, Uint16 regval);

#endif /*TEMPLATE_DMA_AIC3204_H_*/


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of aic3204.h                                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 
