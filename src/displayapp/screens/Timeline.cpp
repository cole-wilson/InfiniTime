#include "displayapp/screens/Timeline.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

Timeline::Timeline(DisplayApp* app, Controllers::MotorController& motorController, Controllers::DateTime& dateTimeController) : Screen(app) {
  lv_obj_t* title = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(title, "My test application");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
}

Timeline::~Timeline() { // destroy
  lv_obj_clean(lv_scr_act());
}
