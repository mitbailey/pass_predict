/**
 * @file predict.h
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version See Git tags for version information.
 * @date 2021.11.23
 * 
 * @copyright Copyright (c) 2021
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#ifndef PREDICT_H
#define PREDICT_H

#define SEC *1000000.0           // nanoseconds to seconds
#define DEG *(180.0 / 3.1415926) // radians to degrees
#define GS_LAT 42.655583
#define GS_LON -71.325433
#define GS_ELEV 0.061                                             // Lowell ASL + Olney Height; Kilometers for some reason.
#define MIN_ELEV 10.0                                          // degrees
#define TLE_LEN 70

#endif // PREDICT_H