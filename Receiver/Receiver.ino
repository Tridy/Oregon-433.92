#include "OregonDecoder.h"
#include "OregonReceiver.h"
const int PinId = 8;
bool _receivedCode[65];


void setup()
{

}
void loop()
{
	String _signal = OregonReceiver(PinId).Receive();
	Serial.println(_signal);
	delay(1000);
}