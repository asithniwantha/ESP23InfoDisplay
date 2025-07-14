#include "LVGLUIManager.h"
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
    using namespace UILayout;
    
    status_bar = lv_obj_create(main_screen);
    lv_obj_set_size(status_bar, SCREEN_WIDTH, STATUS_BAR_HEIGHT);
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
    using namespace UILayout;
    
    struct CardConfig {
        lv_obj_t** card;
        lv_obj_t** label;
        const char* title;
        const char* initial_text;
        int x_offset;
    };
    
    CardConfig cards[] = {
        {&cpu_card, &cpu_label, "CPU", "0.0%", 0},
        {&ram_card, &ram_label, "RAM", "0.0%", 1},
        {&disk_card, &disk_label, "DISK", "0.0%", 2},
        {&temp_card, &temp_label, "TEMP", "0°C", 3}
    };
    
    for (auto& config : cards) {
        int x_pos = CARD_START_X + config.x_offset * (CARD_WIDTH + CARD_SPACING);
        *config.card = createCard(main_screen, x_pos, CARD_START_Y, CARD_WIDTH, CARD_HEIGHT, config.title);
        *config.label = lv_label_create(*config.card);
        lv_label_set_text(*config.label, config.initial_text);
        lv_obj_set_style_text_color(*config.label, UI_COLOR_TEXT, 0);
        lv_obj_align(*config.label, LV_ALIGN_CENTER, 0, 8);
    }
}

void LVGLUIManager::createProgressBars() {
    using namespace UILayout;
    
    struct BarConfig {
        lv_obj_t** bar;
        int x_offset;
    };
    
    BarConfig bars[] = {
        {&cpu_bar, 0},
        {&ram_bar, 1},
        {&disk_bar, 2},
        {&temp_bar, 3}
    };
    
    for (auto& config : bars) {
        int x_pos = CARD_START_X + config.x_offset * (PROGRESS_BAR_WIDTH + CARD_SPACING);
        *config.bar = lv_bar_create(main_screen);
        lv_obj_set_size(*config.bar, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT);
        lv_obj_set_pos(*config.bar, x_pos, PROGRESS_BAR_Y);
        lv_obj_add_style(*config.bar, &style_progress, 0);
        lv_bar_set_value(*config.bar, 0, LV_ANIM_OFF);
    }
}

void LVGLUIManager::createInfoCards() {
    using namespace UILayout;
    
    // Network Card
    network_card = createCard(main_screen, CARD_START_X, INFO_CARD_Y, INFO_CARD_WIDTH, INFO_CARD_HEIGHT, "Net");
    network_label = lv_label_create(network_card);
    lv_label_set_text(network_label, "0KB/s");
    lv_obj_set_style_text_color(network_label, UI_COLOR_TEXT, 0);
    lv_obj_align(network_label, LV_ALIGN_CENTER, 0, 6);
    
    // Volume Card
    volume_card = createCard(main_screen, CARD_START_X + (INFO_CARD_WIDTH + INFO_CARD_SPACING), INFO_CARD_Y, INFO_CARD_WIDTH, INFO_CARD_HEIGHT, "Vol");
    volume_label = lv_label_create(volume_card);
    lv_label_set_text(volume_label, "0%");
    lv_obj_set_style_text_color(volume_label, UI_COLOR_TEXT, 0);
    lv_obj_align(volume_label, LV_ALIGN_CENTER, 0, 6);
    
    // Time Label
    time_label = lv_label_create(main_screen);
    lv_label_set_text(time_label, "Waiting for data...");
    lv_obj_set_style_text_color(time_label, UI_COLOR_MUTED, 0);
    lv_obj_set_pos(time_label, CARD_START_X + 250, INFO_CARD_Y + 8);
}

