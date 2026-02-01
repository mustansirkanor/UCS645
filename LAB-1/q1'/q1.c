#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n = 1 << 20;
    double a = 2.5;
    double *x, *y;
    double start, end;
    double t_seq;
    int threads[] = {2, 4, 8, 16, 32};
    int n_threads = sizeof(threads) / sizeof(threads[0]);

    x = (double *)malloc(n * sizeof(double));
    y = (double *)malloc(n * sizeof(double));
    if (!x || !y) {
        printf("Array allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        x[i] = 1.0;
        y[i] = 2.0;
    }

    start = omp_get_wtime();
    for (int i = 0; i < n; i++) {
        x[i] = x[i] * a + y[i];
    }
    end = omp_get_wtime();
    t_seq = end - start;

    printf("========================================\n");
    printf("DAXPY Performance Analysis (n = %d)\n", n);
    printf("========================================\n");
    printf("Sequential time: %.6f seconds\n\n", t_seq);
    printf("%-10s %-15s %-10s %-12s\n",
           "Threads", "Parallel(s)", "Speedup", "Efficiency");
    printf("--------------------------------------------------\n");

    for (int t = 0; t < n_threads; t++) {
        int thread = threads[t];
        double t_par;

        for (int i = 0; i < n; i++) {
            x[i] = 1.0;
            y[i] = 2.0;
        }

        omp_set_num_threads(thread);
        start = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            x[i] = x[i] * a + y[i];
        }
        end = omp_get_wtime();
        t_par = end - start;

        double speedup = t_seq / t_par;
        double efficiency = (speedup / thread) * 100.0;

        printf("%-10d %-15.6f %-10.2fx %-11.2f%%\n",
               thread, t_par, speedup, efficiency);
    }

    free(x);
    free(y);
    return 0;
}

