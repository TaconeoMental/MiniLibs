#include "mini_path.h"

#include <string.h>

#define LAST_CHAR(chp) (chp[strlen(chp) - 1])

/* Helpers */

// Como un str.split(sep)[-1] para los pythonistas ;)
static uint8_t get_last_between(char *str, char sep, char *buffer)
{
    char final_str[MAX_FILENAME_LENGTH] = "";
    uint8_t final_str_i = 0;

    for (uint8_t i = 0; i < strlen(str); ++i)
    {
        if (str[i] == sep)
        {
            final_str_i = 0;
            continue;
        }
        final_str[final_str_i++] = str[i];
    }
    final_str[final_str_i] = '\0';
    strcpy(buffer, final_str);
    return final_str_i;
}

static char get_next_char(char *str, char *c, uint8_t *curr_ind)
{
    char cr = *c;
    uint8_t str_len = strlen(str);
    if (*curr_ind >= str_len)
    {
        *c = '\0';
    }
    else
    {
        *c = str[*curr_ind];
        *curr_ind += 1;
    }
    return cr;
}

#define ADVANCE_CHAR curr_char = get_next_char(str, &peek_char, &str_index)

// Basicamente, si hay 2+ de "c" juntos, los colapsa y deja solo 1
static void colapse_duplicates(char *str, char c, char *buffer)
{
    char curr_char;
    char peek_char;

    uint8_t buffer_index = 0;
    uint8_t str_index = 0;

    get_next_char(str, &peek_char, &str_index);
    ADVANCE_CHAR;
    while (curr_char != '\0')
    {
        if (curr_char == c && peek_char == c)
        {
            do
            {
                ADVANCE_CHAR;
            }
            while (peek_char == c);
        }
        buffer[buffer_index++] = curr_char;
        ADVANCE_CHAR;
    }
    buffer[buffer_index] = '\0';
}

// Para que no llore el compilador por redefiniciones
#undef ADVANCE_CHAR


/* Funciones */

uint8_t path_get_basename(char *path, char *buffer)
{
    return get_last_between(path, '/', buffer);
}

uint8_t path_get_dirname(char *path, char *buffer)
{
    uint8_t path_len = strlen(path);

    if (LAST_CHAR(path) == '/')
    {
        strncpy(buffer, path, path_len - 1);
        return path_len - 1;
    }

    uint8_t basename_len = path_get_basename(path, buffer);
    strncpy(buffer, path, path_len - basename_len - 1);
    return strlen(buffer);
}

uint8_t path_get_ext(char *path, char *buffer)
{
    char basename[MAX_FILENAME_LENGTH];
    if (path_get_basename(path, basename) == 0)
    {
        return 0;
    }
    return get_last_between(basename, '.', buffer);
}

int8_t path_is_absolute(char *path)
{
    return path[0] == '/';
}

int8_t path_is_relative(char *path)
{
    return !path_is_absolute(path);
}


#define ADVANCE_CHAR curr_char = get_next_char(clean_path, &peek_char, &clean_path_index)

/*
 * Funciones:
 *
 * Tratar con redundancias: /home/user////Documents => /home/user/Documents
 * ..: /A/B/../C/D => /A/C/D
 * .: /A/B/./C/D => /A/B/C/D
 * /A/B/./C/../E/./F => /A/B/E/F
 * TODO: Tratar con el caso "/.."
 */
uint8_t path_normalize(char *path, char *buffer)
{
    // Buffers temporales
    char clean_path[MAX_PATH_LENGTH];
    char temp_path[MAX_PATH_LENGTH];
    char basename[MAX_FILENAME_LENGTH];

    //int8_t is_absolute = path_is_absolute(path);

    //uint8_t path_len = strlen(path);
    char curr_char;
    char peek_char;

    uint8_t clean_path_index = 0;
    uint8_t temp_path_index = 0;

    colapse_duplicates(path, '/', clean_path);

    get_next_char(clean_path, &peek_char, &clean_path_index);
    ADVANCE_CHAR;
    while (curr_char != '\0')
    {
        if (curr_char == '/')
        {
            if (peek_char == '\0')
            {
                break;
            }

            // Casos a tratar:
            // - "/A/B/." => "/A/B"
            // - "/A/./B" => "/A/B"
            // - "/A/B/.." => "/A"
            // - "/A/B/../C/" => "/A/C"
            else if (peek_char == '.')
            {
                /*
                ADVANCE_CHAR;
                if (peek_char == '\0')
                {
                    ADVANCE_CHAR;
                }
                else if (peek_char == '/')
                {
                    ADVANCE_CHAR;
                    continue;
                }
                else if (peek_char == '.')
                {
                    ADVANCE_CHAR;
                    ADVANCE_CHAR;
                    path_get_dirname(temp_path, buffer);
                    temp_path_index -= path_get_basename(buffer, basename) + 1;
                    continue;
                }
                else
                {
                    temp_path[temp_path_index++] = '/';
                }
                */
                ADVANCE_CHAR;
                switch (peek_char)
                {
                case '.':
                    ADVANCE_CHAR;
                    path_get_dirname(temp_path, buffer);
                    temp_path_index -= path_get_basename(buffer, basename) + 1;
                case '/':
                case '\0':
                    ADVANCE_CHAR;
                    continue;
                default:
                    temp_path[temp_path_index++] = '/';
                }
            }
        }
        temp_path[temp_path_index++] = curr_char;
        ADVANCE_CHAR;
    }
    temp_path[temp_path_index] = '\0';
    strcpy(buffer, temp_path);

    return temp_path_index;
}

uint8_t path_join(char *path1, char *path2, char *buffer)
{
    char norm_path[MAX_PATH_LENGTH];
    strcpy(norm_path, path1);
    strcat(norm_path, path2);
    return path_normalize(norm_path, buffer);
}

uint8_t path_join_multi(char **paths, uint8_t n, char *buffer)
{
    char final_path[MAX_PATH_LENGTH];
    // Usar path_join de forma iterativa
    for (uint8_t i = 0; i < n; ++i)
    {
        if (i == n - 1)
        {
            path_join(paths[i], "", final_path);
        }
        else
        {
            path_join(paths[i], paths[++i], final_path);
        }
        strcat(buffer, final_path);
    }
    return strlen(buffer);
}

