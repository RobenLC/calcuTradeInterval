
#include "calcuTradeInterval.h"

#define LOG_CALCU (0)

static inline void ymd_calcu_daysofyear(struct date_YMD_s *pymds, int *monthdays)
{
    if (pymds->m == 1) {
        pymds->daysofyear = pymds->d;
    } else {
        pymds->daysofyear = monthdays[pymds->m - 2] + pymds->d;
    }
}

int ymd_calcu(struct date_YMD_s *pymds, int size, char *leap)
{
    int mthday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int *mthdayAccu=0, *mthdayAccuLeap=0;
    int accu=0;
    struct date_YMD_s *pymd=0;
    
    mthdayAccu = malloc(sizeof(int) * 12);
    mthdayAccuLeap = malloc(sizeof(int) * 12);

    for (int n =0; n < 12; n++) {
        accu += mthday[n];
        mthdayAccu[n] = accu;

        #if LOG_CALCU
        printf("%d. %d ", n, mthdayAccu[n]);
        #endif
    }
    #if LOG_CALCU
    printf("\n");
    #endif

    mthday[1] += 1;
    accu = 0;

    for (int n =0; n < 12; n++) {
        accu += mthday[n];
        mthdayAccuLeap[n] = accu;

        #if LOG_CALCU
        printf("%d. %d ", n, mthdayAccuLeap[n]);
        #endif
    }
    #if LOG_CALCU
    printf("\n");
    #endif

    for (int n = 0; n < (size+1); n++) {
        pymd = &pymds[n];
        if (leap[(pymd->y - 1900) % 2000]) {
            ymd_calcu_daysofyear(pymd, mthdayAccuLeap);
        } else {
            ymd_calcu_daysofyear(pymd, mthdayAccu);
        }
    }

    free(mthdayAccu);
    free(mthdayAccuLeap);
    return 0;
}

int ymd_calcu_itval(int *pitval, struct date_YMD_s *pymds, int size, char *leap)
{
    struct date_YMD_s *pymd=0, *pymdN=0;
    int daysyear=0, curyear=0, interval=0;
    
    for (int n = 0; n < size; n++) {
        pymd = &pymds[n];
        pymdN = &pymds[n+1];

        interval = 0;
        
        if (pymdN->y == pymd->y) {
            pitval[n] = pymdN->daysofyear - pymd->daysofyear;
        } else {
            daysyear = 365 + leap[(pymd->y - 1900) % 2000];
            interval += daysyear - pymd->daysofyear;
            
            curyear = pymd->y + 1;
            while (curyear < pymdN->y) {
                daysyear = 365 + leap[(curyear - 1900) % 2000];
                interval += daysyear;

                curyear++;
            }

            interval += pymdN->daysofyear;

            pitval[n] = interval;
        }
    }

    return 0;
}

int make_leapYearArry(char *leap, int startyear, int size)
{
    int year=0;
    
    for (int n = 0; n < size; n++) {
        year = startyear + n;

        if (year % 4) {
            leap[n] = 0;
        } else {
            if (year % 100) {
                leap[n] = 1;
            } else {
                if (year % 400) {
                    leap[n] = 1;
                } else {
                    leap[n] = 0;
                }
            }
        }
    }
    
    return 0;
}

int ymd_calcu_mthday(struct date_YMD_s *pymds, int *monthdays)
{
    int idx=0;
    int daysyear=0, daysmonth=0;
    daysyear = pymds->daysofyear;
    
    while (idx < 12) {
        if (daysyear <= monthdays[idx]) {
            break;
        }
        idx++;
    }

    if (idx == 12) {
        printf("Error search result not match \n!!");
        return -1;
    }

    if (idx == 0) {
        daysmonth = daysyear;
    } else {
        daysmonth = daysyear - monthdays[idx - 1];
    }

    pymds->m = idx+1;
    pymds->d = daysmonth;

    return 0;
}

int ymd_n_rand_calcu(struct date_YMD_s *pymds, int *rand, int size, char *leap)
{
    int mthday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int itval = 0, totalDaysaYear=0, curday=0, cnt=0, nextyeardays=0, accu=0;
    int *mthdayAccu=0, *mthdayAccuLeap=0;
    
    for (int n = 0; n < size; n++) {
    
        itval = rand[n];
        totalDaysaYear = 365 + leap[(pymds[n].y - 1900) % 2000];

        curday = pymds[n].daysofyear;

        #if LOG_CALCU
        printf("%d. rand %d, %d year %d days \n", n, itval, pymds[n].y, pymds[n].daysofyear);
        #endif
        
        if ((curday + itval) > totalDaysaYear) {
            nextyeardays = 0;
            cnt = 0;
            do {
                cnt++;
                totalDaysaYear += nextyeardays;

                nextyeardays = 365 + leap[(pymds[n].y - 1900 + cnt) % 2000];
            } while ((curday + itval) > (totalDaysaYear + nextyeardays));
            
            pymds[n+1].y = pymds[n].y + cnt;
            pymds[n+1].daysofyear = (curday + itval) - totalDaysaYear;
            
        } else {
            pymds[n+1].y = pymds[n].y;
            pymds[n+1].daysofyear = pymds[n].daysofyear + itval;
        }

        #if LOG_CALCU
        printf("    %d year %d days \n", pymds[n+1].y, pymds[n+1].daysofyear);
        #endif
        
    }

    mthdayAccu = malloc(sizeof(int) * 12);
    mthdayAccuLeap = malloc(sizeof(int) * 12);

    for (int n =0; n < 12; n++) {
        accu += mthday[n];
        mthdayAccu[n] = accu;

        #if LOG_CALCU
        printf("%d. %d ", n, mthdayAccu[n]);
        #endif
    }
    #if LOG_CALCU
    printf("\n");
    #endif

    mthday[1] += 1;
    accu = 0;

    for (int n =0; n < 12; n++) {
        accu += mthday[n];
        mthdayAccuLeap[n] = accu;

        #if LOG_CALCU
        printf("%d. %d ", n, mthdayAccuLeap[n]);
        #endif
    }
    #if LOG_CALCU
    printf("\n");
    #endif
    
    for (int n = 1; n < (size+1); n++) {
        #if LOG_CALCU
        printf("setup %d year %d days \n", pymds[n].y, pymds[n].daysofyear);
        #endif
        if (leap[(pymds[n].y - 1900) % 2000]) {
            ymd_calcu_mthday(&pymds[n], mthdayAccuLeap);
        } else {
            ymd_calcu_mthday(&pymds[n], mthdayAccu);
        }
    }
    
    free(mthdayAccu);
    free(mthdayAccuLeap);

    return 0;
}

