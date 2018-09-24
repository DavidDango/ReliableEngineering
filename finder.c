#include <stdio.h>
#include <stdlib.h>



int main(int argc, char** args) {
    int n;
    FILE *fptr;
    int blocksize = 4096;
    char* buf;
    
    if((fptr = fopen(args[1], "rb")) == NULL){
        printf("Error opening file \n");
        exit(1);
    }
    
    fseek(fptr, -5*sizeof(char), SEEK_END);
    
    for(n = 1; n < 5; ++n) {
        fread(buf, sizeof(char), 5, fptr); 
        printf("read: %s\n", buf);
        fseek(fptr, -10*(sizeof(char)), SEEK_CUR);
   }
   fclose(fptr); 
}
