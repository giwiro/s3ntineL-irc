// s3ntineL-irc.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "pch.h"
#include <iostream>
#include "net.h"
#include "logger.h"

int main()
{
    int fd;
    print_banner();
    create_socketfd(&fd);
    connect_socket(&fd);
    start_reading_loop(&fd);
}
