#include "displayapp/screens/ApplicationList.h"
#include <lvgl/lvgl.h>
#include <functional>
#include "displayapp/Apps.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

constexpr std::array<Tile::Applications, ApplicationList::applications.size()> ApplicationList::applications;

auto ApplicationList::CreateScreenList() const {
  std::array<std::function<std::unique_ptr<Screen>()>, nScreens> screens;
  for (size_t i = 0; i < screens.size(); i++) {
    screens[i] = [this, i]() -> std::unique_ptr<Screen> {
      return CreateScreen(i);
    };
  }
  return screens;
}

ApplicationList::ApplicationList(Pinetime::Applications::DisplayApp* app,
                                 Pinetime::Controllers::Settings& settingsController,
                                 Pinetime::Controllers::Battery& batteryController,
                                 Controllers::DateTime& dateTimeController)
  : Screen(app),
    settingsController {settingsController},
    batteryController {batteryController},
    dateTimeController {dateTimeController},
    screens {app, settingsController.GetAppMenu(), CreateScreenList(), Screens::ScreenListModes::UpDown} {
}

ApplicationList::~ApplicationList() {
  lv_obj_clean(lv_scr_act());
}

bool ApplicationList::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

<<<<<<< HEAD
std::unique_ptr<Screen> ApplicationList::CreateScreen(unsigned int screenNum) const {
  std::array<Tile::Applications, appsPerScreen> apps;
  for (int i = 0; i < appsPerScreen; i++) {
    apps[i] = applications[screenNum * appsPerScreen + i];
  }
=======
std::unique_ptr<Screen> ApplicationList::CreateScreen1() {
  std::array<Screens::Tile::Applications, 6> applications {{
    {Symbols::stopWatch, Apps::StopWatch},
    {Symbols::music, Apps::Music},
    {Symbols::map, Apps::Navigation},
    {Symbols::shoe, Apps::Steps},
    {Symbols::heartBeat, Apps::HeartRate},
    {Symbols::hourGlass, Apps::Timer},
  }};

  return std::make_unique<Screens::Tile>(0, 2, app, settingsController, batteryController, dateTimeController, applications);
}

std::unique_ptr<Screen> ApplicationList::CreateScreen2() {
  std::array<Screens::Tile::Applications, 6> applications {{
    {Symbols::paintbrush, Apps::Paint},
    {Symbols::paddle, Apps::Paddle},
    {"2", Apps::Twos},
    {Symbols::calculator, Apps::Calculator},
    {Symbols::drum, Apps::Metronome},
    {Symbols::clock, Apps::Alarm},
  }};
>>>>>>> 6ff0f94385ffe28e3abce43e335d0f793f4358c9

  return std::make_unique<Screens::Tile>(screenNum, nScreens, app, settingsController, batteryController, dateTimeController, apps);
}
