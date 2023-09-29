// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include <stdio.h>
#include "print.h"
#include QMK_KEYBOARD_H
#define _BASE 0
#define _NUMBER 1
#define _SYMBOL 2
#define _EE 3
#define _EEC 4
#define LIFE 250
#define OLED_BURN_TIMEOUT 70000

unsigned long int session = 0;
unsigned long int lifetime = 0;
long int burnInPrevention = 0;
char sess[10];
char life[10];
char wpm[10];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_ortho_4x12(
    KC_ESC,                KC_Q,                KC_W,           KC_E,           KC_R,           KC_T,                               KC_Y,                  KC_U,                 KC_I,               KC_O,                 KC_P,             KC_BSPC,
    KC_TAB,                KC_A,                KC_S,           KC_D,           KC_F,           KC_G,                               KC_H,                  KC_J,                 KC_K,               KC_L,                 KC_SCLN,          KC_QUOT,
    KC_LEFT_SHIFT,         KC_Z,                KC_X,           KC_C,           KC_V,           KC_B,                               KC_N,                  KC_M,                 KC_COMM,            KC_DOT,               KC_SLSH,          KC_RIGHT_SHIFT,
                                                                                KC_SPC,         MO(_SYMBOL),                        MO(_NUMBER),           KC_SPC                                  
    ),                                                                                     
    [_NUMBER] = LAYOUT_ortho_4x12(                                                                     
    KC_GRAVE,              KC_1,                KC_2,           KC_3,           KC_4,          KC_5,                                KC_6,                  KC_7,                 KC_8,               KC_9,                 KC_0,             KC_BSPC,
    KC_NO,                 KC_NO,               KC_NO,          KC_NO,          KC_NO,         KC_NO,                               KC_NO,                 KC_MS_BTN1,           KC_MS_UP,           KC_MS_BTN2,           KC_WH_U,          KC_NO,
    KC_LEFT_SHIFT,         KC_NO,               KC_NO,          KC_NO,          KC_NO,         KC_NO,                               KC_NO,                 KC_MS_LEFT,           KC_MS_DOWN,         KC_MS_RIGHT,          KC_WH_D,          KC_RIGHT_SHIFT,
                                                                                KC_SPC,        KC_NO,                               KC_TRANSPARENT,        KC_SPC       
    ),                                 
    [_SYMBOL] = LAYOUT_ortho_4x12(                         
    KC_TAB,                 KC_PRINT_SCREEN,    TG(_EE),        KC_NO,          KC_NO,         LCTL(S(KC_T)),                       KC_LEFT_BRACKET,       KC_RIGHT_BRACKET,     KC_MINUS,           KC_EQUAL,             KC_BACKSLASH,     KC_DELETE,
    KC_CAPS_LOCK,           LCTL(KC_A),         LCTL(KC_S),     KC_NO,          LCTL(KC_F),    A(KC_F4),                            KC_AUDIO_VOL_UP,       KC_MEDIA_PREV_TRACK,  KC_UP,              KC_MEDIA_NEXT_TRACK,  KC_NO,            KC_ENTER,
    KC_LEFT_SHIFT,          LCTL(KC_Z),         LCTL(KC_X),     LCTL(KC_C),     LCTL(KC_V),    DB_TOGG,                             KC_AUDIO_VOL_DOWN,     KC_LEFT,              KC_DOWN,            KC_RIGHT,             KC_NO,            KC_RIGHT_SHIFT,
                                                                                KC_LEFT_ALT,   KC_TRANSPARENT,                      KC_NO,                 KC_MEDIA_PLAY_PAUSE
    ),
    [_EE] = LAYOUT_ortho_4x12(                         
    KC_NO,                  KC_NO,              KC_NO,          TG(_BASE),      MO(_EEC),      QK_BOOTLOADER,                       QK_REBOOT,             KC_NO,                KC_NO,              KC_NO,                 KC_NO,           KC_NO,
    KC_NO,                  KC_NO,              KC_NO,          KC_NO,          KC_NO,         KC_NO,                               KC_NO,                 KC_NO,                KC_NO,              KC_NO,                 KC_NO,           KC_NO,
    KC_NO,                  KC_NO,              KC_NO,          KC_NO,          KC_NO,         KC_NO,                               KC_NO,                 KC_NO,                KC_NO,              KC_NO,                 KC_NO,           KC_NO,
                                                                                KC_NO,         KC_NO,                               KC_NO,                 KC_NO
    ),
    [_EEC] = LAYOUT_ortho_4x12(                         
    KC_NO,                  KC_NO,              KC_NO,          KC_NO,          KC_TRANSPARENT,KC_NO,                               KC_NO,                 KC_NO,                KC_NO,              KC_NO,                 KC_NO,           KC_NO,
    KC_NO,                  KC_NO,              KC_NO,          KC_NO,          KC_NO,         KC_NO,                               KC_NO,                 KC_NO,                KC_NO,              KC_NO,                 KC_NO,           KC_NO,
    KC_NO,                  KC_NO,              KC_NO,          KC_NO,          KC_NO,         KC_NO,                               KC_NO,                 KC_NO,                KC_NO,              KC_NO,                 KC_NO,           KC_NO,
                                                                                KC_NO,         KC_NO,                               KC_NO,                 KC_NO
    )              
}; 

