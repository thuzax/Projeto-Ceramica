#ifndef GLOBALS_H_
#define GLOBALS_H_

enum {WINDOWS, UNIX};

void declare_globals();
void destroy_globals();
const char* string_endline();

int idOS();

#endif