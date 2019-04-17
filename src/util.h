#ifndef _UTIL_H_
#define _UTIL_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define _CRT_SECURE_NO_WARNINGS 1

#include <windows.h>

#if defined(_WIN64) || defined(WIN64) || defined(__x86_64__) || defined(__x86_64)
#define __X64
#else
#define __X86
#endif

int LoadFile(const char *filename, char **data);
int SaveFile(const char *filename, const char *data, size_t size);
int SaveHeaderBytes(const char *filename, const char *name, const void *data, size_t size);

#if defined(UNICODE) || defined(_UNICODE)
#define fexe_Execute fexe_ExecuteW
#else
#define fexe_Execute fexe_ExecuteA
#endif

BOOL ExecuteW(wchar_t *cmdLine, DWORD *exitCode);
BOOL ExecuteA(const char *name, char *cmdLine, DWORD *exitCode);
BOOL LoadRCData(HINSTANCE instance, DWORD id, void **mem, size_t *size);

#ifdef __cplusplus
}
#endif

#endif