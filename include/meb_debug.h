/**
 * @file gs_debug.h
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Contains debug-related macros and function-like macros.
 * @version 1.0
 * @date 2021.07.26
 *
 * With revisions by Sunip K. Mukherjee.
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef MEB_DEBUG_H
#define MEB_DEBUG_H

#if defined(_STDIO_H) || defined(_INC_STDIO)

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define TERMINATOR
#ifndef MEB_COLORS
#define MEB_COLORS
#define RESET_ALL
#define RED_FG
#define GREEN_FG
#define YELLOW_FG
#define BLUE_FG
#define MAGENTA_FG
#define CYAN_FG
#define RED_BG
#define GREEN_BG
#define YELLOW_BG
#define BLUE_BG
#define MAGENTA_BG
#define CYAN_BG
#endif // MEB_COLORS

#ifndef MEB_CODES
#define MEB_CODES
#define FATAL "(FATAL) "
#endif // MEB_CODES
#else
#define TERMINATOR "\x1b[0m"
#ifndef MEB_COLORS
#define MEB_COLORS
#define RESET_ALL TERMINATOR
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
#endif

#ifndef dbprintlf
#define dbprintlf(format, ...)                                                                                \
    {                                                                                                         \
        fprintf(stderr, "[%s:%d | %s] " format TERMINATOR "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        fflush(stderr);                                                                                       \
    }
#endif // dbprintlf

#ifndef dbprintf
#define dbprintf(format, ...)                                                                            \
    {                                                                                                    \
        fprintf(stderr, "[%s:%d | %s] " format TERMINATOR, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        fflush(stderr);                                                                                  \
    }
#endif // dbprintf

#ifndef bprintf
#define bprintf(str, ...)                     \
    {                                         \
        printf(str TERMINATOR, ##__VA_ARGS__); \
        fflush(stdout);                       \
    }
#endif // bprintf

#ifndef bprintlf
#define bprintlf(str, ...)                       \
    {                                            \
        printf(str TERMINATOR " \n", ##__VA_ARGS__); \
        fflush(stdout);                          \
    }
#endif // bprintlf

// Intended for use with errno.h; also requires string.h.
#ifdef _ERRNO_H
#ifdef _STRING_H
#ifndef erprintlf
#define erprintlf(error)                                                      \
    {                                                                         \
        fprintf(stderr, "[%s:%d | %s] " RED_FG "ERRNO >>> %d:"RESET_ALL" %s" TERMINATOR "\n", \
                __FILE__, __LINE__, __func__, error, strerror(error));        \
        fflush(stderr);                                                       \
    }
#endif // erprintlf
#endif // _STRING_H
#endif // _ERRNO_H

// Requires time.h.
#ifdef _TIME_H
static char *get_time_now()
{
    static __thread char buf[128];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buf, sizeof(buf), GREEN_FG "[" YELLOW_FG "%02d:%02d:%02d" GREEN_FG "] "
                                        TERMINATOR,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
    return buf;
}

#ifndef tprintf
#define tprintf(str, ...)                                          \
    {                                                              \
        printf("%s" str TERMINATOR, get_time_now(), ##__VA_ARGS__); \
        fflush(stdout);                                            \
    }
#endif

#ifndef tprintlf
#define tprintlf(str, ...)                                           \
    {                                                                \
        printf("%s" str TERMINATOR "\n", get_time_now(), ##__VA_ARGS__); \
        fflush(stdout);                                              \
    }
#endif
#endif // _TIME_H


#endif // _STDIO_H

#endif // MEB_DEBUG_H