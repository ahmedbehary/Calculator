/* 
 * File:   app.c
 * Author: Dell
 *
 * Created on 01 Aug, 2023, 04:11 ?
 */
#include "app.h"
#include "C:\Program Files\Microchip\MPLABX\v6.10\packs\Microchip\PIC18Fxxxx_DFP\1.4.151\xc8\pic\include\builtins.h"
void application_initaialize(void);

lcd_t lcd1 = { 
    .lcd_rs.direction = DIRECTION_OUTPUT,
    .lcd_rs.logic = LOW,
    .lcd_rs.pin = PIN0_INDEX,
    .lcd_rs.port = PORTD_INDEX,
    
    .lcd_enable.direction = DIRECTION_OUTPUT,
    .lcd_enable.logic = LOW,
    .lcd_enable.pin = PIN1_INDEX,
    .lcd_enable.port = PORTD_INDEX,
    
    .lcd_data[0].direction = DIRECTION_OUTPUT,
    .lcd_data[0].logic = LOW,
    .lcd_data[0].pin = PIN0_INDEX,
    .lcd_data[0].port = PORTC_INDEX,
    
    .lcd_data[1].direction = DIRECTION_OUTPUT,
    .lcd_data[1].logic = LOW,
    .lcd_data[1].pin = PIN1_INDEX,
    .lcd_data[1].port = PORTC_INDEX,
    
    .lcd_data[2].direction = DIRECTION_OUTPUT,
    .lcd_data[2].logic = LOW,
    .lcd_data[2].pin = PIN2_INDEX,
    .lcd_data[2].port = PORTC_INDEX,
    
    .lcd_data[3].direction = DIRECTION_OUTPUT,
    .lcd_data[3].logic = LOW,
    .lcd_data[3].pin = PIN3_INDEX,
    .lcd_data[3].port = PORTC_INDEX,
    
    .lcd_data[4].direction = DIRECTION_OUTPUT,
    .lcd_data[4].logic = LOW,
    .lcd_data[4].pin = PIN4_INDEX,
    .lcd_data[4].port = PORTC_INDEX,
    
    .lcd_data[5].direction = DIRECTION_OUTPUT,
    .lcd_data[5].logic = LOW,
    .lcd_data[5].pin = PIN5_INDEX,
    .lcd_data[5].port = PORTC_INDEX,
    
    .lcd_data[6].direction = DIRECTION_OUTPUT,
    .lcd_data[6].logic = LOW,
    .lcd_data[6].pin = PIN6_INDEX,
    .lcd_data[6].port = PORTC_INDEX,
    
    .lcd_data[7].direction = DIRECTION_OUTPUT,
    .lcd_data[7].logic = LOW,
    .lcd_data[7].pin = PIN7_INDEX,
    .lcd_data[7].port = PORTC_INDEX,
};

keypad_t keypad1 = {
  .column_pin[0].direction = DIRECTION_INPUT,
  .column_pin[0].logic = LOW,
  .column_pin[0].pin = PIN2_INDEX,
  .column_pin[0].port = PORTD_INDEX,
  
  .column_pin[1].direction = DIRECTION_INPUT,
  .column_pin[1].logic = LOW,
  .column_pin[1].pin = PIN3_INDEX,
  .column_pin[1].port = PORTD_INDEX,

  .column_pin[2].direction = DIRECTION_INPUT,
  .column_pin[2].logic = LOW,
  .column_pin[2].pin = PIN4_INDEX,
  .column_pin[2].port = PORTD_INDEX,

  .column_pin[3].direction = DIRECTION_INPUT,
  .column_pin[3].logic = LOW,
  .column_pin[3].pin = PIN5_INDEX,
  .column_pin[3].port = PORTD_INDEX,  
  
  .row_pin[0].direction = DIRECTION_OUTPUT,
  .row_pin[0].logic = LOW,
  .row_pin[0].pin = PIN0_INDEX,
  .row_pin[0].port = PORTA_INDEX,
  
  .row_pin[1].direction = DIRECTION_OUTPUT,
  .row_pin[1].logic = LOW,
  .row_pin[1].pin = PIN1_INDEX,
  .row_pin[1].port = PORTA_INDEX,
  
  .row_pin[2].direction = DIRECTION_OUTPUT,
  .row_pin[2].logic = LOW,
  .row_pin[2].pin = PIN2_INDEX,
  .row_pin[2].port = PORTA_INDEX,
  
  .row_pin[3].direction = DIRECTION_OUTPUT,
  .row_pin[3].logic = LOW,
  .row_pin[3].pin = PIN3_INDEX,
  .row_pin[3].port = PORTA_INDEX,
};


