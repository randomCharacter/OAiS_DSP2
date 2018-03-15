#ifndef VEZBA6D_FIR_H_
#define VEZBA6D_FIR_H_


Int16 fir_basic(Int16 input, Int16* coeffs, Int16 *history, Uint16 n_coeff);
Int16 fir_circular(Int16 input, Int16 *coeffs, Int16 *history, Uint16 n_coeff, Uint16 *p_state);
void fir_basic_decimate(Int16 *input, Int16 *output, int n, int m, Int16 *coefs, Int16 *history, Uint16 n_coeff);
void fir_basic_interpolate(Int16 *input, Int16 *output, int n, int l, Int16 *coefs, Int16 *history, Uint16 n_coeff);

#endif /*VEZBA6D_FIR_H_*/
