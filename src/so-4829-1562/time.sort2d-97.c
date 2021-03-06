/* SO 4829-1562 */

#include "time.sort2d-31.h"
#include "emalloc.h"

/*
**  + Enter a square matrix of dimensions n .
**  + Elements below main diagonal sort in ascending order.
**  + Elements above main diagonal sort in descending order.
**  + Elements on main diagonal sort :
**    - first even numbers in ascending order.
**    - then odd numbers in descending order.
*/

/* Variation 4: Use system qsort() and common code to coordinate sorting of triangles */
/* Avoids two matrices lt and ut, thereby reducing the extra data space needed. */

static inline int cmp_asc(int x, int y) { return (x > y) - (x < y); }
static inline int cmp_dsc(int x, int y) { return (x < y) - (x > y); }
static inline int cmp_eaod(int x, int y)
{
    int px = x & 1;
    int py = y & 1;
    if (px != py)
        return px - py;
    if (px == 1)
        return cmp_dsc(x, y);
    return cmp_asc(x, y);
}

static int qs_cmp_int_asc(const void *v1, const void *v2)
{
    int i1 = *(const int *)v1;
    int i2 = *(const int *)v2;
    return cmp_asc(i1, i2);
}

static int qs_cmp_int_dsc(const void *v1, const void *v2)
{
    int i1 = *(const int *)v1;
    int i2 = *(const int *)v2;
    return cmp_dsc(i1, i2);
}

static int qs_cmp_int_eaod(const void *v1, const void *v2)
{
    int i1 = *(const int *)v1;
    int i2 = *(const int *)v2;
    return cmp_eaod(i1, i2);
}

static void sort_diagonal(size_t n, int matrix[n][n])
{
    int data[n];
    for (size_t i = 0; i < n; i++)
        data[i] = matrix[i][i];
    qsort(data, n, sizeof(data[0]), qs_cmp_int_eaod);
    for (size_t i = 0; i < n; i++)
        matrix[i][i] = data[i];
}

/*
**   D0  U0  U1  U2  U3
**   L0  D1  U4  U5  U6
**   L1  L2  D3  U7  U8
**   L3  L4  L5  D4  U9
**   L6  L7  L8  L9  D5
**
** D0 = (0, 0); U0 = (0, 1); U1 = (0, 2); U2 = (0, 3); U3 = (0, 4);
** L0 = (1, 0); D1 = (1, 1); U4 = (1, 2); U5 = (1, 3); U6 = (1, 4);
** L1 = (2, 0); L2 = (2, 1); D2 = (2, 2); U7 = (2, 3); U8 = (2, 4);
** L3 = (3, 0); L4 = (3, 1); L5 = (3, 2); D3 = (3, 3); U9 = (3, 4);
** L6 = (4, 0); L7 = (4, 1); L8 = (4, 2); L9 = (4, 3); D4 = (4, 4); 
*/

typedef void (*Copier)(int *val1, int *val2);

static void copy_a_to_b(int *val1, int *val2) { *val2 = *val1; }
static void copy_b_to_a(int *val1, int *val2) { *val1 = *val2; }

static void copy_lt_data(size_t n, int matrix[n][n], int vector[], Copier copy)
{
    size_t m = (n * (n - 1)) / 2;
    size_t r = 1;
    size_t c = 0;
    for (size_t i = 0; i < m; i++)
    {
        (*copy)(&matrix[r][c++], &vector[i]);
        if (c == r)
        {
            r++;
            c = 0;
        }
    }
}

static void copy_ut_data(size_t n, int matrix[n][n], int vector[], Copier copy)
{
    size_t m = (n * (n - 1)) / 2;
    size_t r = 0;
    size_t c = 0;
    for (size_t i = 0; i < m; i++)
    {
        (*copy)(&matrix[r][++c], &vector[i]);
        if (c == n - 1)
        {
            r++;
            c = r;
        }
    }
}

typedef void (*Mapper)(size_t n, int matrix[n][n], int vector[], Copier copy);
typedef int (*Comparator)(const void *v1, const void *v2);

static void sort_triangle(size_t n, int matrix[n][n], Mapper mapper, Comparator cmp)
{
    size_t m = (n * (n - 1)) / 2;
    int *data = MALLOC(m * sizeof(*data));

    (*mapper)(n, matrix, data, copy_a_to_b);
    qsort(data, m, sizeof(data[0]), cmp);
    (*mapper)(n, matrix, data, copy_b_to_a);

    FREE(data);
}

void quick_sort(size_t n, int matrix[n][n])
{
    sort_diagonal(n, matrix);
    sort_triangle(n, matrix, copy_lt_data, qs_cmp_int_asc);
    sort_triangle(n, matrix, copy_ut_data, qs_cmp_int_dsc);
}
