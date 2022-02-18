
#include "helper.h"

#define MEM_DUMP_EN (0)
#define LOG_HELPER (0)

int itval_dump(int *dat, int size)
{
    int inc=0;

    printf("itval dump size %d \n", size);

    while (inc < size) {
        printf("%.3d ", *dat);

        if (!((inc+1) % 16)) {
            printf(" %d \n", inc+1);
        }
        inc++;
        dat++;
    }

    printf("\n");

    return inc;
}

int ymds_dump(struct date_YMD_s *pymds, int size)
{
    int inc=0;

    printf("date_YMD_s dump size %d \n", size);

    while (inc < size) {
        printf("%.4d%.2d%.2d-%.3d ", pymds[inc].y, pymds[inc].m, pymds[inc].d, pymds[inc].daysofyear);

        if (!((inc+1) % 10)) {
            printf(" %d \n", inc+1);
        }
        inc++;
        //pymds++;
    }

    printf("\n");

    return inc;
}

int leap_dump(char *dat, int size)
{
    int inc=0;

    printf("leap dump size %d \n", size);

    while (inc < size) {
        printf("%.1d ", *dat);

        if (!((inc+1) % 20)) {
            printf(" %d \n", inc+1);
        }
        inc++;
        dat++;
    }

    printf("\n");

    return inc;
}

int rand_dump(int *dat, int size)
{
    int inc=0;

    printf("rand dump size %d \n", size);

    while (inc < size) {
        printf("%.3d ", *dat);

        if (!((inc+1) % 16)) {
            printf(" %d \n", inc+1);
        }
        inc++;
        dat++;
    }

    printf("\n");

    return inc;
}

int mem_dump(void *p, int size)
{
    int inc=0;
    char *src=0;

    src = (char *)p;

    printf("memdump sz%d: \n", size);

    while (inc < size) {
        printf("%.2x ", *src & 0xff);

        if (!((inc+1) % 16)) {
            printf(" %d \n", inc+1);
        }
        inc++;
        src++;
    }

    printf("\n");

    return inc;
}

int itval_to_str(char *str, int *itval, int size)
{
    int len=0, tot=0;
    char *dst=0;

    dst = str;

    for (int n = 0; n < size; n++) {
        sprintf(dst, "%d\n", itval[n]);
        len = strlen(dst);
        tot += len;

        dst += len;
    }

    #if MEM_DUMP_EN
    mem_dump(str, tot);
    #endif
    
    return tot;
}

int rand_to_str(char *str, int *rand, int size)
{
    int len=0, tot=0;
    char *dst=0;
    memset(str, 0, size*4);

    dst = str;

    for (int n = 0; n < size; n++) {
        sprintf(dst, "%d\n", rand[n]);
        len = strlen(dst);
        tot += len;

        dst += len;
    }

    #if MEM_DUMP_EN
    mem_dump(str, tot);
    #endif

    return tot;
}

int ymd_to_str(char *str, struct date_YMD_s *pymds, int size)
{
    int len=0, tot=0;
    char *dst=0;

    dst = str;

    sprintf(dst, "%d\n", size+1);

    len = strlen(dst);
    tot += len;
    dst += len;    

    for (int n = 0; n < (size+1); n++) {
        sprintf(dst, "%.4d %.2d %.2d\n", pymds[n].y, pymds[n].m, pymds[n].d);
        len = strlen(dst);
        tot += len;

        dst += len;
    }

    #if MEM_DUMP_EN
    mem_dump(str, tot);
    #endif

    return tot;
}

int file_to_rand(int *rand, FILE *f, int size)
{
    int len=0, ret=0;
    char *buf=0, *str;

    ret = fseek(f, 0, SEEK_END);
    if (ret) {
        printf("input seek end failed ret: %d\n", ret);
        return ret;
    }

    len = ftell(f);
    printf("file len of input file is %d \n", len);

    buf = malloc(len);

    ret = fseek(f, 0, SEEK_SET);
    if(ret) {
        printf("input seek begin failed ret: %d\n", ret);
        return ret;
    }

    ret = fread(buf, 1, len, f);
    printf("file read size %d ret: %d \n", len, ret);

    str = buf;

    for (int n = 0, ix = 0; n < size; n++) {
    
        rand[n] = atoi(str);

        //printf("%d \n", rand[n]);
        
        while(ix < len) {
            ix++;
            if (buf[ix] == '\n') {
                str = &buf[ix + 1];
                break;
            }
        }
    }

    
    return 0;
}

int file_to_ymds(struct date_YMD_s *pymds, FILE *f, int size)
{
    int len=0, ret=0, ix=0;
    char *buf=0, *str;
    struct date_YMD_s *ymd=0;
    int *ymdint=0;
    
    ret = fseek(f, 0, SEEK_END);
    if (ret) {
        printf("input seek end failed ret: %d\n", ret);
        return ret;
    }

    len = ftell(f);

    #if LOG_HELPER
    printf("file len of input file is %d \n", len);
    #endif

    buf = malloc(len);
    memset(buf, 0, len);

    ret = fseek(f, 0, SEEK_SET);
    if(ret) {
        printf("input seek begin failed ret: %d\n", ret);
        return ret;
    }

    ret = fread(buf, 1, len, f);
    #if LOG_HELPER
    printf("file read size %d ret: %d \n", len, ret);
    #endif

    #if MEM_DUMP_EN
    mem_dump(buf, len);
    #endif
    
    str = buf;
    
    #if LOG_HELPER
    int lenNum=0;
    lenNum = atoi(str);
    printf("number of line: %d size: %d \n", lenNum, size);
    #endif
    
    while(ix < len) {
        ix++;
        if ((buf[ix] == 0x20) || (buf[ix] == '\n')) {
            str = &buf[ix + 1];
            break;
        }
    }
    
    for (int n = 0; n < size; n++) {
        #if LOG_HELPER
        printf("%d \n", n);
        #endif
        ymd = &pymds[n];
        ymdint = &ymd->y;
   
        for (int m = 0; m < 3; m++) {
            *ymdint = atoi(str);
            #if LOG_HELPER
            printf("    %d.%d \n", m, *ymdint);
            #endif
            
            while(ix < len) {
                ix++;
                if ((buf[ix] == 0x20) || (buf[ix] == '\n')) {
                    str = &buf[ix + 1];
                    break;
                }
            }

            ymdint++;
        }
    }
    
    return 0;
}