const uint8 customChar[] = {
  0x0E,
  0x11,
  0x1B,
  0x11,
  0x15,
  0x11,
  0x0E,
  0x00
};
uint8 keypad_read_value(void); 
void cal_display_result(uint8 *operation, uint8 len);
uint8 get_operation_after_mult_div(uint8* operation, uint8 len);
void get_two_operands_result(uint8* num1_text, uint8* num2_text, uint8 op, uint8* num1_len);
uint8 isInputValid(uint8* operation, uint8 len);
uint8 keypadValue , keypadValue1;
uint8 lcd_cursor;
uint8 read_data[20];
uint8 data_index;
uint8 equal_sign_flag;
Std_ReturnType ret = E_NOT_OK;

int main() {
    application_initaialize();
    while(1){
        keypadValue = keypad_read_value();
        if(keypadValue == '#'){
            ret = lcd_send_command(&lcd1, LCD_CLEAR_DISPLAY);
            ret = lcd_send_string_data_pos(&lcd1,2,7,"Welcome");
            ret = lcd_send_custom_char(&lcd1, 2,15,customChar,0);
            __delay_ms(1000);
            ret = lcd_send_command(&lcd1, LCD_CLEAR_DISPLAY);
        }
        if('#' == keypadValue && 1 == equal_sign_flag)
        {
            ret = lcd_send_command(&lcd1, LCD_CLEAR_DISPLAY);
            memset(read_data, '\0', 20);
            lcd_cursor = 0;
            data_index = 0;
            equal_sign_flag = 0;
        }
        else if('#' == keypadValue && 0 < data_index)
        {
            ret = lcd_send_char_data_pos(&lcd1, 1, lcd_cursor, ' ');
            data_index--;
            lcd_cursor--;
            read_data[data_index] = '\0';
        }
        else if('=' == keypadValue)
        {   
            if(!isInputValid(read_data, data_index))
            {
                ret = lcd_send_command(&lcd1, LCD_CLEAR_DISPLAY);
                ret = lcd_send_string_data_pos(&lcd1, 1, 3, "Invalid operation");
                ret = lcd_send_string_data_pos(&lcd1, 2, 6, "Try again");
                __delay_ms(1000);
                ret = lcd_send_command(&lcd1, LCD_CLEAR_DISPLAY);
                memset(read_data, '\0', 20);
                lcd_cursor = 0;
                data_index = 0;
            }
            else{
                ret = lcd_send_char_data_pos(&lcd1, 2, 1, '=');
                cal_display_result(read_data, data_index);
                equal_sign_flag = 1;
            }
        }
        else if('#' != keypadValue && (20 != lcd_cursor))
        {
            ret = lcd_send_char_data_pos(&lcd1, 1, ++lcd_cursor, keypadValue);
            read_data[data_index++] = keypadValue;
        }
        else { /* Do Nothing */ }
    }
    return (EXIT_SUCCESS);    
}

void application_initaialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ret = lcd_initialize(&lcd1);
    ret = keypad_initialize(&keypad1);
}

uint8 keypad_read_value(void){
    uint8 key_pressed_value = ZERO_INIT;
    do{
        __delay_ms(20);
        ret = keypad_get_value(&keypad1,&key_pressed_value);
    }while(key_pressed_value == 0);
    __delay_ms(200);
    return key_pressed_value;
}

