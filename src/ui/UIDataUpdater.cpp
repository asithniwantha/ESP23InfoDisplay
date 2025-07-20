#include "ui/UIDataUpdater.h"
#include "UIConfig.h"

// Modern color palette using UIConfig constants
#define UI_COLOR_PRIMARY   lv_color_hex(UIColors::PRIMARY)
#define UI_COLOR_SUCCESS   lv_color_hex(UIColors::SUCCESS)
#define UI_COLOR_WARNING   lv_color_hex(UIColors::WARNING)
#define UI_COLOR_DANGER    lv_color_hex(UIColors::DANGER)
#define UI_COLOR_INFO      lv_color_hex(UIColors::INFO)
#define UI_COLOR_DARK      lv_color_hex(UIColors::DARK)
#define UI_COLOR_CARD      lv_color_hex(UIColors::CARD)
#define UI_COLOR_TEXT      lv_color_hex(UIColors::TEXT)
#define UI_COLOR_MUTED     lv_color_hex(UIColors::MUTED)

UIDataUpdater::UIDataUpdater() {}

void UIDataUpdater::updateUI(SystemData& data) {
    // This will be filled with the logic from the original LVGLUIManager::updateUI
}

void UIDataUpdater::updateProgressBar(lv_obj_t* bar, int32_t value, lv_color_t color) {
    if (bar) {
        lv_bar_set_value(bar, value, LV_ANIM_ON);
        lv_obj_set_style_bg_color(bar, color, LV_PART_INDICATOR);
    }
}

void UIDataUpdater::updateCPUHistoryChart(float* history, int historySize) {
    // Logic to update CPU history chart
}

void UIDataUpdater::updateNetworkChart(float speed) {
    // Logic to update network chart
}

void UIDataUpdater::updateVolumeSlider(int volume) {
    // Logic to update volume slider
}

void UIDataUpdater::updateTemperatureGauge(int temp) {
    // Logic to update temperature gauge
}

lv_color_t UIDataUpdater::getColorForValue(float value, int warning_threshold, int danger_threshold) {
    if (value >= danger_threshold) {
        return UI_COLOR_DANGER;
    } else if (value >= warning_threshold) {
        return UI_COLOR_WARNING;
    } else {
        return UI_COLOR_SUCCESS;
    }
}