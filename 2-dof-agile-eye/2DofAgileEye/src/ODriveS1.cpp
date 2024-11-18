#include "ODriveS1.h"

void ODriveS1::init() {

    Serial.println("Waiting for " + name_ + "...");
    while (getState() == AXIS_STATE_UNDEFINED) {
        delay(100);
    }
    Serial.print("DC voltage: ");
    Serial.println(getVbus());
    Serial.println(name_ + " initialized");

}

void ODriveS1::enable() {
    Serial.println("Enabling closed loop control...");
    while (getState() != AXIS_STATE_CLOSED_LOOP_CONTROL) {
        clearErrors();
        setState(AXIS_STATE_CLOSED_LOOP_CONTROL);
        delay(10);
    }
    Serial.println(name_ + " running!");
}

void ODriveS1::disable() {
    setIdle();
}

bool ODriveS1::setHome() {

    Serial.println("Setting home position of " + name_ + "...");
    // Release motor
    setIdle();

    // Read current position and set as home position
    setAbsSetpoints();
    //float home_pos = getPosition();
    setPosEstimate(0.0);


    if (abs(getPosition()) < 0.0001) {
        return true;
    }
    else {
        setAbsSetpoints();  // Prevent position control commands until successful setHome()
        return false;
    }

}

void ODriveS1::setPositionLimited(float position) {
    float max_revolution = max_angle_ / 360.0f;
    float limited_output = max(-max_revolution, position);
    limited_output = min(max_revolution, limited_output);
    setPosition(limited_output);
}

void ODriveS1::setAbsSetpoints() {
    setParameter(absolute_setpoints_, String(true));
}

void ODriveS1::setIdle() {
    Serial.println("Setting " + name_ + " to IDLE...");
    while (getState() != AXIS_STATE_IDLE) {
        clearErrors();
        setState(AXIS_STATE_IDLE);
        delay(10);
    }
    Serial.println(name_ + " in IDLE");
}

void ODriveS1::setPosEstimate(float position) {
    setParameter(pos_estimate_, String(position));
}
