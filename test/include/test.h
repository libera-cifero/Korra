#include <stdarg.h>

#ifdef __cplusplus
extern "C"{
#endif 

void printPass(const char *testName);
void printWarning(const char *text, ...);
void printFail(const char *testName, const char *text, ...);
void fail(const char *testName, const char *text, int code, ...);
void printInfo(const char *info, ...);

#ifdef __cplusplus
}
#endif