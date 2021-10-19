 #include "interphase.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum interphase_layers
{
	_ISOPHASE,
	_FUNCTION
};


// enum interphase_keycodes
// {
//     FNKEY = SAFE_RANGE
// };


// Fillers to make layering more clear
#define XXXXXXX KC_NO
// Tap keys
#define T_RSFT MT(MOD_RSFT, KC_UP)
#define T_RCTL MT(MOD_RCTL, KC_RGHT)
#define T_RGUI MT(MOD_RGUI, KC_DOWN)
#define T_LSFT MT(MOD_LSFT, KC_UP)
#define T_LCTL MT(MOD_LCTL, KC_LEFT)
#define T_LGUI MT(MOD_LGUI, KC_DOWN)

#define T_L_FNKEY LT(_FUNCTION, KC_RIGHT)
#define T_R_FNKEY LT(_FUNCTION, KC_LEFT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_ISOPHASE] = {
	{KC_ESC,    KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL },
	{KC_TAB,      KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC },
	{KC_CAPS,   KC_NUBS,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,    KC_BSLS },
	{T_LSFT,   XXXXXXX,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, XXXXXXX,    T_RSFT },
	{T_LCTL,   T_LGUI,   T_L_FNKEY,   KC_LALT, KC_BSPC, KC_SPC,  XXXXXXX,   XXXXXXX, KC_SPC,  KC_ENT,  KC_RALT, T_R_FNKEY,   T_RGUI,    T_RCTL }
},

[_FUNCTION] = {
	{KC_ESC,  XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12},
 	{KC_VOLD, KC_VOLU, XXXXXXX, XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX,   KC_PGUP, KC_HOME, KC_UP,  KC_END,   XXXXXXX, XXXXXXX, XXXXXXX },
 	{XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RIGHT, XXXXXXX,   KC_PGDN, KC_LEFT, KC_DOWN, KC_RIGHT, KC_DELETE, XXXXXXX, XXXXXXX },
 	{XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX },
 	{KC_LCTL, KC_LGUI, XXXXXXX,   KC_LALT, KC_BSPC, KC_SPC,  XXXXXXX,   XXXXXXX, KC_SPC,  KC_ENT,  KC_RALT, XXXXXXX,   KC_RGUI, KC_RCTL }
},

};

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   switch (keycode) {
//     case FNKEY:
//       if (record->event.pressed) {
//         layer_on(_FUNCTION);
//       } else {
//         layer_off(_FUNCTION);
//       }
//       return false;
//       break;
//   }
//   return true;
// }

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   switch (keycode) {
//     case T_L_FNKEY:
//       if (record->tap.count && record->event.pressed) {
//         tap_code16(KC_RIGHT);
//         return false;
//       }
//       else if (record->event.pressed) {
//         layer_on(_FUNCTION);
//       }
//       else {
//         layer_off(_FUNCTION);
//       }
//       return false;
//       break;
//   }
//   return true;
// };

void matrix_scan_user(void) {
		return;
};
