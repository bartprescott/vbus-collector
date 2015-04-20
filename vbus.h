/*****************************************************************************
 *
 * Copyright Hewell Technology Ltd. 2014,2015
 *
 * This file is part of vbus-collector.
 *
 * vbus-collector is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * vbus-collector is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vbus-collector.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

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
