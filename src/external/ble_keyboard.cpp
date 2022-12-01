#include <AceButton.h>
#include <BleKeyboard.h>
#include <EEPROM.h>
#include "external/ble_keyboard.h"


BleKeyboard bleKeyboard("Charlie's Knob","luowei",100);
BLEServer* bleKeyboardServer;


void ble_keyboard_init(void)
{
    // 初始化虚拟蓝牙键盘
    #if BLE_KEYWORD
    bleKeyboard.begin();
    #endif

}

bool check_keyboard_connected(void)
{
    #if BLE_KEYWORD
    return bleKeyboard.isConnected();
    #else
    return false;
    #endif
}

int surface_dail_left(void)
{
    if(!check_keyboard_connected())
    return -1;
    // Surface Dail Rotate
    #if BLE_KEYWORD
    bleKeyboard.sendDialReport(DIAL_L);
    #endif
    return 0;
}

int surface_dail_right(void)
{
    if(!check_keyboard_connected())
    return -1;
    // Surface Dail Rotate
    #if BLE_KEYWORD
    bleKeyboard.sendDialReport(DIAL_R);
    #endif
    return 0;
}

int surface_dail_click(void)
{
    if(!check_keyboard_connected())
    return -1;
    // Surface Dail Click
    #if BLE_KEYWORD
    bleKeyboard.sendDialReport(DIAL_PRESS);
    #endif
    return 0;
}

int surface_dail_release(void)
{
    if(!check_keyboard_connected())
    return -1;
    // Surface Dail Release
    #if BLE_KEYWORD
    bleKeyboard.sendDialReport(DIAL_RELEASE);
    #endif
    return 0;
}

int keyboard_enable_player(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Play、Pause System Media...
    #if BLE_KEYWORD
    // bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    bleKeyboard.write(MMKEY_PLAYPAUSE);
    delay(500);
    #endif
    return 0;
}

int keyboard_enable_qq_music(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Play、Paus QQ Media...
    #if BLE_KEYWORD
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.print(" ");
    delay(50);
    bleKeyboard.releaseAll();
    #endif
    return 0;
}

int keyboard_player_next(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Play Next Media...
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    delay(50);
    #endif
    return 0;
}

int keyboard_player_previous(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Play Previous Media...
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    delay(50);
    #endif
    return 0;
}

int keyboard_player_volume_mute(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Volume Mute...
    #if BLE_KEYWORD
    bleKeyboard.write(MMKEY_MUTE);
    // bleKeyboard.write(KEY_MEDIA_MUTE);
    #endif
    return 0;
}

int keyboard_player_volume_up(void)
{
    if(!check_keyboard_connected())
    return -1;
    // Volume Up...
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    #endif
    return 0;
}

int keyboard_player_volume_down(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Volume Down...
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    #endif
    return 0;
}

int keyboard_next_page(void)
{
    if(!check_keyboard_connected())
    return -1;

    // Next Page...
    #if BLE_KEYWORD
    // bleKeyboard.write(KEY_PAGE_DOWN);
    bleKeyboard.write(KEY_PAGE_DOWN);
    #endif
    return 0;
}

int keyboard_previous_page(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Previous Page...
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_PAGE_UP);
    #endif
    return 0;
}

int keyboard_zoom_up(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Zoom Up
    #if BLE_KEYWORD
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.print("+");
    bleKeyboard.releaseAll();
    #endif
    return 0;
}

int keyboard_zoom_down(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Zoom Down
    #if BLE_KEYWORD
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.print("-");
    bleKeyboard.releaseAll();
    #endif
    return 0;
}