void LVGLUIManager::createCharts() {
    using namespace UILayout;
    
    const int icon_x = CARD_START_X + CHART_WIDTH + CHART_ICON_OFFSET;
    
    // CPU Chart
    cpu_chart = createStandardChart(CHART_START_Y, UI_COLOR_SUCCESS);
    cpu_series = lv_chart_add_series(cpu_chart, UI_COLOR_SUCCESS, LV_CHART_AXIS_PRIMARY_Y);
    createChartIcon(icon_x, CHART_START_Y + 4, LV_SYMBOL_SETTINGS, UI_COLOR_SUCCESS);
    
    // RAM Chart
    ram_chart = createStandardChart(CHART_START_Y + CHART_SPACING, UI_COLOR_SUCCESS);
    ram_series = lv_chart_add_series(ram_chart, UI_COLOR_SUCCESS, LV_CHART_AXIS_PRIMARY_Y);
    createChartIcon(icon_x, CHART_START_Y + CHART_SPACING + 4, LV_SYMBOL_LIST, UI_COLOR_SUCCESS);
    
    // Temperature Chart
    temp_chart = createStandardChart(CHART_START_Y + 2 * CHART_SPACING, UI_COLOR_DANGER);
    temp_series = lv_chart_add_series(temp_chart, UI_COLOR_DANGER, LV_CHART_AXIS_PRIMARY_Y);
    createChartIcon(icon_x, CHART_START_Y + 2 * CHART_SPACING + 4, LV_SYMBOL_WARNING, UI_COLOR_DANGER);
}

lv_obj_t* LVGLUIManager::createStandardChart(lv_coord_t y_pos, lv_color_t color) {
    using namespace UILayout;
    
    lv_obj_t* chart = lv_chart_create(main_screen);
    lv_obj_set_size(chart, CHART_WIDTH, CHART_HEIGHT);
    lv_obj_set_pos(chart, CARD_START_X, y_pos);
    lv_obj_add_style(chart, &style_chart, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart, CHART_POINT_COUNT);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_obj_set_style_line_width(chart, 2, LV_PART_ITEMS);
    lv_obj_set_style_size(chart, 3, LV_PART_INDICATOR);
    
    return chart;
}

lv_obj_t* LVGLUIManager::createChartIcon(lv_coord_t x, lv_coord_t y, const char* symbol, lv_color_t color) {
    lv_obj_t* icon = lv_label_create(main_screen);
    lv_label_set_text(icon, symbol);
    lv_obj_set_style_text_color(icon, color, 0);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_16, 0);
    lv_obj_set_pos(icon, x, y);
    return icon;
}

lv_color_t LVGLUIManager::getColorForValue(float value, int warning_threshold, int danger_threshold) {
    if (value > danger_threshold) return UI_COLOR_DANGER;
    if (value > warning_threshold) return UI_COLOR_WARNING;
    return UI_COLOR_SUCCESS;
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
    // Update main charts if in normal mode
    if (!isFullScreenMode) {
        lv_chart_set_next_value(cpu_chart, cpu_series, cpu_val);
        lv_chart_set_next_value(ram_chart, ram_series, ram_val);
        lv_chart_set_next_value(temp_chart, temp_series, temp_val);
        
        // Refresh all charts at once
        lv_chart_refresh(cpu_chart);
        lv_chart_refresh(ram_chart);
        lv_chart_refresh(temp_chart);
    } else {
        // Update full screen chart if in full screen mode
        if (currentFullScreenChart == 0) {
            lv_chart_set_next_value(fullscreen_chart, fullscreen_series, cpu_val);
        } else if (currentFullScreenChart == 1) {
            lv_chart_set_next_value(fullscreen_chart, fullscreen_series, ram_val);
        } else if (currentFullScreenChart == 2) {
            lv_chart_set_next_value(fullscreen_chart, fullscreen_series, temp_val);
        }
        lv_chart_refresh(fullscreen_chart);
    }
}

