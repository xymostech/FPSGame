#include "data.h"

void data_pack_int32(unsigned char *buffer, int32_t i) {
	*buffer++ = i >> 24;
	*buffer++ = i >> 16;
	*buffer++ = i >> 8;
	*buffer++ = i;
}

void data_pack_int16(unsigned char *buffer, int16_t i) {
	*buffer++ = i >> 8;
	*buffer++ = i;
}

void data_pack_float32(unsigned char *buffer, float f) {
	int32_t i;

	float fnorm;
	int shift;
	long sign, exp, significand;

	if (f == 0.0) i = 0;
	else {
		if (f < 0) { sign = 1; fnorm = -f; }
		else { sign = 0; fnorm = f; }

		shift = 0;
		while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
		while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
		fnorm = fnorm - 1.0;

		significand = fnorm * ((1LL<<23) + 0.5f);

		exp = shift + ((1<<7) - 1); // shift + bias

		i = (sign<<31) | (exp<<23) | significand;
	}
	data_pack_int32(buffer, i);
}

int32_t data_unpack_int32(unsigned char *buffer) {
	return (buffer[0] << 24) |
	       (buffer[1] << 16) |
	       (buffer[2] << 8) |
	       (buffer[3]);
}

int16_t data_unpack_int16(unsigned char *buffer) {
	return (buffer[0] << 8) |
	       (buffer[1]);
}

float data_unpack_float32(unsigned char *buffer) {
	int32_t i = data_unpack_int32(buffer);
	float result;
	long shift;
	unsigned bias;

	if (i == 0) return 0.0;

	result = (i&((1LL<<23)-1));
	result /= (1LL<<23);
	result += 1.0f;

	bias = (1<<7) - 1;
	shift = ((i>>23)&((1LL<<8)-1)) - bias;
	while(shift > 0) { result *= 2.0; shift--; }
	while(shift < 0) { result /= 2.0; shift++; }

	result *= (i>>31)&1? -1.0: 1.0;

	return result;
}
