#include "../io/File.c"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
int MAX = 2560; //five block max

void txtReader(char* input, char* buffer){
    FILE* txtFile = fopen(input, "rb"); // read mode TODO: just r?
 
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
    printf("init finished\n");
    /*
    Free block checking after writing
    write to disk
    read from disk
    verify inode data
    */
   //char inputData[MAX];
   //txtReader(, inputData); //TODO: review index
   //"C:\\Users\\Pando\\Desktop\\SchoolWork\\CSC360-FileSystemProject\\disk\vdisk"
   //printf("txtReader finished\n");
   inputData = "hello world\n";
   writeDataToDisk(disk, inputData, 0);
   printf("DataToDisk finished\n");
}