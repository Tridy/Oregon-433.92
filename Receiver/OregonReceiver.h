// OregonReceiver.h

#ifndef _OREGONRECEIVER_h
#define _OREGONRECEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class OregonReceiver
{

public:
	OregonReceiver(int pinId);
	void Receive(bool receivedData []);

private:
	int _pinId;
	bool* _readValues;
	unsigned long _extractedValues;
	int _counter;
	unsigned long _timing;

	void ResetVariables();
	void ReceiveSignal();
	void WaitForPreamble();
	bool ReadSync();
	void ReadValues();
	String GetHexValue(byte byteValue);
};