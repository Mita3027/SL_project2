#include <Arduino.h>
#include <lvgl.h>
#include "PanelLan.h"
#include "ui.h"



void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void Display_Init();
void Display_Timer();

