#ifndef MINI_PATH_H
#define MINI_PATH_H

// máxima cantidad de subdirectorios a los que se puede acceder, sin contar
// root.
#define MAX_DIRECTORY_LEVELS 3
#define MAX_FILENAME_LENGTH 15
// +1 para los '/' y +1 para el '/' del root
#define MAX_PATH_LENGTH (1 + MAX_DIRECTORY_LEVELS * (MAX_FILENAME_LENGTH + 1))


/*
 * Estas funciones son para hacer más fácil el trabajo con paths. Todo se basa
 * en la premisa de que un path es solo un char*.
 * Está demás decir que los paths deben estar en estilo UNIX.
 */

/* Devuelve el último componente o segmento de un path.
 * Ej:
 * - "/example/path/file.txt" => "file.txt"
 * - "/example/path/" => ""
 * - "/example/path" => "path"
 */
uint8_t path_get_basename(char *path, char *buffer);
uint8_t path_get_dirname(char *path, char *buffer);
uint8_t path_get_ext(char *path, char *buffer);
uint8_t path_normalize(char *path, char *buffer);

/*
 * Pseudocódigo para que se entienda.
 * path_join("/home/./user", "..") => "/home"
 * path_join("/home////user/Documents", "../Downloads") => "/home/user/Downloads"
 */
uint8_t path_join(char *path1, char *path2, char *buffer);
uint8_t path_join_multi(char **paths, uint8_t n, char *buffer);
#endif

