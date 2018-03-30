#include "ezdsp5535.h"
#include "ezdsp5535_lcd.h"
#include "ascii_digits.h"

void initPrintNumber()
{
	int i;

    EZDSP5535_OSD9616_init( );

    /* Zaustavi skrol teksta */
    EZDSP5535_OSD9616_send(0x00,0x2e);

    /* Ocisti ekran na stranici 0 */
    EZDSP5535_OSD9616_send(0x00,0x00);   // Postavi adresu
    EZDSP5535_OSD9616_send(0x00,0x10);
    EZDSP5535_OSD9616_send(0x00,0xb0+0); // Odaberi nultu stranicu

    /* Upisi nule */
    for(i=0; i<128; i++)
    {
        EZDSP5535_OSD9616_send(0x40,0x00);
    }

    /* Ocisti ekran na stranici 0 */
    EZDSP5535_OSD9616_send(0x00,0x00);   // Postavi adresu
    EZDSP5535_OSD9616_send(0x00,0x10);
    EZDSP5535_OSD9616_send(0x00,0xb0+1); // Odaberi prvu stranicu

    /* Upisi nule */
    for(i=0; i<128; i++)
    {
        EZDSP5535_OSD9616_send(0x40,0x00);
    }
}

void clearLCD(){
	int i;
	/* Postavi adresu na prvi karakter na nultoj stranici */
	EZDSP5535_OSD9616_send(0x00,0x00);   // Postavi adresu
	EZDSP5535_OSD9616_send(0x00,0x10);
	EZDSP5535_OSD9616_send(0x00,0xb0+0); // Odaberi nultu stranicu

	/* Upisi nule */
	for(i=0; i<128; i++)
	{
	    EZDSP5535_OSD9616_send(0x40,0x00);
	}

	/* Ocisti ekran na stranici 0 */
	EZDSP5535_OSD9616_send(0x00,0x00);   // Postavi adresu
	EZDSP5535_OSD9616_send(0x00,0x10);
	EZDSP5535_OSD9616_send(0x00,0xb0+1); // Odaberi prvu stranicu

	/* Upisi nule */
	for(i=0; i<128; i++)
	{
	    EZDSP5535_OSD9616_send(0x40,0x00);
	}

	EZDSP5535_OSD9616_send(0x00,0x00);   // Postavi adresu
	EZDSP5535_OSD9616_send(0x00,0x10);
	EZDSP5535_OSD9616_send(0x00,0xb0+0);
}

void setWritePointerToFirstChar(){

	/* Postavi adresu na prvi karakter na nultoj stranici */
    EZDSP5535_OSD9616_send(0x00,0x00);
    EZDSP5535_OSD9616_send(0x00,0x10);
    EZDSP5535_OSD9616_send(0x00,0xb0+0);
}


void printChar(char x)
{
    int i;
	int index = 0;
	switch(x)
	{
		case '#':
			index = 0;
			break;
		case '*':
			index = 1;
			break;
		case '0':
			index = 2;
			break;
		case '1':
			index = 3;
			break;
		case '2':
			index = 4;
			break;
		case '3':
			index = 5;
			break;
		case '4':
			index = 6;
			break;
		case '5':
			index = 7;
			break;
		case '6':
			index = 8;
			break;
		case '7':
			index = 9;
			break;
		case '8':
			index = 10;
			break;
		case '9':
			index = 11;
			break;
		case 'A':
			index = 12;
			break;
		case 'B':
			index = 13;
			break;
		case 'C':
			index = 14;
			break;
		case 'D':
			index = 15;
			break;
		case ' ':
			index = 16;
			break;
		case '.':
			index = 17;
			break;
		default:
			index = -1;
			break;
	}

	if(index >= 0)
	{
		for(i = 0; i < sign_width; i++)
		{
			EZDSP5535_OSD9616_send(0x40, consolas_8ptBitmaps[index*sign_width+i]);
		}
		EZDSP5535_OSD9616_send(0x40, 0x00);
    }
    /* U suporotnom upisi neispravan karakter */
    else
    {
    	for(i = 0; i < sign_width; i++)
    	{
    	    EZDSP5535_OSD9616_send(0x40, 0xFF);
    	}
    	EZDSP5535_OSD9616_send(0x40, 0x00);

    }

}

