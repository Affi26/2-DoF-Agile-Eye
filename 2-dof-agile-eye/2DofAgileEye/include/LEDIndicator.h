
#ifndef LedIndicator_h
#define LedIndicator_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "rgbColor.h"


enum blink_speed {
    SLOW        = 2000,
    AVERAGE     = 1000,
    FAST        = 500,
};

class LEDIndicator {
public:

    /**
     * @brief Constructs an LEDIndictor object with specified RGB pins
    */
    LEDIndicator(Adafruit_NeoPixel& led, uint32_t led_count) : led_(led), led_count_(led_count) {
        led_.begin();
    }

    /**
     * @brief Initialize LEDIndicator object by clearing LEDs
    */
    void init();

    /**
     * @brief Update LEDIndicator object
    */
    void update();

    /**
     * @brief Change static color
    */
    void solidColor(rgbColor color);

    /**
     * @brief Change blink color
    */
    void blinkColor(rgbColor color, blink_speed speed);

    /**
     * @brief Turn off LED
    */
    void turnOFF();

private:
    Adafruit_NeoPixel& led_;
    uint32_t led_count_;

    bool enabled_ = false;
    bool blink_ = false;
    bool last_state_ = false;
    rgbColor current_color_ = whiteColor;
    blink_speed current_speed_ = blink_speed::AVERAGE;
    uint32_t last_blink_ = 0;
};

#endif //LedIndicator_h
