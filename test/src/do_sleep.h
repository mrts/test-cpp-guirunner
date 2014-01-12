#ifndef DO_SLEEP_H__
#define DO_SLEEP_H__

#ifdef _WIN32

#include <windows.h>

void do_sleep(unsigned int seconds)
{ Sleep(seconds * 1000); }

#else

#include <unistd.h>

void do_sleep(unsigned int seconds)
{ sleep(seconds); }

#endif

#endif /* DO_SLEEP_H */
