#pragma once
//	  currentScreen = std::make_unique<Screens::Row>(this, batteryController, dateTimeController, motorController, brightnessController, timerController, motionController, fs);
/*
 *DisplayApp* app,
                        Controllers::DateTime& dateTimeController,
                        Controllers::Battery& batteryController,
                        Controllers::Ble& bleController,
                        Controllers::NotificationManager& notificationManager,
                        Controllers::Settings& settingsController,
						Controllers::TimerController& timerController
 * */

#include "displayapp/screens/Screen.h"
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
				Controllers::BatteryController& batteryController,
				Controllers::DateTimeController& dateTimeController,
				Controllers::MotorController& motorController,
				Controllers::BrightnessController& brightnessController,
				Controllers::TimerController& timerController,
				Controllers::MotionController& motionController);
        ~Row() override;
      };
    }
  }
}

