#include "displayapp/screens/Timeline.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

Timeline::Timeline(DisplayApp* app, Controllers::MotorController& motorController, Controllers::DateTime& dateTimeController) : Screen(app) {
     lv_obj_t * list1 = lv_list_create(lv_scr_act(), nullptr);
     lv_obj_set_size(list1, 160, 200);
     lv_obj_align(list1, nullptr, LV_ALIGN_CENTER, 0, 0);

     lv_obj_t * list_btn;

     for (int i=0;i<2;i++)
       list_btn = lv_list_add_btn(list1, nullptr, "event!");
      /* lv_obj_set_event_cb(list_btn, event_handler); */

}

Timeline::~Timeline() { // destroy
  lv_obj_clean(lv_scr_act());
}
