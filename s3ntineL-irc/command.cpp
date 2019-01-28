#include "pch.h"
#include "command.h"
#include "logger.h"
#include "irc.h"
#ifdef _WIN32
#include <VersionHelpers.h>
#endif

#ifdef _WIN32
void handle_hello(int fd, CHAR *target) {
	CHAR username[INFO_BUFFER_SIZE];
	CHAR hello_content[IRC_MAX_MSG_SIZE];
#elif defined(__linux__) || defined(__APPLE__)
void handle_hello(int fd, char *target) {
	char username[LOGIN_NAME_MAX];
	char hello_content[IRC_MAX_MSG_SIZE];
#endif
	get_username(username);
#ifdef _WIN32
	snprintf(hello_content, IRC_MAX_MSG_SIZE, "[%s] Hello World!\r\n", username);
#elif defined(__linux__) || defined(__APPLE__)
	snprintf(hello_content, IRC_MAX_MSG_SIZE, "[%s] Hello World!\r\n", username);
#endif
	set_message(fd, hello_content, target);
}

#ifdef _WIN32
void handle_os(int fd, CHAR *target) {
	CHAR os_content[OS_INFO_BUFFER_SIZE];
#elif defined(__linux__) || defined(__APPLE__)
void handle_os(int fd, char *target) {
	char os_content[OS_INFO_BUFFER_SIZE];
#endif

#ifdef _WIN32
	if (IsWindows10OrGreater()) {
		strncpy_s(os_content, "WINDOWS_10+", OS_INFO_BUFFER_SIZE);
	}
	else if (IsWindows8Point1OrGreater()) {
		strncpy_s(os_content, "WINDOWS_8.1+", OS_INFO_BUFFER_SIZE);
	}
	else if (IsWindows8OrGreater()) {
		strncpy_s(os_content, "WINDOWS_8+", OS_INFO_BUFFER_SIZE);
	}
	else if (IsWindows7SP1OrGreater()) {
		strncpy_s(os_content, "WINDOWS_7SP1+", OS_INFO_BUFFER_SIZE);
	}
	else if (IsWindowsServer()) {
		strncpy_s(os_content, "WINDOWS_SERVER", OS_INFO_BUFFER_SIZE);
	}
	else {
		strncpy_s(os_content, "WINDOWS", OS_INFO_BUFFER_SIZE);
	}
#elif defined(__linux__)
	FILE *cmd = popen("cat /etc/os-release | grep -m1 \"NAME\" | sed -r 's/NAME=//g'", "r");
	while (fgets(os_content, OS_INFO_BUFFER_SIZE, cmd) != NULL) {}
	pclose(cmd);
#endif
	set_message(fd, os_content, target);
}

#ifdef _WIN32


#endif

#ifdef _WIN32
void handle_cpu(int fd, CHAR *target) {
	// CHAR cpu_content[CPU_INFO_BUFFER_SIZE];
#elif defined(__linux__) || defined(__APPLE__)
void handle_cpu(int fd, char *target) {
	char cpu_content[CPU_INFO_BUFFER_SIZE] = { 0x0 };
#endif

#ifdef _WIN32
	
#elif defined(__linux__)
	FILE *cmd = popen("cat /proc/cpuinfo | grep \"model name\" | uniq | sed -r 's/[mM]odel name\\s{0,}:\\s{0,}//g'", "r");
	while (fgets(cpu_content, CPU_INFO_BUFFER_SIZE, cmd) != NULL) {}
	pclose(cmd);
	set_message(fd, cpu_content, target);
#elif defined(__APPLE__)
	size_t bufferlen = CPU_INFO_BUFFER_SIZE;
	sysctlbyname("machdep.cpu.brand_string", &cpu_content, &bufferlen, NULL, 0);
	set_message(fd, cpu_content, target);
#endif

}
