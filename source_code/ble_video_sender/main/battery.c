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

#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#include "battery.h"

#define BAT_TAG "battery"

static int adc_raw;
static int voltage;

static adc_oneshot_unit_handle_t adc2_handle;
static adc_cali_handle_t adc2_cali_handle = NULL;
static bool do_calibration2;

static bool example_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(BAT_TAG, "calibration scheme version is %s", "Curve Fitting");
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .chan = channel,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(BAT_TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    if (ret == ESP_OK) {
        ESP_LOGI(BAT_TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
        ESP_LOGW(BAT_TAG, "eFuse not burnt, skip software calibration");
    } else {
        ESP_LOGE(BAT_TAG, "Invalid arg or no memory");
    }

    return calibrated;
}

static void example_adc_calibration_deinit(adc_cali_handle_t handle)
{
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    ESP_LOGI(BAT_TAG, "deregister %s calibration scheme", "Curve Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_curve_fitting(handle));

#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    ESP_LOGI(BAT_TAG, "deregister %s calibration scheme", "Line Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(handle));
#endif
}

/*
 * param: void
 * return the init status
 */
esp_err_t battery_init(void)
{
    //-------------ADC Init---------------//
    adc_oneshot_unit_init_cfg_t init_config2 = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config2, &adc2_handle));

    //-------------ADC Calibration Init---------------//
    do_calibration2 = example_adc_calibration_init(ADC_UNIT_2, BATTERY_ADC_CHAN, BATTERY_ADC_ATTEN, &adc2_cali_handle);

    //-------------ADC Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .atten = BATTERY_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, BATTERY_ADC_CHAN, &config));

    return ESP_OK;
}

/*
 * param: void
 * return the battery status
 */
Battery_stat battery_get_stat(void)
{
    Battery_stat battery_stat = BAT_LOW;

    if (!do_calibration2) {
        ESP_LOGI(BAT_TAG, "Battery ADC not init yet, please init it first!!!");
        return battery_stat;
    }

    ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, BATTERY_ADC_CHAN, &adc_raw));
    ESP_LOGI(BAT_TAG, "battery adc Raw Data: %d", adc_raw);

    if (do_calibration2) {
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc2_cali_handle, adc_raw, &voltage));
        //ESP_LOGI(BAT_TAG, "battery Cali Voltage: %d mV", voltage);
    }

    if(voltage <= 2358) //3.8V
        battery_stat = BAT_LOW;
    else if(voltage < 2451) //3.95V
        battery_stat = BAT_MID;
    else
        battery_stat = BAT_FULL;

    return battery_stat;
}

/*
 * param: void
 * return the deinit status
 */
esp_err_t battery_deinit(void)
{
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc2_handle));

    if (do_calibration2) {
        example_adc_calibration_deinit(adc2_cali_handle);
    }

    return ESP_OK;
}

