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

/*
Usage:
predict.out TLE.txt output.txt
predict.out TLE.txt output.txt {Days to Predict}
predict.out NORADID output.txt
predict.out NORADID output.txt {Days to Predict}
 */
int main(int argc, char *argv[])
{
    char TLE[2][TLE_LEN] = {0};
    FILE *fp_tle = NULL;
    FILE *fp_out = NULL;

    char identifier[256] = {0}; // NORAD ID or TLE filename.
    char fname_out[256] = {0};
    int days_to_predict = 2;
    bool use_norad_id = true;

    if (argc == 1)
    {
        // Ask the user for input.
        bprintf("NORAD ID or TLE input filename: ");
        scanf("%s", identifier);

        bprintf("Output filename: ");
        scanf("%s", fname_out);

        bprintf("Days to predict: ");
        scanf("%d", &days_to_predict);
    }
    else if (argc == 3)
    {
        // Default prediction time.

        strcpy(identifier, argv[1]);
        strcpy(fname_out, argv[2]);
    }
    else if (argc == 4)
    {
        // Given prediction time.

        strcpy(identifier, argv[1]);
        strcpy(fname_out, argv[2]);
        days_to_predict = atoi(argv[3]);
    }
    else
    {
        dbprintlf(FATAL "Invalid usage.");
        dbprintlf("Usages (" RED_FG "required" TERMINATOR ", " BLUE_FG "optional" TERMINATOR "):");
        dbprintlf(RED_FG "./predict.out");
        dbprintlf(RED_FG "./predict.out {Five-Digit NORAD ID or Input File} {Output File}" TERMINATOR " " BLUE_FG "{Days to Predict}");

        return -1;
    }

    for (int i = 0; i < 5; i++)
    {
        if (!isdigit(identifier[i]))
        {
            // Non-digit found, cannot be NORAD ID.
            use_norad_id = false;
            break;
        }
    }
    bprintlf();

    if (use_norad_id)
    {
        dbprintlf("NORAD ID detected, using %s.", identifier);
    }
    else
    {
        dbprintlf("Filename detected, using %s.", identifier);
    }

    // If filename is given, continue on as before.
    // If NORAD ID is given, fetch the TLE from the internet.
    // IF LINUX
    char cmd[512] = {0};
    const char url[] = "http://celestrak.com/NORAD/elements/stations.txt";
    snprintf(cmd, sizeof(cmd), "wget -q -O- %s", url);
    FILE *pp = popen(cmd, "r");
    if (pp == NULL)
    {
        dbprintlf(RED_FG "Could not open pipe to obtain online TLE data.");
        return -2;
    }

    char search_str1[128] = {0};
    snprintf(search_str1, sizeof(search_str1), "1 %s", identifier);
    char search_str2[128] = {0};
    snprintf(search_str2, sizeof(search_str2), "2 %s", identifier);
    // char l1[128] = {0};
    // char l2[128] = {0};
    char buf[128] = {0};
    bool l1_updated = false;
    bool l2_updated = false;

    while(fgets(buf, sizeof(buf), pp))
    {
        if (strstr(buf, search_str1) != NULL)
        {
            strcpy(TLE[0], buf);
            l1_updated = true;
        }
        if (strstr(buf, search_str2) != NULL)
        {
            strcpy(TLE[1], buf);
            l2_updated = true;
        }
    }
    pclose(pp);
    if (l1_updated && l2_updated)
    {
        tprintlf(GREEN_FG "Obtained updated TLE for %s.", identifier);
        tprintlf("%s\n%s", TLE[0], TLE[1]);
        // l1[69] = '\0';
        // l2[69] = '\0';
        // Update(l1, l2);
    }
    else
    {
        tprintlf(RED_FG "Failed to update TLE for %s.", identifier);
        return -3;
    }

    exit(42);

    // IF WINDOWS
    // TODO: Make the update functionality work on Windows.

    // This code commented due to deprecation and has been superceded.
    // if (argc == 3)
    // {
    //     // Predict with only console output.
    //     bprintlf(YELLOW_FG "NOT PRINTING DATA TO FILE");
    //     strcpy(fname_tle, argv[1]);
    // }
    // else if (argc == 4)
    // {
    //     // Predict with an output file generated.
    //     fout = true;
    //     strcpy(fname_tle, argv[1]);
    //     strcpy(fname_out, argv[3]);
    //     bprintlf(YELLOW_FG "PRINTING DATA TO %s", fname_out);
    //     fp_out = fopen(fname_out, "w");
    //     if (fp_out == NULL)
    //     {
    //         dbprintlf(FATAL "Failed to open %s for write.", fname_out);
    //         return -1;
    //     }
    // }
    // else
    // {
    //     // Invalid number of command line arguments entered.
    //     bprintlf(FATAL "Invalid syntax. Valid invocations:");
    //     bprintlf(FATAL "./predict.out {Input TLE File} {Days to Predict}");
    //     bprintlf(FATAL "./predict.out {Input TLE File} {Days to Predict} {Output Data File}");
    //     return -1;
    // }
    
    // Read from file into TLE only if we were not given a NORAD ID.
    if (!use_norad_id)
    {
        FILE *fp_tle = fopen(identifier, "r");
        if (fp_tle == NULL)
        {
            dbprintlf(FATAL "Could not open \"%s\"!", identifier);
            return -4;
        }
        fgets(TLE[0], TLE_LEN, fp_tle);
        fgetc(fp_tle);
        fgets(TLE[1], TLE_LEN, fp_tle);
        fclose(fp_tle);
    }

    // TODO: From here down should be re-checked for compatability with the new functionality.

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

    // TODO: How to determine whether it is file-outputted or just on screen?
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