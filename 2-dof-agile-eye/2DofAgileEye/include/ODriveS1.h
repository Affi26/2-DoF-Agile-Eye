
#ifndef ODriveS1_h
#define ODriveS1_h

#include <ODriveUART.h>

class ODriveS1: public ODriveUART {
public:
    /**
     * @brief Constructs an ODriveS1 instance that will communicate over
     * the specified serial port.
     */
    ODriveS1(Stream &serial, uint32_t max_angle, uint8_t id) : ODriveUART(serial), max_angle_(max_angle), id_(id) {
        name_ = "Odrive" + String(id);
    }

    /**
     * @brief Checking ODrive connection by fetching state status successfully.
     */
    void init();

    /**
     * @brief Sets Odrive to Closed Loop Control State.
     */
    void enable();

    /**
     * @brief Sets ODrive to IDLE state.
     */
    void disable();

    /**
     * @brief Sets ODrive position estimate to 0.0.
     * @return True if position estimate successfully set to 0.0, else False
     */
    bool setHome();

    /**
     * @brief Sets ODrive to wanted position with defined angle limitations
     */
    void setPositionLimited(float position);

    /**
     * @brief Gets Vbus voltage from ODrive
     * @return Input voltage as float
     */
    float getVbus() { return getParameterAsFloat(vbus_voltage_); }

    /**
     * @brief Gets motor temperature from ODrive
     * @return Motor temperature as float
     */
    float getMotorTemp() { return getParameterAsFloat(motor_thermistor_); }

    /**
     * @brief Gets controller temperature from ODrive
     * @return Controller temperature as float
     */
    float getFetTemp() { return getParameterAsFloat(fet_thermistor_); }

private:
    /**
     * @brief Sets ODrive to use absolute setpoints.
     * Prevents ODrive initiating closed loop control until position estimate is defined.
     */
    void setAbsSetpoints();

    /**
     * @brief Sets ODrive to IDLE state.
     */
    void setIdle();

    /**
     * @brief Sets ODrive position estimate to wanted value
     */
    void setPosEstimate(float position);

    uint32_t max_angle_ = 0;                                                            // maximum angle variable
    uint8_t id_ = 0;                                                                    // id number for motor
    String name_ = "Odrive0";                                                           // name for motor
    const String absolute_setpoints_ = "axis0.controller.config.absolute_setpoints";    // address for absolute_setpoints
    const String motor_thermistor_ = "axis0.motor.motor_thermistor.temperature";        // address for motor_thermistor.temperature
    //const String motor_thermistor_ = "thermistor0";
    const String fet_thermistor_ = "axis0.motor.fet_thermistor.temperature";            // address for fet_thermistor.temperature
    //const String fet_thermistor_ = "thermistor1";
    const String pos_estimate_ =  "axis0.pos_estimate";                                 // address for position estimate
    const String vbus_voltage_ = "vbus_voltage";                                        // address for vbus_voltage
};

#endif //ODriveS1_h
