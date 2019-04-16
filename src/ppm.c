//
// Created by rinat on 16.04.19.
//

#include "ppm.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>

#define BUFF_SIZE 65536

/* Function allocates memory for width*height
 * matrix */
void allocate_matrix(struct ppm_file *im) {
    u_int8_t ***arr = (char ***) malloc(im->height * sizeof(u_int8_t **));
    for (int i = 0; i < im->height; i++) {
        arr[i] = (char **) malloc(im->width * sizeof(u_int8_t *));
        for (int j = 0; j < im->width; j++) {
            arr[i][j] = (u_int8_t *) malloc(NUMBER_OF_COLORS);
            memset(arr[i][j], 0, NUMBER_OF_COLORS);
        }
    }
    im->matrix = arr;
}

/* Function frees width*height*3 input matrix allocated in heap
 */
void free_matrix(struct ppm_file *im) {
    for (int i = 0; i < im->height; i++) {
        for (int j = 0; j < im->width; j++)
            free(im->matrix[i][j]);
        free(im->matrix[i]);
    }
    free(im->matrix);
}

/* Display each rgb in matrix */
void display_matrix(struct ppm_file *file) {
    for (int i = 0; i < file->height; i++) {
        for (int j = 0; j < file->width; j++) {
            printf(" [%d %d %d] ",
                   file->matrix[i][j][0],
                   file->matrix[i][j][1],
                   file->matrix[i][j][2]);
        }
        printf("\n");
    }
}

void set_color(struct ppm_file *file, int index, int  r, int g, int b) {
    int row = ceil(index / file->height);
    int column = index % file->width;
    file->matrix[row][column][0] = r;
    file->matrix[row][column][1] = g;
    file->matrix[row][column][2] = b;
}

/* Function reads file specified by path and returns pointer to ppm_file
* struct */
struct ppm_file *read_ppm(char *filename) {
    FILE *source = fopen(filename, "r");
    if (source == NULL)
        return NULL;
    struct ppm_file *new_file = malloc(sizeof(struct ppm_file));
    fscanf(source, "%2s", new_file->type);
    if (strcasecmp(new_file->type, "P3") != 0) // More types can be added
    {
        free(new_file);
        return NULL;
    }
    fscanf(source, "%u %u", &new_file->height, &new_file->width);
    int index = 0;
    int  r, g, b;
    allocate_matrix(new_file);
    fscanf(source,"%d",&new_file->max_color);
    while (fscanf(source, "%d %d %d", &r, &g, &b) == 3) {
        set_color(new_file, index, r, g, b);
        index++;
    }
    return new_file;
}
