#include "matrix.h"

int main(int argc,char* argv[])
{
   if (argc < 2) {
      printf("usage: detSeq <file1>\n");
      exit(1);
   }
   Matrix* a = loadMatrix(argv[1]);
   if (a==NULL) {
      printf("Wrong filenames. Couldn't load [%s]\n",argv[1]);
      exit(2);
   }
   int dm = detMatrix(a);
   printf("Determinant = %d \n",dm);
   return 0;
}
