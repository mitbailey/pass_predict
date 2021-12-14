/**
 * @file predict.cpp
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Displays visible pass times for a given satellite.
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
#include "CoordTopocentric.hpp"
#include "DateTime.hpp"
#include "Observer.hpp"
#include "SGP4.hpp"
#include "meb_print.h"
#include "predict.h"

#define WEBSTREAM_URL "http://celestrak.com/NORAD/elements/stations.txt"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <windows.h>
#include <urlmon.h>

#define WEBSTREAM_TMP "webstream.tmp"

#endif

// TODO: Update README.md file.

// TLE Object SX [https://www.n2yo.com/satellite/?s=49278]
// TLE Object SW [https://www.n2yo.com/satellite/?s=49277]

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
    int days_to_predict = 2;   // Predict two days (48 hours) by default.
    bool use_norad_id = true;  // Assume the use of a NORAD ID by default.
    bool use_out_file = false; // No out-file printing by default.

    if (argc == 1)
    {
        // ./predict.out
        // Ask the user for input.

        // TODO: Make last two optional
        
        bprintf("NORAD ID or TLE input filename: ");
        scanf("%s", identifier);

        bprintf("Output filename: ");
        scanf("%s", fname_out);
        use_out_file = true;

        bprintf("Days to predict: ");
        scanf("%d", &days_to_predict);
    }
    else if (argc == 2)
    {
        // ./predict.out {Input File}
        // ./predict.out {NORAD ID}

        // TODO: Add usages printout and exit() if user uses -help.

        strcpy(identifier, argv[1]);
    }
    else if (argc == 3)
    {
        // ./predict.out {Input File} {Output File}
        // ./predict.out {NORAD ID} {Output File}
        // Default prediction time.

        strcpy(identifier, argv[1]);
        strcpy(fname_out, argv[2]);
        use_out_file = true;
    }
    else if (argc == 4)
    {
        // ./predict.out {Input File} {Output File} {Days to Predict}
        // ./predict.out {NORAD ID} {Output File} {Days to Predict}
        // Given prediction time.

        strcpy(identifier, argv[1]);
        strcpy(fname_out, argv[2]);
        use_out_file = true;
        days_to_predict = atoi(argv[3]);
    }
    else
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

        dbprintlf(FATAL "Invalid usage.");
        dbprintlf("Usages (REQuired, OPTional):");
        dbprintlf(RED_FG "./predict.out");
        dbprintlf(RED_FG "./predict.out {REQ: Five-Digit NORAD ID or Input File} {OPT: Output File} {OPT: Days to Predict}");

#else // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

        dbprintlf(FATAL "Invalid usage.");
        dbprintlf("Usages (" RED_FG "required" TERMINATOR ", " BLUE_FG "optional" TERMINATOR "):");
        dbprintlf(RED_FG "./predict.out");
        dbprintlf(RED_FG "./predict.out {Five-Digit NORAD ID or Input File} " TERMINATOR BLUE_FG "{Output File} {Days to Predict}");

#endif // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)


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
    if (use_norad_id)
    {
        // Update from internet.

        FILE *pp = NULL;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

// #include <windows.h>
// #include <urlmon.h>
// #pragma comment(lib, "urlmon.lib")

        LPCTSTR wurl = (LPCTSTR)WEBSTREAM_URL;
        LPCTSTR wdest = (LPCTSTR)WEBSTREAM_TMP;
        char cdest[] = WEBSTREAM_TMP;
        if (S_OK == URLDownloadToFile(NULL, wurl, wdest, 0, NULL))
        {
            dbprintlf("Windows reports file successfully downloaded.");
        }
        else
        {
            dbprintlf("Fail");
        }

        pp = fopen(cdest, "r");

#else // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

        dbprintlf("Non-Windows OS detected, updating TLE from internet.");

        char cmd[512] = {0};

        snprintf(cmd, sizeof(cmd), "wget -q -O- %s", WEBSTREAM_URL);
        pp = popen(cmd, "r");

#endif // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

        if (pp == NULL)
        {
            dbprintlf(RED_FG "Could not open pipe to obtain online TLE data.");
            return -2;
        }

        char search_str1[512] = {0};
        snprintf(search_str1, sizeof(search_str1), "1 %s", identifier);
        char search_str2[512] = {0};
        snprintf(search_str2, sizeof(search_str2), "2 %s", identifier);

        char buf[512] = {0};
        bool l1_updated = false;
        bool l2_updated = false;

        while (fgets(buf, sizeof(buf), pp))
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
            if (l1_updated && l2_updated)
                break;
        }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        fclose(pp);
#else
        pclose(pp);
#endif

        if (l1_updated && l2_updated)
        {
            tprintlf(GREEN_FG "Obtained updated TLE for %s.", identifier);
            TLE[0][69] = '\0';
            TLE[1][69] = '\0';
            tprintlf("%s", TLE[0]);
            tprintlf("%s", TLE[1]);
        }
        else
        {
            tprintlf(RED_FG "Failed to update TLE for %s.", identifier);
            return -3;
        }
    }
    else
    {
        // Read from file.
        fp_tle = fopen(identifier, "r");
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

    // Open the output file.
    if (use_out_file)
    {
        fp_out = fopen(fname_out, "w");
        if (fp_out == NULL)
        {
            dbprintlf(FATAL "Failed to open %s for write.", fname_out);
            return -5;
        }
    }

    bprintlf();
    bprintlf("PASS PREDICTIONS FOR:");
    bprintlf("%s", TLE[0]);
    bprintlf("%s", TLE[1]);

    SGP4 *satellite = new SGP4(Tle(TLE[0], TLE[1]));
    Observer *dish = new Observer(GS_LAT, GS_LON, GS_ELEV);
    DateTime tnow = DateTime::Now(true);

    Eci pos_now = satellite->FindPosition(tnow);
    CoordTopocentric current_pos = dish->GetLookAngle(pos_now);
    CoordGeodetic current_lla = pos_now.ToGeodetic();

    bprintlf(YELLOW_FG "DATA FOR %d DAYS", days_to_predict);

    bprintlf("Current Position: %.2f AZ, %.2f EL | %.2f LA, %.2f LN", current_pos.azimuth DEG, current_pos.elevation DEG, current_lla.latitude DEG, current_lla.longitude DEG);
    DateTime tnext = tnow;
    bool in_pass = false;
    double max_el = 0.0;
    double max_el_az = 0.0;
    DateTime max_el_time;
    int begin_et = 0;

    TimeSpan FiveHours(5, 0, 0);
    DateTime tEST = tnow - FiveHours;

    if (use_out_file)
    {
        fprintf(fp_out, "Report Generated %04d.%02d.%02d %02d:%02d:%02d EST\nData for %d days.\n\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), days_to_predict);

        fprintf(fp_out, "%s\n%s\n", TLE[0], TLE[1]);
        fprintf(fp_out, "======================================================================\n\n");
    }

    bprintlf("It is currently %04d.%02d.%02d %02d:%02d:%02d UTC\n", tnow.Year(), tnow.Month(), tnow.Day(), tnow.Hour(), tnow.Minute(), tnow.Second());

    bprintlf("It is currently %04d.%02d.%02d %02d:%02d:%02d EST\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second());

    for (int i = 0; i < 86400 * days_to_predict; i++)
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
                if (use_out_file)
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
                bprintlf("%04d.%02d.%02d %02d:%02d:%02d    %6.02lf     %6.02lf", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);

                if (use_out_file)
                {
                    fprintf(fp_out, "%04d.%02d.%02d %02d:%02d:%02d    %6.02lf     %6.02lf\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);
                }
            }
        }
        else if (in_pass)
        {
            // Make sure to print final state when pass ends.
            tEST = tnext - FiveHours;
            bprintlf("%04d.%02d.%02d %02d:%02d:%02d    %6.02lf     %6.02lf\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);

            if (use_out_file)
            {
                fprintf(fp_out, "%04d.%02d.%02d %02d:%02d:%02d    %6.02lf     %6.02lf\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), pos_ahd.azimuth DEG, ahd_el);
            }

            bprintlf("Pass Statistics");
            bprintlf("    Maximum Elevation:");
            tEST = max_el_time - FiveHours;
            bprintlf("%04d.%02d.%02d %02d:%02d:%02d    %6.02lf     %6.02lf", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), max_el_az, max_el);
            printf("\n\n");
            if (use_out_file)
            {
                fprintf(fp_out, "\nPass Statistics\n    Maximum Elevation:\n%04d.%02d.%02d %02d:%02d:%02d    %6.02lf     %6.02lf\n\n\n", tEST.Year(), tEST.Month(), tEST.Day(), tEST.Hour(), tEST.Minute(), tEST.Second(), max_el_az, max_el);
            }

            in_pass = false;
            max_el = 0;
        }
    }

    if (use_out_file)
    {
        fclose(fp_out);
    }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    remove(WEBSTREAM_TMP);
#endif // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

    return 0;
}