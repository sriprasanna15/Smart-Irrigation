#include <LPC21xx.H>
#include "gsm.h"
#include "LCD2.H"

#define moisture_pin (1 << 22)
#define relay_pin (1 << 20)

#define DRY 0
#define WET 1

int previous_state = DRY; 
void motor_on(void) {
    IOCLR0 = relay_pin; 
}

void motor_off(void) {
    IOSET0 = relay_pin; 
}

int get_moisture_state() {
    return ((IOPIN0 >> 22) & 1) == 0 ? WET : DRY; 
}

int main() {
    int current_state; 

    LCD_init();
    IODIR0 |= relay_pin; 
    IOCLR0 = relay_pin; 
    LCD_scroll("Smart Irrigation");
    LCD_command(0x01); 
    LCD_command(0x80); 
    LCD_str("The condition:");

    
    current_state = get_moisture_state();
    if (current_state == WET) {
		sms_wet();
		motor_off();
        LCD_command(0xc0);
        LCD_str("Wet Condition");
		delay_ms(1000);
    } else {
		sms_dry();
		motor_on();
        LCD_command(0xc0);
        LCD_str("Dry Condition");
		delay_ms(1000);
    }
    
    previous_state = current_state; 

    while (1) {
        current_state = get_moisture_state(); 
        if (current_state != previous_state) { 
            if (current_state == WET) {
                motor_off();
                sms_wet();
                
                LCD_command(0xc0);
                LCD_str("Wet Condition");
				delay_ms(1000);
            } else {
                motor_on();
                sms_dry();
               
                LCD_command(0xc0);
                LCD_str("Dry Condition");
				delay_ms(1000);
            }

            previous_state = current_state; 
        }

        delay_ms(1000); 
    }
}
  
