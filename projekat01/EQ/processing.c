#include "processing.h"
#include "iir.h"

inline void clip(Int32 *x) {
    if (*x > 32767) {
        *x = 32767;
    } else if (*x < -32768) {
        *x = -32768;
    }
}


void calculateShelvingCoeff(float c_alpha, Int16* output)
{
   Int32 t1, t2;

   t1 = 32767 * c_alpha;
   clip(&t1);

   t2 = -(32768 * c_alpha);
   clip(&t2);

   output[0] = t1;
   output[1] = -32768;
   output[2] = 32767;
   output[3] = t2;
}

void calculatePeekCoeff(float c_alpha, float c_beta, Int16* output)
{
    Int32 t1, t2;

    t1 = 32767 * c_alpha;
    clip(&t1);

    t2 = -c_beta * (32768 * c_alpha);
    t2 >>= 1;
    clip(&t2);

    output[0] = t1;
    output[1] = t2;
    output[2] = 32767;
    output[3] = 32767;
    output[4] = t2;
    output[5] = t1;
}

Int16 shelvingHP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
    Int16 t1, t2;
    Int32 H, L ,res;

    t1 = first_order_IIR(input, coeff, x_history, y_history);

    L = ((input >> 1) + (t1 >> 1));
    H = ((input >> 1) - (t1 >> 1));

    clip(&L);
    clip(&H);

    t2 = _smpy(k,(Int16)L);
    res = t2 + H;

    clip(&res);

    return res;
}

Int16 shelvingLP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
    Int16 t1, t2;
    Int32 H, L, res;

    t1 = first_order_IIR(input, coeff, x_history, y_history);

    H = ((input >> 1) + (t1 >> 1));
    L = ((input >> 1) - (t1 >> 1));

    clip(&L);

    t2 = _smpy(k,(Int16)L);
    res = t2 + H;

    clip(&res);

    return res;
}

Int16 shelvingPeek(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
	Int16 t1, t2;
	Int32 H, L, res;

	t1 = second_order_IIR(input, coeff, x_history, y_history);

	H = ((input >> 1) + (t1 >> 1));
	L = ((input >> 1) - (t1 >> 1));

	clip(&L);

	t2 = _smpy(k, (Int16) L);
	res = t2 + H;

	clip(&res);

	return res;
}
