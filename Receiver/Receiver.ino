#include "OregonReceiver.h"
const int PinId = 8;
bool _receivedCode[65];


void setup()
{

}
void loop()
{
	// ResetResult();
	
	OregonReceiver(PinId).Receive(_receivedCode);
	
	// String decodedValue = OregonReceiver().Decode(_receivedCode);
	// Serial.println(decodedValue);
	// OutputResult();

	Serial.println("GOT IT!");

	delay(1000);
}

//void ResetResult()
//{
//	for (size_t i = 0; i < 65; i++)
//	{
//		_receivedCode[i] = 0;
//	}
//}

//void OutputResult()
//{
//	Serial.println("===");
//
//	for (size_t i = 0; i < 65; i++)
//	{
//		Serial.print(_receivedCode[i]);
//		
//	}
//
//	Serial.println("===");
//}