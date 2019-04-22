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

typedef union {
    struct {
        u_int8_t r, g, b;
    };
    u_int8_t rgb[3];
} color8;

struct grayscale_image {
    int width;
    int height;
    u_int8_t **matrix;
};

struct ppm_image {
    char type[TYPE_SIZE];
    int width;
    int height;
    int max_color;
    color8 ***matrix;
};

enum netpbm_type {
    P1, P2, P3, P4, P5, P6
};

void allocate_matrix(struct ppm_image *im);

void free_matrix(struct ppm_image *im);

void free_ppm_image(struct ppm_image *image);

struct ppm_image *read_ppm(char *file_path);

int save_ppm(struct ppm_image *file, char *file_path, struct grayscale_image *gray);

void set_color(struct ppm_image *file, int index, int r, int g, int b);

#endif //PPM_CONVERTER_PPM_H
