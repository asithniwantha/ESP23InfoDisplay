#include "ui/UIStyleManager.h"
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

UIStyleManager::UIStyleManager() {
    // Constructor
}

void UIStyleManager::initStyles() {
    lv_style_init(&style_card);
    lv_style_set_radius(&style_card, 12);
    lv_style_set_bg_color(&style_card, UI_COLOR_CARD);
    lv_style_set_bg_opa(&style_card, LV_OPA_100);
    lv_style_set_border_width(&style_card, 2);
    lv_style_set_border_color(&style_card, UI_COLOR_PRIMARY);
    lv_style_set_border_opa(&style_card, LV_OPA_20);
    lv_style_set_shadow_width(&style_card, 10);
    lv_style_set_shadow_color(&style_card, lv_color_black());
    lv_style_set_shadow_opa(&style_card, LV_OPA_30);
    lv_style_set_shadow_ofs_x(&style_card, 0);
    lv_style_set_shadow_ofs_y(&style_card, 4);
    lv_style_set_pad_all(&style_card, 8);
    
    lv_style_init(&style_progress);
    lv_style_set_radius(&style_progress, 6);
    lv_style_set_bg_color(&style_progress, UI_COLOR_DARK);
    lv_style_set_bg_opa(&style_progress, LV_OPA_50);
    
    lv_style_init(&style_chart);
    lv_style_set_radius(&style_chart, 8);
    lv_style_set_bg_color(&style_chart, UI_COLOR_CARD);
    lv_style_set_bg_opa(&style_chart, LV_OPA_80);
    lv_style_set_border_width(&style_chart, 1);
    lv_style_set_border_color(&style_chart, UI_COLOR_PRIMARY);
    lv_style_set_border_opa(&style_chart, LV_OPA_30);
    lv_style_set_pad_all(&style_chart, 4);
    lv_style_set_line_width(&style_chart, 2);
    lv_style_set_size(&style_chart, 3);
}