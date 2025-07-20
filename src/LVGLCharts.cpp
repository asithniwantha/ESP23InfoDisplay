#include "LVGLCharts.h"
#include <Arduino.h>

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

LVGLCharts::LVGLCharts(LVGLStyles* styles) : styles(styles) {
    cpu_history_chart = nullptr;
    cpu_history_series = nullptr;
    network_chart = nullptr;
    network_series = nullptr;
    network_label = nullptr;
    volume_card = nullptr;
    volume_slider = nullptr;
    volume_label = nullptr;
}

LVGLCharts::~LVGLCharts() {
    // Objects will be cleaned up by LVGL when parent is deleted
}

void LVGLCharts::createCPUHistoryChart(lv_obj_t* parent) {
    using namespace UILayout;
    
    // Create container for CPU history chart - perfectly centered
    lv_obj_t* chart_container = lv_obj_create(parent);
    lv_obj_set_size(chart_container, CPU_CHART_WIDTH, CPU_CHART_HEIGHT + 18);
    lv_obj_set_pos(chart_container, PADDING, CPU_CHART_Y);
    lv_obj_add_style(chart_container, styles->getCardStyle(), 0);
    
    // Title
    lv_obj_t* title = lv_label_create(chart_container);
    lv_label_set_text(title, "CPU History");
    lv_obj_set_style_text_color(title, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_12, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 3);
    
    // Chart with Task Manager style scrolling
    cpu_history_chart = lv_chart_create(chart_container);
    lv_obj_set_size(cpu_history_chart, CPU_CHART_WIDTH - 12, CPU_CHART_HEIGHT - 12);
    lv_obj_align(cpu_history_chart, LV_ALIGN_BOTTOM_MID, 0, -3);
    lv_chart_set_type(cpu_history_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(cpu_history_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_point_count(cpu_history_chart, CPU_CHART_POINTS);
    lv_chart_set_update_mode(cpu_history_chart, LV_CHART_UPDATE_MODE_SHIFT); // Enable scrolling
    lv_obj_add_style(cpu_history_chart, styles->getChartStyle(), 0);
    
    // Add series with smooth line style (no point dots)
    cpu_history_series = lv_chart_add_series(cpu_history_chart, UI_COLOR_PRIMARY, LV_CHART_AXIS_PRIMARY_Y);
    
    // Remove point dots by setting size to 0
    lv_obj_set_style_size(cpu_history_chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(cpu_history_chart, 2, LV_PART_ITEMS);
    
    // Initialize chart with zeros for smooth start
    for(int i = 0; i < CPU_CHART_POINTS; i++) {
        lv_chart_set_next_value(cpu_history_chart, cpu_history_series, 0);
    }
}

void LVGLCharts::createNetworkChart(lv_obj_t* parent) {
    using namespace UILayout;
    
    // Create container for network chart
    lv_obj_t* chart_container = lv_obj_create(parent);
    lv_obj_set_size(chart_container, BOTTOM_CARD_WIDTH, BOTTOM_CARD_HEIGHT);
    lv_obj_set_pos(chart_container, NETWORK_CHART_X, NETWORK_CHART_Y);
    lv_obj_add_style(chart_container, styles->getCardStyle(), 0);
    
    // Title
    lv_obj_t* title = lv_label_create(chart_container);
    lv_label_set_text(title, "Network");
    lv_obj_set_style_text_color(title, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_12, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 4, 2);
    
    // Speed label
    network_label = lv_label_create(chart_container);
    lv_label_set_text(network_label, "0KB/s");
    lv_obj_set_style_text_color(network_label, UI_COLOR_INFO, 0);
    lv_obj_set_style_text_font(network_label, &lv_font_montserrat_12, 0);
    lv_obj_align(network_label, LV_ALIGN_TOP_RIGHT, -4, 2);
    
    // Chart
    network_chart = lv_chart_create(chart_container);
    lv_obj_set_size(network_chart, BOTTOM_CARD_WIDTH - 12, BOTTOM_CARD_HEIGHT - 20);
    lv_obj_align(network_chart, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_chart_set_type(network_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(network_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1024);
    lv_chart_set_point_count(network_chart, UILayout::NETWORK_CHART_POINTS);
    lv_obj_add_style(network_chart, styles->getChartStyle(), 0);
    
    // Remove point dots by setting size to 0
    lv_obj_set_style_size(network_chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(network_chart, 2, LV_PART_ITEMS);
    
    network_series = lv_chart_add_series(network_chart, UI_COLOR_INFO, LV_CHART_AXIS_PRIMARY_Y);
}

void LVGLCharts::createVolumeControl(lv_obj_t* parent) {
    using namespace UILayout;
    
    // Create container for volume control
    volume_card = lv_obj_create(parent);
    lv_obj_set_size(volume_card, BOTTOM_CARD_WIDTH, BOTTOM_CARD_HEIGHT);
    lv_obj_set_pos(volume_card, VOLUME_CONTROL_X, VOLUME_CONTROL_Y);
    lv_obj_add_style(volume_card, styles->getCardStyle(), 0);
    
    // Title with speaker icon
    lv_obj_t* title = lv_label_create(volume_card);
    lv_label_set_text(title, LV_SYMBOL_VOLUME_MAX " Volume");
    lv_obj_set_style_text_color(title, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_12, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 4, 2);
    
    // Volume percentage label
    volume_label = lv_label_create(volume_card);
    lv_label_set_text(volume_label, "0%");
    lv_obj_set_style_text_color(volume_label, UI_COLOR_WARNING, 0);
    lv_obj_set_style_text_font(volume_label, &lv_font_montserrat_12, 0);
    lv_obj_align(volume_label, LV_ALIGN_TOP_RIGHT, -4, 2);
    
    // Volume slider
    volume_slider = lv_slider_create(volume_card);
    lv_obj_set_size(volume_slider, BOTTOM_CARD_WIDTH - 16, 8);
    lv_obj_align(volume_slider, LV_ALIGN_CENTER, 0, 5);
    lv_slider_set_range(volume_slider, 0, 100);
    lv_slider_set_value(volume_slider, 50, LV_ANIM_OFF);
}

void LVGLCharts::updateCharts(SystemData& data, bool forceUpdate) {
    SystemStats& stats = data.getCurrentData();
    
    // Update CPU history chart
    if (data.isHistoryFull() || forceUpdate) {
        updateCPUHistoryChart(data.getCpuHistory(), data.getHistoryIndex());
    }
    
    // Update Network chart and display
    if (forceUpdate) { // Simplified for now, add threshold check if needed
        updateNetworkChart(stats.networkSpeed);
        lv_label_set_text(network_label, data.getFormattedNetworkSpeed().c_str());
    }
    
    // Update Volume control
    if (stats.volumeLevel != 0 || forceUpdate) { // Simplified condition
        updateVolumeSlider(stats.volumeLevel);
    }
}

void LVGLCharts::updateCPUHistoryChart(float* history, int historySize) {
    if (!cpu_history_series || !history) return;
    
    // Task Manager style scrolling: add newest point and shift left
    // This creates a smooth scrolling effect like Windows Task Manager
    lv_chart_set_next_value(cpu_history_chart, cpu_history_series, (int32_t)history[historySize - 1]);
    lv_chart_refresh(cpu_history_chart);
}

void LVGLCharts::updateNetworkChart(float speed) {
    if (!network_series) return;
    
    // Add new data point to network chart
    lv_chart_set_next_value(network_chart, network_series, (int32_t)speed);
    lv_chart_refresh(network_chart);
}

void LVGLCharts::updateVolumeSlider(int volume) {
    if (!volume_slider || !volume_label) return;
    
    // Update slider position and label
    lv_slider_set_value(volume_slider, volume, LV_ANIM_ON);
    
    char vol_text[10];
    snprintf(vol_text, sizeof(vol_text), "%d%%", volume);
    lv_label_set_text(volume_label, vol_text);
}

void LVGLCharts::handleVolumeTouch(int x, int y, SystemData& data) {
    // Calculate relative position within volume control and update volume
    int local_x = x - UILayout::VOLUME_CONTROL_X;
    if (local_x >= 25 && local_x <= UILayout::BOTTOM_CARD_WIDTH - 15) {
        int slider_width = UILayout::BOTTOM_CARD_WIDTH - 40;
        float volume_percent = ((local_x - 25) * 100.0f) / slider_width;
        volume_percent = (volume_percent < 0) ? 0 : ((volume_percent > 100) ? 100 : volume_percent);
        
        data.setVolume((int)volume_percent);
        updateVolumeSlider((int)volume_percent);
    }
}
