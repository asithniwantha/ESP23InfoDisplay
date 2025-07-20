#include "ui/UIViewFactory.h"
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

UIViewFactory::UIViewFactory(UIStyleManager& styles) : styleManager(styles) {}

lv_obj_t* UIViewFactory::createCard(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* title) {
    lv_obj_t* card = lv_obj_create(parent);
    lv_obj_set_size(card, w, h);
    lv_obj_set_pos(card, x, y);
    lv_obj_add_style(card, &styleManager.style_card, 0);
    
    lv_obj_t* title_label = lv_label_create(card);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, UI_COLOR_MUTED, 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_12, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 5);
    
    return card;
}

void UIViewFactory::createTopRowCards(lv_obj_t* parent) {
    using namespace UILayout;
    
    int card_positions[4];
    for (int i = 0; i < 4; i++) {
        card_positions[i] = PADDING + (i * (CARD_WIDTH + GAP));
    }
    
    lv_obj_t* cpu_card = createCard(parent, card_positions[0], CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "CPU");
    lv_obj_t* ram_card = createCard(parent, card_positions[1], CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "RAM");
    lv_obj_t* temp_card = createCard(parent, card_positions[2], CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "TEMP");
    lv_obj_t* disk_card = createCard(parent, card_positions[3], CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "DISK");
}

