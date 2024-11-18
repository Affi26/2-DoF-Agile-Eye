
#ifndef enums_h
#define enums_h

/**
 * @brief State types for the state machine structure
 */
enum state_type {
  STARTUP             = 0,
  HOMING              = 1,
  READING_COMMANDS    = 2,
  PLANNING_SETPOINTS  = 3,
  START_POSITION      = 4,
  RUNNING             = 5,
  FINISHING           = 6,
};

enum Mode { 
    coordinates = 0, 
    angles = 1, 
    error = 2,
}; 

struct CoordData {
    float x;
    float y;
};
 
struct AngleData { 
    float x_angle; 
    float y_angle; 
}; 

#endif  // enums_h