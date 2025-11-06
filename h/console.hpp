#ifndef _CONSOLE_HPP
#define _CONSOLE_HPP

void consoleInit();

void consoleHandler();

int   consolePUTC(char c);
char  consoleGETC();
void  outputWorkerBody(void*);

#endif  
