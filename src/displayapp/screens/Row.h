#pragma once

#include "displayapp/screens/Screen.h"
#include "systemtask/SystemTask.h"
#include <lvgl/lvgl.h>
#include "components/battery/BatteryController.h"
#include "components/datetime/DateTimeController.h"
#include "components/motor/MotorController.h"
#include "components/brightness/BrightnessController.h"
#include "components/timer/TimerController.h"
#include "components/motion/MotionController.h"
#include "components/fs/FS.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Row : public Screen {
      public:
        Row(DisplayApp* app,
				Controllers::Battery& batteryController,
				Controllers::DateTime& dateTimeController,
				Controllers::MotorController& motorController,
				Controllers::BrightnessController& brightnessController,
				Controllers::TimerController& timerController,
				Controllers::MotionController& motionController,
				Controllers::FS& fs,
				System::SystemTask* systemTask);
        ~Row() override;
		void Refresh() override;
		void OnButtonEvent(lv_obj_t* obj, lv_event_t event);
		bool OnButtonPushed() override;
		bool OnButtonDoubleClicked() override;
		bool OnButtonLongPressed() override;
		bool OnButtonLongerPressed() override;

		lv_task_t* taskRefresh;

		lv_obj_t* timeofday;
		lv_obj_t* timer;
		lv_obj_t* strokerate;
		lv_obj_t* strokecount;
		
		int motiondatalen = 3;
		int motiondata[motiondatalen];

		Controllers::Battery& batteryController;
		Controllers::DateTime& dateTimeController;
		Controllers::MotorController& motorController;
		Controllers::BrightnessController& brightnessController;
		Controllers::TimerController& timerController;
		Controllers::MotionController& motionController;
		Controllers::FS& fs;
		System::SystemTask* systemTask;
      };
    }
  }
}

