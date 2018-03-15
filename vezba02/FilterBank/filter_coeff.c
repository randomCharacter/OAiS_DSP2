/*
 * filter_coeff.c
 *
 *  Created on: 29.02.2016.
 *      Author: Dejan Bokan (dejan.bokan@rt-rk.com)
 */

#include "filter_coeff.h"

Int16 H0[N_COEFF] = { 119, 0, -401, 0,  1124, 0, -2812, 0,  10177, 16355,  10177, 0, -2812, 0,  1124, 0, -401, 0,  119};

Int16 H1[N_COEFF] = { -119, 0,  401, 0, -1124, 0,  2812, 0, -10177, 16355, -10177, 0,  2812, 0, -1124, 0,  401, 0, -119};

