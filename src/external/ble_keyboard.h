#pragma once

#include <BleKeyboard.h>


void ble_keyboard_init(void);
bool check_keyboard_connected(void);
int surface_dail_left(void);
int surface_dail_right(void);
int surface_dail_click(void);
int surface_dail_release(void);
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
