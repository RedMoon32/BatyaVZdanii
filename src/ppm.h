//
// Created by rinat on 16.04.19.
//

#ifndef PPM_CONVERTER_PPM_H
#define PPM_CONVERTER_PPM_H

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>

#define TYPE_SIZE 2
#define NUMBER_OF_COLORS 3
#define P3 "P3"


struct ppm_file {
    char type[TYPE_SIZE];
    int width;
    int height;
    int max_color;
    u_int8_t ***matrix;
};


void allocate_matrix(struct ppm_file *im);

void free_matrix(struct ppm_file *im);

struct ppm_file *read_ppm(char *filename);

void display_matrix(struct ppm_file *file);
#endif //PPM_CONVERTER_PPM_H
