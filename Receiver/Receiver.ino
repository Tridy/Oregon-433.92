#include "OregonReceiver.h"
const int PinId = 8;
bool _receivedCode[65];


void setup()
{

}
void loop()
{
	OregonReceiver(PinId).Receive(_receivedCode);
	Serial.println("GOT IT!");

	delay(1000);
}