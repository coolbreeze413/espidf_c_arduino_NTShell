#ifndef _NTSHELL_ARDUINO_H
#define _NTSHELL_ARDUINO_H

typedef int (*USER_CALLBACK)(const char *text);

void poll_shell();
void initialize_shell(USER_CALLBACK cb, const char* prompt, HardwareSerial* serial);
void set_prompt(const char* prompt);

#endif
