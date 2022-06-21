#pragma once

#include "displayapp/screens/Screen.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Timeline : public Screen {
      public:
        Timeline(DisplayApp* app, Controllers::MotorController& motorController, Controllers::DateTime& dateTimeController);
        ~Timeline() override;
      };
    }
  }
}
