#include "../disk/diskLib.c"
#include <unistd.h>
#include <math.h>
const int INODE_SIZE = 32;
FILE* disk;
char vectorArr[128];
const int iNodeBlockStart = 4;

/*
Question:

how do i write to memory?
how do i write to disk?
how do i read?
is the superblock static or dynamic?
how to update freeblock vector for GC?
different directory format?
how many blocks should memory get? -small, auto write big files to disk
how should inodeMap find data in memory?

superblock magic number is not important
#blocks static
#inodes can be static or dynamic
memory is just a buffer in code
memory is in a struct:
-block cursor
-buffer itself


first 10 blocks:
0-super
1-free block vector
2-inodeMap
3-inodeMap
4-19-inodes

super:
0-3 - magic
4-7 - max possible blocks
8-11 - current # of inodes
12-15 - block index of first inode block
16-19 - current diskhead
20-
*/

typedef struct iNode{
    short inodeID; //2 byte
    int fileSize; //4 byte
    int flags; //4 byte
    short* addyArr; //20 byte
    short indir1; //2 byte
}inode_t;

typedef struct iMap{
    int mapSize;
    short mapping[252];
}imap_t;

struct iNode* makeInode(short inodeID, int fileSize, int flags, short* addyArr, short indir1) { //TODO: incompatible types?? short int*??
    struct iNode* inode = malloc(32);
    inode->inodeID = inodeID; 
    inode->fileSize = fileSize; 
    inode->flags = flags; //TODO: filetype
    inode->addyArr = addyArr; 
    inode->indir1 = indir1; //TODO: method to find file blocks if file too big
    return inode;
}

void writeInode(FILE* disk, int nodeLocation, struct iNode* currNode){
    fseek(disk, nodeLocation, SEEK_SET);
    fwrite(&currNode->inodeID, 2, 1, disk);
    fseek(disk, nodeLocation+2, SEEK_SET);
    fwrite(&currNode->fileSize, 4, 1, disk);
    fseek(disk, nodeLocation+6, SEEK_SET);
    fwrite(&currNode->flags, 4, 1, disk);
    fseek(disk, nodeLocation+10, SEEK_SET);
    fwrite(&currNode->addyArr, 20, 1, disk);
    fseek(disk, nodeLocation+30, SEEK_SET);
    fwrite(&currNode->indir1, 2, 1, disk);

    //gets the number of inodes on disk
    //adds one and writes back
    int* numNode = malloc(4);
    fseek(disk, 0+4+4, SEEK_SET);
    fread(numNode, 4, 1, disk);
    fwrite(numNode+1, 4, 1, disk);
}

void initStartingBlocks(){
    int superBlock[5] = {3, 4096, 0, iNodeBlockStart, iNodeBlockStart+16}; //TODO: not char*?
    writeBlock(disk, 0, superBlock);
    int i;
    int vectorArr[128];
    for(i=0; i<128; i++){
        vectorArr[i]=0xff;
    }
    // fseek(disk, 0+4+4, SEEK_SET);
    // fread(numNode, 4, 1, disk);
    // fwrite(numNode+1, 4, 1, disk);
 }

FILE* InitLLFS(){
    if(access("vdisk", F_OK) != -1){
        if (remove("vdisk") == 0){
            printf("%s wiped clean.\n", "vdisk");
        }else{
            printf("Unable to format disk\n");
            perror("InitLLFS failed to wipe disk.\n");
        }
    }
    /*FILE* */disk = fopen("vdisk", "wb"); // Open the file to be written to in binary mode
    char* init=calloc(BLOCK_SIZE*NUM_BLOCKS, 1);
    fwrite(init, BLOCK_SIZE*NUM_BLOCKS, 1, disk);
    fclose(disk);
    initStartingBlocks();
    return disk;
 }

 int isBitClear(int blocknum){
    char byte = vectorArr[blocknum/8];
    int mask = 1;
    mask = mask<<(blocknum%8);
    return byte & mask;
 }

 void setbit(int blocknum){
    char byte = vectorArr[blocknum/8];
    int mask = 1;
    mask = mask<<(blocknum%8);
    vectorArr[blocknum/8] = byte | mask;
}

