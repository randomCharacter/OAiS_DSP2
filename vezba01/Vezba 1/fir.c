
#include "tistdtypes.h"
#include "math.h"
#include "fir.h"

Int16 fir_basic(Int16 input, Int16* coeffs, Int16 *history, Uint16 n_coeff)
{
	Int16 i;
	Int32 ret_val = 0;

	/* shift delay line */
	for (i = n_coeff - 2; i >= 0; i--)
	{
		history[i + 1] = history[i];
	}

	/* store input at the beginning of the delay line */
	history[0] = input;


	/* calc FIR */
	for (i = 0; i < n_coeff; i++)
	{
		ret_val = _smac(ret_val, coeffs[i], history[i]);
	}

    return (Int16)(ret_val >> 16);
}

Int16 fir_circular(Int16 input, Int16 *coeffs, Int16 *history, Uint16 n_coeff, Uint16 *p_state)
{
	Int16 i;
    Uint16 state;
    Int32 ret_val;

    state = *p_state;               /* copy the filter's state to a local */


	/* store input at the beginning of the delay line */
    history[state] = input;
	if (++state >= n_coeff)
	{         /* incr state and check for wrap */
		state = 0;
	}

	/* calc FIR and shift data */
	ret_val = 0;
	for (i = n_coeff - 1; i >= 0; i--)
	{
		ret_val = _smac(ret_val, coeffs[i], history[state]);
		if (++state >= n_coeff) /* incr state and check for wrap */
		{
			state = 0;
		}
	}

    *p_state = state;               /* return new state to caller */

    return (Int16)(ret_val >> 16);
}
