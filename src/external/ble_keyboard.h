#pragma once

#include <BleKeyboard.h>


void ble_keyboard_init(void);
bool check_keyboard_connected(void);
int keyboard_enable_player(void);
int keyboard_enable_qq_music(void);
int keyboard_player_next(void);
int keyboard_player_previous(void);
int keyboard_player_volume_up(void);
int keyboard_player_volume_down(void);
int keyboard_next_page(void);
int keyboard_previous_page(void);
int keyboard_player_volume_mute(void);
int keyboard_zoom_up(void);
int keyboard_zoom_down(void);
// int keyboard_pass_ctrl_c(void);
// int keyboard_pass_ctrl_v(void);
// int keyboard_windows_show(void);
// int keyboard_write_common(uint8_t c_1, uint8_t c_2);