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

#define TYPE_SIZE 3
#define NUMBER_OF_COLORS 3
#define P3 "P3"

typedef union {
    struct {
        u_int8_t r, g, b;
    };
    u_int8_t rgb[3];
} color8;


struct ppm_file {
    char type[TYPE_SIZE];
    int width;
    int height;
    int max_color;
    color8 ***matrix;
};


void allocate_matrix(struct ppm_file *im);

void free_matrix(struct ppm_file *im);

struct ppm_file *read_ppm(char *file_path);

int save_ppm(struct ppm_file *file, char *file_path);

void display_matrix(struct ppm_file *file);

void set_color(struct ppm_file *file, int index, int r, int g, int b);

int **get_grayscale(struct ppm_file* image);

void convert_to_sobel(struct ppm_file* image);

#endif //PPM_CONVERTER_PPM_H
