#ifndef G_LOG
#define G_LOG 

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define LOG_INFO 0
#define LOG_ERROR 1
#define LOG_WARN 2

void gLog(int level, const char* str, ...){
    const char* header;

    va_list args;
    va_start(args, str);

    switch (level) {
        case LOG_INFO: header = "[INFO]"; break;
        case LOG_ERROR: header = "[ERROR]"; break;
        case LOG_WARN: header = "[WARN]"; break;
    }
    
    
    printf("%s ", header);
    vprintf(str, args);
    printf("\n");
    va_end(args);

    if (level == LOG_ERROR){
        exit(-1);
    }
}




#endif