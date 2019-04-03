#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 4096;

void writeBlock(FILE* disk, int blockNum, char* data){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fwrite(data, BLOCK_SIZE, 1, disk); 
}

void readBlock(FILE* disk, int blockNum, char* buffer){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fread(buffer, BLOCK_SIZE, 1, disk);
}

void readInode(FILE* disk, int blockNum, void* buffer, int size, int offset){
    fseek(disk, (blockNum * BLOCK_SIZE)+offset, SEEK_SET);
    fread(buffer, size, 1, disk);
}

void initDisk(){}

int main(int argc, char* argv[]) {
    FILE* disk = fopen("vdisk", "wb"); // Open the file to be written to in binary mode
    char* init=calloc(BLOCK_SIZE*NUM_BLOCKS, 1);
    fwrite(init, BLOCK_SIZE*NUM_BLOCKS, 1, disk);

    fclose(disk);
    return 0;
}