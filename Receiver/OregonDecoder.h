#ifndef _OREGONDECODER_h
#define _OREGONDECODER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class OregonDecoder
{
public:
	OregonDecoder();
	String DecodeValues(bool readValues[], byte count);

private:
	byte _counter;
	String _result;
	byte _reversedValues[22];
	byte _decodedValues[84];

	void ReverseValues();
	void BuildResult();
	String GetHexValue(byte index);
};