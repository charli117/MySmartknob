#include <AceButton.h>
#include <BleKeyboard.h>
#include "external/ble_keyboard.h"

BleKeyboard bleKeyboard("Charlie's Knob","luowei",100);

void ble_keyboard_init(void)
{
    #if BLE_KEYWORD
    // 初始化虚拟蓝牙键盘
    bleKeyboard.begin();
    Serial.println("Starting BLE work!");
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
    
    Serial.println("Play/Pause Media...");
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
    
    Serial.println("Play/Paus QQ Media...");
    #if BLE_KEYWORD
    //QQ播放器
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
    
    Serial.println("Play Next Media...");
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_PAGE_DOWN);
    delay(50);
    #endif
    return 0;
}

int keyboard_player_previous(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Play Previous Media...");
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_PAGE_UP);
    delay(50);
    #endif
    return 0;
}

int keyboard_player_volume_mute(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Volume Mute...");
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_MEDIA_MUTE);
    #endif
    return 0;
}

int keyboard_player_volume_up(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Volume Up...");
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    #endif
    return 0;
}

int keyboard_player_volume_down(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Volume Down...");
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    #endif
    return 0;
}

int keyboard_next_page(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Next Page...");
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_DOWN_ARROW);
    #endif
    return 0;
}

int keyboard_previous_page(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Previous Page...");
    #if BLE_KEYWORD
    bleKeyboard.write(KEY_UP_ARROW);
    #endif
    return 0;
}

int keyboard_zoom_up(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Zoom Up...");
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
    
    Serial.println("Zoom Down...");
    #if BLE_KEYWORD
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.print("-");
    bleKeyboard.releaseAll();
    #endif
    return 0;
}

// int keyboard_pass_ctrl_c(void)
// {
//     if(!check_keyboard_connected())
//     return -1;
//     Serial.println("Sending ctrl + c");
//     #if BLE_KEYWORD
//     bleKeyboard.press(KEY_LEFT_CTRL);
//     bleKeyboard.press(67);
//     delay(50);
//     bleKeyboard.releaseAll();
//     #endif
//     return 0;
// }

// int keyboard_pass_ctrl_v(void)
// {
//     if(!check_keyboard_connected())
//     return -1;
    
//     Serial.println("Sending ctrl + v");
//     #if BLE_KEYWORD
//     bleKeyboard.press(KEY_LEFT_CTRL);
//     bleKeyboard.press(86);
//     delay(50);
//     bleKeyboard.releaseAll();
//     #endif
//     return 0;
// }

// int keyboard_windows_show(void)
// {
//     if(!check_keyboard_connected())
//     return -1;
//     Serial.println("Sending ctrl + v");
//     #if BLE_KEYWORD
//     bleKeyboard.press(KEY_LEFT_GUI);
//     bleKeyboard.press(KEY_TAB);
//     delay(50);
//     bleKeyboard.releaseAll();
//     #endif
//     return 0;
// }

// int keyboard_write_common(uint8_t c_1, uint8_t c_2)
// {
//     if(!check_keyboard_connected())
//     return -1;
//     bleKeyboard.press(c_1);
//     bleKeyboard.press(c_2);
//     delay(50);
//     bleKeyboard.releaseAll();
//     return 0;
// }
