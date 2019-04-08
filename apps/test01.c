#include "../io/File.c"
#include <stdio.h>
#include <stdlib.h>
int MAX = 2560;

char* txtReader(char* input){
    char* buffer[MAX];
    FILE* txtFile = fopen(input, "r"); // read mode
 
   if (txtFile == NULL) {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
   fgets(buffer, MAX, txtFile) != EOF);
   fclose(fp);
   buffer[strlen(buffer)]=Null;
   return buffer;
}

int main(int argc, char **argv){
    FILE* disk = InitLLFS();
    /*
    Free block checking after writing
    write to disk
    read from disk
    verify inode data
    */
   char* inputData = txtReader(argv[1]); //TODO: review index
   writeDataToDisk(disk, inputData, 0);
}