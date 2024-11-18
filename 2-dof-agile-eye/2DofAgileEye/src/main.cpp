#include "board.h"
#include "kinematics.h"
#include "enums.h"
#include "serialComm.h"
#include <stdlib.h>


// Uncomment wanted test
// #define test1
// #define test2

#ifdef test1
#define local_testing
float coord_array[4][2] ={{10.0f, 10.0f}, {-10.0f, 10.0f}, {-10.0f, -10.0f}, {10.0f, -10.0f}};
float angle_array[4][2];
float z_coordinate = 20.0f;
int interval = 50;
#endif

#ifdef test2
#define local_testing
float start_point = 0.00;
float increment = 0.05;
int steps = 2;
int interval = 50;
#endif

// Choose homing mode
// #define timed_homing
#define button_homing

#define Odrive_connected 

//
// Variables
//

// Initialize serialReader object
readSerial serialReader(Serial, SEPARATOR, BUFFER_SIZE, TIMEOUT);

//
// Arrays for coordinates and angles
//
CoordData coordinate_buf[MAX_SETPOINTS];
AngleData angle_buf[MAX_SETPOINTS];
AngleData* angle_setpoints = (AngleData*)malloc(sizeof(AngleData) * DEFAULT_MAX_ANGLES);
CoordData* coord_setpoints = (CoordData*)malloc(sizeof(CoordData) * 10);
Mode mode = error;

int current_capacity = 10;
int incoming_setpoint_amount = 0;
int coord_setpoints_size = 0;
int loop_size = 0;
int z = 0;
int interval = 0;
float max_spacing = 2;  // In angles


// State variable for current state.
state_type current_state;

bool homed = false;
bool tools_removed = false;
uint32_t last_debounce_time = 0;
uint32_t debounce_delay = 1000;

uint32_t test_positions_delay = 300;


bool readButton() {
  if ((millis() - last_debounce_time) >= debounce_delay) {
    if (digitalRead(HOMING_BUTTON)) {
      return false;
    }
    else {
      last_debounce_time = millis();
      return true;
    }
  }
  else return false;
}

void setup() {

  delay(1000);

  current_state = state_type::STARTUP;
  
  Serial.begin(USB_BAUDRATE);
  
  pinMode(HOMING_BUTTON, INPUT_PULLUP);

  status_light.init();
  status_light.solidColor(redColor);

  #ifdef Odrive_connected
  odrive_serial_1.begin(BAUDRATE);
  odrive_serial_2.begin(BAUDRATE);
  delay(1000);
  odrive1.init();
  odrive2.init();
  #endif

  delay(1000);

}

