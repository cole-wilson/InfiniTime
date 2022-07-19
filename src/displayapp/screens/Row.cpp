#include "displayapp/screens/Row.h"
#include "displayapp/DisplayApp.h"
#include <lvgl/lvgl.h>
#include <vector>
#include <numeric>



using namespace Pinetime::Applications::Screens;

Row::Row(DisplayApp* app,
				Controllers::Battery& batteryController,
				Controllers::DateTime& dateTimeController,
				Controllers::MotorController& motorController,
				Controllers::BrightnessController& brightnessController,
				Controllers::TimerController& timerController,
				Controllers::MotionController& motionController,
				Controllers::FS& fs,
				System::SystemTask* systemTask,
				Controllers::HeartRateController& heartRateController)
	: Screen(app),
    batteryController {batteryController},
    dateTimeController {dateTimeController},
	motorController {motorController},
	brightnessController {brightnessController},
	timerController {timerController},
	motionController {motionController},
	fs {fs},
	systemTask {systemTask},
	heartRateController {heartRateController}
{
	/* systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping); */

    timerController.StartTimer((1 + 13 * 60) * 1000);

	static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_border_width(&style, LV_STATE_DEFAULT, 4);
	lv_style_set_text_color(&style, LV_STATE_DEFAULT, lv_color_hex(0x000000));
	lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x5C7162));

	/* timeofday = lv_label_create(lv_scr_act(), NULL); */
    /* lv_obj_add_style(timeofday, LV_OBJ_PART_MAIN, &style); */
	/* lv_label_set_text(timeofday, "12:34 PM"); */
	/* lv_label_set_align(timeofday, LV_LABEL_ALIGN_CENTER); */
	/* lv_obj_align(timeofday, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10); */

	strokerate = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(strokerate, LV_OBJ_PART_MAIN, &style);
	lv_obj_set_style_local_text_font(strokerate, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
	lv_label_set_text(strokerate, "28");
	lv_label_set_align(strokerate, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(strokerate, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 30);

	timer = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(timer, LV_OBJ_PART_MAIN, &style);
	lv_obj_set_style_local_text_font(timer, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
	lv_label_set_text(timer, "06:27");
	lv_label_set_align(timer, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(timer, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 4);

	strokecount = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(strokecount, LV_OBJ_PART_MAIN, &style);
	/* lv_obj_set_style_local_text_font(strokecount, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42); */
	lv_label_set_text(strokecount, "");
	lv_label_set_align(strokecount, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(strokecount, NULL, LV_ALIGN_IN_LEFT_MID, 10, 10);

	if (heartRateController.State() == Controllers::HeartRateController::States::Stopped)
		heartRateController.Start();

	heartrate = lv_label_create(lv_scr_act(), NULL);
	lv_obj_add_style(heartrate, LV_OBJ_PART_MAIN, &style);
	lv_obj_set_style_local_text_font(heartrate, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42);
	lv_label_set_text(heartrate, "---");
	lv_label_set_align(heartrate, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(heartrate, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 10);


	/* lv_obj_t* bg_clock_img = lv_img_create(lv_scr_act(), NULL); */
	/* lv_img_set_src(bg_clock_img, &bg_clock); */
	/* lv_obj_align(bg_clock_img, NULL, LV_ALIGN_CENTER, 0, 0); */

	taskRefresh = lv_task_create(RefreshTaskCallback, 100, LV_TASK_PRIO_MID, this);

}
float mean(std::vector<float> v) {
	float sum = std::accumulate(v.begin(), v.end(), 0.0);
	return sum / v.size();
}
float stdDev(std::vector<float> v) {
	float gmean = mean(v);
	float sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
	float stdev = std::sqrt(sq_sum / v.size() - gmean * gmean);
	return stdev;
}
std::vector<int> smoothedZScore(int arr[], int n)
{   
	std::vector<int> input(arr, arr + n);

    int lag = 5; //lag 5 for the smoothing functions
    //3.5 standard deviations for signal
    float threshold = 3.5;
    //between 0 and 1, where 1 is normal influence, 0.5 is half
    float influence = .5;

    if (input.size() <= lag + 2)
    {
        std::vector<int> emptyVec;
        return emptyVec;
    }

    //Initialise variables
    std::vector<int> signals(input.size(), 0.0);
    std::vector<float> filteredY(input.size(), 0.0);
    std::vector<float> avgFilter(input.size(), 0.0);
    std::vector<float> stdFilter(input.size(), 0.0);
    std::vector<float> subVecStart(input.begin(), input.begin() + lag);
    avgFilter[lag] = mean(subVecStart);
    stdFilter[lag] = stdDev(subVecStart);

    for (size_t i = lag + 1; i < input.size(); i++)
    {
        if (std::abs(input[i] - avgFilter[i - 1]) > threshold * stdFilter[i - 1])
        {
            if (input[i] > avgFilter[i - 1])
            {
                signals[i] = 1; //# Positive signal
            }
            else
            {
                signals[i] = -1; //# Negative signal
            }
            //Make influence lower
            filteredY[i] = influence* input[i] + (1 - influence) * filteredY[i - 1];
        }
        else
        {
            signals[i] = 0; //# No signal
            filteredY[i] = input[i];
        }
        //Adjust the filters
        std::vector<float> subVec(filteredY.begin() + i - lag, filteredY.begin() + i);
        avgFilter[i] = mean(subVec);
        stdFilter[i] = stdDev(subVec);
    }
    return signals;
}

void Row::Refresh() {
	// timer
	if (timerController.IsRunning()) {
		uint32_t seconds = timerController.GetTimeRemaining() / 1000;
		lv_label_set_text_fmt(timer, "%.2d:%.2d", seconds/60, seconds%60);
	}

	// strokes
	int motiondatalen = sizeof(motiondata)/sizeof(motiondata[0]);
	for (int i=1;i<motiondatalen;i++) motiondata[i-1] = motiondata[i];
	motiondata[motiondatalen-1] = motionController.X();
	
	std::vector<int> signals = smoothedZScore(motiondata, motiondatalen);
	int count = 0;
	for (int x=0;x<signals.size();x++) {
		if (signals[x] == 1) count++;
	}

	lv_label_set_text_fmt(strokecount, " %d", count);

	/* if (abs(motiondata[999] - avg) > 20) { */
		/* motorController.RunForDuration(15); */
		/* motiondata[99] = avg; */
	/* } */


	// heartrate
	auto state = heartRateController.State();
	switch (state) {
		case Controllers::HeartRateController::States::NoTouch:
		case Controllers::HeartRateController::States::NotEnoughData:
		case Controllers::HeartRateController::States::Stopped:
		  lv_label_set_text_static(heartrate, "---");
		  break;
		default:
		  lv_label_set_text_fmt(heartrate, "%03d", heartRateController.HeartRate());
	  }

}
void Row::OnButtonEvent(lv_obj_t* obj, lv_event_t event) {
  /* if (event == LV_EVENT_CLICKED) { */
    /* if (obj == btnPlayPause) { */
	/* } */
  /* } */
}
bool Row::OnButtonPushed() {return true;}
bool Row::OnButtonDoubleClicked() {return true;}
bool Row::OnButtonLongPressed() {return true;}
bool Row::OnButtonLongerPressed() {return true;}

Row::~Row() {
  heartRateController.Stop();
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  /* systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping); */
}
