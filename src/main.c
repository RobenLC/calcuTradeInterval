
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "calcuTradeInterval.h"
#include "helper.h"

#define IS_LEAP_YEAR(x)  leapArry[(x - 1900)%2000]
#define LOG_MAIN (0)
int main(int argc, char *argv[])
{    
    int cnt=0, len=0, lenIn=0, lenOut=0;
    int seed=0, randNnm=0, randMax=100;
    char *inStr=0, *inParm=0;
    char destFilePath[128]={0}, inputFilePath[128]={0};
    char modec=0;
    FILE *fin=0, *fout=0;
    int *randNumAry=0;
    char *rand2str=0, *leapArry=0;
    int retLen=0;
    struct date_YMD_s *pymd_s=0;
    char *ymd2str=0, *itval2str=0;
    int *itvalArry=0;

    #if LOG_MAIN
    printf("argc: %d \n", argc);
    #endif

    if (argc > 1) {
        while (cnt < argc) {
            #if LOG_MAIN
            printf("%d.[%s] \n", cnt, argv[cnt]);
            #endif

            inStr = argv[cnt];

            if (inStr[0] == '-') {
                len = strlen(inStr);
                if (len == 2) {
                    cnt++;
                    if (cnt >= argc) {
                        break;
                    }
                    
                    inParm = argv[cnt];
                    switch (inStr[1]) {
                        case 'm':   /* set range of random number */
                            randMax = atoi(inParm);
                            printf("range of rand: %d \n", randMax);
                            break;
                        case 'n':   /* set total of random number */
                            randNnm = atoi(inParm);
                            printf("number of rand: %d \n", randNnm);
                            break;
                        case 's':   /* set the seed of random number */
                            seed = atoi(inParm);
                            printf("seed: %d, max: %d first 10 number: ", seed, RAND_MAX);
                            srand(seed);
                            for(int n=0; n < 10; n++) {
                                printf("%d ", rand());
                            }
                            printf("\n");
                            break;
                        case 'g':   /* flag to produce test sample via random number generator*/
                            modec = inStr[1];
                            break;
                        case 'i':   /* input file */
                            lenIn = strlen(inParm);
                            if (lenIn > 128) {
                                printf("Error, input string too long len: %d \n", lenIn);
                                break;
                            }
                            strncpy(inputFilePath, inParm, lenIn);
                            fin = fopen(inputFilePath, "r");
                            printf("input path: [%s] strlen: %d, fin: %x \n", inputFilePath, lenIn, (int)fin);
                            break;
                        case 'o':   /* output file */
                            lenOut = strlen(inParm);
                            if (lenOut > 128) {
                                printf("Error, output string too long len: %d \n", lenOut);
                                break;
                            }
                            strncpy(destFilePath, inParm, lenOut);
                            fout = fopen(destFilePath, "w+");
                            printf("output path: [%s] strlen: %d, fout: %x \n", destFilePath, lenOut, (int)fout);
                            break;
                        default:
                            printf("- unknown flag %c \n", inStr[1]);
                            break;
                    }
                }
            }
            else {
                cnt++;
            }
        }        
    }
    else {
        printf("need more parameters \n");
        return 0;
    }

    printf("start %c\n", modec);
    /* Leapyear mapping table start from 1900 1 1 */
    leapArry = (char *)malloc(sizeof(char) * 2000);
    memset(leapArry, 0, 2000);

    make_leapYearArry(leapArry, 1900, 2000);
    //leap_dump(leapArry, 2000);
                
    switch(modec) {
        case 'g':
            if (lenOut) {
                if (fout > 0) {
                    printf("output file [%s] exist.\n", destFilePath);
                } else {
                    printf("Error output file [%s] not exist. \n", destFilePath);
                    break;
                }
            }
            else {
                printf("please set output file using -o \n");
                break;
            }
            
            if (lenIn) {
                if (fin <= 0) {
                    printf("Error input file [%s] not exist. \n", inputFilePath);
                    break;
                }
                /**********************************************************
                 * to get the pre-set peak interval to generate test sample 
                 ***********************************************************/
                randNumAry = (int *)malloc(sizeof(int) * randNnm);
                memset(randNumAry, 0, sizeof(int) * randNnm);
                
                file_to_rand(randNumAry, fin, randNnm);

                //rand_dump(randNumAry, randNnm);

                pymd_s = (struct date_YMD_s *)malloc(sizeof(struct date_YMD_s) * (randNnm + 1));
                memset(pymd_s, 0, sizeof(struct date_YMD_s) * (randNnm + 1));

                pymd_s[0].y = 1900;
                pymd_s[0].m = 1;
                pymd_s[0].d = 1;
                pymd_s[0].daysofyear = 1;

                ymd_n_rand_calcu(pymd_s, randNumAry, randNnm, leapArry);

                //ymds_dump(pymd_s, randNnm);

                ymd2str = (char *)malloc(sizeof(char) * 12 * (randNnm + 1));

                retLen = ymd_to_str(ymd2str, pymd_s, randNnm);

                fwrite(ymd2str, 1, retLen, fout);
                fflush(fout);
                
                free(randNumAry);
                free(pymd_s);
                free(ymd2str);
            }
            else {
                /**********************************************************************************************************
                 * without pre-set peak interval, so generate the pre-set peak interval we need to generate the test sample 
                 **********************************************************************************************************/
                printf("the number of data is %d \n", randNnm);

                randNumAry = (int *)malloc(sizeof(int) * randNnm);
                if (!randNumAry) {
                    printf("Error fail to allocate memory size is %d \n", (int)(sizeof(int) * randNnm));
                    break;
                }

                for (int n = 0; n < randNnm; n++) {
                    randNumAry[n] = (rand() % randMax) + 1;
                }

                //rand_dump(randNumAry, randNnm);

                rand2str = (char *)malloc(sizeof(char) * randNnm * 4);

                retLen = rand_to_str(rand2str, randNumAry, randNnm);

                fwrite(rand2str, 1, retLen, fout);
                fflush(fout);

                free(randNumAry);
                free(rand2str);
            }
            break;
        default:
            if (lenOut) {
                if (fout > 0) {
                    printf("output file [%s] exist.\n", destFilePath);
                } else {
                    printf("Error output file [%s] not exist. \n", destFilePath);
                    break;
                }
            }
            else {
                printf("please set output file using -o \n");
                break;
            }
            
            if (lenIn) {
                if (fin <= 0) {
                    printf("Error input file [%s] not exist. \n", inputFilePath);
                    break;
                }
                /************************************
                 * to get the peak interval from data 
                 ************************************/
                pymd_s = (struct date_YMD_s *)malloc(sizeof(struct date_YMD_s) * (randNnm + 1));
                
                file_to_ymds(pymd_s, fin, randNnm+1);

                ymd_calcu(pymd_s, randNnm, leapArry);

                //ymds_dump(pymd_s, randNnm+1);

                itvalArry = (int *)malloc(sizeof(int) * randNnm);

                ymd_calcu_itval(itvalArry, pymd_s, randNnm, leapArry);

                //itval_dump(itvalArry, randNnm);

                itval2str = malloc(4 * randNnm);

                retLen = itval_to_str(itval2str, itvalArry, randNnm);
                
                fwrite(itval2str, 1, retLen, fout);
                fflush(fout);
                
                free(pymd_s);
                free(itvalArry);
                free(itval2str);
            }
            else {
                printf("please set input file using -i \n");
            }
            break;
    }

    free(leapArry);
}


