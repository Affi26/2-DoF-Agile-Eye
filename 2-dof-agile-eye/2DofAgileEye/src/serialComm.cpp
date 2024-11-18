#include "serialComm.h"

readSerial::readSerial(Stream &serial, char separator, int max_parameter_length, unsigned long timeout_ms) : serial_(serial), separator_(separator), 
max_parameter_length_(max_parameter_length), timeout_ms_(timeout_ms)  {
     buffer = new char[max_parameter_length];
}

readSerial::~readSerial() {
    delete[] buffer; 
}

bool readSerial::waitForByte(){
    
    unsigned long startTime = millis(); // Record the start time

    while (serial_.available() == 0) {
        if (millis() - startTime >= timeout_ms_) {
            serial_.print("Timeout: No bytes in rx buffer");
            return false; // Return false if timeout is reached
        }
    }
    return true;
}

bool readSerial::check_sep(){
    //while (serial_.available() == 0)
    if (!waitForByte()) return false;
    char sep = serial_.read();
    if (sep == separator_) {
        return true;
    }

    return false;
}

Mode readSerial::readMode(){
    
    while (serial_.available() == 0);
    char byte = serial_.read();

    bool sep = check_sep();
    
    if (byte == 'c' && sep){
        return Mode::coordinates;
    }
    if (byte == 'a' && sep){
        return Mode::angles;
    }

    return Mode::error;
}

char* readSerial::readStringUntil(char term_char){

    //static char buffer[max_parameter_length_]; 
    int head = 0;
    bool terminator_found = false;

    // Loop can break if no more data available, index is at end of buffer or if the right char is found
    while (waitForByte()) {
        char byte = serial_.read(); // Read a byte from the serial buffer

        if (byte == term_char) { 
            buffer[head] = '\0'; // Null terminate
            terminator_found = true;
            break;
        } 
        if (head >= max_parameter_length_ - 1){  // If end of buffer is reached
            break;
        }
        else {
            buffer[head++] = byte; // Add the char to string, increment head
        }
    }

    if (!terminator_found){ // Make it an "empty string", to be ignored
        buffer[0] = '\0';
    }
    return &buffer[0]; // Address of first element in string (a bit redundant, as it's always the address
                       // of the same "buffer" array, this function could've been made as a success/fail return value bool too)

}


bool readSerial::readIntegerFromBuffer(int* i){
     
    char* int_str = readStringUntil(separator_);
    if (strlen(int_str) > 0) {

        for (int i = 0; int_str[i] != '\0'; ++i) {
            // Minus sign as first char will be tolerated
            if (!isdigit(int_str[i]) && !(i == 0 && int_str[i] == '-')) {
                return false;
            } 
        }

        int val = atoi(int_str);
        *i = val;
        return true;
    }
    else return false;
}

// Allows for digits, 
int readSerial::isValidFloatFormat(const char *str) {
    int dotCount = 0; // Counter for dots
    int len = strlen(str);

    // Check if the string is empty, the only character is '.', or ends with '.'
    if (len == 0 || (len == 1 && str[0] == '.')) {
        return 0; // False
    }

    // Check if the first character is a minus sign
    int startIdx = 0;
    if (str[0] == '-') {
        
        if (len == 1) {
            return 0; // Just a minus sign will not be tolerated
        }
        startIdx = 1; // start checking from the second character now
        
    }

    for (int i = startIdx; i < len; i++) {
        if (str[i] == '.') {
            dotCount++;
            if (dotCount > 1) {
                return 0; // False if more than one dot
            }
        } else if (!isdigit(str[i])) {
            return 0; // False if non-digit characters are found
        }
    }

    return 1; // True if the string is a valid float format
}

bool readSerial::readFloatFromBuffer(float* f, char sep){
    //if (!check_sep()) return false; // No separator_ in buffer --> message has the wrong format
    
    char* float_str = readStringUntil(sep);
    
    if (strlen(float_str) > 0) {
        if (!isValidFloatFormat(float_str)) return false;
        float val = atof(float_str);
        *f = val;
        return true;
    }
    else {
        return false;
    } 
}

void readSerial::flushRxBuf() {
    while (serial_.available() > 0) {
        serial_.read();  // Read and discard the incoming byte
    }
}


bool readSerial::readCoordinatesSize(int *i){
    return (readIntegerFromBuffer(i) && *i >= 0);
}


bool readSerial::readLoopSize(int *i){
    return (readIntegerFromBuffer(i) && *i >= 0);

}

bool readSerial::readZ(int *i){
    return (readIntegerFromBuffer(i) && *i >= 0);
}

bool readSerial::readInterval(int *i){
    return (readIntegerFromBuffer(i) && *i >= 0);
}

bool readSerial::readSpacing(float *f){
    return (readFloatFromBuffer(f, separator_) && *f >= 0.5);
}

// Arguments: an array of CoordData structs, amount of setpoints (int)
bool readSerial::readCoordinateData(CoordData* coord_setpoints, int length){

    if (!(0 < length && length < MAX_SETPOINTS)) {
        return false;
    }
    float x,y;
    CoordData c_data;

    for (int i = 0; i<length; i++){
        if (!readFloatFromBuffer(&x, ',') || !readFloatFromBuffer(&y, separator_)) return false;
        c_data.x= x;
        c_data.y= y;
        coord_setpoints[i] = c_data;
    }

    return true;
}

// Arguments: an array of AngleData structs, amount of setpoints (int)
bool readSerial::readAngleData(AngleData* angle_setpoints, int length){
    if (!(0 < length && length < MAX_SETPOINTS)) {
        return false;
    }
    float x, y;
    AngleData a_data;

    for (int i = 0; i<length; i++){
        if (i >= MAX_SETPOINTS -1) {
            serial_.print("\nBuffer overflow, aborting angle setpoint reading");
            return false;    
        }
        if (!readFloatFromBuffer(&x, ',') || !readFloatFromBuffer(&y, separator_)) return false;
        a_data.x_angle= x;
        a_data.y_angle= y;
        angle_setpoints[i] = a_data;
    }

    return true;
}