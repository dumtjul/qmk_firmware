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

/* Opmerkingen
 *
 * In aanvulling op de andere Engram-wijzigingen:
 *
 *   Dead-Option interception:
 *     - Option-E; Option-U enz. onderscheppen en verplaatsen naar de posities waar ik ze verwacht: `; "; enz.
 */

#include QMK_KEYBOARD_H
#include "keymap_us_extended.h"

// Define layer names for clarity
enum planck_layers {
  _DVORAK,
  _RAISE
};

// Define cutstom keycodes
enum custom_keycodes {
  glb,
  lock,
  dict,
  sptl
};

// Shortcuts layer 0 and 1
#define hs      KC_F20
#define raise   MO(_RAISE)
//#define lock    LT(0,KC_Q)
#define lpad    KC_LPAD
#define grv     US_GRV
#define pgrph   KC_GRV

// KEYMAP
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DVORAK] = LAYOUT_planck_grid(
    KC_ESC ,  KC_QUOT,  KC_COMM,  KC_DOT ,  KC_P   ,  KC_Y   ,  KC_F   ,  KC_G   ,  KC_R   ,  KC_L   ,  KC_C   ,  KC_BSPC,
    KC_TAB ,  KC_A   ,  KC_O   ,  KC_E   ,  KC_I   ,  KC_U   ,  KC_H   ,  KC_D   ,  KC_N   ,  KC_T   ,  KC_S   ,  KC_ENT ,
    grv    ,  KC_SCLN,  KC_Q   ,  KC_J   ,  KC_K   ,  KC_X   ,  KC_B   ,  KC_M   ,  KC_W   ,  KC_V   ,  KC_Z   ,  KC_SLSH,
    glb    ,  KC_LCTL,  KC_LALT,  KC_LGUI,  hs     ,  KC_LSFT,  KC_SPC ,  raise  ,  KC_RGUI,  KC_RALT,  KC_RCTL,  glb
  ),

  [_RAISE] = LAYOUT_planck_grid(
    XXXXXXX,  KC_1   ,  KC_2   ,  KC_3   ,  KC_4   ,  KC_5   ,  KC_6   ,  KC_7   ,  KC_8   ,  KC_9   ,  KC_0   ,  lock   ,
    XXXXXXX,  KC_LEFT,  KC_DOWN,  KC_UP  ,  KC_RGHT,  XXXXXXX,  XXXXXXX,  KC_LBRC,  KC_RBRC,  KC_MINS,  KC_EQL ,  XXXXXXX,
    pgrph  ,  KC_BRID,  KC_BRIU,  lpad   ,  sptl   ,  dict   ,  KC_MRWD,  KC_MPLY,  KC_MFFD,  KC_VOLD,  KC_VOLU,  KC_BSLS,
    _______,  _______,  _______,  _______,  _______,  _______,  XXXXXXX,  _______,  _______,  _______,  _______,  _______
  ),

};

/* Apple keycodes (0x0)
 * Usage ID  Usage Name                       Notes                     Description
 *     Generic Desktop Page (0x01): host_system_send()
 *       81  System Power Down                ?                         ... to initiate system-wide power down now from Full Power or Sleep states.
 *       82  System Sleep                     ?                         ... to ... If the system is already in the Low Power state, there is no eﬀect.
 *       A8  System Hibernate                 ?                         ... to place the system in a "deeper" sleep state than System Sleep.
 *       9B  System Do Not Disturb            Toggle Focus Mode         Toggle system-wide Do Not Disturb (DND) mode On/Oﬀ.     
 *
 *     Consumer Page (0x0C): host_consumer_send()
 *       30  Power                            Lock (Design Guidelines)  Controls the application-specific power state.
 *       83  Recall Last                      ?                         Returns to the last selected channel or mode.           
 *       CF  Voice Command                    Dictation                 Initiates listening for Voice Command.                  
 *      1AE  AL Keyboard Layout               Toggle Onscreen Keyboard  Launch Keyboard Layout Management application.          
 *      221  AC Search                        Spotlight                 Search for documents (URLs, files, web pages, etc).     
 *      222  AC Go To                         ?                         Display a certain point in the document.                
 *      26B  AC Lock                          ?                         Lock display to current location in document.           
 *      26C  AC Unlock                        ?                         Unlock display from current location in document.       
 *      279  AC Redo/Repeat                   ?                         Redo or Repeat last action.                             
 *      29D  AC Next Keyboard Layout Select   Globe Key                 Switch through set of keyboard layouts.                 
 *
 *     Keyboard/Keypad Page (0x07)
 *       66  Keyboard Power[1]                ?
 *
 * [1] Reserved for typical keyboard status or keyboard errors. Sent as a member of the keyboard array. Not a physical key.
 *
 * case sptl:
 *   host_consumer_send(record->event.pressed ? 0x221 : 0);
 *   return false;
 *
 */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  switch (keycode) {

    case glb: //from: custom_keycodes
      host_consumer_send(record->event.pressed ? 0x29D : 0);
      break;

    //case lock: //from: define LT(0,KC_Q)
    //  if (!record->tap.count && record->event.pressed) {
    //    tap_code16(C(G(KC_Q));
    //  }
    //  break;

    case lock: //from: custom_keycodes
      host_consumer_send(record->event.pressed ? 0x30 : 0);
      break;

    case sptl: //from: custom_keycodes
      host_consumer_send(record->event.pressed ? 0x221 : 0);
      break;

    case dict: //from: custom_keycodes
      host_consumer_send(record->event.pressed ? 0xCF : 0);
      break;

  }//endswitch

  return true;
}

