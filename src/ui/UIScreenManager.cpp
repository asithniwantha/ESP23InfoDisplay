#include "ui/UIScreenManager.h"
#include "UIConfig.h"
#include "lv_font_digital_80.h"

// Modern color palette using UIConfig constants

#define UI_COLOR_PRIMARY   lv_color_hex(UIColors::PRIMARY)
#define UI_COLOR_WARNING   lv_color_hex(UIColors::WARNING)
#define UI_COLOR_DANGER    lv_color_hex(UIColors::DANGER)
#define UI_COLOR_TEXT      lv_color_hex(UIColors::TEXT)
#define UI_COLOR_INFO      lv_color_hex(UIColors::INFO)
#define UI_COLOR_DARK      lv_color_hex(UIColors::DARK)
#define UI_COLOR_CARD      lv_color_hex(UIColors::CARD)
#define UI_COLOR_MUTED     lv_color_hex(UIColors::MUTED)

lv_obj_t* clock_screen = nullptr;
lv_obj_t* clock_time_label = nullptr;
lv_obj_t* clock_seconds_label = nullptr;
lv_obj_t* clock_ampm_label = nullptr;
lv_obj_t* clock_date_label = nullptr;
lv_obj_t* clock_status_label = nullptr;

UIScreenManager::UIScreenManager() {
    Serial.println("UIScreenManager: Constructor called");
    main_screen = nullptr;
    startup_screen = nullptr;
    clock_screen = nullptr;
    clock_time_label = nullptr;
    clock_seconds_label = nullptr;
    clock_ampm_label = nullptr;
    clock_date_label = nullptr;
    clock_status_label = nullptr;
}

