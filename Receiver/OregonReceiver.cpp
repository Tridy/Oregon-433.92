#include "OregonReceiver.h"

const int PreambleMin = 400;
const int PreambleMax = 540;

const int SyncMin = 880;
const int SyncMax = 1020;

unsigned long _timing = 0;

const int MaxReadingsCount = 65;


bool readValues[200];
byte decodedValues[200];
byte counter = 0;
byte invertedValues[20];

OregonReceiver::OregonReceiver(int pinId)
{
	pinMode(pinId, INPUT);
	_pinId = pinId;
}

void OregonReceiver::Receive(bool receivedData [])
{
	_readValues = receivedData;
	ResetVariables();
	ReceiveSignal();
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
	Serial.println("Has Sync = " + String(hasSync));
	if (hasSync)
	{
		ReadValues();
		DecodeValues();
		ReverseValues();
		BuildResult();
	}
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


	Serial.println("");

	while (low < 10000)
	{
		Serial.print(".");
		low = pulseIn(_pinId, LOW, 1000000);
		
		high = micros() - (lastLow + low);
		lastLow = micros();

		readValues[counter] = high > 800;
		readValues[counter + 1] = low > 800;
		
		counter += 2;
	}



}

void  OregonReceiver::DecodeValues()
{
	bool doubleShortAsOne = true;

	byte c = 1;
	byte d = 0;

	while (c < counter)
	{
		if (readValues[c])
		{
			doubleShortAsOne = !doubleShortAsOne;
			decodedValues[d] = doubleShortAsOne;
			c++;
		}
		else
		{
			decodedValues[d] = doubleShortAsOne;
			c += 2;
		}

		d++;
	}
}

void  OregonReceiver::ReverseValues()
{
	invertedValues[0] = 15;

	int i;

	for (i = 1; i < 20; i++)
	{
		byte val = 0;

		int j;

		for (j = 3; j >= 0; j = j - 1)
		{
			bool bit = decodedValues[(i * 4) + j];
			val = val ^ bit;

			if (j > 0)
			{
				val = val << 1;
			}
		}

		invertedValues[i] = val;
	}
}

void  OregonReceiver::BuildResult()
{
	int cs;
	byte checkSumResult = 0;
	for (cs = 0; cs < 15; cs++)
	{
		checkSumResult += invertedValues[cs];
	}

	Serial.println("CHK Result = " + String(checkSumResult, HEX));

	Serial.println("");

	String sensorId = GetHexValue(invertedValues[0]) + GetHexValue(invertedValues[1]) + GetHexValue(invertedValues[2]) + GetHexValue(invertedValues[3]);
	String channel = GetHexValue(invertedValues[4]);
	String rollingCode = GetHexValue(invertedValues[5]) + GetHexValue(invertedValues[6]);
	String lowBattery = invertedValues[7] ? "Yes" : "No";
	String temperature = (invertedValues[11] == 0 ? "" : "-") + GetHexValue(invertedValues[10]) + GetHexValue(invertedValues[9]) + "." + GetHexValue(invertedValues[8]);
	String humidity = GetHexValue(invertedValues[13]) + GetHexValue(invertedValues[12]);
	String checksum = GetHexValue(invertedValues[16]) + GetHexValue(invertedValues[15]);

	Serial.println("Sensor Id: " + sensorId);
	Serial.println("Channel: " + channel);
	Serial.println("Rolling Code: " + rollingCode);
	Serial.println("Low Battery: " + lowBattery);
	Serial.println("Temperature: " + temperature);
	Serial.println("Humidity: " + humidity);
	Serial.println("Checksum: " + checksum);


	Serial.println("");
	Serial.println("#################################");
}

String OregonReceiver::GetHexValue(byte byteValue)
{
	return String(byteValue, HEX);
}