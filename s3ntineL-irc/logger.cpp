#include "pch.h"
#include "logger.h"
#ifdef _WIN32
#pragma once
#endif

#ifdef _WIN32
void log_message(const CHAR* fmt, ...)
#elif defined(__linux) || defined(__APPLE__)
void log_message(const char* fmt, ...)
#endif
{
#ifdef _DEBUG
    // fflush(stdout);
#ifdef _WIN32
    static CHAR buffer[MAX_SIZE_LOGGER_BUFFER];
#elif defined(__linux__) || defined(__APPLE__)
    char buffer[MAX_SIZE_LOGGER_BUFFER];
#endif
    va_list args;
    va_start(args, fmt);
#ifdef _WIN32
    // Function to write the formatted string into another buffer
    vsprintf_s(buffer, MAX_SIZE_LOGGER_BUFFER, fmt, args);
    // static TCHAR bufferPrint[MAX_SIZE_LOGGER_BUFFER];
    // Windows based output log (Into the Output tab in Visual Studio)
    // OutputDebugString(buffer);
    printf("%s", buffer);
#elif defined(__linux__) || defined(__APPLE__)
    int n = vsnprintf(buffer, MAX_SIZE_LOGGER_BUFFER, fmt, args);
    if (n < 0 || n > MAX_SIZE_LOGGER_BUFFER) {
        printf("There was a problem logging...");
        return;
    }
    printf("%s", buffer);
#endif
    // Facilitate a normal return when va_start is used
    va_end(args);
    fflush(stdout);
#endif // DEBUG
}

void print_banner()
{
#ifdef _DEBUG
    printf("%s\n", "                      _                        ");
    printf("%s\n", "                      \\`*-.                    ");
    printf("%s\n", "                       )  _`-.                 ");
    printf("%s\n", "                      .  : `. .                ");
    printf("%s\n", "                      : _   '  \\               ");
    printf("%s\n", "                      ; *` _.   `*-._          ");
    printf("%s\n", "                      `-.-'          `-.       ");
    printf("%s\n", "        0wn3d           ;       `       `.     ");
    printf("%s\n", "                        :.       .        \\    ");
    printf("%s\n", "                        . \\  .   :   .-'   .   ");
    printf("%s\n", "                        '  `+.;  ;  '      :   ");
    printf("%s\n", "                        :  '  |    ;       ;-. ");
    printf("%s\n", "                        ; '   : :`-:     _.`* ;");
    printf("%s\n", "                     .*' /  .*' ; .*`- +'  `*' ");
    printf("%s\n", "                     `*-*   `*-*  `*-*'        ");
    /*
        printf("%s\n", "    _                ___       _.--.");
        printf("%s\n", "    \\`.|\\..----...-'`   `-._.-'_.-'`");
        printf("%s\n", "    /  ' `         ,       __.--'");
        printf("%s\n", "    )/' _/     \\   `-_,   /        ");
        printf("%s\n", "    `-'\" `\"\\_  ,_.-;_.-\\_ ',     ");
        printf("%s\n", "        _.-'_./   {_.'   ; /");
        printf("%s\n", "       {_.-``-'         {_/");
    */
#endif // DEBUG
}
