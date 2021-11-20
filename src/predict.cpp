/**
 * @file predict.cpp
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version See Git tags for version information.
 * @date 2021.11.19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "CoordTopocentric.h"
#include "DateTime.h"
#include "Observer.h"
#include "SGP4.h"
#include "meb_debug.h"
// #include "track.hpp"

#define SERVER_PORT 52040

#define SEC *1000000.0           // nanoseconds to seconds
#define DEG *(180.0 / 3.1415926) // radians to degrees
#define GS_LAT 42.655583
#define GS_LON -71.325433
#define ELEV 0.061                                             // Lowell ASL + Olney Height; Kilometers for some reason.
#define MIN_ELEV 10.0                                          // degrees
#define ELEV_ADJ 0                                             // degrees adjustment +-
#define AZIM_ADJ -34                                           // degrees adjustment +-
#define UTC_TO_EST(hour) (hour > 4 ? hour - 5 : 24 - 5 + hour) // - 5 hours
#define UTC_TO_EDT(hour) (hour > 3 ? hour - 4 : 24 - 4 + hour) // - 4 hours

// TLE Object SX [https://www.n2yo.com/satellite/?s=49278]
const char TLE[2][70] = {"1 49278U 98067SX  21323.34441057  .00027466  00000-0  44695-3 0  9995",
                         "2 49278  51.6405 294.9097 0002575 252.7789 107.2919 15.52448182  5961"};

int main(int argc, char *argv[])
{
    SGP4 *satellite = new SGP4(Tle(TLE[0], TLE[1]));
    Observer *dish = new Observer(GS_LAT, GS_LON, ELEV);
    DateTime tnow = DateTime::Now(true);

    Eci pos_now = satellite->FindPosition(tnow);
    CoordTopocentric current_pos = dish->GetLookAngle(pos_now);
    CoordGeodetic current_lla = pos_now.ToGeodetic();

    printf("\n");
    dbprintlf("PASS PREDICTIONS FOR:");
    dbprintlf("%s", TLE[0]);
    dbprintlf("%s", TLE[1]);

    char filename[256] = {0};
    FILE *fp = NULL;

    if (argc > 3 || argc < 2)
    {
        dbprintlf(FATAL "Invalid command line syntax.");
        dbprintlf(FATAL "Invocation:");
        dbprintlf(FATAL "./predict.out <days to predict> output.txt");
    }
    else if (argc == 3)
    {
        strcpy(filename, argv[2]);
        dbprintlf(YELLOW_FG "PRINTING DATA TO %s", filename);
        fp = fopen(filename, "w");
        if (fp == NULL)
        {
            dbprintlf(FATAL "Failed to open %s for write.", filename);
            return -1;
        }
    }
    else
    {
        dbprintlf(YELLOW_FG "NOT PRINTING DATA");
    }

    int gen_days = atoi(argv[1]);

    dbprintlf(YELLOW_FG "DATA FOR %d DAYS", gen_days);

    dbprintlf("Current Position: %.2f AZ, %.2f EL | %.2f LA, %.2f LN", current_pos.azimuth DEG, current_pos.elevation DEG, current_lla.latitude DEG, current_lla.longitude DEG);

    DateTime tnext = tnow;
    bool in_pass = false;
    double max_el = 0.0;
    double max_el_az = 0.0;
    DateTime max_el_time;
    int begin_et = 0;

    if (argc == 3)
    {
        fprintf(fp, "Report Generated %d.%d.%d %02d:%02d:%02d EST\nData for %d days.\n\n", tnext.Year(), tnext.Month(), tnext.Day(), UTC_TO_EST(max_el_time.Hour()), tnext.Minute(), tnext.Second(), gen_days);

        fprintf(fp, "%s\n%s\n\n\n", TLE[0], TLE[1]);
    }

    for (int i = 0; i < 86400 * gen_days; i++)
    {
        tnext = tnext.AddSeconds(1);
        Eci eci_ahd = satellite->FindPosition(tnext);
        CoordTopocentric pos_ahd = dish->GetLookAngle(eci_ahd);
        double ahd_el = pos_ahd.elevation DEG;
        if (ahd_el > 10.0)
        {
            if (!in_pass)
            {
                dbprintlf("== PASS START (Now + %d minutes) ==", i / 60);
                dbprintlf("Time (EST)             Az (deg)   El (deg)");
                if (argc == 3)
                {
                    fprintf(fp, "== PASS START (Now + %d minutes) ==\n", i / 60);
                    fprintf(fp, "Time (EST)             Az (deg)   El (deg)\n");
                }
                in_pass = true;
                begin_et = i;
            }

            if (ahd_el > max_el)
            {
                max_el = ahd_el;
                max_el_az = pos_ahd.azimuth DEG;
                max_el_time = tnext;
            }

            // Print data for every 60 seconds after the pass begins, and once immediately when the pass begins..
            if ((i - begin_et) % 60 == 0)
            {
                // dbprintlf("60th Sec");
                dbprintlf("%d.%d.%d %02d:%02d:%02d    %003.02lf     %003.02lf", tnext.Year(), tnext.Month(), tnext.Day(), UTC_TO_EST(max_el_time.Hour()), tnext.Minute(), tnext.Second(), pos_ahd.azimuth DEG, ahd_el);
                if (argc == 3)
                {
                    fprintf(fp, "%d.%d.%d %02d:%02d:%02d    %003.02lf     %003.02lf\n", tnext.Year(), tnext.Month(), tnext.Day(), UTC_TO_EST(max_el_time.Hour()), tnext.Minute(), tnext.Second(), pos_ahd.azimuth DEG, ahd_el);
                }
            }
        }
        else if (in_pass)
        {
            // Make sure to print final state when pass ends.
            dbprintlf("%d.%d.%d %02d:%02d:%02d    %003.02lf     %003.02lf", tnext.Year(), tnext.Month(), tnext.Day(), UTC_TO_EST(max_el_time.Hour()), tnext.Minute(), tnext.Second(), pos_ahd.azimuth DEG, ahd_el);
            if (argc == 3)
            {
                fprintf(fp, "%d.%d.%d %02d:%02d:%02d    %003.02lf     %003.02lf\n", tnext.Year(), tnext.Month(), tnext.Day(), UTC_TO_EST(max_el_time.Hour()), tnext.Minute(), tnext.Second(), pos_ahd.azimuth DEG, ahd_el);
            }

            dbprintlf("== PASS END ==\n");
            dbprintlf("Section Statistics");
            dbprintlf("    Maximum Elevation:");
            dbprintlf("%d.%d.%d %02d:%02d:%02d    %003.02lf     %003.02lf", max_el_time.Year(), max_el_time.Month(), max_el_time.Day(), UTC_TO_EST(max_el_time.Hour()), max_el_time.Minute(), max_el_time.Second(), max_el_az, max_el);
            printf("\n");
            if (argc == 3)
            {
                fprintf(fp, "== PASS END ==\n\nSection Statistics\n    Maximum Elevation:\n%d.%d.%d %02d:%02d:%02d    %003.02lf     %003.02lf\n\n", max_el_time.Year(), max_el_time.Month(), max_el_time.Day(), UTC_TO_EST(max_el_time.Hour()), max_el_time.Minute(), max_el_time.Second(), max_el_az, max_el);
            }

            in_pass = false;
            max_el = 0;
        }
    }

    if (argc == 3)
    {
        fclose(fp);
    }

    return 0;
}