#ifndef VEZBA6D_FIR_H_
#define VEZBA6D_FIR_H_


Int16 fir_basic(Int16 input, Int16* coeffs, Int16 *history, Uint16 n_coeff);
Int16 fir_circular(Int16 input, Int16 *coeffs, Int16 *history, Uint16 n_coeff, Uint16 *p_state);

#endif /*VEZBA6D_FIR_H_*/
