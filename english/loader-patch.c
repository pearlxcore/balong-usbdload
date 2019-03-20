#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "patcher.h"


//#######################################################################################################
void main(int argc, char* argv[]) {
  
FILE* in;
FILE* out;
uint8_t* buf;
uint32_t fsize;
int opt;
uint8_t outfilename[100];
int oflag=0,bflag=0;
uint32_t res;


// Разбор командной строки

while ((opt = getopt(argc, argv, "o:bh")) != -1) {
  switch (opt) {
   case 'h': 
     
printf("\nProgram for automatic patch loaders platforms Balong V7\n\n\
%s [keys] <bootloader file usbloader>\n\n\
The following keys are valid.:\n\n\
-o file  - name of the output file. By default, only the patch is checked.\n\
-b       - add a patch that disables validation of defective blocks\n\
\n",argv[0]);
    return;

   case 'o':
    strcpy(outfilename,optarg);
    oflag=1;
    break;

   case 'b':
     bflag=1;
     break;
     
   case '?':
   case ':':  
     return;
    
  }
}  

printf("\nThe program of automatic modification loaders Balong V7, (c) forth32 ");

 if (optind>=argc) {
    printf("\n- File name not specified for download\n - For prompts, specify the key -h\n");
    return;
}  
    
in=fopen(argv[optind],"r");
if (in == 0) {
  printf("\nError opening file %s",argv[optind]);
  return;
}

// определяем размер файла
fseek(in,0,SEEK_END);
fsize=ftell(in);
rewind(in);

// выделяем буфер и читаем туда весь файл
buf=malloc(fsize);
fread(buf,1,fsize,in);
fclose(in);

//==================================================================================

res=pv7r22_2(buf, fsize);
if (res != 0)  {
  printf("\n*Found signature type V7R22_2 by offset %08x",res);
  goto endpatch;
}

res=pv7r22(buf, fsize);
if (res != 0)  {
  printf("\n*Found signature type V7R22 by offset %08x",res);
  goto endpatch;
}  

res=pv7r1(buf, fsize);
if (res != 0)  {
  printf("\n*Found signature type V7R1 by offset %08x",res);
  goto endpatch;
}  

res=pv7r2(buf, fsize);
if (res != 0)  {
  printf("\n*Found V7R2 type signature by offset %08x",res);
  goto endpatch;
}  

res=pv7r11(buf, fsize);
if (res != 0)  {
  printf("\n*Found signature type V7R11 by offset %08x",res);
  goto endpatch;
}   

printf("\nSignature eraseall-patch not found");

//==================================================================================
endpatch:

if (bflag) {
   res=perasebad(buf, fsize);
   if (res != 0) printf("\n*Found signature isbad by offset %08x",res);  
   else  printf("\nIsbad signature not found");  
}

if (oflag) {
  out=fopen(outfilename,"w");
  if (out != 0) {
    fwrite(buf,1,fsize,out);
    fclose(out);
  }
  else printf("\nError opening output file %s",outfilename);
}
free(buf);
printf("\n");
}

   