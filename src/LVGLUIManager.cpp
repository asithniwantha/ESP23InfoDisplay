#include "LVGLUIManager.h"

// Modern color palette
#define UI_COLOR_PRIMARY   lv_color_hex(0x2196F3)  // Blue
#define UI_COLOR_SUCCESS   lv_color_hex(0x4CAF50)  // Green
#define UI_COLOR_WARNING   lv_color_hex(0xFF9800)  // Orange
#define UI_COLOR_DANGER    lv_color_hex(0xF44336)  // Red
#define UI_COLOR_INFO      lv_color_hex(0x00BCD4)  // Cyan
#define UI_COLOR_DARK      lv_color_hex(0x1A1A1A)  // Dark background
#define UI_COLOR_CARD      lv_color_hex(0x2D2D2D)  // Card background
#define UI_COLOR_TEXT      lv_color_hex(0xFFFFFF)  // White text
#define UI_COLOR_MUTED     lv_color_hex(0x888888)  // Gray text

LVGLUIManager::LVGLUIManager() {
    main_screen = nullptr;
    startup_screen = nullptr;
    ui_initialized = false;
}

void LVGLUIManager::init() {
    // Initialize styles
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
    
    createMainScreen();
}

void LVGLUIManager::createMainScreen() {
    main_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(main_screen, UI_COLOR_DARK, 0);
    
    createStatusBar();
    createSystemCards();
    createProgressBars();
    createInfoCards();
    createCharts();
    
    ui_initialized = true;
}

void LVGLUIManager::createStatusBar() {
    status_bar = lv_obj_create(main_screen);
    lv_obj_set_size(status_bar, SCREEN_WIDTH, 20);
    lv_obj_set_pos(status_bar, 0, 0);
    lv_obj_set_style_bg_color(status_bar, UI_COLOR_PRIMARY, 0);
    lv_obj_set_style_radius(status_bar, 0, 0);
    lv_obj_set_style_border_width(status_bar, 0, 0);
    lv_obj_set_style_pad_all(status_bar, 0, 0);
    
    lv_obj_t* status_label = lv_label_create(status_bar);
    lv_label_set_text(status_label, "ESP32 System Monitor");
    lv_obj_set_style_text_color(status_label, UI_COLOR_TEXT, 0);
    lv_obj_center(status_label);
}

void LVGLUIManager::createSystemCards() {
    const int card_width = 75;
    const int card_height = 35;
    const int card_spacing = 5;
    const int start_x = 5;
    const int start_y = 25;
    
    // CPU Card
    cpu_card = createCard(main_screen, start_x, start_y, card_width, card_height, "CPU");
    cpu_label = lv_label_create(cpu_card);
    lv_label_set_text(cpu_label, "0.0%");
    lv_obj_set_style_text_color(cpu_label, UI_COLOR_TEXT, 0);
    lv_obj_align(cpu_label, LV_ALIGN_CENTER, 0, 8);
    
    // RAM Card
    ram_card = createCard(main_screen, start_x + (card_width + card_spacing), start_y, card_width, card_height, "RAM");
    ram_label = lv_label_create(ram_card);
    lv_label_set_text(ram_label, "0.0%");
    lv_obj_set_style_text_color(ram_label, UI_COLOR_TEXT, 0);
    lv_obj_align(ram_label, LV_ALIGN_CENTER, 0, 8);
    
    // DISK Card
    disk_card = createCard(main_screen, start_x + 2 * (card_width + card_spacing), start_y, card_width, card_height, "DISK");
    disk_label = lv_label_create(disk_card);
    lv_label_set_text(disk_label, "0.0%");
    lv_obj_set_style_text_color(disk_label, UI_COLOR_TEXT, 0);
    lv_obj_align(disk_label, LV_ALIGN_CENTER, 0, 8);
    
    // TEMP Card
    temp_card = createCard(main_screen, start_x + 3 * (card_width + card_spacing), start_y, card_width, card_height, "TEMP");
    temp_label = lv_label_create(temp_card);
    lv_label_set_text(temp_label, "0°C");
    lv_obj_set_style_text_color(temp_label, UI_COLOR_TEXT, 0);
    lv_obj_align(temp_label, LV_ALIGN_CENTER, 0, 8);
}

