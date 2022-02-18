#ifndef _CALCUTRADEINTERVAL_H_
#define _CALCUTRADEINTERVAL_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct date_YMD_s{
    int y;
    int m;
    int d;
    int daysofyear;
};

int make_leapYearArry(char *leap, int startyear, int size);
int ymd_n_rand_calcu(struct date_YMD_s *pymds, int *rand, int size, char *leap);
int ymd_calcu(struct date_YMD_s *pymds, int size, char *leap);
int ymd_calcu_itval(int *pitval, struct date_YMD_s *pymds, int size, char *leap);

#endif

