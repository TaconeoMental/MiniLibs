#ifndef MINI_UTF_8_H
#define MINI_UTF_8_H

#include <stdint.h>
/*
 * "Librería" muy extremadamente básica para trabajar con utf-8.
 * Tiene lo justo y necesario para lo que necesito, así que no me funen.
 */

#define MAX_BYTES 4
#define NULL_UTF8 (utf8c_t)0

// UTF-8
// 00000000 -- 0000007F: 0xxxxxxx
// 00000080 -- 000007FF: 110xxxxx 10xxxxxx
// 00000800 -- 0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
// 00010000 -- 001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
typedef uint32_t utf8c_t;

int8_t utf8c_is_ascii(utf8c_t);
char utf8c_to_ascii(utf8c_t);
int8_t utf8c_is_null(utf8c_t);
int8_t utf8c_is_newline(utf8c_t);

// TODO!!!
/*
uint8_t utf8c_strlen(char *, uint8_t);
int8_t utf8c_is_alpha(utf8c_t);
int8_t utf8c_is_numeric(utf8c_t);
int8_t utf8c_is_whitespace(utf8c_t);
*/

// Helpers
void utf8c_add_byte_at(utf8c_t *, uint8_t, uint8_t);
uint8_t get_bit_at(uint8_t, uint8_t);
uint8_t utf8c_get_byte_at(utf8c_t, uint8_t);
uint8_t utf8c_get_byte_length(utf8c_t);
uint8_t utf8c_get_start_offset(utf8c_t);

#endif
