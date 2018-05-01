#include "tools.h"
#include <stdlib.h>		// malloc
#include <stdio.h>      // FILE, fopen, fclose, fseek, ftell, rewind

double **tools_read_data_file(char *path, int *rows, int *cols)
{
    FILE *fp;
    fp = fopen(path, "r");

    if(fp == NULL)
        return NULL;

    void tools_count_rows_and_cols(FILE *, int *, int *);
    tools_count_rows_and_cols(fp, rows, cols);

    // fseek(fp, 0, SEEK_END);
    // long fsize = ftell(fp);        
    // rewind(fp);

    int ch = 0;
    int rowscount = *rows;
    int colscount = *cols;
    int row = 0;
    int col = 0;
    char buffer[255] = { '0' };
    double *(*data);

    data = tools_malloc_matrix(rowscount, colscount);
   
    while(ch != EOF)
    {

        int i = 0;
        while((ch = fgetc(fp)) != ';' && ch != '\t' && ch != '\n' && ch != '#'  && ch != ' ' && ch != EOF && i < 255) 
        { 
            buffer[i++] = ch;
        }         
        
        if(i > 0)
        {
            buffer[i++] = '\0'; 
            double n = atof(buffer);
            data[row][col] = n;
            col++;
        }

        if(ch == '#')
        {
            while((ch = fgetc(fp)) != '\n' && ch != EOF) 
                ;                         
        }

        if((ch == '\n' || ch == EOF) && col > 0)
        {
            row++;      
            col = 0;       
        }
    }
    
    fclose(fp);

    return data;
}

void tools_count_rows_and_cols(FILE *fp, int *rows, int *cols)
{
    int ch = 0;
    int rowscount = 0;
    int colscount = 0;
    int col = 0;
   
    while(ch != EOF)
    {

        int i = 0;
        while((ch = fgetc(fp)) != ';' && ch != '\t' && ch != '\n' && ch != '#'  && ch != ' ' && ch != EOF && i < 255) 
        { 
            i++;
        }         
        
        if(i > 0)
        {
            col++;
        }

        if(ch == '#')
        {
            while((ch = fgetc(fp)) != '\n' && ch != EOF) 
                ;                         
        }

        if((ch == '\n' || ch == EOF) && col > 0)
        {
            rowscount++;      
            if(col > colscount)
                colscount = col;   
            col = 0;       
        }
    }

    *rows = rowscount;
    *cols = colscount;

    rewind(fp);
}

int tools_write_data_file(char *path, int rows, int cols, double **data, char sep, char *format)
{

    FILE *fp;
    fp = fopen(path, "w");

    if(fp == NULL)
        return 0;

    fprintf(fp, "# Neural Tool Write Data File\n");
    fprintf(fp, "# Data file created with %d rows and %d columns\n", rows, cols);
    fprintf(fp, "\n");


    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            fprintf(fp, format, data[r][c]);
            if(c < (cols -1))
                putc(sep, fp);
            else
                putc('\n', fp);
        }
    }

    fclose(fp);

    return 1;
}          

double** tools_malloc_matrix(int rows, int cols)
{
    double *(*data);

    data = malloc(rows * sizeof(*data));

    if(data == NULL)
        return NULL;

    for(int r = 0; r < rows; r++)
    {
        data[r] = malloc(cols * sizeof(*data[r]));
        if(data[r] == NULL)
            return NULL;
    }
    
    return data;
}
