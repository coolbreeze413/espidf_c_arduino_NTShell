#include <Arduino.h>

//NTSHELL
#include "usrcmd_arduino.h"


void setup() 
{
    Serial.begin(115200);
    init_shell();
}

void loop() 
{
    poll_shell();
}