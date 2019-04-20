//
// Created by rinat on 16.04.19.
//

#include <stdio.h>
#include <assert.h>
#include <ppm.h>
#include <time.h>

void test_malloc_matrix() {
    struct ppm_image f1 = {.height = 5, .width = 10};
    allocate_matrix(&f1);
    for (int i = 0; i < f1.height; i++) {
        for (int j = 0; j < f1.width; j++) {
            for (int c = 0; c < 3; c++) {
                assert(f1.matrix[i][j]->rgb[c] == 0);
            }
        }
    }
}

void test_free_matrix() {
    struct ppm_image f1 = {.height = 5, .width = 5};
    allocate_matrix(&f1);
    free_matrix(&f1);
    assert(f1.matrix == NULL);
}

void test_read_file() {
    struct ppm_image *p = read_ppm("../tests/test.ppm");
    display_matrix(p);
    assert(p->width == 2 & p->height == 2);
    assert(p->max_color == 255);
    assert(p->matrix[0][0]->r == 100);
    assert(p->matrix[1][1]->g == 230);
    free_ppm_image(p);
}

void test_read_and_write_file() {
    struct ppm_image f1 = {.height = 2, .width = 4, .type = "P3", .matrix = NULL, .max_color = 255};
    allocate_matrix(&f1);
    for (int i = 0; i < f1.width * f1.height; i++)
        set_color(&f1, i, 100, 50, 200);
    save_ppm(&f1, "../tests/res.ppm");
    struct ppm_image *f2 = read_ppm("../tests/res.ppm");
    assert(f2->max_color == f1.max_color);
    assert(f2->height == f1.height);
    assert(f2->width == f1.width);
    for (int i = 0; i < f1.height; i++)
        for (int j = 0; j < f1.width; j++)
            for (int c = 0; c < NUMBER_OF_COLORS; c++)
                assert(f2->matrix[i][j]->rgb[c] == f1.matrix[i][j]->rgb[c]);
    free_matrix(&f1);
    free_ppm_image(f2);
}

void test_get_grayscale() {
    struct ppm_image f1 = {.height = 2, .width = 2, .type = "P3", .matrix = NULL, .max_color = 255};
    allocate_matrix(&f1);
    for (int i = 0; i < f1.height * f1.width; i++)
        set_color(&f1, i, 100, 50, 200);
    int av = (100 + 50 + 200) / 3;
    struct grayscale_image *grayscale = get_grayscale(&f1);
    for (int i = 0; i < f1.height; i++) {
        for (int j = 0; j < f1.width; j++)
            assert(grayscale->matrix[i][j] == av);
    }
    free_matrix(&f1);
    free_grayscale_image(grayscale);
}

void test_grayscale_from_image() {
    struct ppm_image *f1 = read_ppm("../tests/test.ppm");
    convert_to_grayscale(f1, get_grayscale(f1)->matrix);
    save_ppm(f1, "../tests/res.ppm");
    free_ppm_image(f1);
}

void run_sobel_from_grayscale(char *path) {
    struct ppm_image *f1 = read_ppm(path);
    struct grayscale_image *gray = get_grayscale(f1);
    struct grayscale_image *new_gray = convert_to_sobel(gray, 2);
    convert_to_grayscale(f1, new_gray->matrix);
    save_ppm(f1, "../tests/result.ppm");
    free_ppm_image(f1);
    free_grayscale_image(gray);
    free_grayscale_image(new_gray);
}

void benchmark_1() {
    struct ppm_image f1 = {.width = 1000, .height = 100000, .matrix = NULL, .max_color = 255};
    allocate_matrix(&f1);
    struct grayscale_image *gray = get_grayscale(&f1);
    time_t begin = time(NULL);

    struct grayscale_image *new_gray = convert_to_sobel(gray, 1);
    time_t end = time(NULL);
    printf("Time spent on converting image with 1 thread %d\n", (end - begin));
    free_grayscale_image(new_gray);
    begin = time(NULL);
    new_gray = convert_to_sobel(gray, 4);
    end = time(NULL);
    printf("Time spent on converting image with 4 thread %d\n", (end - begin));
    free_matrix(&f1);
    free_grayscale_image(new_gray);
}

int main() {
    run_sobel_from_grayscale("../tests/engine2.ppm");
    test_malloc_matrix();
    test_free_matrix();
    test_read_file();
    test_read_and_write_file();
    test_get_grayscale();
    test_grayscale_from_image();
    printf("Success tests\n");
    printf("Running benchmark ...\n");
    //benchmark_1();
}
