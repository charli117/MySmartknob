#if SK_LEDS
#include <FastLED.h>
#endif

#if SK_STRAIN
#include <HX711.h>
#endif

#if SK_ALS
#include <Adafruit_VEML7700.h>
#endif

#if BLE_KEYWORD
#include "external/ble_keyboard.h"
#endif 

#include "core/interface_task.h"
#include "util.h"

#define COUNT_OF(A) (sizeof(A) / sizeof(A[0]))

#if SK_LEDS
CRGB leds[NUM_LEDS];
#endif

#if SK_STRAIN
HX711 scale;
#endif

#if SK_ALS
Adafruit_VEML7700 veml = Adafruit_VEML7700();
#endif

static PB_SmartKnobConfig configs[] = {
    // int32_t num_positions;
    // int32_t position;
    // float position_width_radians;
    // float detent_strength_unit;
    // float endstop_strength_unit;
    // float snap_point;
    // char text[51];
    // {
    //     0,
    //     0,
    //     10 * PI / 180,
    //     0,
    //     1,
    //     1.1,
    //     "Unbounded\nNo detents",
    // },
    // {
    //     11,
    //     0,
    //     10 * PI / 180,
    //     0,
    //     1,
    //     1.1,
    //     "Bounded 0-10\nNo detents",
    // },
    // {
    //     73,
    //     0,
    //     10 * PI / 180,
    //     0,
    //     1,
    //     1.1,
    //     "Multi-rev\nNo detents",
    // },
    // {
    //     2,
    //     0,
    //     60 * PI / 180,
    //     1,
    //     1,
    //     0.55, // Note the snap point is slightly past the midpoint (0.5); compare to normal detents which use a snap point *past* the next value (i.e. > 1)
    //     "On/off\nStrong detent",
    // },
    // {
    //     1,
    //     0,
    //     60 * PI / 180,
    //     0.01,
    //     0.6,
    //     1.1,
    //     "Return-to-center",
    // },
    // {
    //     256,
    //     127,
    //     1 * PI / 180,
    //     0,
    //     1,
    //     1.1,
    //     "Fine values\nNo detents",
    // },
    // {
    //     256,
    //     127,
    //     1 * PI / 180,
    //     1,
    //     1,
    //     1.1,
    //     "Fine values\nWith detents",
    // },
    // {
    //     32,
    //     0,
    //     8.225806452 * PI / 180,
    //     2,
    //     1,
    //     1.1,
    //     "Coarse values\nStrong detents",
    // },
    // {
    //     32,
    //     0,
    //     8.225806452 * PI / 180,
    //     0.2,
    //     1,
    //     1.1,
    //     "Coarse values\nWeak detents",
    // },
    {
        256,
        127,
        1 * PI / 180,
        0.6,
        1,
        1.1,
        "Computer",
        "Volume",
        "系统音量\n增大/减小",
    },
    {
        0,
        0,
        10 * PI / 180,
        1.5,
        1,
        1.1,
        "Computer",
        "Switch",
        "媒体切换\n上一首/下一首",
    },
    {
        0,
        0,
        5 * PI / 180,
        0.6,
        1,
        1.1,
        "Computer",
        "UpDown",
        "网页&PPT\n上一页/下一页",
    },
    {
        0,
        0,
        10 * PI / 180,
        0.3,
        1,
        1.1,
        "Computer",
        "Zoom",
        "缩放\n放大/缩小",
    },
    {
        2,
        0,
        60 * PI / 180,
        1,
        1,
        0.55,
        "Computer",
        "Play/Pause",
        "QQ音乐\n播放/暂停",
    },
    {
        1,
        0,
        10 * PI / 180,
        0.01,
        0.6,
        1.1,
        "缺省",
        "缺省",
        "Return-To-Center",
    },
    {
        21,
        0,
        10 * PI / 180,
        3,
        1,
        1.1,
        "缺省",
        "缺省",
        "Bounded 0-20\nStrong Detents",
    },
};

InterfaceTask::InterfaceTask(const uint8_t task_core, MotorTask& motor_task, DisplayTask* display_task) : 
        Task("Interface", 3000, 1, task_core),
        stream_(),
        motor_task_(motor_task),
        display_task_(display_task),
        plaintext_protocol_(stream_, motor_task_),
        proto_protocol_(stream_, motor_task_) {
    #if SK_DISPLAY
        assert(display_task != nullptr);
    #endif

    log_queue_ = xQueueCreate(10, sizeof(std::string *));
    assert(log_queue_ != NULL);

    knob_state_queue_ = xQueueCreate(1, sizeof(PB_SmartKnobState));
    assert(knob_state_queue_ != NULL);
}

