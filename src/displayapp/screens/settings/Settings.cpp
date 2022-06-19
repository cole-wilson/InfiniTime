#include "displayapp/screens/settings/Settings.h"
#include <lvgl/lvgl.h>
#include <functional>
#include "displayapp/Apps.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

constexpr std::array<List::Applications, Settings::entries.size()> Settings::entries;

auto Settings::CreateScreenList() const {
  std::array<std::function<std::unique_ptr<Screen>()>, nScreens> screens;
  for (size_t i = 0; i < screens.size(); i++) {
    screens[i] = [this, i]() -> std::unique_ptr<Screen> {
      return CreateScreen(i);
    };
  }
  return screens;
}

Settings::Settings(Pinetime::Applications::DisplayApp* app, Pinetime::Controllers::Settings& settingsController)
  : Screen(app),
    settingsController {settingsController},
    screens {app, settingsController.GetSettingsMenu(), CreateScreenList(), Screens::ScreenListModes::UpDown} {
}

Settings::~Settings() {
  lv_obj_clean(lv_scr_act());
}

bool Settings::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

<<<<<<< HEAD
std::unique_ptr<Screen> Settings::CreateScreen(unsigned int screenNum) const {
  std::array<List::Applications, entriesPerScreen> screens;
  for (int i = 0; i < entriesPerScreen; i++) {
    screens[i] = entries[screenNum * entriesPerScreen + i];
  }
=======
std::unique_ptr<Screen> Settings::CreateScreen1() {
  std::array<Screens::List::Applications, 4> applications {{
    {Symbols::sun, "Display", Apps::SettingDisplay},
    {Symbols::eye, "Wake Up", Apps::SettingWakeUp},
    {Symbols::clock, "Time format", Apps::SettingTimeFormat},
    {Symbols::home, "Watch face", Apps::SettingWatchFace},
  }};

  return std::make_unique<Screens::List>(0, 4, app, settingsController, applications);
}

std::unique_ptr<Screen> Settings::CreateScreen2() {
  std::array<Screens::List::Applications, 4> applications {{
    {Symbols::shoe, "Steps", Apps::SettingSteps},
    {Symbols::clock, "Set date", Apps::SettingSetDate},
    {Symbols::clock, "Set time", Apps::SettingSetTime},
    {Symbols::batteryHalf, "Battery", Apps::BatteryInfo}}};

  return std::make_unique<Screens::List>(1, 4, app, settingsController, applications);
}

std::unique_ptr<Screen> Settings::CreateScreen3() {

  std::array<Screens::List::Applications, 4> applications {{
    {Symbols::clock, "Chimes", Apps::SettingChimes},
    {Symbols::tachometer, "Shake Calib.", Apps::SettingShakeThreshold},
    {Symbols::check, "Firmware", Apps::FirmwareValidation},
    {Symbols::list, "Airplane mode", Apps::SettingAirplaneMode}
  }};

  return std::make_unique<Screens::List>(2, 4, app, settingsController, applications);
}

std::unique_ptr<Screen> Settings::CreateScreen4() {

  std::array<Screens::List::Applications, 4> applications {{
    {Symbols::chartLine, "Motion", Apps::Motion},
    {Symbols::list, "About", Apps::SysInfo},
    {Symbols::none, "None", Apps::None},
    {Symbols::none, "None", Apps::None}
  }};
>>>>>>> 6ff0f94385ffe28e3abce43e335d0f793f4358c9

  return std::make_unique<Screens::List>(screenNum, nScreens, app, settingsController, screens);
}
