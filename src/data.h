#ifndef DATA_H
#define DATA_H

#include <ctype.h>
#include <stdint.h>

void data_pack_int32(unsigned char*, int32_t);
void data_pack_int16(unsigned char*, int16_t);
void data_pack_float32(unsigned char*, double);

int32_t data_unpack_int32(unsigned char*);
int16_t data_unpack_int16(unsigned char*);
float   data_unpack_float32(unsigned char*);

#endif /* DATA_H */
