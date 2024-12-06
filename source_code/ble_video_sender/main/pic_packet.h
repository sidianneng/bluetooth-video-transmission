/*
MIT License

Copyright (c) 2024 sidianneng(bbear_mail@163.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef PIC_PACKET
#define PIC_PACKET

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PACKET_HEADER 0x55
#define PACK_HEAD_LEN 6

typedef struct PIC_PACKET {
	uint8_t header;
	uint8_t frame_cnt;
	uint8_t all_pack_num;
	uint8_t cur_pack_num;
	uint16_t length;//data length of databuf
	uint8_t *databuf;
} Pic_Packet;

esp_err_t pic_packet_init(Pic_Packet *pic_packet, uint8_t *buffer, uint16_t length);
uint16_t pic_packet_data(Pic_Packet *pic_packet, uint8_t *input, uint16_t inputlen);

#ifdef __cplusplus
}
#endif

#endif
