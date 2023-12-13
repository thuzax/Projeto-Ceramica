#ifndef GLOBALS_H_
#define GLOBALS_H_

enum {WINDOWS, UNIX};

void declareGlobals();
void destroyGlobals();
const char* stringEndline();

int idOS();

#endif