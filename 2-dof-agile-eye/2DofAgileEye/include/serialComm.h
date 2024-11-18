#ifndef serialComm_h
#define serialComm_h

#include <stdio.h>
#include "Arduino.h"
#include "enums.h"
#include <string.h>

// Separator char for the values in the message
const char SEPARATOR = ';';
// Maximum string length of individual values in the buffer (between ;'s or ,'s)
#define BUFFER_SIZE 16
// Don't send more coordinate setpoints than specified below. Otherwise the buffers used in coordinate mode can overflow
#define MAX_SETPOINTS 64
// Serial port timeout
#define TIMEOUT 100
// Angle setpoint array (dynamically allocated) default size
#define DEFAULT_MAX_ANGLES 24
 
class readSerial { 
public: 
 
    readSerial(Stream &serial, char separator, int max_parameter_length, unsigned long timeout_ms);
    ~readSerial();

    Mode readMode(); // angles or coordinates (a or c)
    bool readCoordinatesSize(int *i); // How many setpoints in the shape being drawn
    bool readLoopSize(int* i); // How many times the shape will be drawn
    bool readZ(int *i); // Distance from the surface (cm)
    bool readCoordinateData(CoordData* coord_setpoints, int length); // if mode is c, then they need to be converted to angles
    bool readAngleData(AngleData* angle_setpoints, int length);  // if mode is a, then use as is
    bool readInterval(int* i);
    bool readSpacing(float *f);
    void flushRxBuf();

    // example input string from matlab c;4;50;150;x,y;x,y;x,y;x,y
    // c;4;50;150;1.4444,2.2222;5.5555,6.6666;1.22222,6.66666;7.77777,8.88888;

private:


    Stream& serial_;
    char separator_;
    int max_parameter_length_;
    unsigned long timeout_ms_;
    char *buffer;
    char* readStringUntil(char term_char);
    bool check_sep();
    bool readIntegerFromBuffer(int* i);
    bool readFloatFromBuffer(float* f, char sep);
    int isValidFloatFormat(const char *str);
    bool waitForByte();
};

#endif


