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
	void ResetVariables();
	void ReceiveSignal();
	void WaitForPreamble();
	bool ReadSync();
	void ReadValues();
	
};