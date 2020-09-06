#ifndef _IRC_H
#define _IRC_H

#ifdef _WIN32
#pragma once
#include "pch.h"
#include "net.h"
#include <Windows.h>
#include <time.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#endif

#define IRC_PORT 6667
// #define IRC_HOST "irc.freenode.org"
#define IRC_HOST "ix2.undernet.org"
#define IRC_IP "170.178.191.18"
#define IRC_IP_SIZE (sizeof(IRC_IP)/sizeof(IRC_IP[0]))
#define IRC_CHANNEL "#changemehardmode"

#define IRC_MAX_NICKNAME_SIZE 12
#define IRC_MAX_USER_SIZE 9
#define IRC_MAX_MSG_SIZE 512

// This are for getting the computer username
#define INFO_BUFFER_SIZE 32767
#ifndef __linux__
#define LOGIN_NAME_MAX 9
#endif

#ifdef _WIN32
void generate_nickname(CHAR *nick);
#elif defined(__linux__) || defined(__APPLE__)
void generate_nickname(char *nick);
#endif

#ifdef _WIN32
void get_username(CHAR *username);
#elif defined(__linux__) || defined(__APPLE__)
void get_username(char *username);
#endif

#ifdef _WIN32
void generate_username(CHAR *user);
#elif defined(__linux__) || defined(__APPLE__)
void generate_username(char *user);
#endif

#ifdef _WIN32
void set_nick(int fd, CHAR *nick);
#elif defined(__linux__) || defined(__APPLE__)
void set_nick(int fd, char *nick);
#endif

#ifdef _WIN32
void set_user(int fd, CHAR *nick);
#elif defined(__linux__) || defined(__APPLE__)
void set_user(int fd, char *nick);
#endif

#ifdef _WIN32
void set_message(int fd, CHAR *message, CHAR *target);
#elif defined(__linux__) || defined(__APPLE__)
void set_message(int fd, char *message, char *target);
#endif

void join_channel(int fd);

#ifdef _WIN32
void pong(int fd, CHAR *data);
#elif defined(__linux__) || defined(__APPLE__)
void pong(int fd, char *data);
#endif

#ifdef _WIN32
void ping(int fd, CHAR *data);
#elif defined(__linux__) || defined(__APPLE__)
void ping(int fd, char *data);
#endif

#ifdef _WIN32
void command_handler_proxy(int fd, CHAR *nickname, CHAR *username, CHAR *target, CHAR *message);
#elif defined(__linux__) || defined(__APPLE__)
void command_handler_proxy(int fd, char *nickname, char *username, char *target, char *message);
#endif

#ifdef _WIN32
void line_handler_proxy(int fd, CHAR *line, size_t line_size);
#elif defined(__linux__) || defined(__APPLE__)
void line_handler_proxy(int fd, char *line, size_t line_size);
#endif

#endif
