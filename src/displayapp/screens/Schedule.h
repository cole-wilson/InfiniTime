#pragma once

#include "displayapp/screens/Screen.h"
#include "systemtask/SystemTask.h"
#include <lvgl/lvgl.h>
#include "components/battery/BatteryController.h"
#include "components/datetime/DateTimeController.h"
#include "components/motor/MotorController.h"
#include "components/brightness/BrightnessController.h"
#include "components/timer/TimerController.h"
#include <components/heartrate/HeartRateController.h>
#include "components/motion/MotionController.h"
#include "components/fs/FS.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Schedule : public Screen {
      public:
        Schedule(DisplayApp* app, Controllers::DateTime& dateTimeController);
        ~Schedule() override;
		void Refresh() override;

		lv_task_t* taskRefresh;
		lv_obj_t* page;
			lv_obj_t* classtime;
			lv_obj_t* timeleft;
			lv_obj_t* classes;
			lv_obj_t* rooms;
		lv_obj_t* mapimg;

		Controllers::DateTime& dateTimeController;
      };
    }
  }
}
