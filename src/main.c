//
// Created by rinat on 21.04.19.
//

#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"
#include "sobel.h"
#include <unistd.h>
#include <sys/time.h>
#include <time.h>



typedef struct timeval wallclock_t;

void wallclock_mark(wallclock_t *const tptr) {
    gettimeofday(tptr, NULL);
}

double wallclock_since(wallclock_t *const tptr) {
    struct timeval now;
    gettimeofday(&now, NULL);

    return difftime(now.tv_sec, tptr->tv_sec)
           + ((double) now.tv_usec - (double) tptr->tv_usec) / 1000000.0;
}


void process_image(char *input_file, char *output_file, char *save_mode, int thread_count) {
    printf("[==== NETPBM image processor ====] \n\n");
    printf("Input file path: %s \nOutput file path: %s \nSave mode: %s\nThread count: %d\n",
           input_file, output_file, save_mode,
           thread_count);
    wallclock_t t, sobel_time;
    wallclock_mark(&t);
    double wtime, stime;
    printf("\n=============\n");
    printf("[log] Opening the %s image...\n", input_file);
    struct ppm_image *p = read_ppm(input_file);
    if (p == NULL) {
        printf("[ERROR] Error in reading image");
        return;
    }
    printf("[log] Image successfully read\n[log] Converting rgb image to grayscale...\n");
    struct grayscale_image *gray = get_grayscale(p);
    wallclock_mark(&sobel_time);
    printf("[log] Applying sobel operator...\n");
    struct grayscale_image *new_gray = convert_to_sobel(gray, thread_count);
    stime = wallclock_since(&sobel_time);
    printf("[log] Converting grayscale image back to rgb representation...\n");
    convert_to_grayscale(p, new_gray->matrix);
    strcpy(p->type, save_mode);
    printf("[log] Saving image...\n");
    int s = save_ppm(p, output_file);
    if (s != -1)
        printf("[log] Image successfully saved to %s\n", output_file);
    else {
        printf("[ERROR] Error in saving image\n");
        return;
    }
    free_ppm_image(p);
    free_grayscale_image(gray);
    free_grayscale_image(new_gray);

    wtime = wallclock_since(&t);
    printf("=============\n\n");
    printf("Time of applying Sobel operator to image : %f\n", stime);
    printf("Overall running time of a program : %f\n", wtime);
}


int main(int argc, char *argv[]) {
    int opt;
    char *input_file = NULL,
            *output_file = NULL,
            mode[TYPE_SIZE];
    int thread_count = 2;
    while ((opt = getopt(argc, argv, "i:o:p:t:")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'p':
                strcpy(mode, optarg);
                if (strcmp(mode, "P3") != 0 &&
                    strcmp(mode, "P6") != 0) {
                    printf("[ERROR] Mode is not known\n");
                }
                break;
            case 't':
                thread_count = atoi(optarg);
                if (thread_count <= 0) {
                    printf("[ERROR] Thread count must be greater than 0\n");
                    return -1;
                }
        }
    }
    if (input_file == NULL) {
        printf("[ERROR] No input file provided\n");
        return -1;
    }
    if (output_file == NULL) {
        printf("[ERROR] No output file provided\n");
        return -1;
    }
    process_image(input_file, output_file, mode, thread_count);
}