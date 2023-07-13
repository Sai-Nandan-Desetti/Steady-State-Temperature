# include <stdio.h>
# include <stdbool.h>
# include <omp.h>

void main() {
    bool reached = false;

    #pragma omp parallel shared(reached) num_threads(4)
    {           
        while (reached != true) {
            
            if (omp_get_thread_num() > 2)
                reached = true;

            printf("%d\n", reached);
        }        
    }    
}