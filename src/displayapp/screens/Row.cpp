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
				Controllers::FS& fs,System::SystemTask* systemTask)
	: Screen(app),
    batteryController {batteryController},
    dateTimeController {dateTimeController},
	motorController {motorController},
	brightnessController {brightnessController},
	timerController {timerController},
	motionController {motionController},
	fs {fs},
	systemTask {systemTask}
{
	/* systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping); */

    timerController.StartTimer((1 + 13 * 60) * 1000);

	static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_border_width(&style, LV_STATE_DEFAULT, 4);
	lv_style_set_text_color(&style, LV_STATE_DEFAULT, lv_color_hex(0x000000));
	lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x5C7162));

	/* timeofday = lv_label_create(lv_scr_act(), NULL); */
    /* lv_obj_add_style(timeofday, LV_OBJ_PART_MAIN, &style); */
	/* lv_label_set_text(timeofday, "12:34 PM"); */
	/* lv_label_set_align(timeofday, LV_LABEL_ALIGN_CENTER); */
	/* lv_obj_align(timeofday, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10); */

	strokerate = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(strokerate, LV_OBJ_PART_MAIN, &style);
	lv_obj_set_style_local_text_font(strokerate, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
	lv_label_set_text(strokerate, "28");
	lv_label_set_align(strokerate, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(strokerate, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 30);

	timer = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(timer, LV_OBJ_PART_MAIN, &style);
	lv_obj_set_style_local_text_font(timer, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
	lv_label_set_text(timer, "06:27");
	lv_label_set_align(timer, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(timer, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 4);

	strokecount = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(strokecount, LV_OBJ_PART_MAIN, &style);
	lv_label_set_text(strokecount, "220");
	lv_label_set_align(strokecount, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(strokecount, NULL, LV_ALIGN_IN_LEFT_MID, -10, -10);

	/* lv_obj_t* bg_clock_img = lv_img_create(lv_scr_act(), NULL); */
	/* lv_img_set_src(bg_clock_img, &bg_clock); */
	/* lv_obj_align(bg_clock_img, NULL, LV_ALIGN_CENTER, 0, 0); */

	taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);

}

void Row::Refresh() {
	if (timerController.IsRunning()) {
		uint32_t seconds = timerController.GetTimeRemaining() / 1000;
		lv_label_set_text_fmt(timer, "%.2d:%.2d", seconds/60, seconds%60);
	}
	int x = motionController.X() / 0x10;
	int z = motionController.Z() / 0x10;
	if (z > -60 && x < 0)
		x += (10*(-60 - z));
	else if (z > -60 && x > 0)
		x -= (10*(-60 - z));
	if (abs(x) > 50)
		motorController.RunForDuration(9);
	/* for (int i=1;i<3;i++) { */
	/* 	motiondata[i-1] = motiondata[i]; */
	/* } */
	/* motiondata[3-1] = motionController.X(); */
	/* if (abs(motiondata[1]) > 20) { */
	lv_label_set_text_fmt(strokecount, "%d, %d, %d", x, z, 10*(-60-z));
	/* 	if (motiondata[1] > motiondata[0] && motiondata[1] > motiondata[2]) */
	/* 		motorController.RunForDuration(30); */
	/* 	if (motiondata[1] < motiondata[0] && motiondata[1] < motiondata[2]) */
	/* 		motorController.RunForDuration(30); */
	/* } */
}
void Row::OnButtonEvent(lv_obj_t* obj, lv_event_t event) {
  /* if (event == LV_EVENT_CLICKED) { */
    /* if (obj == btnPlayPause) { */
	/* } */
  /* } */
}
bool Row::OnButtonPushed() {return true;}
bool Row::OnButtonDoubleClicked() {return true;}
bool Row::OnButtonLongPressed() {return true;}
bool Row::OnButtonLongerPressed() {return true;}

Row::~Row() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  /* systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping); */
}
