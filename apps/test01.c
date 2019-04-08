#include "../io/File.c"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
int MAX = 2560;

void txtReader(char* input, char* buffer){
    FILE* txtFile = fopen(input, "r"); // read mode
 
   if (txtFile == NULL) {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
   fgets(buffer, MAX, txtFile);
   fclose(txtFile);
   buffer[strlen(buffer)]='\0';
}

int main(int argc, char **argv){
    FILE* disk = InitLLFS();
    /*
    Free block checking after writing
    write to disk
    read from disk
    verify inode data
    */
   char inputData[MAX];
   txtReader(argv[1], inputData); //TODO: review index
   writeDataToDisk(disk, inputData, 0);
}