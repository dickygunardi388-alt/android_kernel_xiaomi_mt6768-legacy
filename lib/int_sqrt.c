// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2013 Davidlohr Bueso <davidlohr.bueso@hp.com>
 *
 * Fast exact integer square root based on firelzrd's isqrt (CLZ + LUT +
 * Newton-Raphson), replacing the Guy L. Steele shift-and-subtract
 * implementation.
 */

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/bitops.h>
#include <linux/isqrt.h>

/**
 * int_sqrt - rough approximation to sqrt
 * @x: integer of which to calculate the sqrt
 *
 * A very rough approximation to the sqrt() function.
 */
inline unsigned long int_sqrt(unsigned long x)
{
	if (x <= 1)
		return x;

#if BITS_PER_LONG == 64
	return (unsigned long)isqrt64(x);
#else
	return (unsigned long)isqrt32(x);
#endif
}
EXPORT_SYMBOL(int_sqrt);

#if BITS_PER_LONG < 64
/**
 * int_sqrt64 - strongly typed int_sqrt function when minimum 64 bit input
 * is expected.
 * @x: 64bit integer of which to calculate the sqrt
 */
u32 int_sqrt64(u64 x)
{
	u64 b, m, y = 0;

	if (x <= ULONG_MAX)
		return int_sqrt((unsigned long) x);

	m = 1ULL << (fls64(x) & ~1ULL);
	while (m != 0) {
		b = y + m;
		y >>= 1;

		if (x >= b) {
			x -= b;
			y += m;
		}
		m >>= 2;
	}

	return y;
}
EXPORT_SYMBOL(int_sqrt64);
#endif
