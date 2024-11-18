#include "LEDIndicator.h"

void LEDIndicator::init() {
    led_.clear();
    led_.show();
}

void LEDIndicator::update() {
    if (enabled_) {
        if (blink_) {
            if (!last_state_) {
                if ((millis() - last_blink_) >= current_speed_) {
                    // Turn on LED
                    for (uint32_t i = 0; i < led_count_; i++) {
                        led_.setPixelColor(i, led_.Color(current_color_.red()*255, current_color_.green()*255, current_color_.blue()*255));
                    }
                    last_blink_ = millis();
                    last_state_ = true;
                }
            }
            else {
                if ((millis() - last_blink_) >= current_speed_) {
                    // Turn off LED
                    for (uint32_t i = 0; i < led_count_; i++) {
                        led_.setPixelColor(i, led_.Color(0, 0, 0));
                    }
                    last_blink_ = millis();
                    last_state_ = false;
                } 
            }
        }
        else {
            // Turn on LED
            for (uint32_t i = 0; i < led_count_; i++) {
                led_.setPixelColor(i, led_.Color(current_color_.red()*255, current_color_.green()*255, current_color_.blue()*255));
            }
            last_state_ = true;
        }
    }
    else {
        // Turn off LED
        for (uint32_t i = 0; i < led_count_; i++) {
            led_.setPixelColor(i, led_.Color(0, 0, 0));
        }
        last_state_ = false;
    }
    led_.show();
}

void LEDIndicator::solidColor(rgbColor color) {
    blink_ = false;
    current_color_ = color;
    enabled_ = true;
    update();
}

void LEDIndicator::blinkColor(rgbColor color, blink_speed speed) {
    blink_ = true;
    current_color_ = color;
    current_speed_ = speed;
    enabled_ = true;
    update();
}

void LEDIndicator::turnOFF(){
    blink_ = false;
    enabled_ = false;
    update();
}
