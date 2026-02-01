#include <stdio.h>
#include <omp.h>

int main() {
    long num_steps = 100000000;
    double step;
    double start, end;
    double t_seq;
    double pi_seq;
    int threads[] = {2, 4, 8, 16, 32};
    int n_threads = sizeof(threads) / sizeof(threads[0]);

    step = 1.0 / (double)num_steps;

    start = omp_get_wtime();
    {
        double sum = 0.0;
        for (long i = 0; i < num_steps; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        pi_seq = step * sum;
    }
    end = omp_get_wtime();
    t_seq = end - start;

    printf("Sequential time: %.6f seconds, pi ≈ %.12f\n\n", t_seq, pi_seq);
    printf("%-10s %-15s %-10s %-12s %-15s\n",
           "Threads", "Parallel(s)", "Speedup", "Efficiency", "pi (approx)");

    for (int t = 0; t < n_threads; t++) {
        int num_threads = threads[t];
        double t_par, pi_par;

        omp_set_num_threads(num_threads);

        start = omp_get_wtime();
        {
            double sum = 0.0;
            #pragma omp parallel for reduction(+:sum)
            for (long i = 0; i < num_steps; i++) {
                double x = (i + 0.5) * step;
                sum += 4.0 / (1.0 + x * x);
            }
            pi_par = step * sum;
        }
        end = omp_get_wtime();
        t_par = end - start;

        double speedup = t_seq / t_par;
        double efficiency = (speedup / num_threads) * 100.0;

        printf("%-10d %-15.6f %-10.2fx %-11.2f%% %-15.12f\n",
               num_threads, t_par, speedup, efficiency, pi_par);
    }

    return 0;
}

