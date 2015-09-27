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
	String Receive();

private:

	unsigned long _timing;
	bool _readValues[200];
	String _receivedResult;
	int _pinId;
	unsigned long _extractedValues;
	byte _counter;

	void ResetVariables();
	void ReceiveSignal();
	void WaitForPreamble();
	bool ReadSync();
	void ReadValues();
};