#ifndef _COMMAND_H
#define _COMMAND_H

#ifdef _WIN32
#pragma once
#include "pch.h"
#include "irc.h"
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#elif defined(__linux__)
// If needed
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#endif

#define CPU_INFO_BUFFER_SIZE 128
#define OS_INFO_BUFFER_SIZE 128

#ifdef _WIN32
void handle_hello(int fd, CHAR *target);
#elif defined(__linux__) || defined(__APPLE__)
void handle_hello(int fd, char *target);
#endif

#ifdef _WIN32
void handle_os(int fd, CHAR *target);
#elif defined(__linux__) || defined(__APPLE__)
void handle_os(int fd, char *target);
#endif

#ifdef _WIN32
void handle_cpu(int fd, CHAR *target);
#elif defined(__linux__) || defined(__APPLE__)
void handle_cpu(int fd, char *target);
#endif

#endif
