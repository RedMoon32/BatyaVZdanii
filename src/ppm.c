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


/* Function allocates memory for width*height
 * matrix */
void allocate_matrix(struct ppm_file *im) {
    color8 ***arr = (color8 ***) malloc(im->height * sizeof(color8 **));
    for (int i = 0; i < im->height; i++) {
        arr[i] = (char **) malloc(im->width * sizeof(color8 *));
        for (int j = 0; j < im->width; j++) {
            arr[i][j] = (color8 *) malloc(sizeof(color8));
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
                   file->matrix[i][j]->r,
                   file->matrix[i][j]->g,
                   file->matrix[i][j]->b);
        }
        printf("\n");
    }
}

void set_color(struct ppm_file *file, int index, int r, int g, int b) {
    int row = ceil(index / file->height);
    int column = index % file->width;
    file->matrix[row][column]->r = r;
    file->matrix[row][column]->g = g;
    file->matrix[row][column]->b = b;
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
    int r, g, b;
    allocate_matrix(new_file);
    fscanf(source, "%d", &new_file->max_color);
    while (fscanf(source, "%d %d %d", &r, &g, &b) == 3) {
        set_color(new_file, index, r, g, b);
        index++;
    }
    if (index != (new_file->width * new_file->height - 1)) {
        log_warn("Number of retrieved pixels is lesser than in definition");
    }
    return new_file;
}

/*Function to save PPM image to some path*/
int save_ppm(struct ppm_file *file, char *file_path) {
//    FILE *fp = fopen(file_path,"w");
//    if (fp == NULL)
//        return -1;
//    fwrite(file->type,)
//    char buff[1024]
}
