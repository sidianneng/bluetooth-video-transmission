/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#pragma once
#include <stdint.h>
#include "esp_err.h"

#define IMAGE_W 240
#define IMAGE_H 240

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculate the effect for a bunch of lines.
 *
 * @param dest Destination for the pixels. Assumed to be LINECT * 320 16-bit pixel values.
 * @param line Starting line of the chunk of lines.
 * @param frame Current frame, used for animation
 * @param linect Amount of lines to calculate
 */
void decode_calc_lines(uint16_t *dest, int line, int frame, int linect);

/**
 * @brief Initialize the effect
 *
 * @return ESP_OK on success, an error from the jpeg decoder otherwise.
 */
esp_err_t decode_image(uint8_t *jpeg_data, uint32_t jpeg_len);

#ifdef __cplusplus
}
#endif