void loop() {

  //uint32_t updateTimeout = 0;
  //uint32_t last = millis();
  status_light.update();

  switch (current_state) {
    case state_type::STARTUP: {
      
      // DO SOMETHING
      Serial.println("Starting up...");
      status_light.blinkColor(redColor, blink_speed::AVERAGE);
      homed = false;
      tools_removed = false;
      last_debounce_time = millis();

      odrive1.disable();
      odrive2.disable();

      // Five seconds timer
      uint32_t timer = millis();
      while (millis() - timer <= 5000) {
        status_light.update();
      }

      current_state = state_type::HOMING;

    }
    case state_type::HOMING: {

      // DO HOMING
      Serial.println("Starting homing sequence...");

      #ifdef timed_homing

      Serial.println("####################");
      Serial.println("Motor 1:");
      Serial.println("####################");
      Serial.println("Starting setHome()");
      Serial.print("Position before: ");
      Serial.println(odrive1.getPosition());

      if (odrive1.setHome()) {
        Serial.println("setHome() successful");
      }
      else {
        Serial.println("setHome() failed");
      }

      Serial.print("Position after: ");
      Serial.println(odrive1.getPosition());
      
      Serial.println("####################");
      Serial.println("Motor 2:");
      Serial.println("####################");
      Serial.println("Starting setHome()");
      Serial.print("Position before: ");
      Serial.println(odrive2.getPosition());

      if (odrive2.setHome()) {
        Serial.println("setHome() successful");
      }
      else {
        Serial.println("setHome() failed");
      }

      Serial.print("Position after: ");
      Serial.println(odrive2.getPosition());

      Serial.println("REMOVE TOOLS WITHING 10 SECONDS!!!");
      delay(10000);

      #endif


      #ifdef button_homing
      Serial.println("Press button to set home position.");
      status_light.blinkColor(yellowColor, blink_speed::AVERAGE);
      while (!homed) {
        status_light.update();
        if (readButton()) {

          #ifndef Odrive_connected
          homed = true;
          break;
          #endif

          if (odrive1.setHome() && odrive2.setHome()) {
            homed = true;
          }
        }
      }

      Serial.println("Press button to confirm tools are removed.");
      status_light.blinkColor(blueColor, blink_speed::FAST);
      while (!tools_removed) {
        status_light.update();
        if (readButton()) {
          tools_removed = true;
        }
      }

      if (tools_removed && homed) {
        // Move to next state once homing successful.
        Serial.println("Homing successful!");
        status_light.solidColor(greenColor);
        current_state = state_type::READING_COMMANDS;
      }

      #endif

    }
    case state_type::READING_COMMANDS: {

      Serial.println("Starting reading commands sequence...");

      #ifdef test1

      Serial.println("coord_array: ");
      for (int i = 0; i < 4; i++) {
        Serial.print(coord_array[i][0]);
        Serial.print(", ");
        Serial.println(coord_array[i][1]);
      }

      for (int i = 0; i < 4; i++) {
        angle_array[i][0] = calcXRev(coord_array[i][0], coord_array[i][1], z_coordinate);
        angle_array[i][1] = calcYRev(coord_array[i][1], z_coordinate);
      }

      Serial.println("angle_array: ");
      for (int i = 0; i < 4; i++) {
        Serial.print(angle_array[i][0], 4);
        Serial.print(", ");
        Serial.println(angle_array[i][1], 4);
      }

      delay(5000);

      current_state = state_type::START_POSITION;

      #endif


      #ifndef local_testing
      
      while (true){
        mode = serialReader.readMode();
        delay(1000);
      

        if (mode == error){
          Serial.print("\nError with start. Emptying rx buffer");
          serialReader.flushRxBuf();
          continue;
        }
        

        Serial.print("Mode: ");
        Serial.print(mode);

        if (!serialReader.readCoordinatesSize(&incoming_setpoint_amount)){
          Serial.print("\nError reading coordinate amount. Check format");
          continue;
        }
        Serial.print(" Number of setpoints: ");
        Serial.print(incoming_setpoint_amount);
        
        if (!serialReader.readLoopSize(&loop_size)){
          Serial.print("\nError reading loop size. Check format");
          continue;
        }
        Serial.print(" Loop size: ");
        Serial.print(loop_size);

        if (!serialReader.readInterval(&interval)){
          Serial.print("\nError reading interval. Check format");
          continue;
        }
        Serial.print(" Interval: ");
        Serial.print(interval);

        
        if (mode == coordinates){

          if (!serialReader.readZ(&z)){
            Serial.print("\nError reading z value. Check format");
            continue;

          }
          Serial.print(" Z: ");
          Serial.println(z);

          if (!serialReader.readSpacing(&max_spacing)){
            Serial.print("\nError reading spacing. Check format");
            continue;
          }
          Serial.print(" Spacing: ");
          Serial.print(max_spacing);

          if (!serialReader.readCoordinateData(coordinate_buf, incoming_setpoint_amount)) {
            Serial.print("\nError reading coordinate data. Check format (the only special characters allowed are: '.' and '-')");
            continue;
            
          }
          for (int i=0;i<incoming_setpoint_amount;i++){
            angle_buf[i].x_angle = calcXAngle(coordinate_buf[i].x, coordinate_buf[i].y, z);
            angle_buf[i].y_angle = calcYAngle(coordinate_buf[i].y, z);
          }
          for (int i = 0; i < incoming_setpoint_amount; i++) {
            Serial.print("\nAngles: x: ");
            Serial.print(angle_buf[i].x_angle);
            Serial.print(", y: ");
            Serial.print(angle_buf[i].y_angle);
          }
          Serial.println();
          serialReader.flushRxBuf();
          
          current_state = state_type::PLANNING_SETPOINTS;
          break;
          
        }
        else {
          if (incoming_setpoint_amount > DEFAULT_MAX_ANGLES){
            AngleData* temp = (AngleData*)realloc(angle_setpoints, incoming_setpoint_amount * sizeof(AngleData));
            if (temp== NULL) {
                Serial.print("Failed to reallocate memory for setpoints");
                free(angle_setpoints); // free the original if realloc fails
                continue;
              
            }
            angle_setpoints = temp;
          }

          if (!serialReader.readAngleData(angle_setpoints, incoming_setpoint_amount)) {
            Serial.print("\nError reading angle data. Check format (the only special characters allowed are: '.' and '-'), do not include z");
            continue;
          }
          
          Serial.print("\nAngles: x: \n");
          for (int i = 0; i < incoming_setpoint_amount; i++) {
            Serial.print("x: ");
            Serial.print(angle_setpoints[i].x_angle);
            Serial.print(", y: ");
            Serial.print(angle_setpoints[i].y_angle);
            
            angle_setpoints[i].x_angle = angle2rev(angle_setpoints[i].x_angle);
            angle_setpoints[i].y_angle = angle2rev(angle_setpoints[i].y_angle);
        
            Serial.print(" --> Converted to revs --> x: ");
            Serial.print(angle_setpoints[i].x_angle);
            Serial.print(", y: \n");
            Serial.print(angle_setpoints[i].y_angle);

          }
          Serial.println();
          serialReader.flushRxBuf();
          //angle to rev coonversion
          // to setpoints
          current_state = state_type::START_POSITION;
          
          break;
          
        }
        
      }
      
      #endif
  
    }
    case state_type::PLANNING_SETPOINTS: {
      
      // Calculate how many setpoints in total, reserve memory
        // Take two; i,j, init 0, 1
        // Minus the x values, minus the y values (j-i)
        // Divide by the setpoint interval (say, 5 degrees)
        //
        

      // This is here to prevent a VERY strange bug where the program starts executing this case 
      // even though it's not even the current state. Arduino is weird
      Serial.println(current_state);
      if (current_state != state_type::PLANNING_SETPOINTS){
        break;
      }

      Serial.print("Planning setpoints:\n");
      float x_interval_angle = 0;
      float y_interval_angle = 0;

      float x_interval_coord = 0;
      float y_interval_coord = 0;
      int setpoint_count=0;
      bool memory_error = 0;
      

      // 20,20 to 40,40
      // 40-20 = 20
      // 5 deg interval: 20/5 = 4 setpoints in between 
      // + 5 + 5 + 5 +5 : 25, 30, 35, 40 --> in reality only 3
      // 15.6 / 5 = 3.2  -> rounded 4 = steppien määrä
      // 0 3.9 7.8 11.7 15.6
      int new_setpoint_idx = 0;
      for (int i= 0, j= 1; i < incoming_setpoint_amount - 1; i++,j++){ // last iteration j will be incoming_setpoint_amount - 1
        
        x_interval_angle = angle_buf[j].x_angle - angle_buf[i].x_angle;
        y_interval_angle = angle_buf[j].y_angle - angle_buf[i].y_angle;

        x_interval_coord = coordinate_buf[j].x - coordinate_buf[i].x;
        y_interval_coord = coordinate_buf[j].y - coordinate_buf[i].y;
       

        float longer_vec_length = max(abs(x_interval_angle), abs(y_interval_angle));

        setpoint_count = ceil(longer_vec_length / max_spacing);

        float spacing_x = x_interval_coord / setpoint_count;
        float spacing_y = y_interval_coord / setpoint_count;

        int required_size = (new_setpoint_idx + setpoint_count); // in theory, + 2 only in the first allocation, but no harm in extra

        if (required_size > current_capacity) {
          CoordData* temp = (CoordData*)realloc(coord_setpoints, 2 * required_size * sizeof(CoordData));
          if (temp == nullptr) {
              Serial.print("Memory error while allocating for setpoints");
              memory_error = 1;
              current_state = READING_COMMANDS;
              break;
          } else {
              coord_setpoints = temp;
              current_capacity = 2 * required_size;
          }
        }

        coord_setpoints[new_setpoint_idx].x = coordinate_buf[i].x;
        coord_setpoints[new_setpoint_idx].y = coordinate_buf[i].y;

        if (longer_vec_length <= max_spacing){
          new_setpoint_idx++;
          continue;
        }
        
        for (int a = 1; a < setpoint_count; a++){
          coord_setpoints[new_setpoint_idx+a].x = coord_setpoints[new_setpoint_idx+a-1].x + spacing_x;
          coord_setpoints[new_setpoint_idx+a].y = coord_setpoints[new_setpoint_idx+a-1].y + spacing_y;
           
        }
        new_setpoint_idx+= setpoint_count;
       }
      
      if (!memory_error) {
        coord_setpoints[new_setpoint_idx].x = coordinate_buf[incoming_setpoint_amount-1].x;
        coord_setpoints[new_setpoint_idx].y = coordinate_buf[incoming_setpoint_amount-1].y;
        new_setpoint_idx++;
      }
      
      coord_setpoints_size = new_setpoint_idx;
    
      Serial.print(new_setpoint_idx);
      Serial.print(" SETPOINTS:\n ");
      for (int i = 0; i < new_setpoint_idx; i++){
        
        Serial.print("\n Coordinates: x: ");
        Serial.print(coord_setpoints[i].x);
        Serial.print(", y: ");
        Serial.print(coord_setpoints[i].y);

        // Convert coordinates to revs and print. Comment out if needed.
        coord_setpoints[i].x = calcXRev(coord_setpoints[i].x, coord_setpoints[i].y, z);
        coord_setpoints[i].y = calcYRev(coord_setpoints[i].y, z);

        Serial.print(" --> revolutions: x: ");
        Serial.print(coord_setpoints[i].x, 4);
        Serial.print(", y: ");
        Serial.print(coord_setpoints[i].y, 4);
      }

      Serial.println();
      current_state = START_POSITION;
      }
    case state_type::START_POSITION: {

      Serial.println("Move to start position...");

      #ifdef test1

      odrive1.enable();
      odrive2.enable();

      // Test all setpoints
      for (int i = 0; i < 4; i++) {
        odrive1.setPositionLimited(angle_array[i][0]);
        odrive2.setPositionLimited(angle_array[i][1]);
        delay(1000);
      }

      // Go to starting position (first setpoint)
      odrive1.setPositionLimited(angle_array[0][0]);
      odrive2.setPositionLimited(angle_array[0][1]);

      delay(1000);

      current_state = state_type::RUNNING;

      #endif


      #ifndef local_testing
    
      odrive1.enable();
      odrive2.enable();
      // TODO : test all setpoints for angle mode

      if (mode == Mode::coordinates){
        // Test all setpoints
        for (int i = 0; i < coord_setpoints_size - 1; i++) {
            odrive1.setPositionLimited(coord_setpoints[i].x);
            odrive2.setPositionLimited(coord_setpoints[i].y);
            delay(test_positions_delay);
        }

        // Go to starting position (first setpoint)
        odrive1.setPositionLimited(coord_setpoints[0].x);
        odrive2.setPositionLimited(coord_setpoints[0].y);
      }

      else {
        // Test all setpoints
        for (int i = 0; i < incoming_setpoint_amount - 1; i++) {
            odrive1.setPositionLimited(angle_setpoints[i].x_angle);
            odrive2.setPositionLimited(angle_setpoints[i].y_angle);
            delay(test_positions_delay);
        }

        // Go to starting position (first setpoint)
        odrive1.setPositionLimited(angle_setpoints[0].x_angle);
        odrive2.setPositionLimited(angle_setpoints[0].y_angle);


      }
      delay(1000);

      current_state = state_type::RUNNING;
      #endif

    }
    case state_type::RUNNING: {

      Serial.println("Start running setpoints...");

      #ifdef test1

      // Loop 100 times
      for (int i = 0; i < 100; i++) {

        // Loop all setpoints
        for (int j = 0; j < 4; j++) {
          odrive1.setPositionLimited(angle_array[j][0]);
          odrive2.setPositionLimited(angle_array[j][1]);
          delay(interval);
        }
      }

      delay(1000);

      odrive1.setPositionLimited(0.0f);
      odrive2.setPositionLimited(0.0f);

      delay(10000);

      odrive1.disable();
      odrive2.disable();

      current_state = state_type::FINISHING;

      #endif

      
      #ifndef local_testing
      
      // Loop loop_size times
      for (int i = 0; i < loop_size; i++) {

        // Loop all setpoints
        if (mode == Mode::coordinates){
          // Here we use coord_setpoints_size because the size changes based on the amount of intermediate setpoints
          for (int i = 0; i < coord_setpoints_size - 1; i++) {
            odrive1.setPositionLimited(coord_setpoints[i].x);
            odrive2.setPositionLimited(coord_setpoints[i].y);
            delay(interval);
            //Serial.println(millis());
          }
        }
        else {
          for (int i = 0; i < incoming_setpoint_amount; i++) {
            odrive1.setPositionLimited(angle_setpoints[i].x_angle);
            odrive2.setPositionLimited(angle_setpoints[i].y_angle);
            delay(interval);
            /*
            ODriveFeedback feedback = odrive1.getFeedback();
            Serial.print("set:");
            Serial.print(angle_setpoints[i].x_angle, 5);
            Serial.print(", ");
            Serial.print("pos:");
            Serial.print(feedback.pos, 5);
            Serial.println(millis());
            */
          }
        }
        

      }

      // Return back to home
      delay(1000);
      odrive1.setPositionLimited(0.0f);
      odrive2.setPositionLimited(0.0f);
      delay(5000);

      // Disable motors
      odrive1.disable();
      odrive2.disable();

      current_state = state_type::FINISHING;
      #endif

    }
    case state_type::FINISHING: {

      Serial.println("Program finished. Resetting...");

      // Just in case disable
      #ifdef Odrive_connected
      odrive1.disable();
      odrive2.disable();

      Serial.println("####################");
      Serial.println("Motor 1:");
      Serial.println("####################");
      Serial.println("Program finished");
      Serial.print("DC voltage: ");
      Serial.println(odrive1.getVbus());
      Serial.print("Motor temperature: ");
      Serial.println(odrive1.getMotorTemp());
      Serial.print("FET temperature: ");
      Serial.println(odrive1.getFetTemp());
      
      Serial.println("####################");
      Serial.println("Motor 2:");
      Serial.println("####################");
      Serial.println("Program finished");
      Serial.print("DC voltage: ");
      Serial.println(odrive2.getVbus());
      Serial.print("Motor temperature: ");
      Serial.println(odrive2.getMotorTemp());
      Serial.print("FET temperature: ");
      Serial.println(odrive2.getFetTemp());
      
      delay(5000);

      #endif

      // Reset memory and variables
      CoordData* temp_c = coord_setpoints;
      coord_setpoints = (CoordData*)malloc(sizeof(CoordData) * 10);
      coord_setpoints_size = 0;
      current_capacity = 10;
      free(temp_c);

      AngleData* temp_a = angle_setpoints;
      angle_setpoints = (AngleData*)malloc(sizeof(AngleData) * DEFAULT_MAX_ANGLES);
      free(temp_a);
      
      // Return back to READING_COMMANDS state
      current_state = state_type::READING_COMMANDS;

    }
    default: {
      //Add something as default
      break;
    }

  }

}

