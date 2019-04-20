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
#define MAX_NUMBER_OF_THREADS 16

typedef union {
    struct {
        u_int8_t r, g, b;
    };
    u_int8_t rgb[3];
} color8;


struct ppm_image {
    char type[TYPE_SIZE];
    int width;
    int height;
    int max_color;
    color8 ***matrix;
};

struct grayscale_image {
    int width;
    int height;
    u_int8_t **matrix;
};

extern int GX[3][3];
extern int GY[3][3];

void allocate_matrix(struct ppm_image *im);

void free_matrix(struct ppm_image *im);

struct ppm_image *read_ppm(char *file_path);

int save_ppm(struct ppm_image *file, char *file_path);

void display_matrix(struct ppm_image *file);

void set_color(struct ppm_image *file, int index, int r, int g, int b);

struct grayscale_image *get_grayscale(struct ppm_image *image);

struct grayscale_image *convert_to_sobel(struct grayscale_image *gr, int thread_count);

void convert_to_grayscale(struct ppm_image *f1, u_int8_t **gray);

void free_ppm_image(struct ppm_image *image);

void free_grayscale_image(struct grayscale_image *image);

#endif //PPM_CONVERTER_PPM_H
