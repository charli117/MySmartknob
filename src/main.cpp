#include <Arduino.h>

#include "core/display_task.h"
#include "core/interface_task.h"
#include "core/motor_task.h"


#if BLE_KEYWORD
#include "external/ble_keyboard.h"
#endif

#if HOMEKIT
#include <HomeSpan.h>
#include "external/DEV_LED.h"
#endif 

#if SK_DISPLAY
static DisplayTask display_task(0);
static DisplayTask* display_task_p = &display_task;
#else
static DisplayTask* display_task_p = nullptr;
#endif
static MotorTask motor_task(1);


InterfaceTask interface_task(0, motor_task, display_task_p);

void setup() {
  #if SK_DISPLAY
  display_task.setLogger(&interface_task);
  display_task.begin();

  // Connect display to motor_task's knob state feed
  motor_task.addListener(display_task.getKnobStateQueue());
  #endif

  motor_task.setLogger(&interface_task);
  motor_task.begin();
  interface_task.begin();

  #if BLE_KEYWORD
  ble_keyboard_init();
  #endif

  // #if HOMEKIT
  // Serial.begin(115200);

  // homeSpan.begin(Category::Lighting, "MI Monitor Light");
  // new SpanAccessory();  
  //   new Service::AccessoryInformation();
  //     new Characteristic::Identify(); 
  
  // new SpanAccessory(); 
  //   new Service::AccessoryInformation();    
  //     new Characteristic::Identify();
  //     new Characteristic::Name("Dimmable LED");         
  //   new DEV_DimmableLED(6); 
  // #endif

  // Free up the Arduino loop task
  vTaskDelete(NULL);
}

void loop() {
  // #if HOMEKIT
  // homeSpan.poll();
  // #endif
  // char buf[50];
  // static uint32_t last_stack_debug;
  // if (millis() - last_stack_debug > 1000) {
  //   interface_task.log("Stack high water:");
  //   snprintf(buf, sizeof(buf), "  main: %d", uxTaskGetStackHighWaterMark(NULL));
  //   interface_task.log(buf);
  //   #if SK_DISPLAY
  //     snprintf(buf, sizeof(buf), "  display: %d", uxTaskGetStackHighWaterMark(display_task.getHandle()));
  //     interface_task.log(buf);
  //   #endif
  //   snprintf(buf, sizeof(buf), "  motor: %d", uxTaskGetStackHighWaterMark(motor_task.getHandle()));
  //   interface_task.log(buf);
  //   snprintf(buf, sizeof(buf), "  interface: %d", uxTaskGetStackHighWaterMark(interface_task.getHandle()));
  //   interface_task.log(buf);
  //   snprintf(buf, sizeof(buf), "Heap -- free: %d, largest: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT), heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
  //   interface_task.log(buf);
  //   last_stack_debug = millis();
  // }
}