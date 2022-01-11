#ifndef MINI_LOG_H
#define MINI_LOG_H

#include "mini_log_conf.h"

// Helpers
#define ANSI_CODE_STR(code) "\033[" #code "m"
#define COLOR_STR(ansi_c, str) ANSI_CODE_STR(ansi_c) str "\033[0m"
#define VA_ARGS(...) , ##__VA_ARGS__

enum LoggerVLevel
{
    LoggerVLevelOff,   // Log desactivado
    LoggerVLevelError, // Errores
    LoggerVLevelWarn,  // Advertencias
    LoggerVLevelInfo,  // Datos Informativos
    LoggerVLevelDebug, // Para depurar
    LoggerVLevelTrace  // Detalles específicos
};

#define LOG_INFO_STR "[%s] %s:%d:%s(): "
#define LOG_INFO_STR_ARGS __FILE__, __LINE__, __func__

/* Colores */
#ifdef LOGGER_USE_COLOUR
#define BOLD_STR(str)   COLOR_STR(1, str)
#define GREEN_STR(str)  COLOR_STR(92, str)
#define YELLOW_STR(str) COLOR_STR(93, str)
#define RED_STR(str)    COLOR_STR(91, str)
#else
#define BOLD_STR(str)   str
#define GREEN_STR(str)  str
#define YELLOW_STR(str) str
#define RED_STR(str)    str
#endif

#define LOG_IF_VALID(level, lvl_name, fmt, args...) \
    do { \
        if (level <= LOGGER_VERBOSITY_LEVEL) { \
            LOGGER_F_FUNC(LOG_INFO_STR fmt "\r\n", lvl_name, LOG_INFO_STR_ARGS VA_ARGS(args));} \
    } while (0)

#define MLOG_E(fmt, ...) LOG_IF_VALID(LoggerVLevelError, RED_STR("ERROR"), fmt, __VA_ARGS__)
#define MLOG_W(fmt, ...) LOG_IF_VALID(LoggerVLevelWarn, YELLOW_STR("WARN"), fmt, __VA_ARGS__)
#define MLOG_I(fmt, ...) LOG_IF_VALID(LoggerVLevelInfo, GREEN_STR("INFO"), fmt, __VA_ARGS__)
#define MLOG_D(fmt, ...) LOG_IF_VALID(LoggerVLevelDebug, BOLD_STR("DEBUG"), fmt, __VA_ARGS__)
#define MLOG_T(fmt, ...) LOG_IF_VALID(LoggerVLevelTrace, "TRACE", fmt, __VA_ARGS__)

#endif

