#include "ntshell.h"
#include "ntopt.h"
#include "ntlibc.h"

#include "Arduino.h"
#include "ntshell_arduino.h"


USER_CALLBACK user_cb = NULL;
HardwareSerial* serialPtr = NULL;

// NOTE:
// int usrcmd_execute(const char *text)
// needs to be defined somewhere else, conventionally usrcmd_arduino.cpp
// look for this in the project sources.

#define INITCODE    (0x4367)
#define SERIAL_READ(HANDLE, BUF, CNT)   ((HANDLE)->func_read(BUF, CNT, (HANDLE)->extobj))

static ntshell_t ntshell;

static int func_read(char *buf, int cnt, void *extobj)
{
	if (serialPtr->available())
	{
		return serialPtr->readBytes(buf, cnt);
	}
	else
	{
		return 0;
	}
}


static int func_write(const char *buf, int cnt, void *extobj)
{
	return serialPtr->write((const uint8_t*)buf, cnt);
}


static int func_callback(const char *text, void *extobj)
{
#if 0 // local testing.
	ntshell_t *ntshell = (ntshell_t *)extobj;
	//UNUSED_VARIABLE(ntshell);
	//UNUSED_VARIABLE(extobj);
	if (ntlibc_strlen(text) > 0) {
		Serial.print("User input text:");
		Serial.print(text);
		Serial.print("\r\n");
	}
	return 0;
#else
	return user_cb(text);
#endif
}


void initialize_shell(USER_CALLBACK cb, const char* prompt, HardwareSerial* serial)
{
    // set the user callback function
    user_cb = cb;

    // set the serial port instance (HWSerial only) TODO: make this stream oriented !
    serialPtr = serial;

    // init shell with read/write/cb functions
	ntshell_init(
			&ntshell,
			func_read,
			func_write,
			func_callback,
			(void *)(&ntshell));

    // set prompt if passed in
    if(prompt != NULL)
    {
	    ntshell_set_prompt(&ntshell, prompt);
    }
}


void set_prompt(const char* prompt)
{
    ntshell_set_prompt(&ntshell, prompt);
}


void poll_shell()
{
	while(serialPtr->available())
	{
		if (ntshell.initcode != INITCODE) {
			return;
		}

		unsigned char ch;
		SERIAL_READ(&ntshell, (char *)&ch, sizeof(ch));
		vtrecv_execute(&(ntshell.vtrecv), &ch, sizeof(ch));
	}
}

