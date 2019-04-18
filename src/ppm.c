//
// Created by rinat on 16.04.19.
//

#include "ppm.h"
#include "shared/log.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>

#define BUFF_SIZE 65536
int GX[3][3] = {{-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}};
int GY[3][3] = {{-1, -2, -1},
                {0,  0,  0},
                {1,  2,  1}};

/* Function allocates memory for width*height
 * matrix */
void allocate_matrix(struct ppm_image *im) {
    color8 ***arr = (color8 ***) malloc(im->height * sizeof(color8 **));
    for (int i = 0; i < im->height; i++) {
        arr[i] = (color8 **) malloc(im->width * sizeof(color8 *));
        for (int j = 0; j < im->width; j++) {
            arr[i][j] = (color8 *) malloc(sizeof(color8));
            memset(arr[i][j], 0, NUMBER_OF_COLORS);
        }
    }
    im->matrix = arr;
}

/* Function frees width*height*3 input matrix allocated in heap
 */
void free_matrix(struct ppm_image *im) {
    for (int i = 0; i < im->height; i++) {
        for (int j = 0; j < im->width; j++)
            free(im->matrix[i][j]);
        free(im->matrix[i]);
    }
    free(im->matrix);
}

/* Display each rgb in matrix */
void display_matrix(struct ppm_image *file) {
    for (int i = 0; i < file->height; i++) {
        for (int j = 0; j < file->width; j++) {
            printf(" [%d %d %d] ",
                   file->matrix[i][j]->r,
                   file->matrix[i][j]->g,
                   file->matrix[i][j]->b);
        }
        printf("\n");
    }
}

/* Functin sets color of pixel of input image to specified [r g b] image */
void set_color(struct ppm_image *file, int index, int r, int g, int b) {
    int row = ceil(index / file->width);
    int column = index % file->width;
    file->matrix[row][column]->r = r;
    file->matrix[row][column]->g = g;
    file->matrix[row][column]->b = b;
}

/* Function reads file specified by path and returns pointer to ppm_image
* struct */
struct ppm_image *read_ppm(char *filename) {
    FILE *source = fopen(filename, "r");
    if (source == NULL)
        return NULL;
    struct ppm_image *new_file = malloc(sizeof(struct ppm_image));
    fscanf(source, "%2s", new_file->type);
    if (strcasecmp(new_file->type, "P3") != 0) // More types can be added
    {
        free(new_file);
        return NULL;
    }
    fscanf(source, "%u %u", &new_file->width, &new_file->height);
    int index = 0;
    int r, g, b;
    allocate_matrix(new_file);
    fscanf(source, "%d", &new_file->max_color);
    while (fscanf(source, "%d %d %d", &r, &g, &b) == 3) {
        set_color(new_file, index, r, g, b);
        index++;
    }
    if (index != (new_file->width * new_file->height)) {
        log_warn("Number of retrieved pixels is lesser than in definition");
    }
    fclose(source);
    return new_file;
}

/*Function to save PPM image to some cpecified path*/
int save_ppm(struct ppm_image *file, char *file_path) {
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL)
        return -1;
    fprintf(fp, "%s\n", file->type);
    fprintf(fp, "%d %d\n", file->width, file->height);
    if (file->matrix == NULL)
        return -1;
    fprintf(fp, "%d\n", file->max_color);
    for (int i = 0; i < file->height; i++) {
        for (int j = 0; j < file->width; j++) {
            color8 *color = file->matrix[i][j];
            fprintf(fp, "%d %d %d ", color->r, color->g, color->b);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

/* Allocate double matrix of type uint8t with size height*width */
u_int8_t **allocate_double_matrix(int height, int width) {
    u_int8_t **arr = (u_int8_t **) malloc(height * sizeof(u_int8_t **));
    for (int i = 0; i < height; i++) {
        arr[i] = (u_int8_t *) malloc(width * sizeof(u_int8_t));
        memset(arr[i], 0, width * sizeof(u_int8_t));
    }
    return arr;
}

/*Function to convert rgb image to grayscale*/
struct grayscale_image *get_grayscale(struct ppm_image *image) {
    struct grayscale_image *gi = malloc(sizeof(struct grayscale_image));
    u_int8_t **arr = allocate_double_matrix(image->height, image->width);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            color8 *color = image->matrix[i][j];
            int grayscale = (color->r + color->g + color->b) / 3;
            arr[i][j] = grayscale;
        }
    }
    gi->matrix = arr;
    gi->height = image->height;
    gi->width = image->width;
    return gi;
}

/* Function applies Sobel Operator to input grayscale image and returns
 * new output grayscale image with edge detected */
struct grayscale_image *convert_to_sobel(struct grayscale_image *gr) {
    struct grayscale_image *new_im = malloc(sizeof(struct grayscale_image));
    u_int8_t **res = allocate_double_matrix(gr->height, gr->width);
    for (int i = 1; i < gr->height - 1; i++) {
        for (int j = 1; j < gr->width - 1; j++) {
            int s1 = 0, s2 = 0;
            for (int cur_r = 0; cur_r <= 2; cur_r++) {
                for (int cur_c = 0; cur_c <= 2; cur_c++) {
                    s1 += GX[cur_r][cur_c] * gr->matrix[cur_r + i - 1][cur_c + j - 1];
                    s2 += GY[cur_r][cur_c] * gr->matrix[cur_r + i - 1][cur_c + j - 1];
                }
            }
            res[i][j] = fmin(255, fmax(0, ceil(sqrt(s1 * s1 + s2 * s2))));
        }
    }
    new_im->matrix = res;
    new_im->width = gr->width;
    new_im->height = gr->height;
    return new_im;
}

/* Function converts each [i,j] pixel in f1 ppm image to pixel with gray[i][j] color */
void convert_to_grayscale(struct ppm_image *f1, u_int8_t **gray) {
    for (int i = 0; i < f1->width * f1->height; i++) {
        {
            int r = ceil(i / f1->width), c = i % f1->width;
            int av = gray[r][c];
            set_color(f1, i, av, av, av);
        }
    }

}