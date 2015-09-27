#include "OregonReceiver.h"
#include "OregonDecoder.h"

const int PreambleMin = 400;
const int PreambleMax = 540;

const int SyncMin = 880;
const int SyncMax = 1020;


const int MaxReadingsCount = 65;



OregonReceiver::OregonReceiver(int pinId)
{
	ResetVariables();
	pinMode(pinId, INPUT);
	_pinId = pinId;
}

String OregonReceiver::Receive()
{
	ResetVariables();
	ReceiveSignal();
	return _receivedResult;
}

void OregonReceiver::ResetVariables()
{
	_counter = 0;
	_timing = 0;
}

void OregonReceiver::ReceiveSignal()
{
	WaitForPreamble();
	bool hasSync = ReadSync();

	if (!hasSync)
	{
		_receivedResult = "ERROR NO SYNC";
		return;
	}

	ReadValues();

	_receivedResult = OregonDecoder().DecodeValues(_readValues, _counter);
}

void OregonReceiver::WaitForPreamble()
{
	_timing = 0;
	int preambleCount = 0;

	while (preambleCount < 23)
	{
		_timing = pulseIn(_pinId, LOW, 1000000);

		if (_timing > PreambleMin && _timing < PreambleMax)
		{
			preambleCount++;
		}
		else
		{
			preambleCount = 0;
		}
	}
}

bool OregonReceiver::ReadSync()
{
	unsigned long _timing1 = 0;
	unsigned long _timing2 = 0;

	_timing1 = pulseIn(_pinId, LOW, 1000000);
	_timing2 = pulseIn(_pinId, LOW, 1000000);

	return (_timing1 > SyncMin && _timing1 < SyncMax) &&
		(_timing2 > SyncMin && _timing2 < SyncMax);
}

void OregonReceiver::ReadValues()
{
	unsigned long low = 0;
	unsigned long lastLow = micros();
	unsigned long high = 0;

	while (low < 10000)
	{
		low = pulseIn(_pinId, LOW, 1000000);

		high = micros() - (lastLow + low);
		lastLow = micros();

		_readValues[_counter] = high > 800;
		_readValues[_counter + 1] = low > 800;

		_counter += 2;
	}
}