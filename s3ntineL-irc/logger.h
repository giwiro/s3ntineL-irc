#ifndef _LOGGER_H
#define _LOGGER_H

#ifdef _WIN32
#include <Windows.h>
#include "pch.h"
#pragma once
#elif defined(__linux__) || defined(__APPLE__)
#include <stdarg.h>
#endif
#include <stdio.h>

#define MAX_SIZE_LOGGER_BUFFER 1024

// The magical rest parameter, basically we get the format str as first parameter
// and the rest is optional, it will fill into the formatted characters (Ex: %s %d %c %p)
#ifdef _WIN32
void log_message(const CHAR* fmt, ...);
#elif defined(__linux) || defined(__APPLE__)
void log_message(const char* fmt, ...);
#endif

void print_banner();

#endif