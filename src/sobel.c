#include <ppm.h>
#include <sobel.h>
#include <shared/log.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <pthread.h>


struct ptargs {
    u_int8_t **res;
    struct grayscale_image *gr;
    int start_row, end_row;
};

int GX[3][3] = {{-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}};
int GY[3][3] = {{-1, -2, -1},
                {0,  0,  0},
                {1,  2,  1}};

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
    log_info("Converting to grayscale ...");
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

void *calc_part(void *arg) {
    struct ptargs *args = (struct ptargs *) arg;
    int end_row = args->end_row,
            end_col = args->gr->width-1,
            start_col = 1,
            start_row = args->start_row;
    end_row = fmin(end_row, args->gr->height - 1);
    end_col = fmin(end_col, args->gr->width - 1);
    u_int8_t **matrx = args->gr->matrix;
    for (int i = start_row; i < end_row; i++) {
        for (int j = start_col; j < end_col; j++) {
            int s1 = 0, s2 = 0;
            for (int cur_r = 0; cur_r <= 2; cur_r++) {
                for (int cur_c = 0; cur_c <= 2; cur_c++) {
                    s1 += GX[cur_r][cur_c] * matrx[cur_r + i - 1][cur_c + j - 1];
                    s2 += GY[cur_r][cur_c] * matrx[cur_r + i - 1][cur_c + j - 1];
                }
            }
            args->res[i][j] = fmin(255, fmax(0, ceil(sqrt(s1 * s1 + s2 * s2))));
        }
    }
}

/* Function applies Sobel Operator to input grayscale image and returns
 * new output grayscale image with edge detected */
struct grayscale_image *convert_to_sobel(struct grayscale_image *gr, int thread_count) {
    struct grayscale_image *new_im = malloc(sizeof(struct grayscale_image));
    int rows = gr->height, columns = gr->width;
    u_int8_t **res = allocate_double_matrix(rows, columns);
    int rnext = 1;
    thread_count = fmax(thread_count, 1);
    if (thread_count > MAX_NUMBER_OF_THREADS) {
        log_warn("Too many number of threads -> was limited to 16");
        thread_count = MAX_NUMBER_OF_THREADS;
    }
    pthread_t row_threads[thread_count];
    struct ptargs data[thread_count];
    for (int l = 0; l < thread_count; l++) {
        data[l].start_row = rnext;
        data[l].end_row = rnext + ceil(rows / thread_count);
        data[l].gr = gr;
        data[l].res = res;
        pthread_create(&row_threads[l], NULL, calc_part, (void *) &data[l]);
        rnext = rnext + ceil(rows / thread_count);
    }
    log_info("All threads are running");
    for (int i = 0; i < thread_count; i++)
        pthread_join(row_threads[i], NULL);
    new_im->matrix = res;
    new_im->width = gr->width;
    new_im->height = gr->height;
    return new_im;
}


/* Function frees memory used by grayscale image */
void free_grayscale_image(struct grayscale_image *image) {
    if (image->matrix != NULL) {
        for (int i = 0; i < image->height; i++) {
            free(image->matrix[i]);
        }
        free(image->matrix);
        image->matrix = NULL;
    }
    free(image);
}