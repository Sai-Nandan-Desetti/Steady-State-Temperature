#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <omp.h>
#include "randomwalk.h"

// for alloc_matrix()
#define SUCCESS 0
#define OUT_OF_MEMORY_ERROR 1

#define CONVERGENCE_THRESHOLD 0.05

typedef double Element_type;

////////////////////////////// FUNCTION PROTOTYPES //////////////////////////////
void alloc_matrix(
   int      nrows,
   int      ncols,
   size_t   element_size,
   Element_type **matrix_storage,
   Element_type ***matrix,
   int      *errvalue
);

/////////////////////////////////////////// main() /////////////////////////////////////
int main(int argc, char *argv[]) {

    point2d current;
    int t;    
    int i, j;
    int height = 50, width = 50;
    double oldvalue;
    double maxdiff, diff;
    double tolerance;
    int location;    
    double **plate;
    double *plate_storage;
    int allocate;
    unsigned long long count;
    FILE *out_file;

    if (argc != 2) {
        fprintf(stderr, "Command-line error\nUsage: %s <number of threads>", argv[0]);
        exit(-1);
    }
    // Initialize number of threads
    t = atoi(argv[1]);

	/* Allocate space for the table */
    alloc_matrix(height, width, sizeof(Element_type), &plate_storage, &plate, &allocate);
    if (allocate != SUCCESS) {
      fprintf(stderr, "Couldn't allocate memory for the plate!\n");
      exit(-1);
    }
	
	/* Initialize some example boundary temperatures */
	// Here, North and South are 0 degrees, and East and West are 100 degrees, respectively.
	double boundary_temp[4] = {0, 100, 0, 100};
	
    /*
    Initialize temperatures at the four corners as the average 
    of the temperatures of the adjacent stages.
    */
    plate[0][0]              = (boundary_temp[0] + boundary_temp[3]) / 2;
    plate[0][width-1]        = (boundary_temp[0] + boundary_temp[1]) / 2;
    plate[height-1][0]       = (boundary_temp[3] + boundary_temp[2]) / 2;
    plate[height-1][width-1] = (boundary_temp[2] + boundary_temp[1]) / 2;

    /* Initialize the temperatures along the edges of the plate */    
    for (j = 1; j < width - 1; j++) {
        plate[0][j]        = boundary_temp[0];
        plate[height-1][j] = boundary_temp[2];
    }    
    for (i = 1; i < height - 1; i++) {
        plate[i][0]       = boundary_temp[3];
        plate[i][width-1] = boundary_temp[2];
    }

    /* Initialize the interior temperatures to 0. */    
    for (i = 1; i < height - 1; i++)
        for (j = 1; j < width - 1; j++)
            plate[i][j] = 0.0;

    /* Set the acceptable tolerance to a small value and start an iteration counter. */
    tolerance = CONVERGENCE_THRESHOLD;
    maxdiff = tolerance + 1;                       
    count = 0;    
    
    while (maxdiff >= tolerance) {
        // reset maximum difference to 0 at the beginning of each iteration
        maxdiff = 0;
        
        // for each interior grid point [i, j]        
        for (int i = 1; i < height - 1; i++) {
            for (int j = 1; j < width - 1; j++) {

                // do a random walk until a boundary is reached
                current.x = j;
                current.y = i;
                while (0 == (location = on_boundary(current, width, height))) {
                    current = next_point(current, next_dir());
                }

                /* 
                Get difference between old average at this point and average with the 
                new boundary point factored into it.
                */
                oldvalue = plate[i][j];
                plate[i][j] = (oldvalue*count + boundary_temp[location-1]) / (count + 1);
                diff = fabs(plate[i][j] - oldvalue);

                /*
                If the difference at this point is the largest so far in this iteration,
                then update maxdiff. 
                */
                if (diff > maxdiff) 
                    maxdiff = diff;                    
            }
        }            
        count++;
    }            
    
    out_file = fopen("Plate.out", "w");
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++)
            fprintf(out_file, "%lf\t", plate[i][j]);
        fprintf(out_file, "\n");
    }

    fclose(out_file);
}

/////////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
void alloc_matrix(
   // function parameters
   int      nrows,
   int      ncols,
   size_t   element_size,
   Element_type **matrix_storage,
   Element_type ***matrix,
   int      *errval
)
{
   void *ptr_to_row_in_storage;
   void **matrix_row_start;  

   *matrix_storage = malloc(nrows * ncols * element_size);
   if (*matrix_storage == NULL) {
      *errval = OUT_OF_MEMORY_ERROR;      
      return;
   }

   *matrix = malloc (nrows * sizeof(void *));
   if (*matrix == NULL) {
      *errval = OUT_OF_MEMORY_ERROR;      
      return;
   }

   matrix_row_start = (void *) &(*matrix[0]);

   ptr_to_row_in_storage = (void *) *matrix_storage;

   for (int i = 0; i < nrows; i++ ) {
      *matrix_row_start = (void *) ptr_to_row_in_storage;
      matrix_row_start++;
      ptr_to_row_in_storage += ncols * element_size;
   }

   *errval = SUCCESS;
}
