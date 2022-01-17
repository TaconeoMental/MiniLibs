#include <SdFat.h>
#include "mini_utf8.h"

#define SPI_SPEED SD_SCK_MHZ(4)
#define SDCARD_CS 5

#define ERROR_HALT(s) Serial.println(s); return

void write_utf8c(SdFile *file, utf8c_t utf8);
utf8c_t read_utf8c(SdFile *file);

SdFat sd;
SdFile file_out;
SdFile file_in;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    utf8c_t utf8_char;

    if (!sd.begin(SDCARD_CS, SPI_SPEED))
    {
        ERROR_HALT("Error iniciando tarjeta SD");
    }

    if (!sd.exists("test_file.txt"))
    {
        ERROR_HALT("Archivo test_file.txt no encontrado.");
    }

    if (sd.exists("file_out.txt"))
    {
        sd.remove("file_out.txt");
    }

    if (!file_out.open("file_out.txt", FILE_WRITE))
    {
        ERROR_HALT("Error abriendo file_out.txt");
    }

    if (!file_in.open("test_file.txt", O_RDONLY))
    {
        ERROR_HALT("Error abriendo file_out.txt");
    }

    do
    {
        utf8_char = read_utf8c(&file_in);
        Serial.println(utf8_char);
        write_utf8c(&file_out, utf8_char);

    } while (!utf8c_is_null(utf8_char));
}

void loop() {}

utf8c_t read_utf8c(SdFile *file)
{
    utf8c_t utf8_char = NULL_UTF8;

    int current_char = file->read();
    if (current_char < 0 || current_char > 0xFF)
    {
        return NULL_UTF8;
    }

    uint8_t byte_count = utf8c_get_byte_length((utf8c_t)current_char);
    utf8c_add_byte_at(&utf8_char, 5 - byte_count, (uint8_t)current_char);
    if (get_bit_at(current_char, 8) == 0)
    {
        // Es ASCII, aka. no tenemos que leer más carácteres.
        return utf8_char;
    }

    for (byte_count -= 1; byte_count > 0; byte_count--)
    {
        utf8c_add_byte_at(&utf8_char, 5 - byte_count, (uint8_t)file->read());
    }
    return utf8_char;
}

void write_utf8c(SdFile *file, utf8c_t utf8)
{
    for (uint8_t i = utf8c_get_start_offset(utf8); i <= 4; i++)
    {
        file->write(utf8c_get_byte_at(utf8, i));
    }
}
