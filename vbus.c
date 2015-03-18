// vbus.c
//
// (c) Hewell Technology Ltd. 2014
//
//****************************************************************************

#include "vbus.h"

void VBus_ExtractSeptett(unsigned char *Buffer, int Offset, int Length) {
	unsigned char Septett;
	int i;
	Septett = 0;
	for (i = 0; i < Length; i++) {
		if (Buffer [Offset + i] & 0x80) {
			Buffer [Offset + i] &= 0x7F;
			Septett |= (1 << i);
		}
	}
	Buffer [Offset + Length] = Septett;
}

void VBus_InjectSeptett(unsigned char *Buffer, int Offset, int Length) {
	unsigned char Septett;
	int i;
	Septett = Buffer [Offset + Length];
	for (i = 0; i < Length; i++) {
		if (Septett & (1 << i)) {
			Buffer [Offset + i] |= 0x80;
		}
	}
}
