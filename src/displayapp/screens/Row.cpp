#include "displayapp/screens/Row.h"
#include "displayapp/DisplayApp.h"
#include <lvgl/lvgl.h>

using namespace Pinetime::Applications::Screens;

Row::Row(DisplayApp* app,
				Controllers::Battery& batteryController,
				Controllers::DateTime& dateTimeController,
				Controllers::MotorController& motorController,
				Controllers::BrightnessController& brightnessController,
				Controllers::TimerController& timerController,
				Controllers::MotionController& motionController,
				Controllers::FS& fs)
	: Screen(app),
    batteryController {batteryController},
    dateTimeController {dateTimeController},
	motorController {motorController},
	brightnessController {brightnessController},
	timerController {timerController},
	motionController {motionController},
	fs {fs}
{
	//system.PushMessage(Pinetime::System::Messages::DisableSleeping);

	static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_border_width(&style, LV_STATE_DEFAULT, 2);
	lv_style_set_text_color(&style, LV_STATE_DEFAULT, lv_color_hex(0x000000));
	lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x5C7162));

	/* lv_obj_t* timeofday = lv_label_create(lv_scr_act(), NULL); */
    /* lv_obj_add_style(timeofday, LV_OBJ_PART_MAIN, &style); */
	/* lv_label_set_text(timeofday, "12:34 PM"); */
	/* lv_label_set_align(timeofday, LV_LABEL_ALIGN_CENTER); */
	/* lv_obj_align(timeofday, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10); */

	lv_obj_t* strokerate = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(strokerate, LV_OBJ_PART_MAIN, &style);
	lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
	lv_label_set_text(strokerate, "28");
	lv_label_set_align(strokerate, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(strokerate, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 30);

	lv_obj_t* timer = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(timer, LV_OBJ_PART_MAIN, &style);
	lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
	lv_label_set_text(timer, "06:27");
	lv_label_set_align(timer, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(timer, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -5);

	lv_obj_t* strokecount = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(strokecount, LV_OBJ_PART_MAIN, &style);
	lv_label_set_text(strokecount, "220");
	lv_label_set_align(strokecount, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(strokecount, NULL, LV_ALIGN_IN_RIGHT_MID, -10, -10);

	/* lv_obj_t* bg_clock_img = lv_img_create(lv_scr_act(), NULL); */
	/* lv_img_set_src(bg_clock_img, &bg_clock); */
	/* lv_obj_align(bg_clock_img, NULL, LV_ALIGN_CENTER, 0, 0); */
}

Row::~Row() {
  lv_obj_clean(lv_scr_act());
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  //system.PushMessage(Pinetime::System::Messages::EnableSleeping);
}
/* Row::Refresh() { */
/* } */
