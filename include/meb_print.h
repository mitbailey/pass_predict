/**
 * @file meb_print.h
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Contains debug-related macros and function-like macros.
 * @version 1.1
 * @date 2021.07.26
 *
 * With revisions by Sunip K. Mukherjee (sunipkmukherjee@gmail.com).
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef MEB_PRINT_H
#define MEB_PRINT_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

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

#else // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

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
#endif // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#if !defined(_MSC_VER)

#ifndef dbprintlf
#define dbprintlf(format, ...)                                                                                                  \
    (                                                                                                                           \
        {                                                                                                                       \
            int _meb_rc = fprintf(stderr, "[%s:%d | %s] " format TERMINATOR "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
            fflush(stderr);                                                                                                     \
            _meb_rc;                                                                                                            \
        })
#endif // dbprintlf

#ifndef dbprintf
#define dbprintf(format, ...)                                                                                              \
    (                                                                                                                      \
        {                                                                                                                  \
            int _meb_rc = fprintf(stderr, "[%s:%d | %s] " format TERMINATOR, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
            fflush(stderr);                                                                                                \
            _meb_rc;                                                                                                       \
        })
#endif // dbprintf

#ifndef bprintf
#define bprintf(str, ...)                                        \
    (                                                            \
        {                                                        \
            int _meb_rc = printf(str TERMINATOR, ##__VA_ARGS__); \
            fflush(stdout);                                      \
            _meb_rc;                                             \
        })
#endif // bprintf

#ifndef bprintlf
#define bprintlf(str, ...)                                             \
    (                                                                  \
        {                                                              \
            int _meb_rc = printf(str TERMINATOR " \n", ##__VA_ARGS__); \
            fflush(stdout);                                            \
            _meb_rc;                                                   \
        })
#endif // bprintlf

#ifndef erprintlf
#define erprintlf(error)                                                                                          \
    (                                                                                                             \
        {                                                                                                         \
            int _meb_rc = fprintf(stderr, "[%s:%d | %s] " RED_FG "ERRNO >>> %d:" RESET_ALL " %s" TERMINATOR "\n", \
                                  __FILE__, __LINE__, __func__, error, strerror(error));                          \
            fflush(stderr);                                                                                       \
            _meb_rc;                                                                                              \
        })
#endif // erprintlf

static char *get_time_now()
{
    static __thread char buf[128];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buf, sizeof(buf), YELLOW_FG "[" YELLOW_FG "%02d:%02d:%02d" YELLOW_FG "] " TERMINATOR,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
    return buf;
}

#ifndef tprintf
#define tprintf(str, ...)                                                             \
    (                                                                                 \
        {                                                                             \
            int _meb_rc = printf("%s" str TERMINATOR, get_time_now(), ##__VA_ARGS__); \
            fflush(stdout);                                                           \
            _meb_rc;                                                                  \
        })
#endif

#ifndef tprintlf
#define tprintlf(str, ...)                                                                 \
    (                                                                                      \
        {                                                                                  \
            int _meb_rc = printf("%s" str TERMINATOR "\n", get_time_now(), ##__VA_ARGS__); \
            fflush(stdout);                                                                \
            _meb_rc;                                                                       \
        })
#endif // tprintlf

#else // !defined(_MSC_VER)

#ifndef dbprintlf
#define dbprintlf(format, ...)                                                                                \
    0;                                                                                                        \
    {                                                                                                         \
        fprintf(stderr, "[%s:%d | %s] " format TERMINATOR "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        fflush(stderr);                                                                                       \
    }
#endif // dbprintlf

#ifndef dbprintf
#define dbprintf(format, ...)                                                                                          \
    0;                                                                                                                 \
    {                                                                                                                  \
        int _meb_rc = fprintf(stderr, "[%s:%d | %s] " format TERMINATOR, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        fflush(stderr);                                                                                                \
        \                                                                                                              \
    }
#endif // dbprintf

#ifndef bprintf
#define bprintf(str, ...)                      \
    0;                                         \
    {                                          \
        printf(str TERMINATOR, ##__VA_ARGS__); \
        fflush(stdout);                        \
    }
#endif // bprintf

#ifndef bprintlf
#define bprintlf(str, ...)                           \
    0;                                               \
    {                                                \
        printf(str TERMINATOR " \n", ##__VA_ARGS__); \
        fflush(stdout);                              \
    }
#endif // bprintlf

#ifndef erprintlf
#define erprintlf(error)                                                                        \
    0;                                                                                          \
    {                                                                                           \
        fprintf(stderr, "[%s:%d | %s] " RED_FG "ERRNO >>> %d:" RESET_ALL " %s" TERMINATOR "\n", \
                __FILE__, __LINE__, __func__, error, strerror(error));                          \
        fflush(stderr);                                                                         \
    }
#endif // erprintlf

static char *get_time_now()
{
    static __thread char buf[128];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buf, sizeof(buf), YELLOW_FG "[" YELLOW_FG "%02d:%02d:%02d" YELLOW_FG "] " TERMINATOR,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
    return buf;
}

#ifndef tprintf
#define tprintf(str, ...)                                           \
    0;                                                              \
    {                                                               \
        printf("%s" str TERMINATOR, get_time_now(), ##__VA_ARGS__); \
        fflush(stdout);                                             \
    }
#endif // tprintf

#ifndef tprintlf
#define tprintlf(str, ...)                                               \
    0;                                                                   \
    {                                                                    \
        printf("%s" str TERMINATOR "\n", get_time_now(), ##__VA_ARGS__); \
        fflush(stdout);                                                  \
    }
#endif // tprintlf
#endif // _MSC_VER

#endif // MEB_PRINT_H