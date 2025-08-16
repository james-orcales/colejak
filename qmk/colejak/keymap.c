#include QMK_KEYBOARD_H


enum layer_number {
  _BASE = 0,
  _SYMBOLS_LAYER,
  _NUMBERS_LAYER,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


 [_BASE] = LAYOUT(
  KC_LEFT_SHIFT, KC_LEFT_ALT, KC_HOME, KC_ENTER, KC_END,  KC_DELETE,                                  KC_LEFT, KC_DOWN, KC_UP,    KC_RIGHT, KC_RIGHT_ALT, KC_PRINT_SCREEN,
  KC_TAB,        KC_Q,        KC_W,    KC_F,     KC_P,    KC_B,                                       KC_BSPC, KC_L,    KC_U,     KC_Y,     KC_J,         LCSG(KC_LEFT_ALT),
  KC_LEFT_CTRL,  KC_A,        KC_R,    KC_S,     KC_T,    KC_G,                                       KC_M,    KC_N,    KC_E,     KC_I,     KC_O,         KC_RIGHT_CTRL,
  KC_LEFT_GUI,   KC_Z,        KC_X,    KC_C,     KC_D,    KC_V,                KC_ESCAPE,  KC_ESCAPE, KC_K,    KC_H,    KC_GRAVE, KC_COMMA, KC_SLASH,     KC_RIGHT_GUI,
                              XXXXXXX, XXXXXXX, KC_DOT, MO(_SYMBOLS_LAYER),                           KC_RIGHT_SHIFT,   KC_SPACE, XXXXXXX,  XXXXXXX
),


[_SYMBOLS_LAYER] = LAYOUT(
  _______, _______,               _______,         _______,       _______,             _______,                            _______,      _______,              _______,         _______,          _______,                _______,
  _______, KC_AT,                 KC_QUOTE,        KC_EQUAL,      KC_MINUS,            KC_ASTERISK,                        _______,      KC_COLON,             KC_DOUBLE_QUOTE, KC_EXCLAIM,       KC_PLUS,                _______,
  _______, KC_LEFT_ANGLE_BRACKET, KC_LEFT_BRACKET, KC_LEFT_PAREN, KC_LEFT_CURLY_BRACE, KC_PIPE,                            KC_AMPERSAND, KC_RIGHT_CURLY_BRACE, KC_RIGHT_PAREN,  KC_RIGHT_BRACKET, KC_RIGHT_ANGLE_BRACKET, _______,
  _______, XXXXXXX,               XXXXXXX,         KC_CIRCUMFLEX, KC_SEMICOLON,        KC_PERCENT,   KC_ESCAPE, KC_ESCAPE, KC_HASH,      KC_DOLLAR,            KC_TILDE,        KC_BACKSLASH,     KC_QUESTION,            _______,
                                                            XXXXXXX, XXXXXXX, _______,  _______,                            MO(_NUMBERS_LAYER),  _______, XXXXXXX, XXXXXXX
),


[_NUMBERS_LAYER] = LAYOUT(
  _______, _______,            _______,          _______, _______, _______,                   _______, _______,       _______, _______,           _______,         KC_SYSTEM_SLEEP,
  _______, KC_BRIGHTNESS_DOWN, KC_BRIGHTNESS_UP, KC_9,    XXXXXXX, XXXXXXX,                   _______, KC_AUDIO_MUTE, KC_7,    KC_AUDIO_VOL_DOWN, KC_AUDIO_VOL_UP, _______,
  _______, KC_4,               KC_3,             KC_2,    KC_1,    XXXXXXX,                   XXXXXXX, KC_8,          KC_0,    KC_6,              KC_5,            _______,
  _______, KC_F1,              KC_F2,            KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,         KC_F10,  KC_F11,            KC_F12,          _______,
                                        XXXXXXX, XXXXXXX, _______, _______,                   _______,  _______, XXXXXXX, XXXXXXX
)
};


const key_override_t override_period_underscore = ko_make_basic(MOD_MASK_SHIFT, KC_DOT,   KC_UNDERSCORE);
const key_override_t override_comma_backslash   = ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_BACKSLASH );
const key_override_t *key_overrides[] = {
	&override_period_underscore,
	&override_comma_backslash,
};


//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return rotation;
}


// When you add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);


bool oled_task_user(void) {
    oled_write(read_logo(), false);
    return false;
}
#endif // OLED_ENABLE


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef OLED_ENABLE
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }
  return true;
}
