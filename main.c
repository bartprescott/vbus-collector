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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "kbhit.h"
#include "serial.h"
#include "checksum.h"
#include "vbus.h"

char serial_buffer[256];

#pragma pack(1)

union {

    struct {
        short TempSensor1;
        short TempSensor2;
        short TempSensor3;
        short TempSensor4;
        unsigned char PumpSpeed1;
        unsigned char PumpSpeed2;
        unsigned char RelayMask;
        unsigned char ErrorMask;
        unsigned short SystemTime;
        unsigned char Scheme;
        unsigned char OptionCollectorMax : 1;
        unsigned char OptionCollectorMin : 1;
        unsigned char OptionCollectorFrost : 1;
        unsigned char OptionTubeCollector : 1;
        unsigned char OptionRecooling : 1;
        unsigned char OptionHQM : 1;
        unsigned char rfu : 2;
        unsigned short OperatingHoursRelay1;
        unsigned short OperatingHoursRelay2;
        unsigned short HeatQuantityWH;
        unsigned short HeatQuantityKWH;
        unsigned short HeatQuantityMWH;
        unsigned short Version;
    } bsPlusPkt;
    unsigned char asBytes[28];
} BS_Plus_Data_Packet;

int main(int argc, char *argv[]) {

    PVBUS_V1_CMD pPacket = (PVBUS_V1_CMD) & serial_buffer[0];
    unsigned char i = 0, j, k;
    int headerSync = 0;
    int crcOK = 0;
    int loopforever = 0;
    int packet_displayed = 0;

    if (!serial_open_port(argv[1])) {
        printf("Errno(%d) opening %s: %s\n", errno, argv[1], strerror(errno));
        return 2;
    }

    if (argc > 2) {
        loopforever = 1;
    }

    //printf("Collecting data\n");

    if (!serial_set_baud_rate(9600)) {
        printf("Failed to set baud rate: %s\n", serial_get_error());
        return 3;
    }

    do {
        if (caughtSigQuit()) {
            break;
        }
        int count = serial_read(&(serial_buffer[i]), 1); //sizeof(serial_buffer));
        if (count < 1) {
            continue;
        }
        if (serial_buffer[i] == 0xaa) {
            serial_buffer[0] = serial_buffer[i];
            i = 0;
            headerSync = 1;
            //printf("\n\n");
        }
        //printf("%02x ", serial_buffer[i]);
        i++;
        //if (i % 16 == 0) {
            //printf("\n");
        //}

        if (headerSync) {
            if (i > sizeof (VBUS_HEADER)) {
                //we have nearly all the header
                if ((pPacket->h.ver & 0xF0) != 0x10) {
                    headerSync = 0;
                    continue;
                }
                if (i < sizeof (VBUS_V1_CMD)) {
                    continue;
                }
                if (i < ((pPacket->frameCnt * sizeof (FRAME_STRUCT)) + sizeof (VBUS_V1_CMD))) {
                    continue;
                }
                headerSync = 0;
                //We have a whole packet..
                unsigned char crc = VBus_CalcCrc((void*) serial_buffer, 1, 8);
                //printf("\nPacket size: %d. Source: 0x%04x, Destination: 0x%04x, Command: 0x%04x, No of frames: %d, crc: 0x%02x(0x%02x)\n",
                //	i, pPacket->h.source, pPacket->h.dest, pPacket->cmd, pPacket->frameCnt, pPacket->crc, crc);

                if (pPacket->crc != crc) {
                    //printf("CRC Error!\n");
                    continue;
                }

                if (pPacket->cmd != 0x0100 || pPacket->h.dest != 0x10) {
                    //Not sure what this packet is
                    continue;
                }

                //Packet is from DeltaSol BS Plus
                //This is the packet we've been waiting for! Lets decode it....
                for (j = 0; j < pPacket->frameCnt; j++) {
                    crc = VBus_CalcCrc((void*) &pPacket->frame[j], 0, 5);
                    //printf("Bytes: 0x%08lx, Septett: 0x%02x, crc: 0x%02x(0x%02x)\n",
                    //	*(unsigned long *)pPacket->frame[j].bytes, pPacket->frame[j].septett, pPacket->frame[j].crc, crc);

                    crcOK = (pPacket->frame[j].crc == crc);
                    if (!crcOK) {
                        //printf("CRC Error!\n");
                        //crcOK = 0;
                        break;
                    }
                    VBus_InjectSeptett((void *) &(pPacket->frame[j]), 0, 4);
                    for (k = 0; k < 4; k++) {
                        BS_Plus_Data_Packet.asBytes[(j * 4) + k] = pPacket->frame[j].bytes[k];
                    }
                }

                if (!crcOK) {
                    continue;
                }

                //printf("%d\n", sizeof(BS_Plus_Data_Packet));

                printf("System time:%02d:%02d"
                        ", Sensor1 temp:%.1fC"
                        ", Sensor2 temp:%.1fC"
                        ", Sensor3 temp:%.1fC"
                        ", Sensor4 temp:%.1fC"
                        ", Pump speed1:%d%%"
                        ", Pump speed2:%d%%"
                        //", RelayMask:%d"
                        //", ErrorMask:%d"
                        //", Scheme:%d, %d, %d, %d, %d, %d, %d"
                        ", Hours1:%d, Hours2:%d"
                        //", %dWH, %dkWH, %dMWH"
                        //", Version:%.2f"
                        "\n",
                        BS_Plus_Data_Packet.bsPlusPkt.SystemTime / 60,
                        BS_Plus_Data_Packet.bsPlusPkt.SystemTime % 60,
                        BS_Plus_Data_Packet.bsPlusPkt.TempSensor1 * 0.1,
                        BS_Plus_Data_Packet.bsPlusPkt.TempSensor2 * 0.1,
                        BS_Plus_Data_Packet.bsPlusPkt.TempSensor3 * 0.1,
                        BS_Plus_Data_Packet.bsPlusPkt.TempSensor4 * 0.1,
                        BS_Plus_Data_Packet.bsPlusPkt.PumpSpeed1,
                        BS_Plus_Data_Packet.bsPlusPkt.PumpSpeed2,
                        //BS_Plus_Data_Packet.bsPlusPkt.RelayMask,
                        //BS_Plus_Data_Packet.bsPlusPkt.ErrorMask,
                        //BS_Plus_Data_Packet.bsPlusPkt.Scheme,
                        //BS_Plus_Data_Packet.bsPlusPkt.OptionCollectorMax,
                        //BS_Plus_Data_Packet.bsPlusPkt.OptionCollectorMin,
                        //BS_Plus_Data_Packet.bsPlusPkt.OptionCollectorFrost,
                        //BS_Plus_Data_Packet.bsPlusPkt.OptionTubeCollector,
                        //BS_Plus_Data_Packet.bsPlusPkt.OptionRecooling,
                        //BS_Plus_Data_Packet.bsPlusPkt.OptionHQM,
                        BS_Plus_Data_Packet.bsPlusPkt.OperatingHoursRelay1,
                        BS_Plus_Data_Packet.bsPlusPkt.OperatingHoursRelay2
                        //BS_Plus_Data_Packet.bsPlusPkt.HeatQuantityWH,
                        //BS_Plus_Data_Packet.bsPlusPkt.HeatQuantityKWH,
                        //BS_Plus_Data_Packet.bsPlusPkt.HeatQuantityMWH
                        //BS_Plus_Data_Packet.bsPlusPkt.Version * 0.01
                        );
                packet_displayed++;
                continue;
            }
        }

    } while (loopforever > 0 || packet_displayed == 0);
    serial_close_port();
    return 0;
}