void LVGLUIManager::updateUI(SystemData& data) {
    if (!ui_initialized) return;
    
    // Make sure we're on the main screen
    lv_scr_load(main_screen);
    
    SystemStats& stats = data.getCurrentData();
    
    // Check if we need a force update
    unsigned long currentTime = millis();
    bool forceUpdate = (currentTime - lastForceUpdate) >= SystemThresholds::FORCE_UPDATE_INTERVAL;
    if (forceUpdate) {
        lastForceUpdate = currentTime;
    }
    
    // Track if any charts need updating
    bool charts_updated = false;
    int32_t cpu_chart_val = (int32_t)stats.cpuUsage;
    int32_t ram_chart_val = (int32_t)stats.ramUsage;
    int32_t temp_chart_val = stats.cpuTemp;
    
    // Update CPU
    if (abs(stats.cpuUsage - lastStats.cpuUsage) > SystemThresholds::CPU_UPDATE_THRESHOLD || forceUpdate) {
        char cpu_text[10];
        snprintf(cpu_text, sizeof(cpu_text), "%.1f%%", stats.cpuUsage);
        lv_color_t cpu_color = getColorForValue(stats.cpuUsage, SystemThresholds::WARNING_LEVEL, SystemThresholds::DANGER_LEVEL);
        updateCard(cpu_card, cpu_label, cpu_text, cpu_color);
        updateProgressBar(cpu_bar, (int32_t)stats.cpuUsage, cpu_color);
        charts_updated = true;
    }
    
    // Update RAM
    if (abs(stats.ramUsage - lastStats.ramUsage) > SystemThresholds::RAM_UPDATE_THRESHOLD || forceUpdate) {
        char ram_text[10];
        snprintf(ram_text, sizeof(ram_text), "%.1f%%", stats.ramUsage);
        lv_color_t ram_color = getColorForValue(stats.ramUsage, SystemThresholds::WARNING_LEVEL, SystemThresholds::DANGER_LEVEL);
        updateCard(ram_card, ram_label, ram_text, ram_color);
        updateProgressBar(ram_bar, (int32_t)stats.ramUsage, ram_color);
        charts_updated = true;
    }
    
    // Update DISK
    if (abs(stats.diskUsage - lastStats.diskUsage) > SystemThresholds::DISK_UPDATE_THRESHOLD || lastStats.diskUsage == 0 || forceUpdate) {
        char disk_text[10];
        snprintf(disk_text, sizeof(disk_text), "%.1f%%", stats.diskUsage);
        lv_color_t disk_color = getColorForValue(stats.diskUsage, SystemThresholds::WARNING_LEVEL, SystemThresholds::DANGER_LEVEL);
        updateCard(disk_card, disk_label, disk_text, disk_color);
        updateProgressBar(disk_bar, (int32_t)stats.diskUsage, disk_color);
    }
    
    // Update Temperature
    if (stats.cpuTemp != lastStats.cpuTemp || forceUpdate) {
        char temp_text[10];
        snprintf(temp_text, sizeof(temp_text), "%d°C", stats.cpuTemp);
        lv_color_t temp_color = getColorForValue(stats.cpuTemp, SystemThresholds::TEMP_WARNING_LEVEL, SystemThresholds::TEMP_DANGER_LEVEL);
        updateCard(temp_card, temp_label, temp_text, temp_color);
        updateProgressBar(temp_bar, stats.cpuTemp, temp_color);
        charts_updated = true;
    }
    
    // Update all charts simultaneously if any data changed
    if (charts_updated || forceUpdate) {
        updateAllCharts(cpu_chart_val, ram_chart_val, temp_chart_val);
    }
    
    // Update Network
    if (abs(stats.networkSpeed - lastStats.networkSpeed) > SystemThresholds::NETWORK_UPDATE_THRESHOLD || 
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

void LVGLUIManager::handleTouch(int x, int y) {
    if (!ui_initialized) return;
    
    using namespace TouchZones;
    using namespace UILayout;
    
    // If in full screen mode, return to main UI
    if (isFullScreenMode) {
        returnToMainUI();
        return;
    }
    
    // Chart touch detection
    if (x >= CARD_START_X && x <= (CARD_START_X + CHART_WIDTH)) {
        if (y >= CPU_CHART_Y_MIN && y <= CPU_CHART_Y_MAX) {
            showFullScreenChart(static_cast<int>(ChartType::CPU));
        } else if (y >= RAM_CHART_Y_MIN && y <= RAM_CHART_Y_MAX) {
            showFullScreenChart(static_cast<int>(ChartType::RAM));
        } else if (y >= TEMP_CHART_Y_MIN && y <= TEMP_CHART_Y_MAX) {
            showFullScreenChart(static_cast<int>(ChartType::TEMPERATURE));
        }
    }
    
    // Card touch detection
    else if (y >= CARD_START_Y && y <= CARD_END_Y) {
        if (x >= CPU_CARD_X_MIN && x <= CPU_CARD_X_MAX) {
            showFullScreenChart(static_cast<int>(ChartType::CPU));
        } else if (x >= RAM_CARD_X_MIN && x <= RAM_CARD_X_MAX) {
            showFullScreenChart(static_cast<int>(ChartType::RAM));
        } else if (x >= DISK_CARD_X_MIN && x <= DISK_CARD_X_MAX) {
            showFullScreenChart(static_cast<int>(ChartType::DISK));
        } else if (x >= TEMP_CARD_X_MIN && x <= TEMP_CARD_X_MAX) {
            showFullScreenChart(static_cast<int>(ChartType::TEMPERATURE));
        }
    }
}

void LVGLUIManager::showFullScreenChart(int chartType) {
    if (!ui_initialized) return;
    
    isFullScreenMode = true;
    currentFullScreenChart = chartType;
    
    // Clear main screen
    lv_obj_clean(main_screen);
    
    // Create full screen chart
    fullscreen_chart = lv_chart_create(main_screen);
    lv_obj_set_size(fullscreen_chart, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 60);
    lv_obj_set_pos(fullscreen_chart, 10, 30);
    lv_obj_add_style(fullscreen_chart, &style_chart, 0);
    lv_chart_set_type(fullscreen_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(fullscreen_chart, 50);
    lv_chart_set_update_mode(fullscreen_chart, LV_CHART_UPDATE_MODE_SHIFT);
    
    // Create title and configure based on chart type
    lv_obj_t* title_label = lv_label_create(main_screen);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_16, 0);
    lv_obj_set_pos(title_label, 10, 5);
    
    switch(chartType) {
        case 0: // CPU
            lv_label_set_text(title_label, LV_SYMBOL_SETTINGS " CPU Usage History");
            lv_obj_set_style_text_color(title_label, UI_COLOR_SUCCESS, 0);
            lv_chart_set_range(fullscreen_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
            fullscreen_series = lv_chart_add_series(fullscreen_chart, UI_COLOR_SUCCESS, LV_CHART_AXIS_PRIMARY_Y);
            break;
        case 1: // RAM
            lv_label_set_text(title_label, LV_SYMBOL_LIST " RAM Usage History");
            lv_obj_set_style_text_color(title_label, UI_COLOR_SUCCESS, 0);
            lv_chart_set_range(fullscreen_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
            fullscreen_series = lv_chart_add_series(fullscreen_chart, UI_COLOR_SUCCESS, LV_CHART_AXIS_PRIMARY_Y);
            break;
        case 2: // Temperature
            lv_label_set_text(title_label, LV_SYMBOL_WARNING " Temperature History");
            lv_obj_set_style_text_color(title_label, UI_COLOR_DANGER, 0);
            lv_chart_set_range(fullscreen_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
            fullscreen_series = lv_chart_add_series(fullscreen_chart, UI_COLOR_DANGER, LV_CHART_AXIS_PRIMARY_Y);
            break;
        case 3: // Disk
            lv_label_set_text(title_label, LV_SYMBOL_DRIVE " Disk Usage");
            lv_obj_set_style_text_color(title_label, UI_COLOR_WARNING, 0);
            lv_chart_set_range(fullscreen_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
            fullscreen_series = lv_chart_add_series(fullscreen_chart, UI_COLOR_WARNING, LV_CHART_AXIS_PRIMARY_Y);
            break;
    }
    
    // Add instructions
    lv_obj_t* instruction_label = lv_label_create(main_screen);
    lv_label_set_text(instruction_label, "Touch anywhere to return");
    lv_obj_set_style_text_color(instruction_label, UI_COLOR_MUTED, 0);
    lv_obj_set_style_text_font(instruction_label, &lv_font_montserrat_12, 0);
    lv_obj_set_pos(instruction_label, SCREEN_WIDTH - 150, 5);
    
    // Copy existing chart data if available (simplified approach)
    // Initialize with some sample data for demonstration
    for (int i = 0; i < 20; i++) {
        lv_chart_set_next_value(fullscreen_chart, fullscreen_series, 0);
    }
    
    lv_scr_load(main_screen);
}

void LVGLUIManager::returnToMainUI() {
    if (!isFullScreenMode) return;
    
    isFullScreenMode = false;
    currentFullScreenChart = -1;
    
    // Recreate the main UI
    lv_obj_clean(main_screen);
    createStatusBar();
    createSystemCards();
    createProgressBars();
    createInfoCards();
    createCharts();
    
    lv_scr_load(main_screen);
}
