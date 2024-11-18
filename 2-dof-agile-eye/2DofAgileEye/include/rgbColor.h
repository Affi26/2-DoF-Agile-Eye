
#ifndef rgbColor_h
#define rgbColor_h

#include <Arduino.h>

class rgbColor {
public:
    rgbColor(float r, float g, float b);
    // Object accessors (set/get)
    const float red() const;
    const float green() const;
    const float blue() const;

private:
    // Color components
    uint8_t redComponent;
    uint8_t greenComponent;
    uint8_t blueComponent;
};

// Global constant colors 
static const rgbColor redColor(1.0,0.0,0.0);
static const rgbColor greenColor(0.0,1.0,0.0);
static const rgbColor blueColor(0.0,0.0,1.0);
static const rgbColor yellowColor(1.0,0.6,0.0);
static const rgbColor cyanColor(0.0,1.0,1.0);
static const rgbColor magentaColor(1.0,0.0,0.5);
static const rgbColor orangeColor(1.0,0.1,0.0);
static const rgbColor purpleColor(0.2,0.0,0.2);
static const rgbColor whiteColor(1.0,1.0,1.0);
static const rgbColor blackColor(0.0,0.0,0.0);

#endif //rgbColor_h
