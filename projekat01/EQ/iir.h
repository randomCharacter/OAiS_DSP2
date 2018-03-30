
#ifndef IIR_H
#define IIR_H

#include "tistdtypes.h"

Int16 first_order_IIR(Int16 input, Int16* coefficients, Int16* x_history, Int16* y_history);
Int16 second_order_IIR( Int16 input, Int16* coefficients, Int16* x_history, Int16* y_history);

#endif
