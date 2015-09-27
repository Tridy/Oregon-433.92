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
	void ReverseValues();
	void BuildResult();
	String GetHexValue(byte index);
};