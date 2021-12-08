#include "matrix.h"

int main(int argc,char* argv[])
{
   int r = atoi(argv[1]), c = atoi(argv[2]),md = atoi(argv[3]),sh = atoi(argv[4]);
   Matrix* m = makeMatrix(r,c);
   for(int i=0;i < r ;i++)
      for(int j=0;j < c;j++) {
         M(m,i,j) = -sh + random() % md;
      }
   printf("%d %d\n",r,c);
   printMatrix(m);
   return 0;
}
