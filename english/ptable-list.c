//   Программа для замены таблицы разделов в загрузчике usbloader
// 
// 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parts.h"

  
//############################################################################################################3

void main(int argc, char* argv[]) {
  
  

struct ptable_t ptable;
FILE* in;

if (argc != 2) {
    printf("\n- The name of the file with the partition table is not specified.\n");
    return;
}  

in=fopen(argv[optind],"r+");
if (in == 0) {
  printf("\nError opening file %s\n",argv[optind]);
  return;
}

 
// читаем текущую таблицу
fread(&ptable,sizeof(ptable),1,in);

if (strncmp(ptable.head, "pTableHead", 16) != 0) {
  printf("\nFile is not a partition table\n");
  return ;
}
  
show_map(ptable);
}
