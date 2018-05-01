#pragma once

double** tools_read_data_file(
    char *path,                         // file path (es.:/home/luca/data.dat)
    int *rows,                          // pointer to int where save the number of rows
    int *cols                           // pointer to int where save the number of columns
    );                                  // return array of rows*cols data

int tools_write_data_file(
    char *path,                         // file path (es.:/home/luca/data.dat)
    int rows,                           // number of rows
    int cols,                           // number of columns
    double **data,                      // bi-dimensional array of double
    char sep,                           // separator (';', '\t')
    char *format                        // es "%3.2f"
    );                                  // return > 0 if all ok

double** tools_malloc_matrix(
    int rows,                           // number of rows
    int cols                            // number of columns
);                                       // return rows x columns bidimensional array