//void eeconfig_init_user(void) {
//    for(unsigned int i = 0; i <LIFE; i++)
//    {
//        eeprom_write_dword((uint32_t*)(i*4),0);
//    }
//}

void keyboard_pre_init_user(void) 
{
    unsigned long int max = 0;
    for(unsigned int i = 2; i <LIFE+2; i++){
       
        if ((unsigned long int)eeprom_read_dword((const uint32_t*)(i*4))>max)
        {
            max = (unsigned long int)eeprom_read_dword((const uint32_t*)(i*4));
        }
    }
    lifetime = max;    
}
void keyboard_post_init_user(void) 
{
  // Customise these values to desired behaviour
    debug_enable=false;
    debug_keyboard=true; 
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    for(unsigned int i = 0; i <LIFE; i++)
    {
       dprintf("%d, %lu\n",i,(unsigned long int)eeprom_read_dword((const uint32_t*)(i*4+8)));
    }
    if (record->event.pressed) 
    {
        session++;
        burnInPrevention = 0;
        if(session%10==0)
        {
            unsigned long int temp = lifetime + session;
            temp%=2500;
            temp/=10;
            eeprom_write_dword((uint32_t*)((int)temp*4+8), lifetime + session);
        }
    }
    return true; 
};
long burnout_bar = 0;
#ifdef OLED_ENABLE  

oled_rotation_t oled_init_user(oled_rotation_t rotation)
{
    return OLED_ROTATION_180;
}

uint8_t x[] = {73,73,74,74,74,75,75,76,76,76,77,77,78,78,79,79,80,80,81,82,82,83,84,84,85,86,86,87,88,89,90,91,92,93,94,95,96,97,97,98,99,100,100,101,101,102,103,103,104,104,105,105,106,106,107,107,107,108,108,108,109,109,109,110,110,110,110,110,110,110,110,110,110,110,109,109,109,108,108,108,107,107,106, 101,100,99,99,98,97,96,95,94,93,92,91,90,89,88,87,86,86,85,84,84,83,82,82,81,80,80,79,79,78,78,77,77,76,76,76,75,75,75,74,74,74,73,73,73,73,114,114,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,124,125,125,125,124,123,122,121,120,119,118,117,117,116,115,114,113,113,112,111,110,109,110,109,108,107,106,107,106,105,104,103,104,103,102,101,100,101,100,99,98,99,98,97,96,97,96,95,94,93,94,93,92,91,92,91,90,89,90,89,88,87,88,87,86,85,86,85,84,83,84,83,82,82,81,80,80,79,78,79,78,77,77,76,75,75,74,73,74,73,72,72,71,70,71,70,69,69,68,68,67,66,66,65,65,64,64,63,63,62,62,61,60,60,59,59,60,61,61,62,63,64,65,66,67,68,69,70,71,72,72,73,74,75};
uint8_t y[] = {17,16,16,15,14,13,12,12,11,10,10,9,9,8,8,7,7,6,6,6,5,5,5,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,5,5,6,6,6,7,7,8,8,9,9,10,10,11,12,12,13,14,14,15,16,16,17,18,19,20,21,22,23,24,25,26,27,28,29,29,30,31,31,32,33,38,38,38,39,39,39,40,40,40,40,40,40,40,40,40,40,40,39,39,39,38,38,38,37,37,37,36,36,35,35,34,34,33,33,32,31,31,30,29,29,28,27,27,26,25,24,27,28,28,28,29,29,30,30,30,31,31,32,32,33,33,34,34,35,35,36,36,37,38,38,39,40,40,40,40,40,40,40,40,40,39,39,39,39,39,38,38,38,38,38,37,37,37,37,37,36,36,36,36,36,35,35,35,35,35,34,34,34,34,34,33,33,33,33,32,32,32,32,32,31,31,31,31,30,30,30,30,29,29,29,29,28,28,28,28,27,27,27,27,26,26,26,25,25,25,24,24,24,23,23,23,22,22,22,21,21,21,20,20,20,19,19,18,18,17,17,16,16,16,15,15,14,14,13,13,12,12,11,11,10,9,8,7,6,6,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7};

