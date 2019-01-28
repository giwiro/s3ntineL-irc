#include "pch.h"
#include "net.h"
#include "irc.h"
#include "logger.h"

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
#ifdef _WIN32
		sockin.sin_port = htons(IRC_PORT);
		InetPton(AF_INET, _T(IRC_IP), &sockin.sin_addr.s_addr);
#else
		log_message("gonna copy ip: %s\n", IRC_IP);
		// TODO copy ip address to sockein.sin_addr
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
