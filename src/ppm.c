//
// Created by rinat on 16.04.19.
//

#include "ppm.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <pthread.h>


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
            free(im->matrix[i][j]);
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
    if (strcmp(new_file->type, "P6") != 0 &&
        strcmp(new_file->type, "P3") != 0) // More types can be added
    {
        free(new_file);
        return NULL;
    }

    fscanf(source, "%u %u", &new_file->width, &new_file->height);
    int index = 0;
    allocate_matrix(new_file);
    fscanf(source, "%d", &new_file->max_color);
    int r, g, b;
    if (strcmp(new_file->type, "P3") == 0) {
        while (fscanf(source, "%d %d %d", &r, &g, &b) == 3) {
            set_color(new_file, index, r, g, b);
            index++;
        }
    } else if (strcmp(new_file->type, "P6") == 0) {
        for (int i = 0; i < new_file->height; i++) {
            for (int j = 0; j < new_file->width; j++) {
                int read = fread(new_file->matrix[i][j]->rgb, sizeof(u_int8_t), 3, source);
                if (read < 3) {
                    return NULL;
                }
            }
        }
    }
    fclose(source);
    return new_file;
}

/*Function to save PPM image (in P3 or P6 format) to some specified path
 *
 * Returns -1 on error, 0 on success
 */
int save_ppm(struct ppm_image *file, char *file_path) {
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL) {
        return -1;
    }
    fprintf(fp, "%s\n", file->type);
    fprintf(fp, "%d %d\n", file->width, file->height);
    if (file->matrix == NULL)
        return -1;
    fprintf(fp, "%d\n", file->max_color);
    for (int i = 0; i < file->height; i++) {
        for (int j = 0; j < file->width; j++) {
            color8 *color = file->matrix[i][j];
            if (strcmp(file->type, "P3") == 0)
                fprintf(fp, "%d %d %d ", color->r, color->g, color->b);
            else if (strcmp(file->type, "P6") == 0)
                fwrite(&color->rgb, 3, 1, fp);
        }
        if (strcmp(file->type, "P3") == 0)
            fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}
