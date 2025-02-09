/* Copyright 2015-2021 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 *
 *
 *
 *
 *
 * In aanvulling op de andere Engram-wijzigingen:
 *
 *    Dead-Option interception:
 *      - Option-E; Option-U enz. onderscheppen en verplaatsen naar de posities waar ik ze verwacht: `; "; enz.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#include QMK_KEYBOARD_H
#include "keymap_us_extended.h"
#include "flow.h"

// Define layer names for clarity
enum planck_layers {
  _DVORAK,  // On Dvorak: maybe switch U and I, D and H, and L and M
  _NUM,
  _SYM,
  _FN
};

// Shortcuts layer 0 and 1
#define escSYM  LT(_SYM,KC_ESC)
#define bspNUM  LT(_NUM,KC_BSPC)
#define spcGLB  LT(0,KC_SPC)
#define entHS   LT(0,KC_ENT)
//#define glb     LT(0,KC_NO)
#define glb     KC_NO
#define hs      KC_F20
#define btab    S(KC_TAB)
#define lock    C(G(KC_Q))
#define fn      MO(_FN)

// Config for flow keys
const uint16_t flow_config[FLOW_COUNT][2] = {
  {bspNUM, KC_LALT},
  {bspNUM, KC_LGUI},
  {bspNUM, KC_LCTL},
  {bspNUM, KC_LSFT},
};

// KEYMAP
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DVORAK] = LAYOUT_planck_grid(
    KC_SCLN, KC_COMM, KC_DOT , KC_P   , KC_Y   , XXXXXXX, XXXXXXX, KC_F   , KC_G   , KC_R   , KC_L   , KC_V   ,
    KC_O   , KC_I   , KC_E   , KC_A   , KC_U   , XXXXXXX, XXXXXXX, KC_C   , KC_D   , KC_N   , KC_T   , KC_S   ,
    KC_QUOT, KC_Q   , KC_J   , KC_H   , KC_X   , XXXXXXX, XXXXXXX, KC_B   , KC_W   , KC_M   , KC_V   , KC_Z   ,
    XXXXXXX, XXXXXXX, XXXXXXX, escSYM , bspNUM , XXXXXXX, XXXXXXX, spcGLB , entHS  , XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_NUM] = LAYOUT_planck_grid(
    KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , XXXXXXX, XXXXXXX, KC_6   , KC_7   , KC_8   , KC_9   , KC_0   ,
    KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT,
    KC_MINS, KC_EQL , KC_SLSH, KC_BSLS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_TAB , US_GRV , KC_LBRC, KC_RBRC,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, glb    , hs     , XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_SYM] = LAYOUT_planck_grid(
    KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, XXXXXXX, XXXXXXX, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
    KC_MRWD, KC_MPLY, KC_MFFD, CW_TOGG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BRID, KC_BRIU, KC_VOLD, KC_VOLU,
    KC_UNDS, KC_PLUS, KC_QUES, KC_PIPE, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, btab   , KC_TILD, KC_LCBR, KC_RCBR,
    XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, lock   , fn     , XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_FN] = LAYOUT_planck_grid(
    KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , XXXXXXX, XXXXXXX, KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 ,
    KC_F11 , KC_F12 , KC_F13 , KC_F14 , KC_F15 , XXXXXXX, XXXXXXX, KC_F16 , KC_F17 , KC_F18 , KC_F19 , KC_F20 ,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX
  )

};

/* Apple keycodes
 *
 * 0x01AE   AL Keyboard Layout          Toggle Onscreen Keyboard
 * 0x029D   AC Keyboard Layout Select   Globe Key                   AC_NEXT_KEYBOARD_LAYOUT_SELECT
 * 0x0221   AC Search                   Spotlight
 * 0x0030   Power                       Lock
 * 0x009B   System Do Not Disturb       Toggle Focus Mode           host_system_send()
 *
 * case sptl:
 *   host_consumer_send(record->event.pressed ? 0x0221 : 0);
 *   return false;
 *
 */

bool glb_down = false;
bool glb_double_tapped = false;
bool glb_sent = false;
bool glb_timer_running = false;
uint16_t glb_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  if (!update_flow(keycode, record->event.pressed, record->event.key)) return false;

  if (glb_down && record->event.pressed) {
    host_consumer_send(0x029D);
    glb_sent = true;
  }

  switch (keycode) {

    case bspNUM:  //LT(_NUM,KC_BSPC)
      if (glb_down && record->event.pressed) {
        tap_code16(KC_DEL);
        return false;
      }
      break;

    case glb:  //KC_NO
      if (record->event.pressed) {
        host_consumer_send(0x029D);
        host_consumer_send(0);
        host_consumer_send(0x029D);
        glb_sent = true;
      } else {
        host_consumer_send(0);
        glb_sent = false;
      }
      break;

    case spcGLB:  //LT(0,KC_SPC)
      if (record->tap.count) {
      } else {
        if (record->event.pressed) {
          glb_down = true;
        } else {
          glb_down = false;
          if (glb_sent) {
            host_consumer_send(0);
            glb_sent = false;
          }
        }
      }
      break;

    case entHS:  //LT(0,KC_ENT)
      if (record->tap.count) {
      } else {
        if (record->event.pressed) {
          register_code16(KC_F20);
        } else {
          unregister_code16(KC_F20);
        }
      }
      break;

  }// Einde switch

  return true;
}

void matrix_scan_user(void) {
  flow_matrix_scan();
}
