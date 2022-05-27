#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

double getRandomNumber(unsigned int *seed) {
    return (double) rand_r(seed) * 2 / (double) (RAND_MAX) - 1;
}

long double SequentialPi(long long iterations) {
    long long numberInCircle = 0;
    unsigned int seed = (unsigned int) time(NULL);

    for (long long int i = 0; i < iterations; i++) {
        double x = getRandomNumber(&seed);
        double y = getRandomNumber(&seed);
        double distanceSquared = x*x + y*y;

        if (distanceSquared <= 1)
            numberInCircle++;
    }

    return 4 *(double) numberInCircle / ((double) iterations);
}

long double ParallelPi(long long iterations) {
    long long numberInCircle = 0;
#pragma omp parallel num_threads(4)
    {
        /* генеруємо зерно для кожного потоку різним */
        unsigned int seed = (unsigned int) time(NULL) + (unsigned int) omp_get_thread_num();
#pragma omp for reduction(+: numberInCircle)
        for (long long int i = 0; i < iterations; i++) {
            double x = getRandomNumber(&seed);
            double y = getRandomNumber(&seed);
            double distanceSquared = x*x + y*y;

            if (distanceSquared <= 1)
                numberInCircle++;
        }
    }
    return 4 * (double) numberInCircle/((double) iterations);
}

// gcc-11 -fopenmp monteCarlo.c -o monteCarlo
// ./monteCarlo

int main() {
    struct timeval start, end;

    long long iterations = 1000000000;

    printf("\nПослідовне обчислення: \n");
    gettimeofday(&start, NULL);
    long double sequentialPi = SequentialPi(iterations);
    gettimeofday(&end, NULL);
    printf("Час виконання %f секунд\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

//    printf("\nПаралельне обчислення: \n");
//    gettimeofday(&start, NULL);
//    long double parallelPi = ParallelPi(iterations);
//    gettimeofday(&end, NULL);
//    printf("Час виконання %f секунд\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    printf("Послідовне: Pi = %.10Lf \n", sequentialPi);
//    printf("Паралельне: Pi = %.10Lf \n", parallelPi);

    return 0;
}