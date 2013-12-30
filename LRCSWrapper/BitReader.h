#pragma once
#include "Constants.h"
#include "CodingException.h"

class BitReader
{
public:
	BitReader(byte* buffer_, int numBitsInBuffer_);
	virtual ~BitReader();

	// Read an arbitrary number of bits into an int
	bool readBits(unsigned int& value, int numBits_);

	// Starts reading the buffer from the start
	void resetPos();

	// Start reading from a specified bit postion
	bool skipToBitPos(int bitPos_);

	// Get current bit position
	int getCurrentBitPosition();

	// Get max bit
	int getNumBitsInBuffer();

	void setBuffer(byte* buffer_, int numBitsInBuffer_);
protected:
	void initReader();
	byte* buffer;
	int numBitsInBuffer;
	unsigned int* currentInt;
	byte currentBit;
	int numBitsRead;
};

