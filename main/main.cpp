/**********************************************************************************
 * LIBRARIES
 **********************************************************************************/
#include <stdio.h>
#include <TFT_eSPI.h>
#include <EEPROM.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "driver/adc.h"
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <esp_http_server.h>
#include <freertos/task.h>
#include "C:\Users\SMRi\esp\v4.4.6\esp-idf\components\app_update\include\esp_ota_ops.h"
#include <esp_system.h>
#include <sys/param.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include"lvgl.h"
#include "ui/ui.h"

#define CANVAS_WIDTH 320
#define CANVAS_HEIGHT 240
LV_IMG_DECLARE(hydroflyerBg);
LV_IMG_DECLARE(startupLogo);
#define LCD_PWM 25


static const uint16_t screenWidth = CANVAS_WIDTH;
static const uint16_t screenHeight = CANVAS_HEIGHT;
/*Change to your screen resolution*/

lv_disp_draw_buf_t draw_buf;
lv_color_t buf[screenWidth * 10];
EXT_RAM_ATTR static uint8_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
lv_disp_drv_t disp_drv;
lv_obj_t *canvas;
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */


void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

#define DEBUG_MODE

/**********************************************************************************
 * HARDWARE PIN DECLEARTION
 **********************************************************************************/

/**********************************************************************************
 * MACROS OR CONST VARIABLES
 **********************************************************************************/
#define LCD_ROTATION 1

void ble_task_func(void *pvParameters)
{

    for (;;)
    {

    vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}

void display_task_func(void *pvParameters)
{
    lv_init();
    tft.begin(); /* TFT init */
    tft.setRotation(LCD_ROTATION);
    tft.invertDisplay(true);
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);
    /*Initialize the display*/
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    /*Create a buffer for the canvas*/
   
    ui_init();
    lv_timer_handler();


    for (;;)
    {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(40));
    }
}



/**********************************************************************************
 * FSM(FINITE STATE MACHINE) FUNCTIONS
 ***********************************************************************************/

/**********************************************************************************
 * MAIN TASK FUNCTIONS
 ***********************************************************************************/
void main_task_func(void *pvParameters)
{
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}




#define TAG "MAIN"
extern "C"
{
    void app_main();
}

void app_main(void)
{

    vTaskDelay(pdMS_TO_TICKS(100));
    /* Initialize NVS */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
#ifdef DEBUG_MODE
    Serial.begin(115200);
#endif



    // xTaskCreatePinnedToCore(
    //     ble_task_func,
    //     "BluetoothTask",
    //     4096,
    //     &pRmt,
    //     1,
    //     &pRmt.ble.BLETask, 1);



    xTaskCreatePinnedToCore(
        display_task_func,
        "DisplayTask",
        3072,
        NULL,
        2,
        NULL, 0);
    
    //    delay(10); // Wait till display is Initialized completely
    // xTaskCreatePinnedToCore(
    //     main_task_func,
    //     "MainTask",
    //     4096,
    //     &pRmt,
    //     1,
    //     &pRmt.MainTask, 0);
}
