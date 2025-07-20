#include "LVGLScreens.h"
#include "NetworkManager.h"
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

LVGLScreens::LVGLScreens() {
    startup_screen = nullptr;
    clock_screen = nullptr;
    wifi_loading_screen = nullptr;
    clock_time_label = nullptr;
    clock_date_label = nullptr;
    clock_status_label = nullptr;
    clock_ampm_label = nullptr;
    clock_seconds_label = nullptr;
    isClockMode = false;
    lastDataReceived = 0;
}

LVGLScreens::~LVGLScreens() {
    // Objects will be cleaned up by LVGL
}

void LVGLScreens::showStartupScreen() {
    // if (startup_screen) {
    //     lv_obj_del(startup_screen);
    // }
    
    startup_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(startup_screen, UI_COLOR_DARK, 0);
    
    lv_obj_t* logo_label = lv_label_create(startup_screen);
    lv_label_set_text(logo_label, "ESP32\nSystem Monitor");
    lv_obj_set_style_text_color(logo_label, UI_COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(logo_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(logo_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(logo_label);
    
    lv_scr_load(startup_screen);
}

void LVGLScreens::showWiFiInfo(String ip, String mdns, int port) {
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

void LVGLScreens::showUDPText(String text) {
    lv_obj_t* text_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(text_screen, UI_COLOR_DARK, 0);
    
    lv_obj_t* text_label = lv_label_create(text_screen);
    lv_label_set_text(text_label, text.c_str());
    lv_obj_set_style_text_color(text_label, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(text_label);
    
    lv_scr_load(text_screen);
}

void LVGLScreens::showWiFiLoadingScreen() {
    if (wifi_loading_screen) {
        lv_obj_del(wifi_loading_screen);
    }
    wifi_loading_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(wifi_loading_screen, UI_COLOR_DARK, 0);
    lv_obj_t* loading_label = lv_label_create(wifi_loading_screen);
    lv_label_set_text(loading_label, "Connecting to WiFi...\nPlease wait");
    lv_obj_set_style_text_color(loading_label, UI_COLOR_PRIMARY, 0);
    lv_obj_set_style_text_align(loading_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(loading_label);
    lv_scr_load(wifi_loading_screen);
}

void LVGLScreens::hideWiFiLoadingScreen() {
    if (wifi_loading_screen) {
        lv_obj_del(wifi_loading_screen);
        wifi_loading_screen = nullptr;
    }
}

void LVGLScreens::showClockScreen() {
    if (isClockMode) return; // Already in clock mode

    isClockMode = true;

    // Create clock screen if it doesn't exist
    if (!clock_screen) {
        // Use the root screen as the parent
        clock_screen = lv_obj_create(NULL);
        // Remove all styles and set black background for all parts and states
        lv_obj_remove_style_all(clock_screen);
        lv_obj_set_size(clock_screen, 320, 240);
        lv_obj_set_style_bg_color(clock_screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(clock_screen, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(clock_screen, lv_color_hex(0x000000), LV_PART_ANY | LV_STATE_ANY);
        lv_obj_set_style_bg_opa(clock_screen, LV_OPA_100, LV_PART_ANY | LV_STATE_ANY);
        lv_obj_set_style_border_width(clock_screen, 0, LV_PART_ANY | LV_STATE_ANY);
        lv_obj_set_style_outline_width(clock_screen, 0, LV_PART_ANY | LV_STATE_ANY);
        lv_obj_set_style_pad_all(clock_screen, 0, LV_PART_ANY | LV_STATE_ANY);
        // Load the screen immediately so it becomes the root
        lv_scr_load(clock_screen);

        // AM/PM indicator - white, vertically centered next to hour
        clock_ampm_label = lv_label_create(clock_screen);
        lv_obj_set_style_text_color(clock_ampm_label, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_text_font(clock_ampm_label, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_align(clock_ampm_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_letter_space(clock_ampm_label, 2, 0);
        lv_obj_set_style_bg_opa(clock_ampm_label, LV_OPA_0, 0);
        lv_obj_set_style_border_width(clock_ampm_label, 0, 0);
        lv_obj_align(clock_ampm_label, LV_ALIGN_LEFT_MID, 20, 0); // Left, vertically centered
        
        // Seconds label - below main time, centered
        clock_seconds_label = lv_label_create(clock_screen);
        lv_obj_set_style_text_color(clock_seconds_label, lv_color_hex(0xAAAAAA), 0);
        lv_obj_set_style_text_font(clock_seconds_label, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_align(clock_seconds_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_bg_opa(clock_seconds_label, LV_OPA_0, 0);
        lv_obj_set_style_border_width(clock_seconds_label, 0, 0);
        lv_obj_align(clock_seconds_label, LV_ALIGN_CENTER, -40, 50); // Below main time, shifted left

        // MAIN DIGITAL TIME - Maximum size and impact, white, centered, 7-segment font
        clock_time_label = lv_label_create(clock_screen);
        lv_obj_set_size(clock_time_label, 300, 120); // Nearly full width, very large height
        lv_obj_set_style_text_color(clock_time_label, lv_color_hex(0xFFFFFF), 0);
        // Use the digital 7-segment font (replace placeholder with real font file!)
        lv_obj_set_style_text_font(clock_time_label, &lv_font_digital_80, 0);
        lv_obj_set_style_text_align(clock_time_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_bg_opa(clock_time_label, LV_OPA_0, 0);
        lv_obj_set_style_border_width(clock_time_label, 0, 0);
        lv_obj_set_style_pad_all(clock_time_label, 0, 0);
        lv_obj_set_style_text_letter_space(clock_time_label, 2, 0); // Minimal spacing, font is already wide
        lv_obj_align(clock_time_label, LV_ALIGN_CENTER, 0, -20); // Centered, shifted slightly left
        // NOTE: You must generate and add a real 7-segment font file for this to work!

        // Date display - subtle, below time
        clock_date_label = lv_label_create(clock_screen);
        lv_obj_set_style_text_color(clock_date_label, lv_color_hex(0x444444), 0);
        lv_obj_set_style_text_font(clock_date_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_align(clock_date_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_letter_space(clock_date_label, 2, 0);
        lv_obj_set_style_bg_opa(clock_date_label, LV_OPA_0, 0);
        lv_obj_set_style_border_width(clock_date_label, 0, 0);
        lv_obj_align(clock_date_label, LV_ALIGN_CENTER, 0, 60);

        // Status message - very dim, at bottom
        clock_status_label = lv_label_create(clock_screen);
        lv_obj_set_style_text_color(clock_status_label, lv_color_hex(0x222222), 0);
        lv_obj_set_style_text_font(clock_status_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_align(clock_status_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_bg_opa(clock_status_label, LV_OPA_0, 0);
        lv_obj_set_style_border_width(clock_status_label, 0, 0);
        lv_label_set_text(clock_status_label, "WAITING FOR DATA");
        lv_obj_align(clock_status_label, LV_ALIGN_BOTTOM_MID, 0, -5);
    }

    // Update time immediately
    updateClockScreen(nullptr);

    // Load the clock screen
    lv_scr_load(clock_screen);

    Serial.println("Digital clock activated - pure black background with white digits");
}

void LVGLScreens::updateClockScreen(NetworkManager* networkManager) {
    if (!isClockMode || !clock_time_label || !clock_date_label || !clock_ampm_label) return;
    
    if (networkManager && networkManager->isTimeSync()) {
        // Use real 12-hour time from NTP with digital styling
        String timeFull = networkManager->getCurrentTime12Hour(); // e.g. 01:59:30
        String dateStr = networkManager->getCurrentDate();
        String ampmStr = networkManager->getAMPM();
        
        // Split time into HH:MM and SS
        int lastColon = timeFull.lastIndexOf(':');
        String timeMain = timeFull;
        String seconds = "00";
        if (lastColon > 0) {
            timeMain = timeFull.substring(0, lastColon); // HH:MM
            seconds = timeFull.substring(lastColon + 1); // SS
        }
        lv_label_set_text(clock_time_label, timeMain.c_str());
        lv_label_set_text(clock_seconds_label, seconds.c_str());
        lv_label_set_text(clock_date_label, dateStr.c_str());
        lv_label_set_text(clock_ampm_label, ampmStr.c_str());
    } else {
        // Fallback to millis-based 12-hour time with digital styling
        unsigned long currentTime = millis();
        unsigned long totalSeconds = (currentTime / 1000) % 86400;
        unsigned long hours24 = (totalSeconds / 3600) % 24;
        unsigned long minutes = (totalSeconds / 60) % 60;
        unsigned long seconds = totalSeconds % 60;
        unsigned long hours12 = hours24 == 0 ? 12 : (hours24 > 12 ? hours24 - 12 : hours24);
        String ampm = hours24 < 12 ? "AM" : "PM";
        char timeStr[10];
        snprintf(timeStr, sizeof(timeStr), "%lu:%02lu", hours12, minutes);
        char secStr[4];
        snprintf(secStr, sizeof(secStr), "%02lu", seconds);
        lv_label_set_text(clock_time_label, timeStr);
        lv_label_set_text(clock_seconds_label, secStr);
        lv_label_set_text(clock_ampm_label, ampm.c_str());
        char dateStr[32];
        if (networkManager && !networkManager->isTimeSync()) {
            snprintf(dateStr, sizeof(dateStr), "SYNC...");
        } else {
            unsigned long days = currentTime / 86400000;
            snprintf(dateStr, sizeof(dateStr), "DAY %lu", days + 1);
        }
        lv_label_set_text(clock_date_label, dateStr);
    }
    
    // Update status with digital styling
    if (lastDataReceived > 0) {
        unsigned long currentTime = millis();
        unsigned long timeSinceData = (currentTime - lastDataReceived) / 1000;
        char statusStr[64];
        snprintf(statusStr, sizeof(statusStr), "NO DATA • %lu SEC", timeSinceData);
        lv_label_set_text(clock_status_label, statusStr);
    } else {
        lv_label_set_text(clock_status_label, "WAITING FOR DATA");
    }
}

bool LVGLScreens::hasRecentData() {
    if (lastDataReceived == 0) return false;
    
    unsigned long currentTime = millis();
    unsigned long timeSinceData = currentTime - lastDataReceived;

    // Consider data "recent" if received within last 10 seconds
    return timeSinceData < 10000;
}

void LVGLScreens::setDataReceived() {
    lastDataReceived = millis();
    
    // If we were in clock mode, we can return to main UI (handled by caller)
    if (isClockMode) {
        isClockMode = false;
        Serial.println("Data received - ready to return to main UI");
    }
}

void LVGLScreens::checkDataTimeout() {
    // Only check timeout if we're not already in clock mode and we have received data before
    if (!isClockMode && lastDataReceived > 0 && !hasRecentData()) {
        Serial.println("Data timeout detected - switching to clock screen");
        showClockScreen();
    }
}