void clearbit(int blocknum){
    char byte = vectorArr[blocknum/8];
    int mask = 254;
    mask = mask<<(blocknum%8);
    vectorArr[blocknum/8] = byte & mask;
}

void markVectorBlocks(int currDiskHead, int numBits, int isSetting){
    int i;
    for(i=0; i<numBits; i++){
        if(isSetting){
            setbit(currDiskHead+i);
        }else{
            clearbit(currDiskHead+i);
        }
    }
    //TODO: overwrite vector block with vectorArr[]
}

short getNewInodeID(){
     int* inodeID = malloc(4);
     getNumInodes(disk, inodeID);
     int temp = (*inodeID);
     return (short)temp;
 }

 void writeDataToDisk(FILE* disk, char* inputData, int isDir){ //TODO: seg faults in here
    int* totalInodes = malloc(4);
    getNumInodes(disk, totalInodes);
    printf("current inodes = %d\n", *totalInodes); //TODO: remove
    int inodeMapBlock;
    if(*totalInodes>256){
        printf("File System at capacity. No space in inodeMap.\n");
        return;
    }else if(*totalInodes>128){
        inodeMapBlock = 3;
    }else{
        inodeMapBlock = 2;
    }

    //writes data
    int i;
    int blocksNeeded = ceil(strlen(inputData)/BLOCK_SIZE)+1;
    printf("ceil = %d\n", blocksNeeded);
    int currDiskHead;
    getDiskHead(disk, &currDiskHead);
    printf("post getDiskHead()\n");
    char* pointToIndex;
    for(i=0; i<blocksNeeded; i++){
        printf("pre writeblock #%d", i);
        writeBlock(disk, currDiskHead+i, &inputData[i*BLOCK_SIZE]);
    } //TODO: fix indirect blocking
    printf("pre addDiskHead()");
    addDiskHead(disk, blocksNeeded);
    printf("post addDiskHead()");

    //writes inode
    markVectorBlocks(currDiskHead, i+1, 0);
    short addyArr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(i=0; i<blocksNeeded; i++){
        addyArr[i]=currDiskHead+i;
    }
    struct iNode* currNode = makeInode(getNewInodeID(), strlen(inputData), isDir, addyArr, 0);

    int nodeSpot = iNodeBlockStart+(floor(currNode->inodeID/16))+(32*currNode->inodeID%16);
    writeInode(disk, nodeSpot, currNode);
    
    //writes inode to map
    updateImap(disk, inodeMapBlock, *totalInodes%127, currNode->inodeID, currDiskHead);

    //free(currNode); //TODO: fix this
 }
 
 int getInodeFromMap(short inodeID){
     //TODO: search for inode in map
 }

 struct iNode* getInodeByID(short inodeID){
    //TODO: check inodeMap for ID and read the data to make an inode
     int inodeLocation = getInodeFromMap(inodeID);
     short* idBuff = malloc(2);
     readInode(disk, inodeLocation, idBuff, 2, 0);
     int* sizeBuff = malloc(4);
     readInode(disk, inodeLocation, sizeBuff, 4, 2);
     int* flagBuff = malloc(4);
     readInode(disk, inodeLocation, flagBuff, 4, 6);
     short* addyArrBuff = malloc(2*10);
     readInode(disk, inodeLocation, addyArrBuff, 20, 10);
     short* indir1Buff = malloc(2);
     readInode(disk, inodeLocation, indir1Buff, 2, 30);
     struct iNode* readNode = makeInode((*idBuff), (*sizeBuff), (*flagBuff), addyArrBuff, (*indir1Buff));
     return readNode;
 }

 char* readDataFromDisk(FILE* disk, short inodeID){
     struct iNode* readNode = getInodeByID(inodeID);
     char* buffer = (char*)malloc(readNode->fileSize); //TODO: does this need to be in a multiple of BLOCK_SIZE?
     int i;
     int blocksUsed = ceil(readNode->fileSize/BLOCK_SIZE);
     for(i=0; i<blocksUsed; i++){
         readBlock(disk, readNode->addyArr[i], &buffer[i*BLOCK_SIZE]);
     }
     return buffer;
 } //TODO: doesn't account for indirect yet.




 /*
 Inode points to file blocks
 InodeMap points to inode blocks
 Memory segments are our log
 vdisk is our Disk
 
 */