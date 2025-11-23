#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

extern void imgCvtGrayIntToDouble(uint8_t *pixel, double *doubles, size_t count);

// measures average execution time
static double measureTime(uint8_t *pixel, double *doubles, size_t n, int runs)
{
    double total = 0.0;
    for (int i = 0; i < runs; ++i)
    {
        clock_t t0 = clock();
        imgCvtGrayIntToDouble(pixel, doubles, n);
        clock_t t1 = clock();
        total += (double)(t1 - t0) / CLOCKS_PER_SEC;
    }
    return total / runs;
}

// check correctness of  conversion against expected vals
static int checker(uint8_t *pixel, double *doubles, size_t n)
{
    const double eps = 1e-12;
    for (size_t i = 0; i < n; ++i)
    {
        double ref = ((double)pixel[i]) / 255.0;
        if (fabs(ref - doubles[i]) > eps)
            return 0;
    }
    return 1;
}

// main function
int main(void)
{
    // convert and print sample output
    const int h = 3, w = 4;
    uint8_t pixel[] = {64, 89, 114, 84, 140, 166, 191, 84, 216, 242, 38, 84};
    double doubles[h * w];

    imgCvtGrayIntToDouble(pixel, doubles, h * w);
    printf("Sample output:\n");
    for (int r = 0; r < h; r++)
    {
        for (int c = 0; c < w; c++)
            printf("%.2f ", doubles[r * w + c]);
        printf("\n");
    }

    // randomized input and output with different sizes
    int runs = 30;
    size_t sizes[3][2] = {{10, 10}, {100, 100}, {1000, 1000}};
    srand((unsigned)time(NULL));

    for (int t = 0; t < 3; t++)
    {
        size_t H = sizes[t][0];
        size_t W = sizes[t][1];
        size_t N = H * W;

        // allocate memory
        uint8_t *randomPixel = malloc(N);
        double *convertedVal = malloc(N * sizeof(double));
        if (!randomPixel || !convertedVal)
        {
            fprintf(stderr, "Memory allocation failed for %zux%zu\n", H, W);
            return 1;
        }

        // initialize  with random  values
        for (size_t i = 0; i < N; i++)
            randomPixel[i] = rand() % 256;

        double avgTime = measureTime(randomPixel, convertedVal, N, runs);

        printf("\n");
        printf("Size: %zux%zu\n", H, W, N);
        printf("Average time: %.9fs\n", avgTime);
        printf("Run count: %d\n", runs);
        printf("Correctness: %s\n", checker(randomPixel, convertedVal, N) ? "CORRECT" : "WRONG");

        free(randomPixel);
        free(convertedVal);
    }

    return 0;
}
