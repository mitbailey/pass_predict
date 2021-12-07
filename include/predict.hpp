/**
 * @file predict.hpp
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version See Git tags for version information.
 * @date 2021.11.23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef PREDICT_HPP
#define PREDICT_HPP

#define SEC *1000000.0           // nanoseconds to seconds
#define DEG *(180.0 / 3.1415926) // radians to degrees
#define GS_LAT 42.655583
#define GS_LON -71.325433
#define GS_ELEV 0.061                                             // Lowell ASL + Olney Height; Kilometers for some reason.
#define MIN_ELEV 10.0                                          // degrees
#define TLE_LEN 70

#endif // PREDICT_HPP