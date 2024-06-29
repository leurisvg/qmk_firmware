/*
Copyright 2023 @ Nuphy <https://nuphy.com/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define DIRECTION_CHANGE_DELAY 1250
#include QMK_KEYBOARD_H

enum user_custom_keycodes {
    MOVE_CURSOR = SAFE_RANGE
};

bool activate_mouse_movement = false;
uint32_t last_activity_timer = 0;
uint32_t direction_change_timer = 0;
uint8_t move_step = 0; 

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) { // Disable macro when any key is pressed
        activate_mouse_movement = false; 
        mousekey_off(KC_MS_UP); 
        mousekey_off(KC_MS_RIGHT); 
        mousekey_off(KC_MS_DOWN); 
        mousekey_off(KC_MS_LEFT); 
    }
        
    switch (keycode) {
        case MOVE_CURSOR:
            if (record->event.pressed) {
                activate_mouse_movement = true;
                layer_on(2);
                last_activity_timer = timer_read32();
                direction_change_timer = timer_read32();
                layer_off(2);
            } else {
                clear_keyboard();
            }
            break;
    }
    return true;
}

void matrix_scan_user(void) {
    if (activate_mouse_movement) { 
        if (timer_elapsed(direction_change_timer) > DIRECTION_CHANGE_DELAY) {
            switch (move_step) {
                case 0:
                    mousekey_on(KC_MS_UP);  // Move cursor up
                    mousekey_send();
                    break;
                case 1:
                    mousekey_off(KC_MS_UP);
                    mousekey_on(KC_MS_RIGHT);  // Move cursor right
                    mousekey_send();
                    break;
                case 2:
                    mousekey_off(KC_MS_RIGHT);
                    mousekey_on(KC_MS_DOWN);  // Move cursor down
                    mousekey_send();
                    break;
                case 3:
                    mousekey_off(KC_MS_DOWN);
                    mousekey_on(KC_MS_LEFT);  // Move cursor left
                    mousekey_send();
                    break;
                case 4:
                    mousekey_off(KC_MS_LEFT);
                    move_step = -1;  // Reset step counter
                    break;
            }
            move_step++;
            last_activity_timer = timer_read32();  // Reset last_activity_timer
            direction_change_timer = timer_read32();
        }
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// layer Mac
[0] = LAYOUT_75_ansi(
    KC_ESC,     KC_BRID,   KC_BRIU,     KC_MCTL,   MAC_SEARCH, MAC_VOICE,   MAC_DND,   KC_MPRV,    KC_MPLY,     KC_MNXT,   KC_MUTE,    KC_VOLD,    KC_VOLU,     MAC_PRTA,   KC_INS,     KC_DEL,
    KC_GRV,     KC_1,      KC_2,        KC_3,      KC_4,       KC_5,        KC_6,      KC_7,       KC_8,        KC_9,      KC_0,       KC_MINS,    KC_EQL,                  KC_BSPC,    KC_PGUP,
    KC_TAB,     KC_Q,      KC_W,        KC_E,      KC_R,       KC_T,        KC_Y,      KC_U,       KC_I,        KC_O,      KC_P,       KC_LBRC,    KC_RBRC,                 KC_BSLS,    KC_PGDN,
    KC_CAPS,    KC_A,      KC_S,        KC_D,      KC_F,       KC_G,        KC_H,      KC_J,       KC_K,        KC_L,      KC_SCLN,    KC_QUOT,                             KC_ENT,     KC_HOME,
    KC_LSFT,               KC_Z,        KC_X,      KC_C,       KC_V,        KC_B,      KC_N,       KC_M,        KC_COMM,   KC_DOT,     KC_SLSH,                 KC_RSFT,    KC_UP,      KC_END,
    KC_LCTL,    KC_LALT,   KC_LGUI,                                         KC_SPC,                             KC_RGUI,   MO(1),      KC_RCTL,                 KC_LEFT,    KC_DOWN,    KC_RGHT),

// layer Mac Fn
[1] = LAYOUT_75_ansi(
    _______,    KC_F1,      KC_F2,      KC_F3,     KC_F4,      KC_F5,       KC_F6,     KC_F7,      KC_F8,       KC_F9,     KC_F10,     KC_F11,     KC_F12,      SYS_PRT,    _______,    MOVE_CURSOR,
    _______,    LNK_BLE1,   LNK_BLE2,   LNK_BLE3,  LNK_RF,     _______,     _______,   _______,    _______,     _______,   _______,    _______,	   _______,                 _______,    _______,
    _______,    _______,    _______,    _______,   _______,    _______,     _______,   _______,    _______,     _______,   _______,    DEV_RESET,  _______,                 BAT_SHOW,   _______,
    _______,    _______,    _______,    _______,   _______,    _______,     _______,   _______,    _______,     _______,   _______,    _______,                             _______,    _______,
    _______,                _______,    _______,   _______,    _______,     BAT_NUM,   _______,    MO(4),       RGB_SPD,   RGB_SPI,    _______,                 _______,    RGB_VAI,    _______,
    _______,    _______,    _______,                                        _______,                            _______,   MO(1),      _______,                 RGB_MOD,    RGB_VAD,    RGB_HUI),

// layer win
[2] = LAYOUT_75_ansi(
    KC_ESC,     KC_F1,     KC_F2,       KC_F3,     KC_F4,      KC_F5,       KC_F6,     KC_F7,      KC_F8,       KC_F9,     KC_F10,     KC_F11,     KC_F12,      KC_PSCR,    KC_INS,     KC_DEL,
    KC_GRV,     KC_1,      KC_2,        KC_3,      KC_4,       KC_5,        KC_6,      KC_7,       KC_8,        KC_9,      KC_0,       KC_MINS,    KC_EQL,                  KC_BSPC,    KC_PGUP,
    KC_TAB,     KC_Q,      KC_W,        KC_E,      KC_R,       KC_T,        KC_Y,      KC_U,       KC_I,        KC_O,      KC_P,       KC_LBRC,    KC_RBRC,                 KC_BSLS,    KC_PGDN,
    KC_CAPS,    KC_A,      KC_S,        KC_D,      KC_F,       KC_G,        KC_H,      KC_J,       KC_K,        KC_L,      KC_SCLN,    KC_QUOT,                             KC_ENT,     KC_HOME,
    KC_LSFT,               KC_Z,        KC_X,      KC_C,       KC_V,        KC_B,      KC_N,       KC_M,        KC_COMM,   KC_DOT,     KC_SLSH,                 KC_RSFT,    KC_UP,      KC_END,
    KC_LCTL,    KC_LGUI,   KC_LALT,                                         KC_SPC,                             KC_RALT,   MO(3),      KC_RCTL,                 KC_LEFT,    KC_DOWN,    KC_RGHT),

// layer win Fn
[3] = LAYOUT_75_ansi(
    _______,    KC_BRID,    KC_BRIU,    KC_CALC,   _______,    _______,     _______,   KC_MPRV,    KC_MPLY,     KC_MNXT,   KC_MUTE,    KC_VOLD,    KC_VOLU,     _______,    _______,    _______,
    _______,    LNK_BLE1,   LNK_BLE2,   LNK_BLE3,  LNK_RF,     _______,     _______,   _______,    _______,     _______,   _______,    _______,	   _______,                 _______,    _______,
    _______,    _______,    _______,    _______,   _______,    _______,     _______,   _______,    _______,     _______,   _______,    DEV_RESET,  _______,                 BAT_SHOW,   _______,
    _______,    _______,    _______,    _______,   _______,    _______,     _______,   _______,    _______,     _______,   _______,    _______,                             _______,    _______,
    _______,                _______,    _______,   _______,    _______,     BAT_NUM,   _______,    MO(4),       RGB_SPD,   RGB_SPI,    _______,                 _______,    RGB_VAI,    _______,
    _______,    _______,    _______,                                        _______,                            _______,   MO(3),      _______,                 RGB_MOD,    RGB_VAD,    RGB_HUI),

// layer 4
[4] = LAYOUT_75_ansi(
    _______,    _______,    _______,    _______,   _______,    _______,     _______,   _______,    _______,     _______,   _______,    _______,     _______,    _______,    _______,    MOVE_CURSOR,
    _______,    _______,    _______,    _______,   _______,    _______,     _______,   _______,    _______,     _______,   _______,    _______,     _______,                _______,    _______,
    _______,    _______,    _______,    _______,   _______,    _______,     _______,   _______,    _______,     _______,   _______,    _______,     SLEEP_MODE,             _______,    _______, 
    _______,    _______,    KB_SLP,     DB_TOGG,   _______,    _______,     _______,   _______,    _______,     _______,   _______,    _______,                             _______,    _______,
    _______,                LINK_TO,    _______,   RGB_TEST,   _______,     _______,   _______,    _______,     SIDE_SPD,  SIDE_SPI,   _______,                 _______,    SIDE_VAI,   _______,
    _______,    _______,    _______,                                        _______,                            _______,   MO(4),      _______,                 SIDE_MOD,	SIDE_VAD,   SIDE_HUI)
};
