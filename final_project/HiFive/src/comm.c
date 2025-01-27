#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "eecs388_lib.h"

#define SERVO_PULSE_MAX 2400
#define SERVO_PULSE_MIN 544
#define SERVO_PERIOD 20000

void auto_brake(int devid)
{
    // Task-1: 
    // Your code here (Use Lab 02 - Lab 04 for reference)
    // Use the directions given in the project document

    uint16_t distance_data = 0;

    if ('Y' == ser_read(devid) && 'Y' == ser_read(devid)) {
        
        uint8_t low = ser_read(devid);
        uint8_t high = ser_read(devid);

        distance_data = (high << 8) | low;
        printf("\nDistance %d", distance_data)
        
        if (distance_data > 200) {
            gpio_write(BLUE_LED, OFF); 
            gpio_write(RED_LED, OFF); 
            gpio_write(GREEN_LED, ON); 
        }
        else if (distance_data > 100 && distance_data <= 200) {
            gpio_write(BLUE_LED, OFF); 
            gpio_write(RED_LED, ON); 
            gpio_write(GREEN_LED, ON); 
        }
        else if (distance_data > 60 && distance_data <= 100) {
            gpio_write(BLUE_LED, OFF); 
            gpio_write(GREEN_LED, OFF); 
            gpio_write(RED_LED, ON); 
        }
        else if (distance_data <= 60) {
            gpio_write(BLUE_LED, OFF);
            gpio_write(GREEN_LED, OFF);
            gpio_write(RED_LED, ON);
            delay(100);
            gpio_write(RED_LED, OFF);
            delay(100);

        }                
    }
}

int read_from_pi(int devid)
{
    int angle = 0;

    if (ser_isready(1)) {
        char buffer[100];
        ser_readline(1, 100, buffer);
        // printf("from PI to HIFIVE: read (%d): %s\n", read_data, buffer);
        sscanf(buffer, "%d", &angle);

    }
    return angle;
}

void steering(int gpio, int pos)
{
    int degree = round((SERVO_PULSE_MAX - SERVO_PULSE_MIN) / 180);
    int time = SERVO_PULSE_MIN + (pos * degree);
    int time_remain = SERVO_PERIOD - time;

    gpio_write(gpio, ON);
    delay_usec(time);
    gpio_write(gpio, OFF);
    delay_usec(time_remain);
}

int main()
{
    // initialize UART channels
    ser_setup(0); // uart0
    ser_setup(1); // uart1
    int pi_to_hifive = 1; //The connection with Pi uses uart 1
    int lidar_to_hifive = 0; //the lidar uses uart 0
    
    printf("\nUsing UART %d for Pi -> HiFive", pi_to_hifive);
    printf("\nUsing UART %d for Lidar -> HiFive", lidar_to_hifive);
    
    //Initializing PINs
    gpio_mode(PIN_19, OUTPUT);
    gpio_mode(RED_LED, OUTPUT);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_mode(GREEN_LED, OUTPUT);

    printf("Setup completed.\n");
    printf("Begin the main loop.\n");

    while (1) {

        auto_brake(lidar_to_hifive); // measuring distance using lidar and braking
        int angle = read_from_pi(pi_to_hifive); //getting turn direction from pi
        printf("\nangle=%d", angle) 

        int gpio = PIN_19; 
        for (int i = 0; i < 10; i++){
            // Here, we set the angle to 180 if the prediction from the DNN is a positive angle
            // and 0 if the prediction is a negative angle.
            // This is so that it is easier to see the movement of the servo.
            // You are welcome to pass the angle values directly to the steering function.
            // If the servo function is written correctly, it should still work,
            // only the movements of the servo will be more subtle
            // if(angle>0){
            //      steering(gpio, 180);
            //  }
            // else {
            //     steering(gpio,0);
            // }
            
            // Uncomment the line below to see the actual angles on the servo.
            // Remember to comment out the if-else statement above!
            steering(gpio, angle);
        }

    }
    return 0;
}
