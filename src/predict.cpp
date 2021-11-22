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

#define SEC *1000000.0           // nanoseconds to seconds
#define DEG *(180.0 / 3.1415926) // radians to degrees
#define GS_LAT 42.655583
#define GS_LON -71.325433
#define GS_ELEV 0.061                                             // Lowell ASL + Olney Height; Kilometers for some reason.
#define MIN_ELEV 10.0                                          // degrees

// TLE Object SX [https://www.n2yo.com/satellite/?s=49278]
const char TLE[2][70] = {"1 49278U 98067SX  21325.91916002  .00034883  00000-0  56191-3 0  9992",
                         "2 49278  51.6409 282.1060 0002497 259.5267 100.5442 15.52605836  6366"};

// TLE Object SW [https://www.n2yo.com/satellite/?s=49277]
// const char TLE[2][70] = {"1 49277U 98067SW  21325.87577406  .00016703  00000-0  28990-3 0  9994",
//                          "2 49277  51.6415 282.5649 0004170 274.5768  85.4745 15.50971000  6333"};

int main(int argc, char *argv[])
{
    SGP4 *satellite = new SGP4(Tle(TLE[0], TLE[1]));
    Observer *dish = new Observer(GS_LAT, GS_LON, GS_ELEV);
    DateTime tnow = DateTime::Now(true);

    Eci pos_now = satellite->FindPosition(tnow);
    CoordTopocentric current_pos = dish->GetLookAngle(pos_now);
    CoordGeodetic current_lla = pos_now.ToGeodetic();

    printf("\n");
    bprintlf("PASS PREDICTIONS FOR:");
    bprintlf("%s", TLE[0]);
    bprintlf("%s", TLE[1]);

    char filename[256] = {0};
    FILE *fp = NULL;

    if (argc > 3 || argc < 2)
    {
        bprintlf(FATAL "Invalid command line syntax.");
        bprintlf(FATAL "Invocation:");
        bprintlf(FATAL "./predict.out <days to predict> output.txt");
    }
    else if (argc == 3)
    {
        strcpy(filename, argv[2]);
        bprintlf(YELLOW_FG "PRINTING DATA TO %s", filename);
        fp = fopen(filename, "w");
        if (fp == NULL)
        {
            bprintlf(FATAL "Failed to open %s for write.", filename);
            return -1;
        }
    }
    else
    {
        bprintlf(YELLOW_FG "NOT PRINTING DATA");
    }

    int gen_days = atoi(argv[1]);

    bprintlf(YELLOW_FG "DATA FOR %d DAYS", gen_days);

    bprintlf("Current Position: %.2f AZ, %.2f EL | %.2f LA, %.2f LN", current_pos.azimuth DEG, current_pos.elevation DEG, current_lla.latitude DEG, current_lla.longitude DEG);

    DateTime tnext = tnow;
    bool in_pass = false;
    double max_el = 0.0;
    double max_el_az = 0.0;
    DateTime max_el_time;
    int begin_et = 0;

    TimeSpan FiveHours(5, 0, 0);
    DateTime tEST = tnow - FiveHours;

    if (argc == 3)
    {
        fprintf(fp, "Report Generated %d.%d.%d %02d:%02d:%02d EST\nData for %d days.\n\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), gen_days);

        fprintf(fp, "%s\n%s\n\n\n", TLE[0], TLE[1]);
    }

    bprintlf("It is currently %d.%d.%d %02d:%02d:%02d UTC\n", tnow.Year(), tnow.Month(), tnow.Day(), tnow.Hour(), tnow.Minute(), tnow.Second());

    bprintlf("It is currently %d.%d.%d %02d:%02d:%02d EST\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second());

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
                bprintlf("== PASS START (Now + %d minutes) ==", i / 60);
                bprintlf("Time (EST)             Az (deg)   El (deg)");
                bprintlf("------------------------------------------");
                if (argc == 3)
                {
                    fprintf(fp, "== PASS START (Now + %d minutes) ==\n", i / 60);
                    fprintf(fp, "Time (EST)             Az (deg)   El (deg)\n");
                    fprintf(fp, "------------------------------------------\n");
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
                
                tEST = tnext - FiveHours;
                bprintlf("%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);

                if (argc == 3)
                {
                    fprintf(fp, "%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);
                }
            }
        }
        else if (in_pass)
        {
            // Make sure to print final state when pass ends.
            tEST = tnext - FiveHours;
            bprintlf("%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);

            if (argc == 3)
            {
                fprintf(fp, "%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);
            }

            bprintlf("== PASS END ==\n");
            bprintlf("Section Statistics");
            bprintlf("    Maximum Elevation:");
            tEST = max_el_time - FiveHours;
            bprintlf("%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), max_el_az, max_el);
            printf("\n");
            if (argc == 3)
            {
                fprintf(fp, "== PASS END ==\n\nSection Statistics\n    Maximum Elevation:\n%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf\n\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), max_el_az, max_el);
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