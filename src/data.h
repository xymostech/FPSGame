/*
 * data.h
 *
 * Functions for packing and unpacking data into char arrays,
 * for sending through packets
 *
 * All functions should be endian-agnostic
 */

#ifndef DATA_H
#define DATA_H

#include <ctype.h>
#include <stdint.h>

/*
 * pack a 32 bit integer, 16 bit integer, and
 * 32 bit float into a buffer
 */
void data_pack_int32(unsigned char*, int32_t);
void data_pack_int16(unsigned char*, int16_t);
void data_pack_float32(unsigned char*, double);

/*
 * retrieve a 32 bit integer, 16 bit integer,
 * and 32 bit float from a buffer
 */
int32_t data_unpack_int32(unsigned char*);
int16_t data_unpack_int16(unsigned char*);
void   data_unpack_float32(unsigned char*, float*);

#endif /* DATA_H */
