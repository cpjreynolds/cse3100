#include "matrix.h"
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

/* TODO: Complete functions detMatrix and detMatrixPar for Q2 */

Matrix* makeMatrix(int r,int c)
{
   Matrix* m = malloc(sizeof(Matrix) + r * c * sizeof(int));
   m->r = r;
   m->c = c;
   return m;
}

int readValue(FILE* fd)
{
   int v = 0;
   char ch;
   int neg=1;
   while (((ch = getc_unlocked(fd)) != EOF) && isspace(ch)); // skip WS.      
   while (!isspace(ch)) {
      if (ch=='-')
         neg=-1;
      else
         v = v * 10 + ch - '0';
      ch = getc_unlocked(fd);
   }
   return neg * v;
}

Matrix* readMatrix(FILE* fd)
{
   int r,c,v;
   int nv = fscanf(fd,"%d %d",&r,&c);
   Matrix* m = makeMatrix(r,c);
   flockfile(fd);
   for(int i=0;i < r;i++)
      for(int j=0;j < c;j++) {
         v = readValue(fd);
         M(m,i,j) = v;
      }
   funlockfile(fd);
   return m;
}

Matrix* loadMatrix(char* fName)
{
   FILE* fd = fopen(fName,"r");
   if (fd==NULL) return NULL;
   Matrix* m = readMatrix(fd);
   fclose(fd);
   return m;
}

void freeMatrix(Matrix* m)
{
   free(m);
}

void printMatrix(Matrix* m)
{
   for(int i=0;i<m->r;i++) {
      for(int j=0;j < m->c; j++)
         printf("%3d ",M(m,i,j));     
      printf("\n");
   }
}

int detMatrix(Matrix* m)
{
   /* TODO 
    * This function calculates the determinant of matrix m sequentially.
    * Hint: you may create an auxiliary function for ease of implementation.
    * Note that while the indexing of the matrix starts with 0, the inductive definition in the pdf handout starts with a 1.
    * The function returns the determinant of matrix m.
    */
   return 0; /* TODO: fix me */
}

int detMatrixPar(Matrix* m,int nbW)
{
   /* TODO
    * This function calculates the determinant of matrix m using nbW threads.
    * Notice that the computation consists of n terms (equal to number of rows or columns).
    * You may split the computation of n terms fairly among the nbW worker threads, and aggregate their results upon completion.
    * The function returns the determinant of matrix m.
    * Note: do not forget the base case for a 0x0 matrix.
    */
    return 0; /* TODO: fix me */
}
