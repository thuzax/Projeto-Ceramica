#include "globals.h"

#include <cstring>


int os_code;
char* str_endline;

void declare_globals() {
    #if defined(__WIN32) || defined(_WIN64)
        os_code = WINDOWS;
        str_endline = new char[2];
        strcpy(str_endline, "\r\n");
    #endif

    #ifdef __unix__
        os_code = UNIX;
        str_endline = new char[2];
        strcpy(str_endline, "\n");
    #endif

}

const char* string_endline() {
    return str_endline;
}

int idOS() {
    return os_code;
}