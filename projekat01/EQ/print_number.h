//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 2
// * Godina: 2017
// *
// * Stampanje broja na LCD ekran na EZDSP5535 razvojnoj ploci
// *
//////////////////////////////////////////////////////////////////////////////

#ifndef DTMF_PREDAJNIK_PRINT_NUMBER_H_
#define DTMF_PREDAJNIK_PRINT_NUMBER_H_

/* Inicijalizacija stampanja - brisanje ekrana */
void initPrintNumber();

/* Stampanje karaktera na ekran */
void printChar(char x);

/* Postavi pokazivac pisanja na pocetak ekrana*/
void setWritePointerToFirstChar();

/* Ocisti ekran */
void clearLCD();

#endif /* DTMF_PREDAJNIK_PRINT_NUMBER_H_ */
