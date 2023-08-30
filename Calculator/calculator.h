/* 
 * File:   app.h
 * Author: Dell
 *
 * Created on 01 ?????, 2023, 04:11 ?
 */

#ifndef APP_H
#define	APP_H
#include "ECU/LED/ecu_led.h"
#include "ECU/PUSH_BUTTON/ecu_button.h"
#include "ECU/RELAY/ecu_relay.h"
#include "ECU/DC_Motor/ecu_dc_motor.h"
#include "ECU/7-Segments/ecu_7_segments.h"
#include "ECU/KeyPad/ecu_keypad.h"
#include "ECU/LCD/ecu_lcd.h"
#include "string.h"
#define _XTAL_FREQ 8000000UL
void application_intialize();
#endif	/* APP_H */