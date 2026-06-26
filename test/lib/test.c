#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "test.h"

const char *pattern="\x1b[1m%s\x1b[0m";

void printFormatted(const char *text, ...){
    va_list args;
    va_start(args, text);

    printf("\x1b[1;34mINFO\x1b[0m ");
    
    vprintf(text, args);
    va_end(args);
    printf("\n");
}

void printFail_origin(const char *testName, const char *text, va_list args) {
    printf("\x1b[1;91mFAIL\x1b[0m ");
    printf(pattern, testName);
    printf(" ");
    
    vprintf(text, args);
}

void printFail(const char *testName, const char *text, ...)
{
    va_list args;
    va_start(args, text);

    printFail_origin(testName, text, args);

    va_end(args);
    printf("\n\n");
}

void printPass(const char *testName)
{
    printf("\x1b[1;92mPASS\x1b[0m ");
    printf(pattern, testName);
    printf("\n\n");
}

void printWarning(const char *text, ...)
{
    va_list args;
    va_start(args, text);

    printf("\x1b[1;93mWARN\x1b[0m ");

    vprintf(text, args);
    va_end(args);
    printf("\n");
}

void printInfo(const char *info, ...){
    const char *printText=info==NULL?"":info;
    va_list args;
    va_start(args, info);
    printf("\x1b[1;34mINFO\x1b[0m ");
    vprintf(info, args);
    va_end(args);
    printf("\n");
}

void fail(const char *testName, const char *text, int code, ...){
    va_list args;
    va_start(args, code);
    printFail_origin(testName, text, args);
    va_end(args);
    exit(code);
}