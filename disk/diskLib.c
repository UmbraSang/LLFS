#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 4096;

void writeBlock(FILE* disk, int blockNum, void* data){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fwrite(data, BLOCK_SIZE, 1, disk); 
}

void readBlock(FILE* disk, int blockNum, void* buffer){
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fread(buffer, BLOCK_SIZE, 1, disk);
}

void getNumInodes(FILE* disk, int* numNodes){
    fseek(disk, 0+4+4, SEEK_SET); //offsets to current # inodes in superblock
    fread(numNodes, 4, 1, disk);
}

void getDiskHead(FILE* disk, int* blocksIndex){
    fseek(disk, 16, SEEK_SET); //offsets to current diskhead in superblock
    fread(blocksIndex, 4, 1, disk);
}

void addDiskHead(FILE* disk, int blocksAdded){
    //Moves diskHead by blocksAdded
    int* currHead = malloc(4);
    fseek(disk, 16, SEEK_SET); //offsets to current diskhead in superblock
    fread(currHead, 4, 1, disk);
    fseek(disk, 16, SEEK_SET); //offsets to current diskhead in superblock
    fwrite(currHead+blocksAdded, 4, 1, disk);
    free(currHead);
}

void readInode(FILE* disk, int blockNum, void* buffer, int size, int offset){
    fseek(disk, (blockNum * BLOCK_SIZE)+offset, SEEK_SET);
    fread(buffer, size, 1, disk);
}

void updateImap(FILE* disk, int mapBlock, int blockInodes, short inodeID, short inodeAddy){
    int location = mapBlock*BLOCK_SIZE+blockInodes*4;
    fseek(disk, location, SEEK_SET);
    fwrite(&inodeID, 2, 1, disk); //TODO: should this be derefrenced?
    fseek(disk, location+2, SEEK_SET);
    fwrite(&inodeAddy, 2, 1, disk); //TODO: should this be derefrenced?
}

void initDisk(){}

/*
int main(int argc, char* argv[]) {
    FILE* disk = fopen("vdisk", "wb"); // Open the file to be written to in binary mode
    char* init=calloc(BLOCK_SIZE*NUM_BLOCKS, 1);
    fwrite(init, BLOCK_SIZE*NUM_BLOCKS, 1, disk);

    fclose(disk);
    return 0;
}
*/