void InterfaceTask::run() {
    stream_.begin();
    
    #if SK_LEDS
        FastLED.addLeds<SK6812, PIN_LED_DATA, GRB>(leds, NUM_LEDS);
    #endif

    #if SK_ALS && PIN_SDA >= 0 && PIN_SCL >= 0
        Wire.begin(PIN_SDA, PIN_SCL);
        Wire.setClock(400000);
    #endif

    #if SK_STRAIN
        scale.begin(38, 2);
    #endif

    #if SK_ALS
        if (veml.begin()) {
            veml.setGain(VEML7700_GAIN_2);
            veml.setIntegrationTime(VEML7700_IT_400MS);
        } else {
            log("ALS sensor not found!");
        }
    #endif

    motor_task_.setConfig(configs[0]);
    motor_task_.addListener(knob_state_queue_);


    // Start in legacy protocol mode
    plaintext_protocol_.init([this] () {
        changeConfig(true);
    });
    SerialProtocol* current_protocol = &plaintext_protocol_;

    ProtocolChangeCallback protocol_change_callback = [this, &current_protocol] (uint8_t protocol) {
        switch (protocol) {
            case SERIAL_PROTOCOL_LEGACY:
                current_protocol = &plaintext_protocol_;
                break;
            case SERIAL_PROTOCOL_PROTO:
                current_protocol = &proto_protocol_;
                break;
            default:
                log("Unknown protocol requested");
                break;
        }
    };

    plaintext_protocol_.setProtocolChangeCallback(protocol_change_callback);
    proto_protocol_.setProtocolChangeCallback(protocol_change_callback);

    // Interface loop:
    while (1) {
        PB_SmartKnobState state;
        if (xQueueReceive(knob_state_queue_, &state, 0) == pdTRUE) {
            current_protocol->handleState(state);
        }

        current_protocol->loop();

        std::string* log_string;
        while (xQueueReceive(log_queue_, &log_string, 0) == pdTRUE) {
            current_protocol->log(log_string->c_str());
            delete log_string;
        }
        
        // LED、压力传感器等状态更新
        updateHardware();

        // 蓝牙虚拟键盘指令推送
        #if BLE_KEYWORD
            const char* device_type_ = state.config.device_type;
            const char* device_operate_ = state.config.device_operate;

            if(strcmp(device_type_,"Computer") == 0 && strcmp(device_operate_,"Volume")==0) {
                static int now_volume_num = 0;
                static int old_volume_num = 0;
                now_volume_num = state.current_position;
                if (now_volume_num > old_volume_num)
                {
                    keyboard_player_volume_up();
                    old_volume_num = state.current_position;
                } else if (now_volume_num < old_volume_num) {
                    keyboard_player_volume_down();
                    old_volume_num = state.current_position;
                }
            }
            else if (strcmp(device_type_,"Computer") == 0 && strcmp(device_operate_,"UpDown") == 0){
                static int now_updown_num = 1;
                static int old_updown_num = 0;
                now_updown_num = state.current_position;
                if (now_updown_num > old_updown_num)
                {
                    keyboard_next_page();
                    old_updown_num = state.current_position;
                } else if (now_updown_num < old_updown_num) {
                    keyboard_previous_page();
                    old_updown_num = state.current_position;
                }
            } else if (strcmp(device_type_,"Computer") == 0 && strcmp(device_operate_,"Switch") == 0){
                static int now_switch_num = 1;
                static int old_switch_num = 0;
                now_switch_num = state.current_position;
                if (now_switch_num > old_switch_num)
                {
                    keyboard_player_next();
                    old_switch_num = state.current_position;
                } else if (now_switch_num < old_switch_num) {
                    keyboard_player_previous();
                    old_switch_num = state.current_position;
                }
            }
            else if (strcmp(device_type_,"Computer") == 0 && strcmp(device_operate_,"Play/Pause") == 0){
                static int now_play_num = 0;
                static int old_play_num = 0;
                now_play_num = state.current_position;
                if (now_play_num != old_play_num)
                {
                    keyboard_enable_qq_music();
                    old_play_num = state.current_position;
                } 
            } else if(strcmp(device_type_,"Computer") == 0 && strcmp(device_operate_,"Zoom") == 0) {
                static int now_zoom_num = 0;
                static int old_zoom_num = 0;
                now_zoom_num = state.current_position;
                if (now_zoom_num > old_zoom_num)
                {
                    keyboard_zoom_up();
                    old_zoom_num = state.current_position;
                } else if (now_zoom_num < old_zoom_num) {
                    keyboard_zoom_down();
                    old_zoom_num = state.current_position;
                }
            }
        #endif

        

        // 主控配置信息查询
        #if GET_STATUS
            GetChipAndMemoryDetails();
            delay(3000);
        #endif

        delay(10);
    }
}

