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
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "CoordTopocentric.h"
#include "DateTime.h"
#include "Observer.h"
#include "SGP4.h"
#include "meb_debug.h"
#include "predict.h"

// TLE Object SX [https://www.n2yo.com/satellite/?s=49278]
// TLE Object SW [https://www.n2yo.com/satellite/?s=49277]

// Invocation:
// ./predict.out TLE.txt {Days to Predict} output.txt
// ./predict.out {Input TLE File} {Days to Predict} {Output Data File}
// Arguments 1 and 2 are required, argument 3 is optional.
int main(int argc, char *argv[])
{
    bool fout = false;
    char fname_tle[256] = {0};
    char fname_out[256] = {0};
    char TLE[2][TLE_LEN] = {0};
    FILE* fp_out = NULL;

    if (argc == 3)
    {
        // Predict with only console output.
        bprintlf(YELLOW_FG "NOT PRINTING DATA TO FILE");
        strcpy(fname_tle, argv[1]);
    }
    else if (argc == 4)
    {
        // Predict with an output file generated.
        fout = true;
        strcpy(fname_tle, argv[1]);
        strcpy(fname_out, argv[3]);
        bprintlf(YELLOW_FG "PRINTING DATA TO %s", fname_out);
        fp_out = fopen(fname_out, "w");
        if (fp_out == NULL)
        {
            dbprintlf(FATAL "Failed to open %s for write.", fname_out);
            return -1;
        }
    }
    else
    {
        // Invalid number of command line arguments entered.
        bprintlf(FATAL "Invalid syntax. Valid invocations:");
        bprintlf(FATAL "./predict.out {Input TLE File} {Days to Predict}");
        bprintlf(FATAL "./predict.out {Input TLE File} {Days to Predict} {Output Data File}");
        return -1;
    }

    FILE* fp_tle = fopen(fname_tle, "r");
    if (fp_tle == NULL)
    {
        dbprintlf(FATAL "Could not open \"%s\"!", fname_tle);
        return -1;
    }
    fgets(TLE[0], TLE_LEN, fp_tle);
    fgetc(fp_tle);
    fgets(TLE[1], TLE_LEN, fp_tle);
    fclose(fp_tle);


    printf("\n");
    bprintlf("PASS PREDICTIONS FOR:");
    bprintlf("%s", TLE[0]);
    bprintlf("%s", TLE[1]);

    SGP4 *satellite = new SGP4(Tle(TLE[0], TLE[1]));
    Observer *dish = new Observer(GS_LAT, GS_LON, GS_ELEV);
    DateTime tnow = DateTime::Now(true);

    Eci pos_now = satellite->FindPosition(tnow);
    CoordTopocentric current_pos = dish->GetLookAngle(pos_now);
    CoordGeodetic current_lla = pos_now.ToGeodetic();

    int gen_days = atoi(argv[2]);

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

    if (fout)
    {
        fprintf(fp_out, "Report Generated %d.%d.%d %02d:%02d:%02d EST\nData for %d days.\n\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), gen_days);

        fprintf(fp_out, "%s\n%s\n", TLE[0], TLE[1]);
        fprintf(fp_out, "======================================================================\n\n");
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
                bprintlf("== SATELLITE PASS (Now + %d minutes) ==", i / 60);
                bprintlf("Time (EST)             Az (deg)   El (deg)");
                bprintlf("------------------------------------------");
                if (fout)
                {
                    fprintf(fp_out, "== SATELLITE PASS (Now + %d minutes) ==\n", i / 60);
                    fprintf(fp_out, "Time (EST)             Az (deg)   El (deg)\n");
                    fprintf(fp_out, "------------------------------------------\n");
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

                if (fout)
                {
                    fprintf(fp_out, "%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);
                }
            }
        }
        else if (in_pass)
        {
            // Make sure to print final state when pass ends.
            tEST = tnext - FiveHours;
            bprintlf("%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);

            if (fout)
            {
                fprintf(fp_out, "%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);
            }

            bprintlf("Pass Statistics");
            bprintlf("    Maximum Elevation:");
            tEST = max_el_time - FiveHours;
            bprintlf("%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), max_el_az, max_el);
            printf("\n\n");
            if (fout)
            {
                fprintf(fp_out, "\nPass Statistics\n    Maximum Elevation:\n%d.%d.%d %02d:%02d:%02d    %6.02lf     %6.02lf\n\n\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), max_el_az, max_el);
            }

            in_pass = false;
            max_el = 0;
        }
    }

    if (fout)
    {
        fclose(fp_out);
    }

    return 0;
}