/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 

/*
 * name: Vrushank Agrawal
 * userid: vrushank.agrawal
 */

#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void transpose_32 (int M, int N, int A[N][M], int B[M][N]);
void transpose_64 (int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    if (N==64)
        transpose_64(M, N, A, B);
    else
        transpose_32(M, N, A, B);
}


// transpose a 32x32 matrix
char transpose_32_desc[] = "64x64 matrix transpose";
void transpose_32 (int M, int N, int A[N][M], int B[M][N]) {

    /* After drawing out the caching pattern,
     * we realize that diagonal elements result
     * in cache misses because of the sizes of the 
     * matrices and hence we need to avoid diagonal
     * elements in our basic algorithm of direct 
     * swapping to maximize cache efficiency.
     */

    // for diagonal elements to avoid cache overwrite
    int temp=0, diag=0;
    int i, j, row, col;
    for (i=0; i<M; i+=8)
        for (j=0; j<N; j+=8)
            for (row=j; row<j+8 && row<N; row++) {
                for (col=i; col<i+8 && col<M; col++) {
                    if (row!=col) {
                        B[col][row] = A[row][col];
                    }
                    else {  // to avoid cache overwrite
                        temp=A[row][row];
                        diag=row;
                    }
                }
                if (i==j) { // move diagonal element
                    B[diag][diag]=temp;
                }
            }
}

// transpose a 64x64 matrix
char transpose_64_desc[] = "32x32 matrix transpose";
void transpose_64 (int M, int N, int A[N][M], int B[M][N]) {

    /* Once again, after drawing out the caching
     * pattern, we see that because of the size of
     * the matrix, we can only store 4 rows of cache
     * at once and hence we need to partially store
     * the last 4 elements of each 8*8 block at a
     * different location. Then when we move to the
     * next 4 rows, we first move the data from the
     * temporary location and then repeat the process.
     * This results in the following algorithm that
     * maximizes cache efficiency.
     */

    int cache[8];
    int i, j, row, col;
    for (i=0; i < N; i += 8) {
        for (j=0; j < M; j += 8) {
            // invert first 4 rows
            for (row=i; row<i+4; row++) {
                // cache row wise
                for (col=0; col<8; col++)
                    cache[col] = A[row][j+col];
                // invert first actual block
                for (col=0; col<4; col++)
                    B[j+col][row] = cache[col];
                // invert and store all other blocks
                for (col=0; col<4; col++)
                    B[j+col][row+4] = cache[col+4];
            }
            // invert last 4 rows
            for (row=j; row<j+4; row++) {
                // cache new line
                for (col=0; col<4; col++)
                    cache[col+4] = A[i+col+4][row];
                // retrieve the temporary block
                for (col=0; col<4; col++)
                    cache[col] = B[row][i+col+4];
                // invert first block of new lines
                for (col=0; col<4; col++)
                    B[row][i+col+4] = cache[col+4];
                // transfer temporary blocks
                for (col=0; col<4; col++)
                    B[row+4][i+col] = cache[col];
                // invert the last actual block
                for (col=0; col<4; col++)
                    B[row+4][i+col+4] = A[i+col+4][row+4];
	        }   // inner block
        }
    }   // outer blocks
}   // function

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, tmp;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions () {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register 32x32 transpose function */
    registerTransFunction(transpose_32, transpose_32_desc); 

    /* Register 64x64 transpose function */
    registerTransFunction(transpose_64, transpose_64_desc); 
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
    int i, j;
    for (i = 0; i < N; i++)  {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
