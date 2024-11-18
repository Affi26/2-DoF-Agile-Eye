#include "rgbColor.h"

rgbColor::rgbColor(float r, float g, float b) {
  // Limit red component between 0 and 255
  if (r <= 0.0) {
    redComponent = 0;
  }
  else if (r >= 1.0) {
    redComponent = 255;
  }
  else {
    redComponent = (uint8_t)(r*255.0);
  }

  // Limit green component between 0 and 255
  if (g <= 0.0) {
    greenComponent = 0;
  }
  else if (g >= 1.0) {
    greenComponent = 255;
  }
  else {
    greenComponent = (uint8_t)(g*255.0);
  }

  // Limit blue component between 0 and 255
  if (b <= 0.0) {
    blueComponent = 0;
  }
  else if (b >= 1.0) {
    blueComponent = 255;
  }
  else {
    blueComponent = (uint8_t)(b*255.0);
  }    
}

const float rgbColor::red() const {
  return redComponent / 255.0;
}

const float rgbColor::green() const {
  return greenComponent / 255.0;
}

const float rgbColor::blue() const {
  return blueComponent / 255.0;
}
