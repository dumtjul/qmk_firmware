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
 */

/* On Dvorak: maybe switch U and I, D and H, and L and M */

#include QMK_KEYBOARD_H
#include "keymap_us_extended.h"

// Define layer names for clarity
enum planck_layers {
  _DVORAK,
  _NUM,
  _FN
};

// Add custom keycodes like Apple's globe key
enum custom_keycodes {
  osGLB = SAFE_RANGE,
};

// Shortcuts layer 0 and 1
#define entFN   LT(_FN,KC_ENT)
#define spcNUM  LT(_NUM,KC_SPC)
#define bspSFT  LSFT_T(KC_BSPC)
#define escHS   LT(0,KC_ESC)

// Shortcuts layer 2
//#define osGLB   LT(0,KC_NO)
#define osCTL   OSM(MOD_LCTL)
#define osALT   OSM(MOD_LALT)
#define osGUI   OSM(MOD_LGUI)
#define delSFT  LSFT_T(KC_DEL)
#define lockHS  LT(0,C(G(KC_Q)))

// Shortcuts layer 3
#define sptl    G(KC_SPC)
#define hs      KC_F20

// KEYMAP
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DVORAK] = LAYOUT_planck_grid(
    KC_QUOT, KC_COMM, KC_DOT , KC_P   , KC_Y   , XXXXXXX, XXXXXXX, KC_F   , KC_G   , KC_C   , KC_R   , KC_L   ,
    KC_A   , KC_O   , KC_E   , KC_U   , KC_I   , XXXXXXX, XXXXXXX, KC_D   , KC_H   , KC_T   , KC_N   , KC_S   ,
    KC_SCLN, KC_Q   , KC_J   , KC_K   , KC_X   , XXXXXXX, XXXXXXX, KC_B   , KC_M   , KC_W   , KC_V   , KC_Z   ,
    XXXXXXX, XXXXXXX, XXXXXXX, entFN  , spcNUM , XXXXXXX, XXXXXXX, bspSFT , escHS  , XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_NUM] = LAYOUT_planck_grid(
    KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , XXXXXXX, XXXXXXX, KC_6   , KC_7   , KC_8   , KC_9   , KC_0   ,
    osGLB  , osCTL  , osALT  , osGUI  , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT,
    KC_MINS, KC_EQL , KC_SLSH, KC_BSLS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_TAB , US_GRV , KC_LBRC, KC_RBRC,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, delSFT , lockHS , XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_FN] = LAYOUT_planck_grid(
    KC_BRID, KC_BRIU, KC_LPAD, sptl   , MS_BTN1, XXXXXXX, XXXXXXX, KC_MRWD, KC_MPLY, KC_MFFD, KC_VOLD, KC_VOLU,
    KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , XXXXXXX, XXXXXXX, KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 ,
    KC_F11 , KC_F12 , KC_F13 , KC_F14 , KC_F15 , XXXXXXX, XXXXXXX, KC_F16 , KC_F17 , KC_F18 , KC_F19 , KC_F20 ,
    XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_LSFT, hs     , XXXXXXX, XXXXXXX, XXXXXXX
  )

};

/* Apple keycodes
 * Globe                    0x029D    AC_NEXT_KEYBOARD_LAYOUT_SELECT
 * Spotlight                0x0221    AC_SEARCH
 * Grave Accent and Tilde   0x0035

  case spot:
    host_consumer_send(record->event.pressed ? 0x0221 : 0);
    return false;

 */

// Variables
bool glb_down = false;
bool glb_tapping_term = false; // add this near the beginning of keymap.c
bool glb_subsequent_press = false;
bool glb_sent = false;
//bool glb_dictate;
uint16_t glb_timer = 0;     // we will be using them soon.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {

  case escHS:
    if (record->tap.count && record->event.pressed) {
      tap_code16(KC_ESC);
    } else if (record->event.pressed) {
      register_code16(KC_F20);
    } else {
      unregister_code16(KC_F20);
    }
    break;

  case lockHS:
    if (record->tap.count && record->event.pressed) {
      tap_code16(C(G(KC_Q)));
    } else if (record->event.pressed) {
      register_code16(KC_F20);
    } else {
      unregister_code16(KC_F20);
    }
    break;

  case osGLB:
    if (record->event.pressed) {
      glb_down = true;
      if (glb_tapping_term) {
        //glb_tapping_term = false;
        glb_subsequent_press = true;
      }
      glb_timer = timer_read();
      glb_tapping_term = true;
      if (glb_sent) {
      } else {
        host_consumer_send(0x029D);
        glb_sent = true;
      }
    } else {
      glb_down = false;
      //if (glb_sent) {
      //  host_consumer_send(0);
      //  glb_sent = false;
      //}
      if (glb_tapping_term) {
      } else {
        //if (glb_sent) {
        //} else {
        //  host_consumer_send(0x029D);
        //}
        //host_consumer_send(0);
        //glb_sent = false;
        //if (glb_sent) {
        //  host_consumer_send(0);
        //  glb_sent = false;
        //}
        //host_consumer_send(0x029D);
        //host_consumer_send(0);
        //glb_sent = false;
      }
    }
    break;

  }
  return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
  //if (record->event.pressed && glb_currently_latent && keycode != osGLB) {
  if (record->event.pressed && keycode != osGLB) {
    if (glb_sent && !glb_down) {
      host_consumer_send(0);
      glb_sent = false;
    } else {
    }
  }

}

void matrix_scan_user(void) { // The very important timer.
  if (glb_tapping_term) {
    //if (timer_elapsed(glb_timer) > 500) {
    if (timer_elapsed(glb_timer) > 1.5 * TAPPING_TERM) {
      glb_tapping_term = false;
      if (glb_down) {
        if (glb_sent) {
          host_consumer_send(0);
        } else {
        }
        host_consumer_send(0x029D);
        host_consumer_send(0);
        if (glb_subsequent_press) {
          host_consumer_send(0x029D);
          host_consumer_send(0);
        }
        glb_sent = false;
      }
      glb_subsequent_press = false;
    }
  }
}


  //case osGLB:
    //if (record->event.pressed) {    // PRESS
    //  host_consumer_send(0);
    //  if (glb_tapping_term) {
    //    glb_tapping_term--;
    //    glb_double_pressed++;
    //  } else {
    //    glb_tapping_term++;
    //  }
    //  if (record->tap.count) {          // <TERM
    //  } else {                          // >TERM
    //    if (glb_double_pressed) {
    //      glb_double_pressed--;
    //      glb_dictate++;
    //      host_consumer_send(0x029D);
    //      host_consumer_send(0);
    //      host_consumer_send(0x029D);
    //    } else {
    //      host_consumer_send(0x029D);
    //      host_consumer_send(0);
    //    }
    //  }
    //// POST
    //} else {                        // RELEASE
    //  if (glb_dictate) {
    //    glb_dictate--;
    //    host_consumer_send(0);
    //  }
    //  if (glb_tapping_term) {
    //    glb_currently_latent++;
    //    host_consumer_send(0x029D);
    //  }
    //  if (record->tap.count) {          // <TERM
    //  } else {                          // >TERM
    //  }
    //}
    //break;
    //

  //case KC_A ... KC_EXSL:
  //  if (record->event.pressed) {
  //    if (glb_currently_latent) {
  //      host_consumer_send(0x029D);
  //    }
  //    register_code(keycode);
  //  } else {
  //    unregister_code(keycode);
  //    if (glb_currently_latent) {
  //      glb_currently_latent--;
  //      host_consumer_send(0);
  //    }
  //  }
  //  break;