void InterfaceTask::log(const char* msg) {
    // Allocate a string for the duration it's in the queue; it is free'd by the queue consumer
    std::string* msg_str = new std::string(msg);

    // Put string in queue (or drop if full to avoid blocking)
    xQueueSendToBack(log_queue_, &msg_str, 0);
}

void InterfaceTask::changeConfig(bool next) {
    if (next) {
        current_config_ = (current_config_ + 1) % COUNT_OF(configs);
    } else {
        if (current_config_ == 0) {
            current_config_ = COUNT_OF(configs) - 1;
        } else {
            current_config_ --;
        }
    }
    
    char buf_[256];
    snprintf(buf_, sizeof(buf_), "Changing config to %d -- %s", current_config_, configs[current_config_].text);
    log(buf_);
    motor_task_.setConfig(configs[current_config_]);
}

void InterfaceTask::updateHardware() {
    // How far button is pressed, in range [0, 1]
    float press_value_unit = 0;

    #if SK_ALS
        const float LUX_ALPHA = 0.005;
        static float lux_avg;
        float lux = veml.readLux();
        lux_avg = lux * LUX_ALPHA + lux_avg * (1 - LUX_ALPHA);
        static uint32_t last_als;
        if (millis() - last_als > 1000) {
            snprintf(buf_, sizeof(buf_), "millilux: %.2f", lux*1000);
            log(buf_);
            last_als = millis();
        }
    #endif

    #if SK_STRAIN
        if (scale.wait_ready_timeout(100)) {
            int32_t reading = scale.read();

            static uint32_t last_reading_display;
            if (millis() - last_reading_display > 1000) {
                snprintf(buf_, sizeof(buf_), "HX711 reading: %d", reading);
                log(buf_);
                last_reading_display = millis();
            }

            // TODO: calibrate and track (long term moving average) zero point (lower); allow calibration of set point offset
            const int32_t lower = 300000;
            const int32_t upper = 900000;
            // Ignore readings that are way out of expected bounds
            if (reading >= lower - (upper - lower) && reading < upper + (upper - lower)*2) {
                long value = CLAMP(reading, lower, upper);
                press_value_unit = 1. * (value - lower) / (upper - lower);

                static bool pressed;
                if (!pressed && press_value_unit > 0.75) {
                    motor_task_.playHaptic(true);
                    pressed = true;
                    changeConfig(true);
                } else if (pressed && press_value_unit < 0.25) {
                    motor_task_.playHaptic(false);
                    pressed = false;
                }
            }
        } else {
            log("HX711 not found.");

            #if SK_LEDS
                for (uint8_t i = 0; i < NUM_LEDS; i++) {
                    leds[i] = CRGB::Red;
                }
                FastLED.show();
            #endif
        }
    #endif

    uint16_t brightness = UINT16_MAX;
    // TODO: brightness scale factor should be configurable (depends on reflectivity of surface)
    #if SK_ALS
        brightness = (uint16_t)CLAMP(lux_avg * 13000, (float)1280, (float)UINT16_MAX);
    #endif

    #if SK_DISPLAY
        display_task_->setBrightness(brightness); // TODO: apply gamma correction
    #endif

    #if SK_LEDS
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            leds[i].setHSV(200 * press_value_unit, 255, brightness >> 8);

            // Gamma adjustment
            leds[i].r = dim8_video(leds[i].r);
            leds[i].g = dim8_video(leds[i].g);
            leds[i].b = dim8_video(leds[i].b);
        }
        FastLED.show();
    #endif
}

void InterfaceTask::GetChipAndMemoryDetails (){  
// This function display the details of the integrated chip (memory, core, bluetooth)
// It also give external memory size 
    /* Print chip information */
    Serial.println();
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    Serial.printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
                  chip_info.cores,
                  (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
                  (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    Serial.println();
    Serial.printf("silicon revision %d, ", chip_info.revision);
    Serial.println();
    Serial.printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
                  (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}