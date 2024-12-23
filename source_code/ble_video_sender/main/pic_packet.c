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

#include<string.h>
#include "pic_packet.h"
#include "esp_log.h"

#define PIC_PACK_TAG "pic packet"

/*
 * param *pic_packet: packet struct need to be initilized
 * return the init status
 */
esp_err_t    pic_packet_init(Pic_Packet *pic_packet, uint8_t *buffer, uint16_t length)
{
	if(!pic_packet || !buffer)
		return ESP_ERR_INVALID_ARG;
	if(length <= PACK_HEAD_LEN)
		return ESP_ERR_INVALID_ARG;

	pic_packet->header = PACKET_HEADER;
	pic_packet->frame_cnt = 0;
	pic_packet->all_pack_num = 0;
	pic_packet->cur_pack_num = 0;
	pic_packet->length = length - PACK_HEAD_LEN; 
	pic_packet->databuf = buffer;

	return ESP_OK;
}

/*
 * param *input: picture data get from camera
 * param *output: data packet will be sent via bluetooth
 * param outputlen: output data length
 * return the picture data length which is used by this function 
 */
uint16_t pic_packet_data(Pic_Packet *pic_packet, uint8_t *input, uint16_t inputlen)
{
	uint16_t length = 0;
	if(!pic_packet || !input || !inputlen) 
		return ESP_ERR_INVALID_ARG;

	if(pic_packet->header != PACKET_HEADER)
		return ESP_ERR_INVALID_STATE;

	pic_packet->databuf[0] = pic_packet->header;
	pic_packet->databuf[1] = pic_packet->frame_cnt;
	pic_packet->databuf[2] = pic_packet->all_pack_num;
	pic_packet->databuf[3] = pic_packet->cur_pack_num;

	length = (pic_packet->length < inputlen ? pic_packet->length : inputlen);
	memcpy(pic_packet->databuf + PACK_HEAD_LEN, input, length);
	pic_packet->length = length;

	pic_packet->databuf[4] = pic_packet->length >> 8;
	pic_packet->databuf[5] = pic_packet->length & 0xff;

	return pic_packet->length;
}