void UIViewFactory::createCPUHistoryChart(lv_obj_t* parent) {
    using namespace UILayout;
    
    lv_obj_t* chart_container = lv_obj_create(parent);
    lv_obj_set_size(chart_container, CPU_CHART_WIDTH, CPU_CHART_HEIGHT + 18);
    lv_obj_set_pos(chart_container, PADDING, CPU_CHART_Y);
    lv_obj_add_style(chart_container, &styleManager.style_card, 0);
    
    lv_obj_t* title = lv_label_create(chart_container);
    lv_label_set_text(title, "CPU History");
    lv_obj_set_style_text_color(title, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_12, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 3);
    
    lv_obj_t* cpu_history_chart = lv_chart_create(chart_container);
    lv_obj_set_size(cpu_history_chart, CPU_CHART_WIDTH - 12, CPU_CHART_HEIGHT - 12);
    lv_obj_align(cpu_history_chart, LV_ALIGN_BOTTOM_MID, 0, -3);
    lv_chart_set_type(cpu_history_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(cpu_history_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_point_count(cpu_history_chart, CPU_CHART_POINTS);
    lv_chart_set_update_mode(cpu_history_chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_obj_add_style(cpu_history_chart, &styleManager.style_chart, 0);
}

void UIViewFactory::createNetworkChart(lv_obj_t* parent) {
    using namespace UILayout;
    
    lv_obj_t* chart_container = lv_obj_create(parent);
    lv_obj_set_size(chart_container, BOTTOM_CARD_WIDTH, BOTTOM_CARD_HEIGHT);
    lv_obj_set_pos(chart_container, NETWORK_CHART_X, NETWORK_CHART_Y);
    lv_obj_add_style(chart_container, &styleManager.style_card, 0);
    
    lv_obj_t* title = lv_label_create(chart_container);
    lv_label_set_text(title, "Network");
    lv_obj_set_style_text_color(title, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_12, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 4, 2);
    
    lv_obj_t* network_label = lv_label_create(chart_container);
    lv_label_set_text(network_label, "0KB/s");
    lv_obj_set_style_text_color(network_label, UI_COLOR_INFO, 0);
    lv_obj_set_style_text_font(network_label, &lv_font_montserrat_12, 0);
    lv_obj_align(network_label, LV_ALIGN_TOP_RIGHT, -4, 2);
    
    lv_obj_t* network_chart = lv_chart_create(chart_container);
    lv_obj_set_size(network_chart, BOTTOM_CARD_WIDTH - 12, BOTTOM_CARD_HEIGHT - 20);
    lv_obj_align(network_chart, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_chart_set_type(network_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(network_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1024);
    lv_chart_set_point_count(network_chart, UILayout::NETWORK_CHART_POINTS);
    lv_obj_add_style(network_chart, &styleManager.style_chart, 0);
}

void UIViewFactory::createVolumeControl(lv_obj_t* parent) {
    using namespace UILayout;
    
    lv_obj_t* volume_card = lv_obj_create(parent);
    lv_obj_set_size(volume_card, BOTTOM_CARD_WIDTH, BOTTOM_CARD_HEIGHT);
    lv_obj_set_pos(volume_card, VOLUME_CONTROL_X, VOLUME_CONTROL_Y);
    lv_obj_add_style(volume_card, &styleManager.style_card, 0);
    
    lv_obj_t* title = lv_label_create(volume_card);
    lv_label_set_text(title, LV_SYMBOL_VOLUME_MAX " Volume");
    lv_obj_set_style_text_color(title, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_12, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 4, 2);
    
    lv_obj_t* volume_label = lv_label_create(volume_card);
    lv_label_set_text(volume_label, "0%");
    lv_obj_set_style_text_color(volume_label, UI_COLOR_WARNING, 0);
    lv_obj_set_style_text_font(volume_label, &lv_font_montserrat_12, 0);
    lv_obj_align(volume_label, LV_ALIGN_TOP_RIGHT, -4, 2);
    
    lv_obj_t* volume_slider = lv_slider_create(volume_card);
    lv_obj_set_size(volume_slider, BOTTOM_CARD_WIDTH - 16, 8);
    lv_obj_align(volume_slider, LV_ALIGN_CENTER, 0, 5);
    lv_slider_set_range(volume_slider, 0, 100);
    lv_slider_set_value(volume_slider, 50, LV_ANIM_OFF);
}

void UIViewFactory::createTemperatureGauge(lv_obj_t* parent) {
    lv_obj_t* temp_gauge = lv_meter_create(parent);
    lv_obj_set_size(temp_gauge, UILayout::TEMP_GAUGE_SIZE, UILayout::TEMP_GAUGE_SIZE);
    lv_obj_center(temp_gauge);
    lv_obj_remove_style(temp_gauge, NULL, LV_PART_MAIN);
    lv_obj_remove_style(temp_gauge, NULL, LV_PART_INDICATOR);
    
    lv_meter_scale_t* scale = lv_meter_add_scale(temp_gauge);
    lv_meter_set_scale_ticks(temp_gauge, scale, 6, 1, 8, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(temp_gauge, scale, 2, 2, 12, UI_COLOR_TEXT, 8);
    lv_meter_set_scale_range(temp_gauge, scale, 0, 100, 240, 90);
    
    lv_meter_indicator_t* indic1 = lv_meter_add_arc(temp_gauge, scale, 6, UI_COLOR_SUCCESS, -3);
    lv_meter_set_indicator_start_value(temp_gauge, indic1, 0);
    lv_meter_set_indicator_end_value(temp_gauge, indic1, 50);
    
    lv_meter_indicator_t* indic2 = lv_meter_add_arc(temp_gauge, scale, 6, UI_COLOR_WARNING, -3);
    lv_meter_set_indicator_start_value(temp_gauge, indic2, 50);
    lv_meter_set_indicator_end_value(temp_gauge, indic2, 70);
    
    lv_meter_indicator_t* indic3 = lv_meter_add_arc(temp_gauge, scale, 6, UI_COLOR_DANGER, -3);
    lv_meter_set_indicator_start_value(temp_gauge, indic3, 70);
    lv_meter_set_indicator_end_value(temp_gauge, indic3, 100);
    
    lv_meter_indicator_t* temp_needle = lv_meter_add_needle_line(temp_gauge, scale, 3, UI_COLOR_TEXT, -8);
    lv_meter_set_indicator_value(temp_gauge, temp_needle, 0);
    
    lv_obj_t* temp_label = lv_label_create(temp_gauge);
    lv_label_set_text(temp_label, "0°C");
    lv_obj_set_style_text_color(temp_label, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_12, 0);
    lv_obj_center(temp_label);
}