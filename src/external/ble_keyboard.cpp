#include <AceButton.h>
#include <BleKeyboard.h>
#include "external/ble_keyboard.h"


BleKeyboard bleKeyboard("Charlie's Knob","luowei",100);


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

int keyboard_enable_player(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    // Play、Pause System Media...
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
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
    bleKeyboard.write(KEY_MEDIA_MUTE);
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
