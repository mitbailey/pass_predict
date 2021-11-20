/**
 * @file gs_debug.hpp
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Contains debug-related macros and function-like macros.
 * @version 0.1
 * @date 2021.07.26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef MEB_DEBUG_HPP
#define MEB_DEBUG_HPP

#include <stdio.h>

#ifndef dbprintlf
#define dbprintlf(format, ...)                                                                        \
    fprintf(stderr, "[%s:%d | %s] " format "\x1b[0m\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    fflush(stderr);
#endif // dbprintlf

#ifndef dbprintf
#define dbprintf(format, ...)                                                                       \
    fprintf(stderr, "[%s:%d | %s] " format "\x1b[0m", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    fflush(stderr);
#endif // dbprintf

#ifndef erprintlf
#define erprintlf(error)                                                                                               \
    fprintf(stderr, "[%s:%d | %s] \x1b[94m>>> %d: %s\x1b[0m\n", __FILE__, __LINE__, __func__, error, strerror(error)); \
    fflush(stderr);
#endif // erprintlf

#ifndef MEB_COLORS
#define MEB_COLORS
#define RESET_ALL "\x1b[0m"
#define RED_FG "\x1b[91m"
#define GREEN_FG "\x1b[92m"
#define YELLOW_FG "\x1b[33m"
#define BLUE_FG "\x1b[94m"
#define MAGENTA_FG "\x1b[95m"
#define CYAN_FG "\x1b[96m"
#define RED_BG "\x1b[101m"
#define GREEN_BG "\x1b[102m"
#define YELLOW_BG "\x1b[43m"
#define BLUE_BG "\x1b[104m"
#define MAGENTA_BG "\x1b[105m"
#define CYAN_BG "\x1b[106m"
#endif // MEB_COLORS

#ifndef MEB_CODES
#define MEB_CODES
#define FATAL "\033[1m\x1b[107m\x1b[31m(FATAL) "
#endif // MEB_CODES

#endif // MEB_DEBUG_HPP