void cal_display_result(uint8 *operation, uint8 len){
    uint8 high_priority_op = 0;
    uint8 low_priority_op = 0;
    uint8 different_priority_flag = 0;
    for(uint8 op_index = 1 ; op_index < len ; op_index++){
        if('*' == operation[op_index] || '/' == operation[op_index]){
            high_priority_op = 1;
            if(low_priority_op){
                different_priority_flag = 1;
                break;
            }
            else{/* Do Nothing */}
        }
        else if('+' == operation[op_index] || ('-' == operation[op_index] && '0' <= operation[op_index-1])){
            low_priority_op = 1;
            if(high_priority_op){
                different_priority_flag = 1;
                break;
            }
            else{/* Do Nothing */}
        }
        else{/* Do Nothing */}
    }
    
    if(different_priority_flag){
        len = get_operation_after_mult_div(operation, len);
    }
    else {/* Do Nothing */}
    
    uint8 num1_text[8] = {[0] = operation[0],'\0','\0','\0','\0','\0','\0','\0'};
    uint8 num2_text[8] = {0};
    uint8 num1_index = 1, n2_index = 0;

    for(uint8 op_index = 1 ; op_index < len ; op_index++)
    {
        if('0' <= operation[op_index] && '9' >= operation[op_index])
        {
            num1_text[num1_index++] = operation[op_index];
        }
        else if('0' > operation[op_index])
        {
            uint8 op = operation[op_index];
            num2_text[n2_index++] = operation[++op_index];
            op_index++;
            while('0' <= operation[op_index] && '9' >= operation[op_index]){
                num2_text[n2_index++] = operation[op_index++];
            }
            op_index--;
            get_two_operands_result(num1_text, num2_text, op, &num1_index);
            n2_index = 0;
            memset(num2_text, '\0', 8);
        }
    }
    ret = lcd_send_string_data_pos(&lcd1,2,3,num1_text);
}

uint8 isInputValid(uint8* operation, uint8 len){
    uint8 op_valid = 1;
    if((operation[0] < '0' && operation[0] != '-') || '0' > operation[len-1]){
        op_valid = 0;
    }
    else{
        uint8 last_char = operation[0];
        for(uint8 op_index = 1;op_index < len;++op_index){
            if(('0' > last_char) && ('0' > operation[op_index]) && ('-' != operation[op_index])){
                op_valid = 0;
                break;
            }
            else if('/' == last_char && '0' == operation[op_index]){
                op_valid = 0;
                break;
            }
            else if('-' == last_char && '-' == operation[op_index] && operation[op_index+1]){
                op_valid = 0;
                break;
            }
            last_char = operation[op_index];
        }
    }
    return op_valid;
}

uint8 get_operation_after_mult_div(uint8* operation, uint8 len){
    uint8 result[20] = {0};
    uint8 result_index = 0;
    uint8 num1_text[8] = {[0] = operation[0],'\0','\0','\0','\0','\0','\0','\0'};
    uint8 num2_text[8] = {0};
    uint8 num1_index = 1, num2_index = 0;
    for(int op_index = 1 ; op_index < len ; op_index++)
    {
        if('0' <= operation[op_index] && '9' >= operation[op_index])
        {
            num1_text[num1_index++] = operation[op_index];
            if(len-1 == op_index){
                while(num1_index--){
                    result[result_index+num1_index] = num1_text[num1_index];
                }
            }
        }
        else if('-' == operation[op_index] || '+' == operation[op_index])
        {
            uint8 temp = num1_index;
            while(num1_index--){
                result[result_index+num1_index] = num1_text[num1_index];
            }
            result_index += temp;
            result[result_index++] = operation[op_index];

            num1_index = 0;
            memset(num1_text, '\0', 8);
            num1_text[num1_index++] = operation[++op_index];

            if(len-1 == op_index){
                result[result_index] = num1_text[num1_index-1];
            }
        }
        else if('*' == operation[op_index] || '/' == operation[op_index])
        {
            uint8 op = operation[op_index];
            num2_text[num2_index++] = operation[++op_index];
            op_index++;
            while('0' <= operation[op_index] && '9' >= operation[op_index]){
                num2_text[num2_index++] = operation[op_index++];
            }
            op_index--;
            get_two_operands_result(num1_text, num2_text, op, &num1_index);
            num2_index = 0;
            memset(num2_text, '\0', 8);
            
            if(len-1 == op_index){
                while(num1_index--){
                    result[result_index+num1_index] = num1_text[num1_index];
                }
            }
        }
        else{/* Do Nothing */}
    }
    memset(operation, '\0', 20);
    strcpy(operation, result);
    return strlen(operation);
}

void get_two_operands_result(uint8* num1_text, uint8* num2_text, uint8 op, uint8* num1_len){
    sint32 num1_value = 0, num2_value = 0, result = 0;
    num1_value = atol(num1_text);
    num2_value = atol(num2_text);

    switch (op)
    {
        case '+': result = num1_value + num2_value; break;
        case '-': result = num1_value - num2_value; break;
        case '*': result = num1_value * num2_value; break;
        case '/': result = num1_value / num2_value; break;
        default:                                    break;
    }
    
    memset(num1_text, '\0', 8);
    sprintf(num1_text, "%d", result);

    *num1_len = strlen(num1_text);
}
