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
