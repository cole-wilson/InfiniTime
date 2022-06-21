#include "displayapp/screens/Timeline.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

Timeline::Timeline(DisplayApp* app, Controllers::MotorController& motorController, Controllers::DateTime& dateTimeController, Controllers::FS& fsController) : Screen(app) {
  lv_obj_t* title = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(title, "+"+ReadFile()+"+");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
}
char* Timeline::ReadFile() {
	char* buffer;
	lfs_file_t calFile;
	fs.FileOpen(&calFile, calFileName, LFS_O_RDONLY | LFS_O_CREAT);
	fs.FileRead(&calFile, reinterpret_cast<char*>(&buffer), 100);
	fs.FileClose(&calFile);
	return buffer;
}

Timeline::~Timeline() { // destroy
  lv_obj_clean(lv_scr_act());
}