void UIScreenManager::showStartupScreen() {
    Serial.println("UIScreenManager: showStartupScreen called");
    lv_obj_t* screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, UI_COLOR_DARK, 0);

    lv_obj_t* logo_label = lv_label_create(screen);
    lv_label_set_text(logo_label, "ESP32\nSystem Monitor");
    lv_obj_set_style_text_color(logo_label, UI_COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(logo_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(logo_label, LV_TEXT_ALIGN_CENTER, 0);
        clock_seconds_label = lv_label_create(clock_screen);

    lv_scr_load(screen);
    Serial.println("UIScreenManager: Loaded startup screen to display");
}

void UIScreenManager::showWiFiInfo(String ip, String mdns, int port) {
    lv_obj_t* wifi_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(wifi_screen, UI_COLOR_DARK, 0);
    lv_obj_t* info_label = lv_label_create(wifi_screen);
    String info_text = "WiFi Connected\n\nIP: " + ip + "\nHostname: " + mdns + "\nPort: " + String(port);
    lv_label_set_text(info_label, info_text.c_str());
    lv_obj_set_style_text_color(info_label, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_align(info_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(info_label);
    lv_scr_load(wifi_screen);
}

void UIScreenManager::showUDPText(String text) {
    lv_obj_t* text_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(text_screen, UI_COLOR_DARK, 0);
    lv_obj_t* text_label = lv_label_create(text_screen);
    lv_label_set_text(text_label, text.c_str());
    lv_obj_set_style_text_color(text_label, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(text_label);
    lv_scr_load(text_screen);
}
void UIScreenManager::showWiFiLoadingScreen() {
    Serial.println("UIScreenManager: showWiFiLoadingScreen called");
    // if (wifi_loading_screen) {
    //     lv_obj_del(wifi_loading_screen);
    // }
    Serial.println("UIScreenManager: Deleted existing wifi_loading_screen");
    wifi_loading_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(wifi_loading_screen, UI_COLOR_DARK, 0);
    lv_obj_t* loading_label = lv_label_create(wifi_loading_screen);
    lv_label_set_text(loading_label, "Connecting to WiFi...\nPlease wait");
    lv_obj_set_style_text_color(loading_label, UI_COLOR_PRIMARY, 0);
    lv_obj_set_style_text_align(loading_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(loading_label);
        if (!clock_screen || !clock_time_label || !clock_seconds_label || !clock_ampm_label || !clock_date_label || !clock_status_label) return;
    Serial.println("UIScreenManager: Loaded wifi_loading_screen to display");
}

void UIScreenManager::hideWiFiLoadingScreen() {
    if (wifi_loading_screen) {
        lv_obj_del(wifi_loading_screen);
        wifi_loading_screen = nullptr;
    }
}

void UIScreenManager::showClockScreen() {
    Serial.println("UIScreenManager: showClockScreen called");
    if (clock_screen) {
        lv_obj_del(clock_screen);
    }
    clock_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(clock_screen, lv_color_hex(0x000000), 0);

    // Main digital time label (centered, large, white, 7-segment font)
    clock_time_label = lv_label_create(clock_screen);
    lv_obj_set_size(clock_time_label, 300, 120); // Nearly full width, very large height
    lv_obj_set_style_text_color(clock_time_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(clock_time_label, &lv_font_digital_80, 0);
    lv_obj_set_style_text_align(clock_time_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_opa(clock_time_label, LV_OPA_0, 0);
    lv_obj_set_style_border_width(clock_time_label, 0, 0);
    lv_obj_set_style_pad_all(clock_time_label, 0, 0);
    lv_obj_set_style_text_letter_space(clock_time_label, 0, 0); // No extra spacing
    lv_obj_align(clock_time_label, LV_ALIGN_CENTER, 0, -20); // Centered
    lv_label_set_text(clock_time_label, ""); // Will be set by updateClockScreen

    // Seconds label (below main time, centered, gray)
    clock_seconds_label = lv_label_create(clock_screen);
    lv_obj_set_style_text_color(clock_seconds_label, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_text_font(clock_seconds_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(clock_seconds_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_opa(clock_seconds_label, LV_OPA_0, 0);
    lv_obj_set_style_border_width(clock_seconds_label, 0, 0);
    lv_obj_align(clock_seconds_label, LV_ALIGN_CENTER, 0, 50); // Centered below time
    lv_label_set_text(clock_seconds_label, ""); // Will be set by updateClockScreen

    // AM/PM indicator (left, vertically centered, white)
    clock_ampm_label = lv_label_create(clock_screen);
    lv_obj_set_style_text_color(clock_ampm_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(clock_ampm_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(clock_ampm_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_letter_space(clock_ampm_label, 2, 0);
    lv_obj_set_style_bg_opa(clock_ampm_label, LV_OPA_0, 0);
    lv_obj_set_style_border_width(clock_ampm_label, 0, 0);
    lv_obj_align(clock_ampm_label, LV_ALIGN_LEFT_MID, 20, 0);
    lv_label_set_text(clock_ampm_label, ""); // Will be set by updateClockScreen

    // Date label (below time, subtle, gray)
    clock_date_label = lv_label_create(clock_screen);
    lv_obj_set_style_text_color(clock_date_label, lv_color_hex(0x444444), 0);
    lv_obj_set_style_text_font(clock_date_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(clock_date_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_letter_space(clock_date_label, 2, 0);
    lv_obj_set_style_bg_opa(clock_date_label, LV_OPA_0, 0);
    lv_obj_set_style_border_width(clock_date_label, 0, 0);
    lv_obj_align(clock_date_label, LV_ALIGN_CENTER, 0, 60);
    lv_label_set_text(clock_date_label, ""); // Will be set by updateClockScreen

    // Status message (bottom, dim)
    clock_status_label = lv_label_create(clock_screen);
    lv_obj_set_style_text_color(clock_status_label, lv_color_hex(0x222222), 0);
    lv_obj_set_style_text_font(clock_status_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(clock_status_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_opa(clock_status_label, LV_OPA_0, 0);
    lv_obj_set_style_border_width(clock_status_label, 0, 0);
    lv_obj_align(clock_status_label, LV_ALIGN_BOTTOM_MID, 0, -5);

    lv_scr_load(clock_screen);
    Serial.println("UIScreenManager: Loaded clock screen to display");
}

void UIScreenManager::updateClockScreen(NetworkManager* networkManager) {
    // Find the clock screen and its labels (assume static from showClockScreen)
    if (!clock_screen) return;
    if (!clock_time_label || !clock_seconds_label || !clock_ampm_label || !clock_date_label || !clock_status_label) return;
    lv_obj_t* time_label = clock_time_label;
    lv_obj_t* seconds_label = clock_seconds_label;
    lv_obj_t* ampm_label = clock_ampm_label;
    lv_obj_t* date_label = clock_date_label;
    lv_obj_t* status_label = clock_status_label;

    // Get current time from system
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    int hour = t->tm_hour;
    int minute = t->tm_min;
    int second = t->tm_sec;
    int day = t->tm_mday;
    int month = t->tm_mon + 1;
    int year = t->tm_year + 1900;

    // Format time for 12-hour display
    bool is_pm = hour >= 12;
    int hour12 = hour % 12;
    if (hour12 == 0) hour12 = 12;
    String ampm = is_pm ? "PM" : "AM";
    char time_str[6];
    snprintf(time_str, sizeof(time_str), "%02d:%02d", hour12, minute);
    char seconds_str[3];
    snprintf(seconds_str, sizeof(seconds_str), "%02d", second);
    char date_str[16];
    snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d", year, month, day);

    // Update labels
    lv_label_set_text(time_label, time_str);
    lv_label_set_text(seconds_label, seconds_str);
    lv_label_set_text(ampm_label, ampm.c_str());
    lv_label_set_text(date_label, date_str);
}

void UIScreenManager::returnToMainUI() {
    // Restore main screen
    if (main_screen) {
        lv_scr_load(main_screen);
        Serial.println("UIScreenManager: Returned to main UI");
    }
}

lv_obj_t* UIScreenManager::getMainScreen() {
    return main_screen;
}