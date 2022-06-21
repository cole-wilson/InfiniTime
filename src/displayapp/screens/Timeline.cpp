#include "displayapp/screens/Timeline.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

Timeline::Timeline(DisplayApp* app, Controllers::MotorController& motorController, Controllers::DateTime& dateTimeController, Controllers::FS& fsController) : Screen(app) {
}

Timeline::~Timeline() { // destroy
	lv_obj_clean(lv_scr_act());
}
