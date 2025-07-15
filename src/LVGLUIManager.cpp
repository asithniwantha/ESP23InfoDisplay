#include "LVGLUIManager.h"
#include "UIConfig.h"
#include "NetworkManager.h"

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
    clock_screen = nullptr;
    ui_initialized = false;
    lastDataReceived = 0;
    isClockMode = false;
}

void LVGLUIManager::init() {
    // (LVGL theme disabling removed for compatibility)
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
    
    // Create landscape layout
    createLandscapeUI();
    
    ui_initialized = true;
    
    // Don't show clock automatically - let main loop handle it
}

void LVGLUIManager::createLandscapeUI() {
    createTopRowCards();
    createCPUHistoryChart();
    createNetworkChart();
    createVolumeControl();
}

void LVGLUIManager::createTopRowCards() {
    using namespace UILayout;
    
    // Calculate exact positions for 4 cards to fit perfectly in 320px
    // Total available width: 320 - (2 * PADDING) = 308px
    // 4 cards + 3 gaps: 4 * CARD_WIDTH + 3 * GAP = 4*72 + 3*6 = 306px ✓
    int card_positions[4];
    for (int i = 0; i < 4; i++) {
        card_positions[i] = PADDING + (i * (CARD_WIDTH + GAP));
    }
    
    // CPU Card (Position: 6px)
    cpu_card = createCard(main_screen, card_positions[0], CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "CPU");
    cpu_bar = lv_bar_create(cpu_card);
    lv_obj_set_size(cpu_bar, CARD_WIDTH - 14, 8);
    lv_obj_align(cpu_bar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(cpu_bar, &style_progress, 0);
    cpu_label = lv_label_create(cpu_card);
    lv_label_set_text(cpu_label, "0%");
    lv_obj_align(cpu_label, LV_ALIGN_BOTTOM_MID, 0, -4);
    
    // RAM Card (Position: 84px)
    ram_card = createCard(main_screen, card_positions[1], CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "RAM");
    ram_bar = lv_bar_create(ram_card);
    lv_obj_set_size(ram_bar, CARD_WIDTH - 14, 8);
    lv_obj_align(ram_bar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(ram_bar, &style_progress, 0);
    ram_label = lv_label_create(ram_card);
    lv_label_set_text(ram_label, "0%");
    lv_obj_align(ram_label, LV_ALIGN_BOTTOM_MID, 0, -4);
    
    // Temperature Card (Position: 162px) - with gauge
    temp_card = createCard(main_screen, card_positions[2], CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "TEMP");
    createTemperatureGauge();
    
    // Disk Card (Position: 240px)
    disk_card = createCard(main_screen, card_positions[3], CARD_ROW_Y, CARD_WIDTH, CARD_HEIGHT, "DISK");
    disk_bar = lv_bar_create(disk_card);
    lv_obj_set_size(disk_bar, CARD_WIDTH - 14, 8);
    lv_obj_align(disk_bar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(disk_bar, &style_progress, 0);
    disk_label = lv_label_create(disk_card);
    lv_label_set_text(disk_label, "0%");
    lv_obj_align(disk_label, LV_ALIGN_BOTTOM_MID, 0, -4);
}

void LVGLUIManager::createCPUHistoryChart() {
    using namespace UILayout;
    
    // Create container for CPU history chart - perfectly centered
    lv_obj_t* chart_container = lv_obj_create(main_screen);
    lv_obj_set_size(chart_container, CPU_CHART_WIDTH, CPU_CHART_HEIGHT + 18);
    lv_obj_set_pos(chart_container, PADDING, CPU_CHART_Y);
    lv_obj_add_style(chart_container, &style_card, 0);
    
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
    lv_obj_add_style(cpu_history_chart, &style_chart, 0);
    
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

void LVGLUIManager::createNetworkChart() {
    using namespace UILayout;
    
    // Create container for network chart
    lv_obj_t* chart_container = lv_obj_create(main_screen);
    lv_obj_set_size(chart_container, BOTTOM_CARD_WIDTH, BOTTOM_CARD_HEIGHT);
    lv_obj_set_pos(chart_container, NETWORK_CHART_X, NETWORK_CHART_Y);
    lv_obj_add_style(chart_container, &style_card, 0);
    
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
    lv_obj_add_style(network_chart, &style_chart, 0);
    
    // Remove point dots by setting size to 0
    lv_obj_set_style_size(network_chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(network_chart, 2, LV_PART_ITEMS);
    
    network_series = lv_chart_add_series(network_chart, UI_COLOR_INFO, LV_CHART_AXIS_PRIMARY_Y);
}

void LVGLUIManager::createVolumeControl() {
    using namespace UILayout;
    
    // Create container for volume control
    volume_card = lv_obj_create(main_screen);
    lv_obj_set_size(volume_card, BOTTOM_CARD_WIDTH, BOTTOM_CARD_HEIGHT);
    lv_obj_set_pos(volume_card, VOLUME_CONTROL_X, VOLUME_CONTROL_Y);
    lv_obj_add_style(volume_card, &style_card, 0);
    
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

void LVGLUIManager::createTemperatureGauge() {
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

lv_obj_t* LVGLUIManager::createCard(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* title) {
    lv_obj_t* card = lv_obj_create(parent);
    lv_obj_set_size(card, w, h);
    lv_obj_set_pos(card, x, y);
    lv_obj_add_style(card, &style_card, 0);
    
    lv_obj_t* title_label = lv_label_create(card);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, UI_COLOR_MUTED, 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_12, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 5);
    
    return card;
}

// Core UI functionality methods

void LVGLUIManager::updateUI(SystemData& data) {
    if (!ui_initialized) return;
    
    // If we're in clock mode, don't update the main UI
    if (isClockMode) {
        updateClockScreen(nullptr); // Keep updating the clock
        return;
    }
    
    // Make sure we're on the main screen
    lv_scr_load(main_screen);
    
    SystemStats& stats = data.getCurrentData();
    
    // Check if we need a force update
    unsigned long currentTime = millis();
    bool forceUpdate = (currentTime - lastForceUpdate) >= SystemThresholds::FORCE_UPDATE_INTERVAL;
    if (forceUpdate) {
        lastForceUpdate = currentTime;
    }
    
    // Update CPU card and progress bar
    if (abs(stats.cpuUsage - lastStats.cpuUsage) > SystemThresholds::CPU_UPDATE_THRESHOLD || forceUpdate) {
        char cpu_text[10];
        snprintf(cpu_text, sizeof(cpu_text), "%.1f%%", stats.cpuUsage);
        lv_color_t cpu_color = getColorForValue(stats.cpuUsage, SystemThresholds::WARNING_LEVEL, SystemThresholds::DANGER_LEVEL);
        lv_label_set_text(cpu_label, cpu_text);
        updateProgressBar(cpu_bar, (int32_t)stats.cpuUsage, cpu_color);
    }
    
    // Update RAM card and progress bar
    if (abs(stats.ramUsage - lastStats.ramUsage) > SystemThresholds::RAM_UPDATE_THRESHOLD || forceUpdate) {
        char ram_text[10];
        snprintf(ram_text, sizeof(ram_text), "%.1f%%", stats.ramUsage);
        lv_color_t ram_color = getColorForValue(stats.ramUsage, SystemThresholds::WARNING_LEVEL, SystemThresholds::DANGER_LEVEL);
        lv_label_set_text(ram_label, ram_text);
        updateProgressBar(ram_bar, (int32_t)stats.ramUsage, ram_color);
    }
    
    // Update Disk card and progress bar
    if (abs(stats.diskUsage - lastStats.diskUsage) > SystemThresholds::DISK_UPDATE_THRESHOLD || forceUpdate) {
        char disk_text[10];
        snprintf(disk_text, sizeof(disk_text), "%.1f%%", stats.diskUsage);
        lv_color_t disk_color = getColorForValue(stats.diskUsage, SystemThresholds::WARNING_LEVEL, SystemThresholds::DANGER_LEVEL);
        lv_label_set_text(disk_label, disk_text);
        updateProgressBar(disk_bar, (int32_t)stats.diskUsage, disk_color);
    }
    
    // Update Temperature gauge
    if (stats.cpuTemp != lastStats.cpuTemp || forceUpdate) {
        updateTemperatureGauge(stats.cpuTemp);
    }
    
    // Update CPU history chart
    if (data.isHistoryFull() || forceUpdate) {
        updateCPUHistoryChart(data.getCpuHistory(), data.getHistoryIndex());
    }
    
    // Update Network chart and display
    if (abs(stats.networkSpeed - lastStats.networkSpeed) > SystemThresholds::NETWORK_UPDATE_THRESHOLD || forceUpdate) {
        updateNetworkChart(stats.networkSpeed);
        lv_label_set_text(network_label, data.getFormattedNetworkSpeed().c_str());
    }
    
    // Update Volume control
    if (stats.volumeLevel != lastStats.volumeLevel || forceUpdate) {
        updateVolumeSlider(stats.volumeLevel);
    }
    
    lastStats = stats;
}

// Update helper methods
void LVGLUIManager::updateProgressBar(lv_obj_t* bar, int32_t value, lv_color_t color) {
    if (bar) {
        lv_bar_set_value(bar, value, LV_ANIM_ON);
        lv_obj_set_style_bg_color(bar, color, LV_PART_INDICATOR);
    }
}

void LVGLUIManager::updateCPUHistoryChart(float* history, int historySize) {
    if (!cpu_history_series || !history) return;
    
    // Task Manager style scrolling: add newest point and shift left
    // This creates a smooth scrolling effect like Windows Task Manager
    lv_chart_set_next_value(cpu_history_chart, cpu_history_series, (int32_t)history[historySize - 1]);
    lv_chart_refresh(cpu_history_chart);
}

void LVGLUIManager::updateNetworkChart(float speed) {
    if (!network_series) return;
    
    // Add new data point to network chart
    lv_chart_set_next_value(network_chart, network_series, (int32_t)speed);
    lv_chart_refresh(network_chart);
}

void LVGLUIManager::updateVolumeSlider(int volume) {
    if (!volume_slider || !volume_label) return;
    
    // Update slider position and label
    lv_slider_set_value(volume_slider, volume, LV_ANIM_ON);
    
    char vol_text[10];
    snprintf(vol_text, sizeof(vol_text), "%d%%", volume);
    lv_label_set_text(volume_label, vol_text);
}

void LVGLUIManager::updateTemperatureGauge(int temp) {
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

lv_color_t LVGLUIManager::getColorForValue(float value, int warning_threshold, int danger_threshold) {
    if (value >= danger_threshold) {
        return UI_COLOR_DANGER;
    } else if (value >= warning_threshold) {
        return UI_COLOR_WARNING;
    } else {
        return UI_COLOR_SUCCESS;
    }
}

// Startup and display methods
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
    
    lv_scr_load(startup_screen);
}

void LVGLUIManager::showWiFiInfo(String ip, String mdns, int port) {
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

void LVGLUIManager::showUDPText(String text) {
    lv_obj_t* text_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(text_screen, UI_COLOR_DARK, 0);
    
    lv_obj_t* text_label = lv_label_create(text_screen);
    lv_label_set_text(text_label, text.c_str());
    lv_obj_set_style_text_color(text_label, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(text_label);
    
    lv_scr_load(text_screen);
}

void LVGLUIManager::showTouchDebug(int x, int y, int z) {
    // Simple touch debug display - could be enhanced
    Serial.printf("Touch: X=%d, Y=%d, Z=%d\n", x, y, z);
}

// WiFi loading screen implementation
static lv_obj_t* wifi_loading_screen = nullptr;

void LVGLUIManager::showWiFiLoadingScreen() {
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

void LVGLUIManager::hideWiFiLoadingScreen() {
    if (wifi_loading_screen) {
        lv_obj_del(wifi_loading_screen);
        wifi_loading_screen = nullptr;
    }
}

// Touch handling with landscape layout
void LVGLUIManager::handleTouch(int x, int y) {
    using namespace TouchZones;
    
    // Check touch zones for landscape layout
    if (y >= CARD_Y_MIN && y <= CARD_Y_MAX) {
        // Top row cards
        if (x >= CPU_CARD_X_MIN && x <= CPU_CARD_X_MAX) {
            Serial.println("CPU card touched");
        } else if (x >= RAM_CARD_X_MIN && x <= RAM_CARD_X_MAX) {
            Serial.println("RAM card touched");  
        } else if (x >= TEMP_CARD_X_MIN && x <= TEMP_CARD_X_MAX) {
            Serial.println("Temperature card touched");
        } else if (x >= DISK_CARD_X_MIN && x <= DISK_CARD_X_MAX) {
            Serial.println("Disk card touched");
        }
    } else if (x >= CPU_CHART_X_MIN && x <= CPU_CHART_X_MAX && 
               y >= CPU_CHART_Y_MIN && y <= CPU_CHART_Y_MAX) {
        Serial.println("CPU chart touched");
    } else if (x >= NETWORK_CHART_X_MIN && x <= NETWORK_CHART_X_MAX &&
               y >= NETWORK_CHART_Y_MIN && y <= NETWORK_CHART_Y_MAX) {
        Serial.println("Network chart touched");
    }
    // Volume touch is handled separately in main.cpp
}

void LVGLUIManager::handleVolumeTouch(int x, int y, SystemData& data) {
    // This method can be called from the main touch handler
    // Calculate relative position within volume control and update volume
    int local_x = x - UILayout::VOLUME_CONTROL_X;
    if (local_x >= 25 && local_x <= UILayout::BOTTOM_CARD_WIDTH - 15) {
        int slider_width = UILayout::BOTTOM_CARD_WIDTH - 40;
        float volume_percent = ((local_x - 25) * 100.0f) / slider_width;
        volume_percent = constrain(volume_percent, 0, 100);
        
        data.setVolume((int)volume_percent);
        updateVolumeSlider((int)volume_percent);
    }
}

// Placeholder methods for full screen functionality
void LVGLUIManager::showFullScreenChart(int chartType) {
    isFullScreenMode = true;
    currentFullScreenChart = chartType;
    Serial.printf("Full screen chart mode: %d\n", chartType);
}

void LVGLUIManager::returnToMainUI() {
    if (!isFullScreenMode) return;
    
    isFullScreenMode = false;
    currentFullScreenChart = -1;
    
    // Recreate the main UI with landscape layout
    lv_obj_clean(main_screen);
    createLandscapeUI();
    
    lv_scr_load(main_screen);
}

void LVGLUIManager::showClockScreen() {
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
        
        // MAIN DIGITAL TIME - Maximum size and impact
        clock_time_label = lv_label_create(clock_screen);
        lv_obj_set_size(clock_time_label, 320, 100); // Full width, large height
        
        // BRIGHT DIGITAL GREEN - Classic digital clock color
        lv_obj_set_style_text_color(clock_time_label, lv_color_hex(0x00FF00), 0);
        lv_obj_set_style_text_font(clock_time_label, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_align(clock_time_label, LV_TEXT_ALIGN_CENTER, 0);
        
        // Clean digital appearance
        lv_obj_set_style_bg_opa(clock_time_label, LV_OPA_0, 0);
        lv_obj_set_style_border_width(clock_time_label, 0, 0);
        lv_obj_set_style_pad_all(clock_time_label, 0, 0);
        
        // WIDE DIGITAL SPACING - Makes text appear much larger
        lv_obj_set_style_text_letter_space(clock_time_label, 12, 0);
        
        // INTENSE GREEN GLOW - Digital display effect
        lv_obj_set_style_shadow_width(clock_time_label, 30, 0);
        lv_obj_set_style_shadow_color(clock_time_label, lv_color_hex(0x00FF00), 0);
        lv_obj_set_style_shadow_opa(clock_time_label, LV_OPA_90, 0);
        lv_obj_set_style_shadow_ofs_x(clock_time_label, 0, 0);
        lv_obj_set_style_shadow_ofs_y(clock_time_label, 0, 0);
        lv_obj_set_style_shadow_spread(clock_time_label, 5, 0);
        
        // Center the time display
        lv_obj_align(clock_time_label, LV_ALIGN_CENTER, 0, -20);
        
        // AM/PM indicator - smaller but visible
        clock_ampm_label = lv_label_create(clock_screen);
        lv_obj_set_style_text_color(clock_ampm_label, lv_color_hex(0x00AA00), 0);
        lv_obj_set_style_text_font(clock_ampm_label, &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_align(clock_ampm_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_letter_space(clock_ampm_label, 4, 0);
        lv_obj_set_style_bg_opa(clock_ampm_label, LV_OPA_0, 0);
        lv_obj_set_style_border_width(clock_ampm_label, 0, 0);
        
        // Subtle glow for AM/PM
        lv_obj_set_style_shadow_width(clock_ampm_label, 15, 0);
        lv_obj_set_style_shadow_color(clock_ampm_label, lv_color_hex(0x00AA00), 0);
        lv_obj_set_style_shadow_opa(clock_ampm_label, LV_OPA_60, 0);
        lv_obj_set_style_shadow_ofs_x(clock_ampm_label, 0, 0);
        lv_obj_set_style_shadow_ofs_y(clock_ampm_label, 0, 0);
        
        lv_obj_align(clock_ampm_label, LV_ALIGN_CENTER, 140, -20); // Right of time
        
        // Date display - minimal digital style
        clock_date_label = lv_label_create(clock_screen);
        lv_obj_set_style_text_color(clock_date_label, lv_color_hex(0x004400), 0);
        lv_obj_set_style_text_font(clock_date_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_align(clock_date_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_letter_space(clock_date_label, 2, 0);
        lv_obj_set_style_bg_opa(clock_date_label, LV_OPA_0, 0);
        lv_obj_set_style_border_width(clock_date_label, 0, 0);
        lv_obj_align(clock_date_label, LV_ALIGN_CENTER, 0, 60);
        
        // Status message - very dim
        clock_status_label = lv_label_create(clock_screen);
        lv_obj_set_style_text_color(clock_status_label, lv_color_hex(0x002200), 0);
        lv_obj_set_style_text_font(clock_status_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_align(clock_status_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_bg_opa(clock_status_label, LV_OPA_0, 0);
        lv_obj_set_style_border_width(clock_status_label, 0, 0);
        lv_label_set_text(clock_status_label, "WAITING FOR DATA");
        lv_obj_align(clock_status_label, LV_ALIGN_BOTTOM_MID, 0, -5);
    }
    
    // Update time immediately with NetworkManager
    updateClockScreen(nullptr);
    
    // Load the clock screen
    lv_scr_load(clock_screen);
    
    Serial.println("Digital clock activated - pure black background with green digits");
}

void LVGLUIManager::updateClockScreen(NetworkManager* networkManager) {
    if (!isClockMode || !clock_time_label || !clock_date_label || !clock_ampm_label) return;
    
    if (networkManager && networkManager->isTimeSync()) {
        // Use real 12-hour time from NTP with digital styling
        String timeStr = networkManager->getCurrentTime12Hour();
        String dateStr = networkManager->getCurrentDate();
        String ampmStr = networkManager->getAMPM();
        
        // Format time for digital display (remove seconds for cleaner big digits)
        int colonPos = timeStr.lastIndexOf(':');
        if (colonPos > 0) {
            timeStr = timeStr.substring(0, colonPos); // Remove seconds for bigger appearance
        }
        
        lv_label_set_text(clock_time_label, timeStr.c_str());
        lv_label_set_text(clock_date_label, dateStr.c_str());
        lv_label_set_text(clock_ampm_label, ampmStr.c_str());
    } else {
        // Fallback to millis-based 12-hour time with digital styling
        unsigned long currentTime = millis();
        unsigned long totalSeconds = (currentTime / 1000) % 86400;
        unsigned long hours24 = (totalSeconds / 3600) % 24;
        unsigned long minutes = (totalSeconds / 60) % 60;
        
        // Convert to 12-hour format (no seconds for bigger digits)
        unsigned long hours12 = hours24 == 0 ? 12 : (hours24 > 12 ? hours24 - 12 : hours24);
        String ampm = hours24 < 12 ? "AM" : "PM";
        
        // Digital clock format - BIG DIGITS only
        char timeStr[10];
        snprintf(timeStr, sizeof(timeStr), "%lu:%02lu", hours12, minutes);
        lv_label_set_text(clock_time_label, timeStr);
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

bool LVGLUIManager::hasRecentData() {
    if (lastDataReceived == 0) return false;
    
    unsigned long currentTime = millis();
    unsigned long timeSinceData = currentTime - lastDataReceived;

    // Consider data "recent" if received within last 10 seconds
    return timeSinceData < 10000;
}

void LVGLUIManager::setDataReceived() {
    lastDataReceived = millis();
    
    // If we were in clock mode, return to main UI
    if (isClockMode) {
        isClockMode = false;
        lv_scr_load(main_screen);
        Serial.println("Returning to main UI - UDP data received");
    }
}

void LVGLUIManager::checkDataTimeout() {
    // Only check timeout if we're not already in clock mode and we have received data before
    if (!isClockMode && lastDataReceived > 0 && !hasRecentData()) {
        Serial.println("Data timeout detected - switching to clock screen");
        showClockScreen();
    }
}
