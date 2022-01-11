#ifndef MINI_LOG_CONF_H
#define MINI_LOG_CONF_H

#define LOGGER_VERBOSITY_LEVEL LoggerVLevelInfo
#define LOGGER_USE_COLOUR
#define LOGGER_F_FUNC printf

/* Como ejemplo más común, trabajando en algún framework de Arduino, uno
 * podría usar lo siguiente:
 *
 * #define SERIAL_BAUD 9600
 * #define LOGGER_F_FUNC Serial.printf
 * #define LOGGER_INIT() \
 *  do { \
 *      if (LOGGER_VERBOSITY_LEVEL != LoggerVLevelOff) { \
 *          Serial.begin(SERIAL_BAUD); \
 *          vTaskDelay(pdMS_TO_TICKS(500));} \
 *  } while (0)
 */

#endif

