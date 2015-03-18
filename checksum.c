//****************************************************************************
// checksum.c
//
// (c) Hewell Technology Ltd. 2014
//
//****************************************************************************


#include "checksum.h"

unsigned char VBus_CalcCrc(const unsigned char *Buffer, int Offset, int Length) {
	unsigned char Crc;
	int i;
	Crc = 0x7F;
	for (i = 0; i < Length; i++) {
		Crc = (Crc - Buffer [Offset + i]) & 0x7F;
	}
	return Crc;
}