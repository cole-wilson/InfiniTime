#include "displayapp/screens/WatchFaceAnalog.h"
#include <cmath>
#include <lvgl/lvgl.h>
#include "displayapp/screens/BatteryIcon.h"
#include "displayapp/screens/BleIcon.h"
#include "displayapp/screens/Symbols.h"
#include "displayapp/screens/NotificationIcon.h"
#include "components/settings/Settings.h"

LV_IMG_DECLARE(bg_clock);

using namespace Pinetime::Applications::Screens;

namespace {
  constexpr int16_t HourLength = 70;
  constexpr int16_t MinuteLength = 90;
  constexpr int16_t SecondLength = 110;

  // sin(90) = 1 so the value of _lv_trigo_sin(90) is the scaling factor
  const auto LV_TRIG_SCALE = _lv_trigo_sin(90);

  int16_t Cosine(int16_t angle) {
    return _lv_trigo_sin(angle + 90);
  }

  int16_t Sine(int16_t angle) {
    return _lv_trigo_sin(angle);
  }

  int16_t CoordinateXRelocate(int16_t x) {
    return (x + LV_HOR_RES / 2);
  }

  int16_t CoordinateYRelocate(int16_t y) {
    return std::abs(y - LV_HOR_RES / 2);
  }

  lv_point_t CoordinateRelocate(int16_t radius, int16_t angle) {
    return lv_point_t {.x = CoordinateXRelocate(radius * static_cast<int32_t>(Sine(angle)) / LV_TRIG_SCALE),
                       .y = CoordinateYRelocate(radius * static_cast<int32_t>(Cosine(angle)) / LV_TRIG_SCALE)};
  }

}

void moon(lv_obj_t* mooncover) {
  float daysfromnewmoon = (std::chrono::system_clock::now().time_since_epoch().count()/1000000/86400) - (947211240/86400);
  /* float part = std::fmod(daysfromnewmoon / 29.53, 1.0); */
  float part = 14.0;

  if (part >= 0 && part < 1) {
	  // new moon
	  lv_obj_set_size(mooncover, 43, 43);
  }
  else if (part >= 1 && part < 6.38264692644001) {
	//waxing crescent
	lv_obj_set_size(mooncover, 32, 43);
  }
  else if (part >= 6.38264692644001 && part < 8.38264692644) {
	  //first quarter
	  lv_obj_set_size(mooncover, 21, 43);

	}
  else if (part >= 8.38264692644 && part < 13.76529385288) {
	  // waxing gibbous
	  lv_obj_set_size(mooncover, 10, 43);
  }
  else if (part >= 13.76529385288 && part < 15.76529385288) {
	  // full: do nothing
  }
  else if (part >= 15.76529385288 && part < 21.14794077932) {
	  // waning gibbous
	  lv_obj_set_size(mooncover, 10, 43);
  }
  else if (part >= 21.14794077932 && part < 23.14794077932) {
	  // last quarter
	  lv_obj_set_size(mooncover, 21, 43);
  }
  else if (part >= 23.14794077932 && part < 28.53058770576) {
	  // waning crescent
	  lv_obj_set_size(mooncover, 32, 43);
  }
  else {
	  // new moon
	  lv_obj_set_size(mooncover, 43, 43);
  }
}