void LVGLUIManager::createProgressBars() {
    const int bar_width = 75;
    const int bar_height = 8;
    const int bar_spacing = 5;
    const int start_x = 5;
    const int start_y = 65;
    
    // CPU Progress Bar
    cpu_bar = lv_bar_create(main_screen);
    lv_obj_set_size(cpu_bar, bar_width, bar_height);
    lv_obj_set_pos(cpu_bar, start_x, start_y);
    lv_obj_add_style(cpu_bar, &style_progress, 0);
    lv_bar_set_value(cpu_bar, 0, LV_ANIM_OFF);
    
    // RAM Progress Bar
    ram_bar = lv_bar_create(main_screen);
    lv_obj_set_size(ram_bar, bar_width, bar_height);
    lv_obj_set_pos(ram_bar, start_x + (bar_width + bar_spacing), start_y);
    lv_obj_add_style(ram_bar, &style_progress, 0);
    lv_bar_set_value(ram_bar, 0, LV_ANIM_OFF);
    
    // DISK Progress Bar
    disk_bar = lv_bar_create(main_screen);
    lv_obj_set_size(disk_bar, bar_width, bar_height);
    lv_obj_set_pos(disk_bar, start_x + 2 * (bar_width + bar_spacing), start_y);
    lv_obj_add_style(disk_bar, &style_progress, 0);
    lv_bar_set_value(disk_bar, 0, LV_ANIM_OFF);
    
    // TEMP Progress Bar
    temp_bar = lv_bar_create(main_screen);
    lv_obj_set_size(temp_bar, bar_width, bar_height);
    lv_obj_set_pos(temp_bar, start_x + 3 * (bar_width + bar_spacing), start_y);
    lv_obj_add_style(temp_bar, &style_progress, 0);
    lv_bar_set_value(temp_bar, 0, LV_ANIM_OFF);
}

void LVGLUIManager::createInfoCards() {
    const int card_width = 120;
    const int card_height = 30;
    const int card_spacing = 8;
    const int start_x = 5;
    const int start_y = 85;
    
    // Network Card
    network_card = createCard(main_screen, start_x, start_y, card_width, card_height, "Net");
    network_label = lv_label_create(network_card);
    lv_label_set_text(network_label, "0KB/s");
    lv_obj_set_style_text_color(network_label, UI_COLOR_TEXT, 0);
    lv_obj_align(network_label, LV_ALIGN_CENTER, 0, 6);
    
    // Volume Card
    volume_card = createCard(main_screen, start_x + (card_width + card_spacing), start_y, card_width, card_height, "Vol");
    volume_label = lv_label_create(volume_card);
    lv_label_set_text(volume_label, "0%");
    lv_obj_set_style_text_color(volume_label, UI_COLOR_TEXT, 0);
    lv_obj_align(volume_label, LV_ALIGN_CENTER, 0, 6);
    
    // Time Label
    time_label = lv_label_create(main_screen);
    lv_label_set_text(time_label, "Waiting for data...");
    lv_obj_set_style_text_color(time_label, UI_COLOR_MUTED, 0);
    lv_obj_set_pos(time_label, start_x + 250, start_y + 8);
}

