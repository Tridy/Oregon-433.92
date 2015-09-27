#include "OregonDecoder.h"

byte _reversedValues[22];
byte _counter;
byte _decodedValues[22];
String _result;

OregonDecoder::OregonDecoder()
{
	
}

String OregonDecoder::DecodeValues(bool readValues[], byte count)
{
	_counter = count;

	bool doubleShortAsOne = true;

	byte c = 1;
	byte d = 0;

	while (c < _counter)
	{
		if (readValues[c])
		{
			doubleShortAsOne = !doubleShortAsOne;
			_decodedValues[d] = doubleShortAsOne;
			c++;
		}
		else
		{
			_decodedValues[d] = doubleShortAsOne;
			c += 2;
		}

		d++;
	}

	ReverseValues();
	BuildResult();

	return _result;
}

void  OregonDecoder::ReverseValues()
{
	_reversedValues[0] = 15;

	int i;

	for (i = 1; i < 20; i++)
	{
		byte val = 0;

		int j;

		for (j = 3; j >= 0; j = j - 1)
		{
			bool bit = _decodedValues[(i * 4) + j];
			val = val ^ bit;

			if (j > 0)
			{
				val = val << 1;
			}
		}

		_reversedValues[i] = val;
	}
}

void OregonDecoder::BuildResult()
{
	int cs;
	byte checkSumResult = 0;

	for (cs = 0; cs < 15; cs++)
	{
		checkSumResult += _reversedValues[cs];
	}

	String sensorId = GetHexValue(0) + GetHexValue(1) + GetHexValue(2) + GetHexValue(3);
	String channel = GetHexValue(4);
	String rollingCode = GetHexValue(5) + GetHexValue(6);
	String lowBattery = _reversedValues[7] < 2 ? "Yes" : "No";
	String temperature = (_reversedValues[11] == 0 ? "" : "-") + GetHexValue(10) + GetHexValue(9) + "." + GetHexValue(8);
	String humidity = GetHexValue(13) + GetHexValue(12);
	String checksum = GetHexValue(16) + GetHexValue(15);

	_result = String(
	"Sensor Id: " + sensorId +
	"\nChannel: " + channel +
	"\nRolling Code: " + rollingCode +
	"\nLow Battery: " + lowBattery +
	"\nTemperature: " + temperature +
	"\nHumidity: " + humidity +
	"\nChecksum: " + checksum);
}

String OregonDecoder::GetHexValue(byte index)
{
	return String(_reversedValues[index], HEX);
}