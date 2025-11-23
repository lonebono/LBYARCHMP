#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

extern void imgCvtGrayIntToDouble(uint8_t *pixel, double *doubles, size_t count);

int validateConversion(uint8_t *input, double *output, size_t pixelCount)
{
    const double tolerance = 1e-12;

    for (size_t idx = 0; idx < pixelCount; idx++)
    {
        double expectedValue = ((double)input[idx]) / 255.0;
        double actualValue = output[idx];

        if (fabs(expectedValue - actualValue) > tolerance)
        {
            return 0;
        }
    }

    return 1;
}

double benchmarkFunction(uint8_t *input, double *output, size_t count, int iterations)
{
    double totalElapsed = 0.0;

    for (int run = 0; run < iterations; run++)
    {
        clock_t startTime = clock();
        imgCvtGrayIntToDouble(input, output, count);
        clock_t endTime = clock();

        double elapsed = (double)(endTime - startTime) / CLOCKS_PER_SEC;
        totalElapsed += elapsed;
    }

    return totalElapsed / iterations;
}

void displaySampleOutput(double *data, int rows, int cols)
{
    printf("Output:\n");

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            int index = row * cols + col;
            printf("%.2f ", data[index]);
        }
        printf("\n");
    }
}

int main()
{

    const int sampleHeight = 3;
    const int sampleWidth = 4;
    const size_t sampleSize = sampleHeight * sampleWidth;

    uint8_t sampleInput[] = {
        64, 89, 114, 84,
        140, 166, 191, 84,
        216, 242, 38, 84};

    double sampleOutput[sampleSize];

    imgCvtGrayIntToDouble(sampleInput, sampleOutput, sampleSize);

    displaySampleOutput(sampleOutput, sampleHeight, sampleWidth);

    const int ITERATIONS = 30;
    int testSizes[][2] = {
        {10, 10},
        {100, 100},
        {1000, 1000}};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);

    srand((unsigned)time(NULL));

    for (int testIdx = 0; testIdx < numTests; testIdx++)
    {
        int testHeight = testSizes[testIdx][0];
        int testWidth = testSizes[testIdx][1];
        size_t totalPixels = testHeight * testWidth;

        uint8_t *testInput = (uint8_t *)malloc(totalPixels * sizeof(uint8_t));
        double *testOutput = (double *)malloc(totalPixels * sizeof(double));

        if (testInput == NULL || testOutput == NULL)
        {
            fprintf(stderr, "Memory allocation failed for %dx%d\n", testHeight, testWidth);
            return 1;
        }

        for (size_t i = 0; i < totalPixels; i++)
        {
            testInput[i] = rand() % 256;
        }

        double avgExecutionTime = benchmarkFunction(testInput, testOutput, totalPixels, ITERATIONS);

        int isCorrect = validateConversion(testInput, testOutput, totalPixels);

        printf("\n");
        printf("Size: %dx%d\n", testHeight, testWidth);
        printf("Average time: %.9fs\n", avgExecutionTime);
        printf("Run count: %d\n", ITERATIONS);
        printf("Correctness: %s\n", isCorrect ? "CORRECT" : "WRONG");

        free(testInput);
        free(testOutput);
    }

    return 0;
}
