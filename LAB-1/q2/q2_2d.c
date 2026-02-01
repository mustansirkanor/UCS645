#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int N = 1000;
    double *A, *B, *BT, *C;
    double start, end;
    double t_seq_simple, t_seq_transpose;
    int threads[] = {2, 4, 8, 16, 32};
    int n_threads = sizeof(threads) / sizeof(threads[0]);

    A  = (double *)malloc(N * N * sizeof(double));
    B  = (double *)malloc(N * N * sizeof(double));
    BT = (double *)malloc(N * N * sizeof(double));
    C  = (double *)malloc(N * N * sizeof(double));
    if (!A || !B || !BT || !C) {
        printf("Matrix allocation failed\n");
        return 1;
    }

    for (int i = 0; i < N * N; i++) {
        A[i]  = 1.0;
        B[i]  = 2.0;
        C[i]  = 0.0;
        BT[i] = 0.0;
    }

    /* SIMPLE 2D VERSION (collapse)  */
    start = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
    end = omp_get_wtime();
    t_seq_simple = end - start;

    printf("========================================\n");
    printf("Matrix Multiply 2D  \n");
    printf("========================================\n");
    printf("Sequential time: %.6f seconds\n\n", t_seq_simple);
    printf("%-10s %-15s %-10s %-12s\n",
           "Threads", "Parallel(s)", "Speedup", "Efficiency");

    for (int t = 0; t < n_threads; t++) {
        int thread = threads[t];
        double t_par;

        for (int i = 0; i < N * N; i++) {
            C[i] = 0.0;
        }

        omp_set_num_threads(thread);
        start = omp_get_wtime();
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                double sum = 0.0;
                for (int k = 0; k < N; k++) {
                    sum += A[i * N + k] * B[k * N + j];
                }
                C[i * N + j] = sum;
            }
        }
        end = omp_get_wtime();
        t_par = end - start;

        double speedup = t_seq_simple / t_par;
        double efficiency = (speedup / thread) * 100.0;

        printf("%-10d %-15.6f %-10.2fx %-11.2f%%\n",
               thread, t_par, speedup, efficiency);
    }

    printf("========================================\n\n");

    /* RANSPOSED 2D VERSION */

    for (int i = 0; i < N * N; i++) {
        C[i] = 0.0;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            BT[j * N + i] = B[i * N + j];
        }
    }

    start = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * BT[j * N + k];
            }
            C[i * N + j] = sum;
        }
    }
    end = omp_get_wtime();
    t_seq_transpose = end - start;

    printf("========================================\n");
    printf("Matrix Multiply 2D (transpose)\n");
    printf("========================================\n");
    printf("Sequential time: %.6f seconds\n\n", t_seq_transpose);
    printf("%-10s %-15s %-10s %-12s\n",
           "Threads", "Parallel(s)", "Speedup", "Efficiency");
    printf("--------------------------------------------------\n");

    for (int t = 0; t < n_threads; t++) {
        int thread = threads[t];
        double t_par;

        for (int i = 0; i < N * N; i++) {
            C[i] = 0.0;
        }

        omp_set_num_threads(thread);
        start = omp_get_wtime();
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                double sum = 0.0;
                for (int k = 0; k < N; k++) {
                    sum += A[i * N + k] * BT[j * N + k];
                }
                C[i * N + j] = sum;
            }
        }
        end = omp_get_wtime();
        t_par = end - start;

        double speedup = t_seq_transpose / t_par;
        double efficiency = (speedup / thread) * 100.0;

        printf("%-10d %-15.6f %-10.2fx %-11.2f%%\n",
               thread, t_par, speedup, efficiency);
    }
    free(A);
    free(B);
    free(BT);
    free(C);
    return 0;
}

