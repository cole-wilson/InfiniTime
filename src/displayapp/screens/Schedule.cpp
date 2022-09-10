#include "displayapp/screens/Schedule.h"
#include "displayapp/DisplayApp.h"
#include <lvgl/lvgl.h>
#include <vector>
#include <numeric>

#define P1 "Orchestra"
#define P2 "AP Calculus"
#define P3 "Deutsch 2"
#define LUNCH "1st Lunch"
#define P4 "AP US History"
#define P5 "AP Literature"
#define P6 "AP Physics"

using namespace Pinetime::Applications::Screens;

Schedule::Schedule(DisplayApp* app, Controllers::DateTime& dateTimeController): Screen(app), dateTimeController {dateTimeController} {
	taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);

	page = lv_page_create(lv_scr_act(), NULL);
	lv_obj_set_size(page, 240, 240); // fill screen
	lv_obj_align(page, NULL, LV_ALIGN_CENTER, 0, 0);

	int offset = 0;


	classtime = lv_label_create(page, NULL);
	lv_obj_set_style_local_text_font(classtime, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42);
	lv_label_set_text(classtime, "1:35-2:25");
	lv_label_set_align(classtime, LV_LABEL_ALIGN_CENTER);
	lv_obj_set_width(classtime, lv_page_get_width_fit(page));
	lv_label_set_long_mode(classtime, LV_LABEL_LONG_SCROLL_CIRC);
	offset += 45;


	timeleft = lv_label_create(page, NULL);
	lv_label_set_text(timeleft, "#808080" + "2 MINUTES LEFT" + "#");
	lv_obj_set_width(timeleft, lv_page_get_width_fit(page));
	lv_label_set_long_mode(timeleft, LV_LABEL_LONG_SCROLL_CIRC);
	lv_obj_align(timeleft, NULL, LV_ALIGN_IN_TOP_MID, 0, offset);
	lv_label_set_recolor(timeleft, true);
	offset += 21;

	classes = lv_label_create(page, NULL);
	lv_label_set_text(classes,
			"#808080 " + P1 + "#" + std::string(10-std::strlen(P1)-3, '-') + "603" + "#\n"
			"#808080 " + P2 + "#" + std::string(10-std::strlen(P2)-3, '-') + "603" + "#\n"
			"#808080 " + P3 + "#" + std::string(10-std::strlen(P3)-3, '-') + "603" + "#\n"
			"#808080 " + P4 + "#" + std::string(10-std::strlen(P4)-3, '-') + "603" + "#\n"
			"#00FFE7 " + P5 + "#" + std::string(10-std::strlen(P5)-3, '-') + "603" + "#\n"
			"#FFFFFF " + P6 + "#" + std::string(10-std::strlen(P6)-3, '-') + "603" + "#\n"
	);
	lv_label_set_align(classes, LV_LABEL_ALIGN_LEFT);
	lv_obj_set_width(classes, lv_page_get_width_fit(page));
	lv_label_set_long_mode(classes, LV_LABEL_LONG_SCROLL_CIRC);
	lv_obj_align(classes, NULL, LV_ALIGN_IN_TOP_MID, 0, offset);
	lv_label_set_recolor(classes, true);
}

void Schedule::Refresh() {

}
void Schedule::OnButtonEvent(lv_obj_t* obj, lv_event_t event) {
}

Schedule::~Schedule() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}
