#include "test.h"
#include "tools.h"


DEFINE_TEST(test_read_write_data_file)
{
    int rows;
    int cols;
    double **data;
    data = tools_read_data_file("/home/maddalena/Source/Repos/neural/tests/foo.sample", &rows, & cols);

    if(data == NULL)
        perror("Error ");
    
    test_assert(data != NULL, "data is NULL");
    test_assert(rows == 7, "invalid rows value");
    test_assert(cols == 6, "invalid cols value");

    double val = 0.0;

    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            val = (r + 1.0) + (c + 1.0) / 10.0;
            test_assert(data[r][c] == val, "invalid value");
        }
    }

    int ret = tools_write_data_file("/home/maddalena/Source/Repos/neural/tests/foo2.sample", rows, cols, data, '\t', "%3.2f");
    test_assert(ret > 0, "Failed write data file");

    return NULL;
}

DEFINE_TEST(all_tests)
{
    test_run(test_read_write_data_file);

    return NULL;
}

TEST_SUITE(all_tests);