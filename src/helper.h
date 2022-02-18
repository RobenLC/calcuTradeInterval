#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "calcuTradeInterval.h"

int itval_dump(int *dat, int size);
int ymds_dump(struct date_YMD_s *pymds, int size);
int leap_dump(char *dat, int size);
int rand_dump(int *dat, int size);
int mem_dump(void *src, int size);
int itval_to_str(char *str, int *itval, int size);
int rand_to_str(char *str, int *rand, int size);
int ymd_to_str(char *str, struct date_YMD_s *pymds, int size);
int file_to_rand(int *rand, FILE *f, int size);
int file_to_ymds(struct date_YMD_s *pymds, FILE *f, int size);
//int readfile(char *filename, uint8_t **data);
//int print2file(char *filename, struct data_linklist_s *max32list, int size, uint16_t *last32);

#endif
