#include "ntshell.h"
#include "ntopt.h"
#include "ntlibc.h"

#include "Arduino.h"
#include "ntshell_arduino.h"
#include "usrcmd_arduino.h"

// NOTE:
// int usrcmd_execute(const char *text)
// needs to be defined somewhere else, conventionally usrcmd_arduino.cpp
// look for this in the project sources.

#define INITCODE    (0x4367)
#define SERIAL_READ(HANDLE, BUF, CNT)   ((HANDLE)->func_read(BUF, CNT, (HANDLE)->extobj))

static ntshell_t ntshell;

static int func_read(char *buf, int cnt, void *extobj)
{
	if (Serial.available())
	{
		return Serial.readBytes(buf, cnt);
	}
	else
	{
		return 0;
	}
}


static int func_write(const char *buf, int cnt, void *extobj)
{
	return Serial.write((const uint8_t*)buf, cnt);
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
	return usrcmd_execute(text);
#endif
}



void initialize_shell()
{
	ntshell_init(
			&ntshell,
			func_read,
			func_write,
			func_callback,
			(void *)(&ntshell));

	ntshell_set_prompt(&ntshell, SKELETOR_NTSHELL_PROMPT);
	Serial.println(F("welcome to s.k.e.l.e.t.o.r \r\n type 'help' for help."));
	Serial.print(SKELETOR_NTSHELL_PROMPT);
	Serial.flush();
}


void poll_shell()
{
	while(Serial.available())
	{
		if (ntshell.initcode != INITCODE) {
			return;
		}

		unsigned char ch;
		SERIAL_READ(&ntshell, (char *)&ch, sizeof(ch));
		vtrecv_execute(&(ntshell.vtrecv), &ch, sizeof(ch));
	}
}

