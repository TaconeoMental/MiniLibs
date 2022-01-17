#include <stdbool.h>
#include "mini_utf8.h"

// Helpers, muchos helpers
uint8_t get_bit_at(uint8_t byte_flag, uint8_t bit_pos)
{
    return (byte_flag & (1 << (bit_pos - 1))) ? 1 : 0;
}

// Por convención, el primer byte de un punto utf-8 es el número 1, y no el 0
// (de izq a der)
uint8_t utf8c_get_byte_at(utf8c_t utf8, uint8_t byte_number)
{
    if (byte_number > 4)
    {
        return 0;
    }
    return (utf8 >> ((4 - byte_number) * 8)) & 0xFF;
}

uint8_t utf8c_get_start_offset(utf8c_t utf8)
{
    uint8_t offset;
    for (offset = 1; offset <= 4; offset++)
    {
        if (utf8c_get_byte_at(utf8, offset))
        {
            break;
        }
    }
    return offset;
}

// más helpers jeje
uint8_t utf8c_get_byte_length(utf8c_t utf8)
{
    uint8_t number_bytes = 1;
    uint8_t first_byte = utf8c_get_byte_at(utf8, utf8c_get_start_offset(utf8));

    if (get_bit_at(first_byte, 8) == 0)
    {
        return number_bytes;
    }

    while (number_bytes < 8)
    {
        if (get_bit_at(first_byte, 8 - number_bytes) == 0)
        {
            return number_bytes;
        }
        number_bytes++;
    }
}


void utf8c_add_byte_at(utf8c_t *utf8, uint8_t byte_number, uint8_t byte)
{
    byte_number = (4 - byte_number) * 8; // Offset
    *utf8 = (*utf8 & ~(0xFF << byte_number)) | (byte << byte_number);
}

int8_t utf8c_is_ascii(utf8c_t utf8)
{
    return utf8c_get_byte_length(utf8) == 1;
}

char utf8c_to_ascii(utf8c_t utf8)
{
    if (!utf8c_is_ascii(utf8))
    {
        return '\0';
    }
    return (char)utf8c_get_byte_at(utf8, 1);
}

int8_t utf8c_is_null(utf8c_t utf8)
{
    return utf8 == '\0';
}

int8_t utf8c_is_newline(utf8c_t uchar)
{
    // CR: Carriage Return, U+000D (UTF-8 in hex: 0D)
    // LF: Line Feed, U+000A (UTF-8 in hex: 0A)
    // CR+LF: CR (U+000D) + LF (U+000A) (UTF-8 in hex: 0D0A)
    //
    // UTF-8 https://en.wikipedia.org/wiki/Newline#Unicode
    // NEL: Next Line, U+0085 (UTF-8 in hex: C285)
    // LS: Line Separator, U+2028 (UTF-8 in hex: E280A8)

    switch (uchar)
    {
    case 0x0A:
    case 0x0D:
    case 0x0D0A:
    case 0xC285:
    case 0xE280A8:
        return true;
    default:
        return false;
    }
}