void LVGLUIManager::createCharts() {
    const int chart_width = 275;
    const int chart_height = 25;
    const int chart_spacing = 32;
    const int start_x = 5;
    const int start_y = 125;
    
    // CPU Chart
    cpu_chart = lv_chart_create(main_screen);
    lv_obj_set_size(cpu_chart, chart_width, chart_height);
    lv_obj_set_pos(cpu_chart, start_x, start_y);
    lv_obj_add_style(cpu_chart, &style_chart, 0);
    lv_chart_set_type(cpu_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(cpu_chart, 50);
    lv_chart_set_range(cpu_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_update_mode(cpu_chart, LV_CHART_UPDATE_MODE_SHIFT);
    cpu_series = lv_chart_add_series(cpu_chart, UI_COLOR_SUCCESS, LV_CHART_AXIS_PRIMARY_Y);
    lv_obj_set_style_line_width(cpu_chart, 2, LV_PART_ITEMS);
    lv_obj_set_style_size(cpu_chart, 3, LV_PART_INDICATOR);
    
    // RAM Chart
    ram_chart = lv_chart_create(main_screen);
    lv_obj_set_size(ram_chart, chart_width, chart_height);
    lv_obj_set_pos(ram_chart, start_x, start_y + chart_spacing);
    lv_obj_add_style(ram_chart, &style_chart, 0);
    lv_chart_set_type(ram_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ram_chart, 50);
    lv_chart_set_range(ram_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_update_mode(ram_chart, LV_CHART_UPDATE_MODE_SHIFT);
    ram_series = lv_chart_add_series(ram_chart, UI_COLOR_SUCCESS, LV_CHART_AXIS_PRIMARY_Y);
    lv_obj_set_style_line_width(ram_chart, 2, LV_PART_ITEMS);
    lv_obj_set_style_size(ram_chart, 3, LV_PART_INDICATOR);
    
    // Temperature Chart
    temp_chart = lv_chart_create(main_screen);
    lv_obj_set_size(temp_chart, chart_width, chart_height);
    lv_obj_set_pos(temp_chart, start_x, start_y + 2 * chart_spacing);
    lv_obj_add_style(temp_chart, &style_chart, 0);
    lv_chart_set_type(temp_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(temp_chart, 50);
    lv_chart_set_range(temp_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_update_mode(temp_chart, LV_CHART_UPDATE_MODE_SHIFT);
    temp_series = lv_chart_add_series(temp_chart, UI_COLOR_DANGER, LV_CHART_AXIS_PRIMARY_Y);
    lv_obj_set_style_line_width(temp_chart, 2, LV_PART_ITEMS);
    lv_obj_set_style_size(temp_chart, 3, LV_PART_INDICATOR);
}

lv_obj_t* LVGLUIManager::createCard(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* title) {
    lv_obj_t* card = lv_obj_create(parent);
    lv_obj_set_size(card, w, h);
    lv_obj_set_pos(card, x, y);
    lv_obj_add_style(card, &style_card, 0);
    
    lv_obj_t* title_label = lv_label_create(card);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, UI_COLOR_MUTED, 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_12, 0);
    lv_obj_align(title_label, LV_ALIGN_CENTER, 0, -8);
    
    return card;
}

void LVGLUIManager::updateCard(lv_obj_t* card, lv_obj_t* label, const char* value, lv_color_t color) {
    lv_label_set_text(label, value);
    lv_obj_set_style_text_color(label, color, 0);
}

void LVGLUIManager::updateProgressBar(lv_obj_t* bar, int32_t value, lv_color_t color) {
    lv_bar_set_value(bar, value, LV_ANIM_ON);
    lv_obj_set_style_bg_color(bar, color, LV_PART_INDICATOR);
}

void LVGLUIManager::updateChart(lv_chart_series_t* series, int32_t value) {
    // Find the chart object from our stored chart references
    if (series == cpu_series) {
        lv_chart_set_next_value(cpu_chart, series, value);
    } else if (series == ram_series) {
        lv_chart_set_next_value(ram_chart, series, value);
    } else if (series == temp_series) {
        lv_chart_set_next_value(temp_chart, series, value);
    }
}

void LVGLUIManager::updateAllCharts(int32_t cpu_val, int32_t ram_val, int32_t temp_val) {
    // Update all charts simultaneously for smoother animation
    lv_chart_set_next_value(cpu_chart, cpu_series, cpu_val);
    lv_chart_set_next_value(ram_chart, ram_series, ram_val);
    lv_chart_set_next_value(temp_chart, temp_series, temp_val);
    
    // Refresh all charts at once
    lv_chart_refresh(cpu_chart);
    lv_chart_refresh(ram_chart);
    lv_chart_refresh(temp_chart);
}

void LVGLUIManager::updateUI(SystemData& data) {
    if (!ui_initialized) return;
    
    // Make sure we're on the main screen
    lv_scr_load(main_screen);
    
    SystemStats& stats = data.getCurrentData();
    
    // Check if we need a force update (every 5 seconds)
    unsigned long currentTime = millis();
    bool forceUpdate = (currentTime - lastForceUpdate) >= FORCE_UPDATE_INTERVAL;
    if (forceUpdate) {
        lastForceUpdate = currentTime;
    }
    
    // Track if any charts need updating
    bool charts_updated = false;
    int32_t cpu_chart_val = (int32_t)stats.cpuUsage;
    int32_t ram_chart_val = (int32_t)stats.ramUsage;
    int32_t temp_chart_val = stats.cpuTemp;
    
    // Update CPU
    if (abs(stats.cpuUsage - lastStats.cpuUsage) > 0.5 || forceUpdate) {
        char cpu_text[10];
        snprintf(cpu_text, sizeof(cpu_text), "%.1f%%", stats.cpuUsage);
        lv_color_t cpu_color = stats.cpuUsage > 80 ? UI_COLOR_DANGER : 
                              stats.cpuUsage > 60 ? UI_COLOR_WARNING : UI_COLOR_SUCCESS;
        updateCard(cpu_card, cpu_label, cpu_text, cpu_color);
        updateProgressBar(cpu_bar, (int32_t)stats.cpuUsage, cpu_color);
        charts_updated = true;
    }
    
    // Update RAM
    if (abs(stats.ramUsage - lastStats.ramUsage) > 0.5 || forceUpdate) {
        char ram_text[10];
        snprintf(ram_text, sizeof(ram_text), "%.1f%%", stats.ramUsage);
        lv_color_t ram_color = stats.ramUsage > 80 ? UI_COLOR_DANGER : 
                              stats.ramUsage > 60 ? UI_COLOR_WARNING : UI_COLOR_SUCCESS;
        updateCard(ram_card, ram_label, ram_text, ram_color);
        updateProgressBar(ram_bar, (int32_t)stats.ramUsage, ram_color);
        charts_updated = true;
    }
    
    // Update DISK - reduce threshold for more frequent updates
    if (abs(stats.diskUsage - lastStats.diskUsage) > 0.1 || lastStats.diskUsage == 0 || forceUpdate) {
        char disk_text[10];
        snprintf(disk_text, sizeof(disk_text), "%.1f%%", stats.diskUsage);
        lv_color_t disk_color = stats.diskUsage > 80 ? UI_COLOR_DANGER : 
                               stats.diskUsage > 60 ? UI_COLOR_WARNING : UI_COLOR_SUCCESS;
        updateCard(disk_card, disk_label, disk_text, disk_color);
        updateProgressBar(disk_bar, (int32_t)stats.diskUsage, disk_color);
    }
    
    // Update Temperature
    if (stats.cpuTemp != lastStats.cpuTemp || forceUpdate) {
        char temp_text[10];
        snprintf(temp_text, sizeof(temp_text), "%d°C", stats.cpuTemp);
        lv_color_t temp_color = stats.cpuTemp > 70 ? UI_COLOR_DANGER : 
                               stats.cpuTemp > 50 ? UI_COLOR_WARNING : UI_COLOR_INFO;
        updateCard(temp_card, temp_label, temp_text, temp_color);
        updateProgressBar(temp_bar, stats.cpuTemp, temp_color);
        charts_updated = true;
    }
    
    // Update all charts simultaneously if any data changed
    if (charts_updated || forceUpdate) {
        updateAllCharts(cpu_chart_val, ram_chart_val, temp_chart_val);
    }
    
    // Update Network - reduce threshold and add force update condition
    if (abs(stats.networkSpeed - lastStats.networkSpeed) > 0.01 || 
        lastStats.networkSpeed == 0 || 
        stats.networkSpeed == 0 || 
        forceUpdate) {
        updateCard(network_card, network_label, data.getFormattedNetworkSpeed().c_str(), UI_COLOR_INFO);
    }
    
    // Update Volume
    if (stats.volumeLevel != lastStats.volumeLevel || forceUpdate) {
        char vol_text[10];
        snprintf(vol_text, sizeof(vol_text), "%d%%", stats.volumeLevel);
        updateCard(volume_card, volume_label, vol_text, UI_COLOR_PRIMARY);
    }
    
    // Update time
    if (stats.lastTime != lastStats.lastTime || forceUpdate) {
        String time_text = "Updated: " + stats.lastTime;
        lv_label_set_text(time_label, time_text.c_str());
    }
    
    lastStats = stats;
}

void LVGLUIManager::showStartupScreen() {
    if (startup_screen) {
        lv_obj_del(startup_screen);
    }
    
    startup_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(startup_screen, UI_COLOR_DARK, 0);
    
    lv_obj_t* logo_label = lv_label_create(startup_screen);
    lv_label_set_text(logo_label, "ESP32\nSystem Monitor");
    lv_obj_set_style_text_color(logo_label, UI_COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(logo_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(logo_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(logo_label);
    
    lv_obj_t* loading_spinner = lv_spinner_create(startup_screen, 1000, 60);
    lv_obj_set_size(loading_spinner, 40, 40);
    lv_obj_align(loading_spinner, LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_style_arc_color(loading_spinner, UI_COLOR_PRIMARY, LV_PART_INDICATOR);
    
    lv_scr_load(startup_screen);
}

void LVGLUIManager::showWiFiInfo(String ip, String mdns, int port) {
    if (main_screen) {
        lv_obj_clean(main_screen);
        
        lv_obj_t* wifi_label = lv_label_create(main_screen);
        String wifi_info = "WiFi Connected!\nIP: " + ip + "\nMDNS: " + mdns + "\nPort: " + String(port);
        lv_label_set_text(wifi_label, wifi_info.c_str());
        lv_obj_set_style_text_color(wifi_label, UI_COLOR_SUCCESS, 0);
        lv_obj_set_style_text_font(wifi_label, &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_align(wifi_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_center(wifi_label);
        
        lv_scr_load(main_screen);
    }
}

void LVGLUIManager::showUDPText(String text) {
    if (main_screen) {
        lv_obj_clean(main_screen);
        
        lv_obj_t* text_label = lv_label_create(main_screen);
        lv_label_set_text(text_label, text.c_str());
        lv_obj_set_style_text_color(text_label, UI_COLOR_TEXT, 0);
        lv_obj_set_style_text_font(text_label, &lv_font_montserrat_14, 0);
        lv_obj_center(text_label);
        
        lv_scr_load(main_screen);
    }
}

void LVGLUIManager::showTouchDebug(int x, int y, int z) {
    if (main_screen) {
        lv_obj_clean(main_screen);
        
        lv_obj_t* touch_label = lv_label_create(main_screen);
        String touch_info = "Touch Debug\nX: " + String(x) + "\nY: " + String(y) + "\nZ: " + String(z);
        lv_label_set_text(touch_label, touch_info.c_str());
        lv_obj_set_style_text_color(touch_label, UI_COLOR_INFO, 0);
        lv_obj_set_style_text_font(touch_label, &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_align(touch_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_center(touch_label);
        
        lv_scr_load(main_screen);
    }
}
