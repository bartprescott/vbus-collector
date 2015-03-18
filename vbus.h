// vbus.h
//
// (c) Hewell Technology Ltd. 2014
//
//****************************************************************************


#ifndef _VBUS_H
#define _VBUS_H

#pragma pack(1)

typedef struct _VBUS_HEADER {
	unsigned char syncByte;
	unsigned short dest;
	unsigned short source;
	unsigned char ver;
} VBUS_HEADER, *PVBUS_HEADER;

typedef struct _FRAME_STRUCT {
	unsigned char bytes[4];
	unsigned char septett;
	unsigned char crc;
} FRAME_STRUCT, *PFRAME_STRUCT;

typedef struct _VBUS_V1_CMD {
	VBUS_HEADER h;
	unsigned short cmd;
	unsigned char frameCnt;
	unsigned char crc;
	FRAME_STRUCT frame[];
} VBUS_V1_CMD, *PVBUS_V1_CMD;


extern void VBus_ExtractSeptett(unsigned char *Buffer, int Offset, int Length);
extern void VBus_InjectSeptett(unsigned char *Buffer, int Offset, int Length);

#endif //_VBUS_H
