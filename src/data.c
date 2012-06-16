#include "data.h"

/*
 * manually pack a 32 bit integer into a buffer using
 * shifting, to ignore different endians
 */
void data_pack_int32(unsigned char *buffer, int32_t i) {
	*buffer++ = i >> 24;
	*buffer++ = i >> 16;
	*buffer++ = i >> 8;
	*buffer++ = i;
}

/*
 * same with 16 bits
 */
void data_pack_int16(unsigned char *buffer, int16_t i) {
	*buffer++ = i >> 8;
	*buffer++ = i;
}

/*
 * pack a float by manually turning it into IEEE 754 format,
 * as a 32 bit integer, and then packing that
 *
 * I don't think this actually covers some of the oddities of
 * the format, but should work well enough for reasonably-sized
 * numbers (i.e. not reaaaally small)
 */
void data_pack_float32(unsigned char *buffer, double f) {
	int32_t i;

	float fnorm;
	int shift;
	long sign, exp, significand;

	/* special case */
	if (f == 0.0) i = 0;
	else {
		/* check sign */
		if (f < 0) { sign = 1; fnorm = -f; }
		else { sign = 0; fnorm = f; }

		/* shift to find normalized version */
		shift = 0;
		while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
		while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
		fnorm = fnorm - 1.0;

		/* normalize the significand */
		significand = fnorm * ((1LL<<23) + 0.5f);

		/* bias the exponent */
		exp = shift + ((1<<7) - 1);

		/* put it all together */
		i = (sign<<31) | (exp<<23) | significand;
	}
	/* and pack */
	data_pack_int32(buffer, i);
}

/*
 * manually unpack a 32 bit integer from a buffer
 * using shifting, again to ignore endianness
 */
int32_t data_unpack_int32(unsigned char *buffer) {
	return (buffer[0] << 24) |
	       (buffer[1] << 16) |
	       (buffer[2] << 8) |
	       (buffer[3]);
}

/*
 * again, same for 16 bits
 */
int16_t data_unpack_int16(unsigned char *buffer) {
	return (buffer[0] << 8) |
	       (buffer[1]);
}

/*
 * unpack a 32 bit integer from the buffer, and then read
 * back the data from the integer
 *
 * this function operates a bit differently than the other
 * two, meaning that it doesn't simply return the data,
 * because there were some very strange bugs having to do with
 * numbers getting changed simply through the act of returning
 * (probably having something to do with how floats are weird
 * and not stored normally on stack) and so this solution works
 * much better
 */
void data_unpack_float32(unsigned char *buffer, float *f) {
	int32_t i = data_unpack_int32(buffer);
	float result;
	long shift;
	unsigned bias;

	/* special case */
	if (i == 0) {
		*f = 0.0;
		return;
	}

	/* un-normalize the significant */
	result = (i&((1LL<<23)-1));
	result /= (1LL<<23);
	result += 1.0f;

	/* shift based on exponent */
	bias = (1<<7) - 1;
	shift = ((i>>23)&((1LL<<8)-1)) - bias;
	while(shift > 0) { result *= 2.0; shift--; }
	while(shift < 0) { result /= 2.0; shift++; }

	/* multiply in the sign */
	result *= (i>>31)&1?(-1.0):(1.0);

	/* store it */
	*f = result;
}
