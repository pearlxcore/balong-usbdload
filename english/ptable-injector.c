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
  
  
int opt;  
int mflag=0;
char ptfile[100];
int rflag=0,xflag=0;

uint32_t ptaddr;
struct ptable_t ptable;

FILE* ldr;
FILE* out;
FILE* in;

while ((opt = getopt(argc, argv, "mr:hx")) != -1) {
  switch (opt) {
   case 'h': 
     
printf("\nUtility to replace the partition table in the usbloader loaders\
\nModem must be in fastboot mode\
\n\n\
%s [keys] <usbloader file name>\n\n\
 The following are valid keys:\n\n\
-m       - show current partition map in usbloader\n\
-x       - extract current map to file ptable.bin\n\
-r <file>- replace the partition card with a card from the specified file\n\
\n",argv[0]);
    return;
    
   case 'm':
    mflag=1;
    break;
    
   case 'x':
    xflag=1;
    break;
    
   case 'r':
     rflag=1;
     strcpy (ptfile,optarg);
     break;
     
   case '?':
   case ':':  
     return;
  
  }  
}  
if (optind>=argc) {
    printf("\n- The bootloader file name is not specified.\n");
    return;
}  

ldr=fopen(argv[optind],"r+");
if (ldr == 0) {
  printf("\nError opening file %s\n",argv[optind]);
  return;
}

 
// Ищем таблицу разделов в файле загрузчика  

ptaddr=find_ptable(ldr);
if (ptaddr == 0) {
  printf("\nThe partition table in the loader is not found\n");
  return ;
}
// читаем текущую таблицу
fread(&ptable,sizeof(ptable),1,ldr);

if (xflag) {
   out=fopen("ptable.bin","w");
   fwrite(&ptable,sizeof(ptable),1,out);
   fclose(out);
}   

if (mflag) {
  show_map(ptable);
}

if (mflag | xflag) return;

  
if (rflag) { 
  in=fopen(ptfile,"r");
  if (in == 0) {
    printf("\nError opening file %s",ptfile);
    return;
  }
  fread(&ptable,sizeof(ptable),1,in);
  fclose(in);
  
  // проверяем файл
  if (memcmp(ptable.head,headmagic,16) != 0) {
    printf("\nThe input file is not a partition table.\n");
    return;
  }
  fseek(ldr,ptaddr,SEEK_SET);
  fwrite(&ptable,sizeof(ptable),1,ldr);
  fclose(ldr);
  
}  
}
