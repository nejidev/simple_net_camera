/**
 * @defgroup   LOG log
 *
 * @brief      This file implements log.
 *
 * @author     nejidev
 * @date       2020-10-01 10:37
 */
#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C"{
#endif

#define LOG_DEBUG_LEVEL     1
#define LOG_INFO_LEVEL      2
#define LOG_ERROR_LEVEL     3

#ifndef LOG_DEFAULT_LEVEL
    #define LOG_DEFAULT_LEVEL LOG_DEBUG_LEVEL
#endif

inline static struct tm *get_time(struct tm *tm_buf)
{
    struct timeval tv;
    struct tm *p;

    gettimeofday(&tv,NULL);
    p = localtime_r(&tv.tv_sec, tm_buf);
    if(!p){
        memset((void*)tm_buf, 0, sizeof(*tm_buf));
        p = tm_buf;
    }

    return p;
}

#define LOG_DEBUG(...) do{ \
    if(LOG_DEFAULT_LEVEL <= LOG_DEBUG_LEVEL) { \
        struct tm tm_buf; \
        struct tm *tm_ptr = get_time(&tm_buf); \
        fprintf(stdout, "[%02d-%02d %02d:%02d:%02d][%s:%d][D] : ", \
            tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, \
            __FUNCTION__, __LINE__); \
        fprintf(stdout, __VA_ARGS__); \
        fprintf(stdout,"\n"); \
    } \
}while(0)

#define LOG_INFO(...) do{ \
    if(LOG_DEFAULT_LEVEL <= LOG_INFO_LEVEL) { \
        struct tm tm_buf; \
        struct tm *tm_ptr = get_time(&tm_buf); \
        fprintf(stdout, "[%02d-%02d %02d:%02d:%02d][%s:%d][I] : ", \
            tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, \
            __FUNCTION__, __LINE__); \
        fprintf(stdout, __VA_ARGS__); \
        fprintf(stdout,"\n"); \
    } \
}while(0)

#define LOG_ERROR(...) do{ \
    if(LOG_DEFAULT_LEVEL <= LOG_ERROR_LEVEL) { \
        struct tm tm_buf; \
        struct tm *tm_ptr = get_time(&tm_buf); \
        fprintf(stdout, "[%02d-%02d %02d:%02d:%02d][%s:%d][E] : ", \
            tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, \
            __FUNCTION__, __LINE__); \
        fprintf(stdout, __VA_ARGS__); \
        fprintf(stdout,"\n"); \
    } \
}while(0)

#ifdef __cplusplus
}
#endif

#endif /* __LOG_H__ */
