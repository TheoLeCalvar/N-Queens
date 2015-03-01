#ifndef _log_h_
#define _log_h_

#ifdef DEBUG
#define log(_type, _stream, _format, ...) \
        fprintf(_stream, \
                _type": %s:%d in %s: "_format"\n", \
                __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
                fflush(_stream);
#else
#define log(_type, _stream, _format, ...) \
        fprintf(_stream, \
                _format"\n", \
                __VA_ARGS__); \
                fflush(_stream);
#endif

#define log_err(_format, ...) \
        log("error", stderr, _format, __VA_ARGS__)

#define log_info(_format, ...) \
        log("info", stderr, _format, __VA_ARGS__)

#define log_warn(_format, ...) \
        log("warning", stdout, _format, __VA_ARGS__)

#endif