//{114,114,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,124,125,125,125,124,123,122,121,120,119,118,117,117,116,115,114,113,113,112,111,110,109,110,109,108,107,106,107,106,105,104,103,104,100,101,100,99,98,99,98,97,96,97,96,95,94,93,94,93,92,91,92,91,90,89,90,89,88,87,88,87,86,85,86,85,84,83,84,83,82,82,81,80,80,79,78,79,78,77,77,76,75,75,74,73,74,73,72,72,71,70,71,70,69,69,68,68,67,66,66,65,65,64,64,63,63,62,62,61,60,60,59,59,60,61,61,62,63,64,65,66,67,68,69,70,71,72,72,73,75,75}
//{27,28,28,28,29,29,30,30,30,31,31,32,32,33,33,34,34,35,35,36,36,37,38,38,39,40,40,40,40,40,40,40,40,40,39,39,39,39,39,38,38,38,38,38,37,37,37,37,37,36,36,36,36,36,35,35,34,34,34,34,34,33,33,33,33,32,32,32,32,32,31,31,31,31,30,30,30,30,29,29,29,29,28,28,28,28,27,27,27,27,26,26,26,25,25,25,24,24,24,23,23,23,22,22,22,21,21,21,20,20,20,19,19,18,18,17,17,16,16,16,15,15,14,14,13,13,12,12,11,11,10,9,8,7,6,6,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7}

int frame = 0;
int count = 0;
bool del = false;
bool oled_task_user(){
    
    oled_set_cursor(0, 1);
    switch (get_highest_layer(layer_state)) 
    {
        case _BASE :
            oled_write("base   ", false);
            break;
        case _NUMBER :
            oled_write("number ", false);
            break;
        case _SYMBOL : 
            oled_write("symbol ", false);
            break;
        case _EE : 
            oled_write("EE     ", false);
            break;
        case _EEC : 
            oled_write("EEC    ", false);
            for(int i = 0; i <1010; i++)
            {
                eeprom_write_byte((uint8_t*)i,0);
            }
            eeprom_write_dword((uint32_t*)(0), 0);
            eeprom_write_dword((uint32_t*)(4), 0);
            break;
    }
    if(burnInPrevention >= OLED_BURN_TIMEOUT) 
    {
        oled_off();
        return false;
    }
    else
    {
        oled_on();
        burnout_bar = (double)burnInPrevention/OLED_BURN_TIMEOUT * 128 - 2;
        for(int i = 0; i<128;i++)
        {
            if(i<burnout_bar)
            {
                oled_write_pixel(i, 1, true);
                oled_write_pixel(i, 2, true);
            }
            else
            {
                oled_write_pixel(i, 1, false);
                oled_write_pixel(i, 2, false);
            }
        }
        burnInPrevention++;
    }
    oled_set_cursor(0, 2);
    //Caps lock status
    led_t led_state = host_keyboard_led_state();
    oled_write(led_state.caps_lock ? "caps locked" : "caps off   ", false);
    
    oled_set_cursor(0, 3);
    if(debug_enable==false)
        oled_write("     ",false);
    else
        oled_write("debug",false);
    oled_set_cursor(0, 5); 
    oled_write("wpm:",false);
    oled_set_cursor(4,5);
    sprintf(wpm,"%3d",get_current_wpm());
    if (get_current_wpm() <2)
        sprintf(wpm,"%3d",0);
    if(get_current_wpm()>220)
        oled_write("boi",false);
    else
        oled_write(wpm,false); 
    oled_set_cursor(0, 6); 
    oled_write("session:",false);
    oled_set_cursor(8,6);
    sprintf(sess,"%lu",session);
    oled_write(sess,false);
    oled_set_cursor(0, 7);
    oled_write("lifetime:",false);
    oled_set_cursor(9,7);
    unsigned long int temp = lifetime + session;
    sprintf(life,"%lu",temp);
    oled_write_ln(life,false);
          if(count+get_current_wpm()/3>=50)
    {
        if(del == false)
        {
            oled_write_pixel(x[frame]+1, y[frame], true);
            frame++;
            if(frame == sizeof(x)/sizeof(uint8_t))
            {
                frame = 0;
                del = true;
            }
        }
        else
        {
            oled_write_pixel(x[frame]+1, y[frame], false);
            frame++;
            if(frame == sizeof(x)/sizeof(uint8_t))
            {
                frame = 0;
                del = false;
            }
        }
        count = 0;
    }
    count++;
    oled_set_cursor(0, 4);
    sprintf(sess,"%6ld",burnInPrevention);
    if(debug_enable==true)
        oled_write(sess,false);
    else
        oled_write("      ",false);
    return false;
}   

#endif
