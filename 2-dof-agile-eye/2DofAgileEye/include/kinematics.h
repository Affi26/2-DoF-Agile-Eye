
#ifndef kinematics
#define kinematics

#include <math.h>

/**
 * @brief Calculate Y-axis angle (deg) from Y- and Z-coordinates.
 */
float calcYAngle(float yCoord, float zCoord) {
    return 180.0f * atan2f(-yCoord, zCoord) / PI;
}

/**
 * @brief Calculate X-axis angle (deg) from X-, Y- and Z-coordinates.
 */
float calcXAngle(float xCoord, float yCoord, float zCoord) {
    return 180.0f * atan2f(-xCoord * zCoord, pow(yCoord, 2) + pow(zCoord, 2)) / PI;
}

/**
 * @brief Calculate revolutions from angle (deg) input.
 */
float angle2rev(float angle) {
    return (angle / 360.0f);
}

/**
 * @brief Calculate Y-axis revolutions from Y- and Z-coordinates.
 */
float calcYRev(float yCoord, float zCoord) {
    return angle2rev(calcYAngle(yCoord, zCoord));
}

/**
 * @brief Calculate X-axis revolutions from X-, Y- and Z-coordinates.
 */
float calcXRev(float xCoord, float yCoord, float zCoord) {
    return angle2rev(calcXAngle(xCoord, yCoord, zCoord));
}

#endif //kinematics