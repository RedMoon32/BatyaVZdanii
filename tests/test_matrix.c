//
// Created by rinat on 16.04.19.
//

#include <stdio.h>
#include <assert.h>
#include <ppm.h>

void test_malloc_matrix() {
    struct ppm_file f1 = {.height = 5, .width = 10};
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
    struct ppm_file f1 = {.height = 5, .width = 5};
    allocate_matrix(&f1);
    free_matrix(&f1);
}

void test_read_file() {
    struct ppm_file *p = read_ppm("../tests/test.ppm");
    assert(p->width == 2 & p->height == 2);
    assert(p->max_color == 255);
    assert(p->matrix[0][0]->r == 100);
    assert(p->matrix[1][1]->g == 230);
}

void test_read_and_write_file(){
    struct ppm_file f1 = {.height = 4, .width = 2, .type = "P3", .matrix = NULL, .max_color = 255};
    allocate_matrix(&f1);
    for (int i = 0;i<f1.width*f1.height;i++)
        set_color(&f1,i,100,50,200);
    save_ppm(&f1,"../tests/res.ppm");
    struct ppm_file* f2 = read_ppm("../tests/res.ppm");
    assert(f2->max_color == f1.max_color);
    assert(f2->height == f1.height);
    assert(f2->width == f1.width);
    for (int i = 0;i<f1.height;i++)
        for (int j=0;j<f1.width;j++)
            for (int c=0;c<NUMBER_OF_COLORS;c++)
                assert(f2->matrix[i][j]->rgb[c]==f1.matrix[i][j]->rgb[c]);
}

int main() {
    test_malloc_matrix();
    test_free_matrix();
    test_read_file();
    test_read_and_write_file();
    printf("Success tests\n");
}
