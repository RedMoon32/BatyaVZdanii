//
// Created by rinat on 21.04.19.
//

#ifndef PPM_SOBEL_PPM_H
#define PPM_SOBEL_PPM_H


extern int GX[3][3];
extern int GY[3][3];

#define MAX_NUMBER_OF_THREADS 16

struct grayscale_image {
    int width;
    int height;
    u_int8_t **matrix;
};


struct grayscale_image *convert_rgb_to_grayscale(struct ppm_image *image);

void convert_grayscale_to_rgb(struct ppm_image *f1, u_int8_t **gray);

struct grayscale_image *convert_to_sobel(struct grayscale_image *gr, int thread_count);

void free_grayscale_image(struct grayscale_image *image);

u_int8_t **allocate_double_matrix(int height, int width);

#endif