#include "LVGLCards.h"
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

LVGLCards::LVGLCards(LVGLStyles* styles) : styles(styles) {
    cpu_card = nullptr;
    ram_card = nullptr;
    temp_card = nullptr;
    disk_card = nullptr;
    cpu_bar = nullptr;
    ram_bar = nullptr;
    disk_bar = nullptr;
    cpu_label = nullptr;
    ram_label = nullptr;
    disk_label = nullptr;
    temp_gauge = nullptr;
    temp_needle = nullptr;
    temp_label = nullptr;
}

LVGLCards::~LVGLCards() {
    // Objects will be cleaned up by LVGL when parent is deleted
}

void LVGLCards::createTopRowCards(lv_obj_t* parent) {
    using namespace UILayout;
    
    // Calculate exact positions for 4 cards to fit perfectly in 320px
    int card_positions[4];
    for (int i = 0; i < 4; i++) {
        card_positions[i] = PADDING + (i * (CARD_WIDTH + GAP));
    }
    
    createCPUCard(parent, card_positions[0]);
    createRAMCard(parent, card_positions[1]);
    createTempCard(parent, card_positions[2]);
    createDiskCard(parent, card_positions[3]);
}

void LVGLCards::createCPUCard(lv_obj_t* parent, int x_position) {
    using namespace UILayout;
    
    cpu_card = createCard(parent, x_position, CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "CPU");
    cpu_bar = lv_bar_create(cpu_card);
    lv_obj_set_size(cpu_bar, CARD_WIDTH - 14, 8);
    lv_obj_align(cpu_bar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(cpu_bar, styles->getProgressStyle(), 0);
    cpu_label = lv_label_create(cpu_card);
    lv_label_set_text(cpu_label, "0%");
    lv_obj_align(cpu_label, LV_ALIGN_BOTTOM_MID, 0, -4);
}

void LVGLCards::createRAMCard(lv_obj_t* parent, int x_position) {
    using namespace UILayout;
    
    ram_card = createCard(parent, x_position, CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "RAM");
    ram_bar = lv_bar_create(ram_card);
    lv_obj_set_size(ram_bar, CARD_WIDTH - 14, 8);
    lv_obj_align(ram_bar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(ram_bar, styles->getProgressStyle(), 0);
    ram_label = lv_label_create(ram_card);
    lv_label_set_text(ram_label, "0%");
    lv_obj_align(ram_label, LV_ALIGN_BOTTOM_MID, 0, -4);
}

void LVGLCards::createTempCard(lv_obj_t* parent, int x_position) {
    using namespace UILayout;
    
    temp_card = createCard(parent, x_position, CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "TEMP");
    createTemperatureGauge();
}

void LVGLCards::createDiskCard(lv_obj_t* parent, int x_position) {
    using namespace UILayout;
    
    disk_card = createCard(parent, x_position, CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "DISK");
    disk_bar = lv_bar_create(disk_card);
    lv_obj_set_size(disk_bar, CARD_WIDTH - 14, 8);
    lv_obj_align(disk_bar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(disk_bar, styles->getProgressStyle(), 0);
    disk_label = lv_label_create(disk_card);
    lv_label_set_text(disk_label, "0%");
    lv_obj_align(disk_label, LV_ALIGN_BOTTOM_MID, 0, -4);
}

void LVGLCards::createTemperatureGauge() {
    // Create meter for temperature
    temp_gauge = lv_meter_create(temp_card);
    lv_obj_set_size(temp_gauge, UILayout::TEMP_GAUGE_SIZE, UILayout::TEMP_GAUGE_SIZE);
    lv_obj_center(temp_gauge);
    lv_obj_remove_style(temp_gauge, NULL, LV_PART_MAIN);
    lv_obj_remove_style(temp_gauge, NULL, LV_PART_INDICATOR);
    
    // Add scale with proper configuration
    lv_meter_scale_t* scale = lv_meter_add_scale(temp_gauge);
    lv_meter_set_scale_ticks(temp_gauge, scale, 6, 1, 8, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(temp_gauge, scale, 2, 2, 12, UI_COLOR_TEXT, 8);
    lv_meter_set_scale_range(temp_gauge, scale, 0, 100, 240, 90);
    
    // Add colored arcs for different temperature ranges
    lv_meter_indicator_t* indic1 = lv_meter_add_arc(temp_gauge, scale, 6, UI_COLOR_SUCCESS, -3);
    lv_meter_set_indicator_start_value(temp_gauge, indic1, 0);
    lv_meter_set_indicator_end_value(temp_gauge, indic1, 50);
    
    lv_meter_indicator_t* indic2 = lv_meter_add_arc(temp_gauge, scale, 6, UI_COLOR_WARNING, -3);
    lv_meter_set_indicator_start_value(temp_gauge, indic2, 50);
    lv_meter_set_indicator_end_value(temp_gauge, indic2, 70);
    
    lv_meter_indicator_t* indic3 = lv_meter_add_arc(temp_gauge, scale, 6, UI_COLOR_DANGER, -3);
    lv_meter_set_indicator_start_value(temp_gauge, indic3, 70);
    lv_meter_set_indicator_end_value(temp_gauge, indic3, 100);
    
    // Add needle with proper styling
    temp_needle = lv_meter_add_needle_line(temp_gauge, scale, 3, UI_COLOR_TEXT, -8);
    lv_meter_set_indicator_value(temp_gauge, temp_needle, 0); // Initialize to 0
    
    // Temperature label in center
    temp_label = lv_label_create(temp_gauge);
    lv_label_set_text(temp_label, "0°C");
    lv_obj_set_style_text_color(temp_label, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_12, 0);
    lv_obj_center(temp_label);
}

lv_obj_t* LVGLCards::createCard(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* title) {
    lv_obj_t* card = lv_obj_create(parent);
    lv_obj_set_size(card, w, h);
    lv_obj_set_pos(card, x, y);
    lv_obj_add_style(card, styles->getCardStyle(), 0);
    
    lv_obj_t* title_label = lv_label_create(card);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, UI_COLOR_MUTED, 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_12, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 5);
    
    return card;
}

void LVGLCards::updateCards(SystemData& data, bool forceUpdate) {
    SystemStats& stats = data.getCurrentData();
    
    // Update CPU card
    if (abs(stats.cpuUsage - lastStats.cpuUsage) > SystemThresholds::CPU_UPDATE_THRESHOLD || forceUpdate) {
        updateCPUCard(stats.cpuUsage, forceUpdate);
    }
    
    // Update RAM card
    if (abs(stats.ramUsage - lastStats.ramUsage) > SystemThresholds::RAM_UPDATE_THRESHOLD || forceUpdate) {
        updateRAMCard(stats.ramUsage, forceUpdate);
    }
    
    // Update Disk card
    if (abs(stats.diskUsage - lastStats.diskUsage) > SystemThresholds::DISK_UPDATE_THRESHOLD || forceUpdate) {
        updateDiskCard(stats.diskUsage, forceUpdate);
    }
    
    // Update Temperature card
    if (stats.cpuTemp != lastStats.cpuTemp || forceUpdate) {
        updateTemperatureCard(stats.cpuTemp, forceUpdate);
    }
    
    lastStats = stats;
}

void LVGLCards::updateCPUCard(float cpuUsage, bool forceUpdate) {
    char cpu_text[10];
    snprintf(cpu_text, sizeof(cpu_text), "%.1f%%", cpuUsage);
    lv_color_t cpu_color = LVGLStyles::getColorForValue(cpuUsage, SystemThresholds::WARNING_LEVEL, SystemThresholds::DANGER_LEVEL);
    lv_label_set_text(cpu_label, cpu_text);
    updateProgressBar(cpu_bar, (int32_t)cpuUsage, cpu_color);
}

void LVGLCards::updateRAMCard(float ramUsage, bool forceUpdate) {
    char ram_text[10];
    snprintf(ram_text, sizeof(ram_text), "%.1f%%", ramUsage);
    lv_color_t ram_color = LVGLStyles::getColorForValue(ramUsage, SystemThresholds::WARNING_LEVEL, SystemThresholds::DANGER_LEVEL);
    lv_label_set_text(ram_label, ram_text);
    updateProgressBar(ram_bar, (int32_t)ramUsage, ram_color);
}

void LVGLCards::updateDiskCard(float diskUsage, bool forceUpdate) {
    char disk_text[10];
    snprintf(disk_text, sizeof(disk_text), "%.1f%%", diskUsage);
    lv_color_t disk_color = LVGLStyles::getColorForValue(diskUsage, SystemThresholds::WARNING_LEVEL, SystemThresholds::DANGER_LEVEL);
    lv_label_set_text(disk_label, disk_text);
    updateProgressBar(disk_bar, (int32_t)diskUsage, disk_color);
}

void LVGLCards::updateTemperatureCard(int temperature, bool forceUpdate) {
    updateTemperatureGauge(temperature);
}

void LVGLCards::updateProgressBar(lv_obj_t* bar, int32_t value, lv_color_t color) {
    if (bar) {
        lv_bar_set_value(bar, value, LV_ANIM_ON);
        lv_obj_set_style_bg_color(bar, color, LV_PART_INDICATOR);
    }
}

void LVGLCards::updateTemperatureGauge(int temp) {
    if (!temp_needle || !temp_label || !temp_gauge) return;
    
    // Clamp temperature to valid range (0-100)
    int clampedTemp = temp < 0 ? 0 : (temp > 100 ? 100 : temp);
    
    // Update gauge needle with animation
    lv_meter_set_indicator_value(temp_gauge, temp_needle, clampedTemp);
    
    // Update center label
    char temp_text[10];
    snprintf(temp_text, sizeof(temp_text), "%d°C", clampedTemp);
    lv_label_set_text(temp_label, temp_text);
}