WatchFaceAnalog::WatchFaceAnalog(Pinetime::Applications::DisplayApp* app,
                                 Controllers::DateTime& dateTimeController,
                                 Controllers::Battery& batteryController,
                                 Controllers::Ble& bleController,
                                 Controllers::NotificationManager& notificationManager,
                                 Controllers::Settings& settingsController,
								 Controllers::TimerController& timerController)
  : Screen(app),
    currentDateTime {{}},
    dateTimeController {dateTimeController},
    batteryController {batteryController},
    bleController {bleController},
    notificationManager {notificationManager},
    settingsController {settingsController},
	timerController {timerController} {

  sHour = 99;
  sMinute = 99;
  sSecond = 99;

  /* lv_obj_t* bg_clock_img = lv_img_create(lv_scr_act(), NULL); */
  /* lv_img_set_src(bg_clock_img, &bg_clock); */
  /* lv_obj_align(bg_clock_img, NULL, LV_ALIGN_CENTER, 0, 0); */

  mooncover = lv_bar_create(lv_scr_act(), NULL);
  lv_obj_set_size(mooncover, 43, 43);
  lv_obj_set_style_local_bg_opa(mooncover, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
  lv_obj_set_style_local_bg_color(mooncover, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_obj_align(mooncover, NULL, LV_ALIGN_IN_TOP_LEFT, 149, 63);
  moon(mooncover);


  batteryIcon.Create(lv_scr_act());
  lv_obj_align(batteryIcon.GetObject(), nullptr, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

  plugIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(plugIcon, Symbols::plug);
  lv_obj_set_style_local_text_color(plugIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_obj_align(plugIcon, nullptr, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(notificationIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));
  lv_label_set_text_static(notificationIcon, NotificationIcon::GetIcon(false));
  lv_obj_align(notificationIcon, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

  // steps
  /* steps = lv_arc_create(lv_scr_act(), NULL); */
  /* lv_obj_set_style_local_line_color(steps, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, lv_color_hex(0x264d8c)); */
  /* lv_obj_set_style_local_line_width(steps, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, 7); */
  /* lv_obj_set_style_local_line_color(steps, LV_OBJ_PART_MAIN,  LV_STATE_DEFAULT, lv_color_hex(0x000000)); */
  /* lv_obj_set_style_local_line_width(steps, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 7); */
  /* lv_obj_set_size(steps, 205, 205); */

  /* lv_arc_set_range(steps, 0, 360); */
  /* lv_arc_set_bg_angles(steps, 0, 360); */
  /* lv_arc_set_angles(steps, 0, 360); */
  /* lv_arc_set_rotation(steps, 270); */
  /* lv_obj_align(steps, NULL, LV_ALIGN_CENTER, 0, 0); */
  /* lv_arc_set_value(steps, 360); */  

  // timer
  /* timer = lv_arc_create(lv_scr_act(), NULL); */
  /* lv_obj_set_style_local_line_color(timer, LV_OBJ_PART_MAIN,  LV_STATE_DEFAULT, lv_color_hex(0x000000)); */
  /* lv_obj_set_style_local_line_width(timer, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, 7); */
  /* lv_obj_set_style_local_line_color(timer, LV_ARC_PART_INDIC,  LV_STATE_DEFAULT, lv_color_hex(0xff9d00)); */
  /* lv_obj_set_style_local_line_width(timer, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 7); */
  /* lv_obj_set_size(timer, 190, 190); */

  /* lv_arc_set_range(timer, 0, 360); */
  /* lv_arc_set_bg_angles(timer, 0, 360); */
  /* lv_arc_set_angles(timer, 0, 360); */
  /* lv_arc_set_rotation(timer, 270); */
  /* lv_obj_align(timer, NULL, LV_ALIGN_CENTER, 0, 0); */
  /* lv_arc_set_value(timer, 360); */

  /* timer_label = lv_label_create(lv_scr_act(), NULL); */
  /* lv_label_set_text(timer_label, ""); */
  /* lv_label_set_align(timer_label, LV_LABEL_ALIGN_CENTER); */
  /* lv_obj_align(timer_label, NULL, LV_ALIGN_IN_TOP_LEFT, 7, 7); */


  /* // heart */
  /* heart_label = lv_label_create(lv_scr_act(), NULL); */
  /* lv_label_set_text(heart_label, "134"); */
  /* lv_label_set_align(heart_label, LV_LABEL_ALIGN_LEFT); */
  /* lv_obj_align(heart_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 7, -7); */

  /* // stroke rate */
  /* stroke_label = lv_label_create(lv_scr_act(), NULL); */
  /* lv_label_set_text(stroke_label, "28"); */
  /* lv_label_set_align(stroke_label, LV_LABEL_ALIGN_RIGHT); */
  /* lv_obj_align(stroke_label, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -7, -7); */

  /* // info label */
  /* info_label = lv_label_create(lv_scr_act(), NULL); */
  /* lv_label_set_recolor(info_label, true); */
  /* lv_label_set_text(info_label, "#665577 PineTime#"); */
  /* lv_label_set_align(info_label, LV_LABEL_ALIGN_CENTER); */
  /* lv_obj_align(info_label, NULL, LV_ALIGN_CENTER, 0, 50); */



  // Date - Day / Week day

  label_date_day = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(label_date_day, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xff, 0xb0, 0x0));
  lv_label_set_text_fmt(label_date_day, "%s\n%02i", dateTimeController.DayOfWeekShortToString(), dateTimeController.Day());
  lv_label_set_align(label_date_day, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(label_date_day, NULL, LV_ALIGN_CENTER, 0, -40);

  minute_body = lv_line_create(lv_scr_act(), NULL);
  minute_body_trace = lv_line_create(lv_scr_act(), NULL);
  hour_body = lv_line_create(lv_scr_act(), NULL);
  hour_body_trace = lv_line_create(lv_scr_act(), NULL);
  second_body = lv_line_create(lv_scr_act(), NULL);

  lv_style_init(&second_line_style);
  lv_style_set_line_width(&second_line_style, LV_STATE_DEFAULT, 3);
  lv_style_set_line_color(&second_line_style, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_style_set_line_rounded(&second_line_style, LV_STATE_DEFAULT, true);
  lv_obj_add_style(second_body, LV_LINE_PART_MAIN, &second_line_style);

  lv_style_init(&minute_line_style);
  lv_style_set_line_width(&minute_line_style, LV_STATE_DEFAULT, 7);
  lv_style_set_line_color(&minute_line_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_line_rounded(&minute_line_style, LV_STATE_DEFAULT, true);
  lv_obj_add_style(minute_body, LV_LINE_PART_MAIN, &minute_line_style);

  lv_style_init(&minute_line_style_trace);
  lv_style_set_line_width(&minute_line_style_trace, LV_STATE_DEFAULT, 3);
  lv_style_set_line_color(&minute_line_style_trace, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_line_rounded(&minute_line_style_trace, LV_STATE_DEFAULT, false);
  lv_obj_add_style(minute_body_trace, LV_LINE_PART_MAIN, &minute_line_style_trace);

  lv_style_init(&hour_line_style);
  lv_style_set_line_width(&hour_line_style, LV_STATE_DEFAULT, 7);
  lv_style_set_line_color(&hour_line_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_line_rounded(&hour_line_style, LV_STATE_DEFAULT, true);
  lv_obj_add_style(hour_body, LV_LINE_PART_MAIN, &hour_line_style);

  lv_style_init(&hour_line_style_trace);
  lv_style_set_line_width(&hour_line_style_trace, LV_STATE_DEFAULT, 3);
  lv_style_set_line_color(&hour_line_style_trace, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_line_rounded(&hour_line_style_trace, LV_STATE_DEFAULT, false);
  lv_obj_add_style(hour_body_trace, LV_LINE_PART_MAIN, &hour_line_style_trace);

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);

  Refresh();
}

WatchFaceAnalog::~WatchFaceAnalog() {
  lv_task_del(taskRefresh);

  lv_style_reset(&hour_line_style);
  lv_style_reset(&hour_line_style_trace);
  lv_style_reset(&minute_line_style);
  lv_style_reset(&minute_line_style_trace);
  lv_style_reset(&second_line_style);

  lv_obj_clean(lv_scr_act());
}

void WatchFaceAnalog::UpdateClock() {
  uint8_t hour = dateTimeController.Hours();
  uint8_t minute = dateTimeController.Minutes();
  uint8_t second = dateTimeController.Seconds();

  if (sMinute != minute) {
    auto const angle = minute * 6;
    minute_point[0] = CoordinateRelocate(30, angle);
    minute_point[1] = CoordinateRelocate(MinuteLength, angle);

    minute_point_trace[0] = CoordinateRelocate(5, angle);
    minute_point_trace[1] = CoordinateRelocate(31, angle);

    lv_line_set_points(minute_body, minute_point, 2);
    lv_line_set_points(minute_body_trace, minute_point_trace, 2);
  }

  if (sHour != hour || sMinute != minute) {
    sHour = hour;
    sMinute = minute;
    auto const angle = (hour * 30 + minute / 2);

    hour_point[0] = CoordinateRelocate(30, angle);
    hour_point[1] = CoordinateRelocate(HourLength, angle);

    hour_point_trace[0] = CoordinateRelocate(5, angle);
    hour_point_trace[1] = CoordinateRelocate(31, angle);

    lv_line_set_points(hour_body, hour_point, 2);
    lv_line_set_points(hour_body_trace, hour_point_trace, 2);
  }

  if (sSecond != second) {
    sSecond = second;
    auto const angle = second * 6;

    second_point[0] = CoordinateRelocate(-20, angle);
    second_point[1] = CoordinateRelocate(SecondLength, angle);
    lv_line_set_points(second_body, second_point, 2);
  }
}

void WatchFaceAnalog::SetBatteryIcon() {
  auto batteryPercent = batteryPercentRemaining.Get();
  batteryIcon.SetBatteryPercentage(batteryPercent);
}

void WatchFaceAnalog::Refresh() {
  isCharging = batteryController.IsCharging();
  if (isCharging.IsUpdated()) {
    if (isCharging.Get()) {
      lv_obj_set_hidden(batteryIcon.GetObject(), true);
      lv_obj_set_hidden(plugIcon, false);
    } else {
      lv_obj_set_hidden(batteryIcon.GetObject(), false);
      lv_obj_set_hidden(plugIcon, true);
      SetBatteryIcon();
    }
  }
  if (!isCharging.Get()) {
    batteryPercentRemaining = batteryController.PercentRemaining();
    if (batteryPercentRemaining.IsUpdated()) {
      SetBatteryIcon();
    }
  }

  notificationState = notificationManager.AreNewNotificationsAvailable();

  if (notificationState.IsUpdated()) {
    lv_label_set_text_static(notificationIcon, NotificationIcon::GetIcon(notificationState.Get()));
  }

  currentDateTime = dateTimeController.CurrentDateTime();

  if (currentDateTime.IsUpdated()) {
    Pinetime::Controllers::DateTime::Months month = dateTimeController.Month();
    uint8_t day = dateTimeController.Day();
    Pinetime::Controllers::DateTime::Days dayOfWeek = dateTimeController.DayOfWeek();

    UpdateClock();

    if ((month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {
      lv_label_set_text_fmt(label_date_day, "%s\n%02i", dateTimeController.DayOfWeekShortToString(), day);
	  moon(mooncover);

      currentMonth = month;
      currentDayOfWeek = dayOfWeek;
      currentDay = day;
    }
  }
}
