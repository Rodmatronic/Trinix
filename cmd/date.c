#include "../include/types.h"
#include "../include/stat.h"
#include "../include/stdio.h"
#include "../include/errno.h"

int day_of_year(int year, int month, int day) {
    int days = day;
    for (int i = 0; i < month - 1; i++) {
        days += days_in_month[i];
    }
    if (month > 2 && isleapyear(year)) {
        days++;
    }
    return days;
}

#define PROGRAM_NAME "date"
#define AUTHORS proper_name ("David MacKenzie")

static struct option long_options[] = {
    {"help",    no_argument,       0, 'h'},
    {"version", no_argument,       0, 'v'},
    {0,         0,                 0,  0 }
};

void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    emit_try_help ();
  else
    {
      printf ("Usage: %s [FORMAT]\n  or:  %s [MMDDhhmm[[CC]YY][.ss]]\n", program_name, program_name);
      printf ("Display date and time in the given FORMAT.\nWith [MMDDhhmm[[CC]YY][.ss]], set the date and time.\n", stdout);

      printf ("\nExamples:\nSet the date to the 8th of October (in the current year)\n  # date 10080045\nOr, by specifying a year with/without a century:\n  # date 100800451980\n", stdout);
    }
  exit (status);
}

void version() {
	fprintf(stderr, "%s - (Exnix Coreutil)\n", PROGRAM_NAME);
}

int main(int argc, char *argv[]) {
    int opt;
    int option_index = 0;

    set_program_name (argv[0]);
    while ((opt = getopt_long(argc, argv, "hv", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                usage(0);
                exit(0);
            case 'v':
                version();
                exit(0);
            case '?':
                exit(1);
        }
    }

    if (optind == argc) {
        goto print;
    }

    if (optind < argc - 1) {
        fprintf(stderr, "date: too many arguments\n");
        usage(1);
        exit(1);
    }

    char *s = argv[optind];
    int len = strlen(s);
    if (len != 8 && len != 10 && len != 12) {
        fprintf(stderr, "date: invalid format\n");
        exit(1);
    }
        int MM = (s[0] - '0') * 10 + (s[1] - '0');
        int DD = (s[2] - '0') * 10 + (s[3] - '0');
        int hh = (s[4] - '0') * 10 + (s[5] - '0');
        int mm = (s[6] - '0') * 10 + (s[7] - '0');
        int yy = 0;
        if (len == 10) {
            yy = (s[8] - '0') * 10 + (s[9] - '0');
        } else if (len == 12) {
            yy = (s[8] - '0') * 1000 + (s[9] - '0') * 100 + (s[10] - '0') * 10 + (s[11] - '0');
        }

        if (MM < 1 || MM > 12) {
            fprintf(stderr, "date: invalid month\n");
            exit(1);
        }
        if (DD < 1 || DD > 31) {
            fprintf(stderr, "date: invalid day\n");
            exit(1);
        }
        if (hh < 0 || hh > 23) {
            fprintf(stderr, "date: invalid hour\n");
            exit(1);
        }
        if (mm < 0 || mm > 59) {
            fprintf(stderr, "date: invalid minute\n");
            exit(1);
        }

        unsigned long now = time(0);
        struct tm now_tm;
        epoch_to_tm(now, &now_tm);

        struct tm new_tm;
        new_tm.tm_sec = 0;
        new_tm.tm_min = mm;
        new_tm.tm_hour = hh;
        new_tm.tm_mday = DD;
        new_tm.tm_mon = MM - 1;

        int century;

        if (len == 10 || len == 12) {
            if (yy < 100) {
                century = ((now_tm.tm_year+1970)/100)*100;
                new_tm.tm_year = century+yy-1970;
            } else {
                new_tm.tm_year = yy - 1970;
            }
        } else {
            new_tm.tm_year = now_tm.tm_year;
        }

        int full_year = 1970 + new_tm.tm_year;
        int max_days = days_in_month[new_tm.tm_mon];
        if (new_tm.tm_mon == 1 && isleapyear(full_year)) { // February in leap year
            max_days++;
        }
        if (DD < 1 || DD > max_days) {
            fprintf(stderr, "date: invalid day for month\n");
            exit(1);
        }

        unsigned long new_epoch = mktime(&new_tm);
        if (stime(new_epoch) != 0) { // not root, or failed for whatever reason.
	    printf("%s\n", ctime(&new_tm));
            exit(1);
        }
print:
    unsigned long epoch = 0;
    epoch = time(epoch);
    struct tm tm;
    epoch_to_tm(epoch, &tm);
    printf("%s\n", ctime(&tm));
    exit(0);
}
