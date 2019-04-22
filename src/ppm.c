//
// Created by rinat on 16.04.19.
//

#include "ppm.h"
#include "sobel.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

/* Function to convert string representation of format into a number
 * for fast number comparisons */
int get_itype(char* type){
    enum netpbm_type res = -1;
    if (strcmp(type, "P3") == 0)
        res = P3;
    else if (strcmp(type, "P6") == 0)
        res = P6;
    else if (strcmp(type, "P2") == 0)
        res = P2;
    else if (strcmp(type, "P5") == 0)
        res = P5;
    return res;
}

/* Function allocates memory for image (image.width * image.height * 3 size) */
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

/* Function frees memory allocated for image matrix*/
void free_matrix(struct ppm_image *im) {
    if (im->matrix == NULL)
        return;
    for (int i = 0; i < im->height; i++) {
        for (int j = 0; j < im->width; j++)
            //free every pixel
            free(im->matrix[i][j]);
        //free every row
        free(im->matrix[i]);
    }
    free(im->matrix);
    im->matrix = NULL;
}

/* Function frees memory used by ppm image */
void free_ppm_image(struct ppm_image *f1) {
    free_matrix(f1);
    free(f1);
}

/* Function sets the color of specified pixel to given [r g b]  */
void set_color(struct ppm_image *file, int index, int r, int g, int b) {
    int row = ceil(index / file->width);
    int column = index % file->width;
    file->matrix[row][column]->r = r;
    file->matrix[row][column]->g = g;
    file->matrix[row][column]->b = b;
}

/* Function reads ppm-file (P3 or P6 format) specified by path and returns pointer to ppm_image struct
 *
 * Returns NULL on error, or pointer to ppm image struct in case of success*/
struct ppm_image *read_ppm(char *filename) {
    FILE *source = fopen(filename, "r");
    if (source == NULL) {
        return NULL;
    }
    struct ppm_image *new_file = malloc(sizeof(struct ppm_image));
    fscanf(source, "%2s", new_file->type);

    enum netpbm_type image_format = get_itype(new_file->type);
    if (image_format == -1)
        return NULL;
    fscanf(source, "%u %u", &new_file->width, &new_file->height);
    allocate_matrix(new_file);
    fscanf(source, "%d", &new_file->max_color);

    for (int i = 0; i < new_file->height; i++) {
        for (int j = 0; j < new_file->width; j++) {
            if (image_format == P6) {
                fread(new_file->matrix[i][j]->rgb, sizeof(u_int8_t), 3, source);
            } else if (image_format == P3) {
                u_int8_t *c = new_file->matrix[i][j]->rgb;
                fscanf(source, "%d %d %d", c, c + 1, c + 2);
            }
        }
    }
    fclose(source);
    return new_file;
}


/*Function to save PPM image (in P3 or P6 format) to some specified path
 * Gray - pointer to grayscale image (for saving file in P2 and P5 formats)
 * Returns -1 on error, 0 on success
 */
int save_ppm(struct ppm_image *file, char *file_path, struct grayscale_image *gray) {
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL || file->matrix == NULL) {
        return -1;
    }
    fprintf(fp, "%s\n", file->type);
    fprintf(fp, "%d %d\n%d\n", file->width, file->height, file->max_color);
    enum netpbm_type image_format = get_itype(file->type);
    for (int i = 0; i < file->height; i++) {
        for (int j = 0; j < file->width; j++) {
            color8 *color = file->matrix[i][j];
            if (image_format == P3)
                fprintf(fp, "%d %d %d ", color->r, color->g, color->b);
            else if (image_format == P6)
                fprintf(fp, "%c%c%c", color->r, color->g, color->b);
            else if (image_format == P2)
                fprintf(fp, "%d ", gray->matrix[i][j]);
            else if (image_format == P5)
                fprintf(fp, "%c",  gray->matrix[i][j]);
        }
        if (image_format == P3)
            fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}
