#ifndef _log_h_
#define _log_h_

#define INFO_LEVEL 0x8
#define WARN_LEVEL 0x4
#define ERR_LEVEL  0x2

#ifndef LOG_LEVEL
#define LOG_LEVEL  0x2
#endif

#ifdef DEBUG
#define log__(_type, _stream, _format, ...) \
        fprintf(_stream, \
                _type": %s:%d in %s: "_format"\n", \
                __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
                fflush(_stream);
#else
#define log__(_type, _stream, _format, ...) \
        fprintf(_stream, \
                _format"\n", \
                ##__VA_ARGS__); \
                fflush(_stream);
#endif

#define log_err(_format, ...) \
        if (LOG_LEVEL >= ERR_LEVEL) \
        log__("error", stderr, "\033[31m" _format "\033[0m", ##__VA_ARGS__)

#define log_info(_format, ...) \
        if (LOG_LEVEL >= WARN_LEVEL) \
        log__("info", stderr, "\033[32m" _format "\033[0m", ##__VA_ARGS__)

#define log_warn(_format, ...) \
        if (LOG_LEVEL >= INFO_LEVEL) \
        log__("warning", stdout, "\033[33m" _format "\033[0m", ##__VA_ARGS__)

#endif
