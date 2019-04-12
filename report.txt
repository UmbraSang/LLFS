Connor McLeod
V00725080

my assignment has three folders:
apps -  which holds the makefile and test files that are the
        entry point for the system
disk -  which holds the vdisk itself, and the diskLib which
        has all the functions most base level to the reading
        and writing to the disk.
io   -  which holds File.c, the main logic of the enitre endevor.

outside of these is the readme.

I chose in my design to hold these values in the first blocks of my system:
Block 0-super
Block 1-free block vector
Block 2-inodeMap
Block 3-inodeMap
Blocks 4-19-inodes

My super block would hold important logistical data:
Bytes 0-3   - magic
Bytes 4-7   - max possible blocks
Bytes 8-11  - current # of inodes
Bytes 12-15 - block index of first inode block
Bytes 16-19 - current diskhead
Bytes 20-?  - TBD

The free block vector was the immediate occupation map for later cleanings.

My inodeMap would be 2 blocks filled with 256 4 byte entries: 
 - an inodeID short,
 - and an address short for it's location.

The next 16 blocks would be filled with 256 32 byte entries consisting of:
 - short inodeID; //2 bytes - for confirmation of correct inode
 - int fileSize; //4 bytes  - for seeing how many blocks i'd have to pull
 - int flags; //4 bytes     - for file type
 - short* addyArr; //20 byte- a list of up to 10 blocks the file resides in
 - short indir1; //2 bytes  - address of the indirect block for if the file
                              took more than 10 blocks

    The idea being that I could use the superblock to store the main information about
the disk so that such meta data will be easily accessible if I lose memory (ie. from a
crash). This is also why I chose to forgo the memory buffer and write straight to Disk.
The inode map will point to the offsets to locate immediately the inodes for the files
we're looking for in the inode blocks and therefore would lessen the chance of mistaken
data loss or missallocation. Putting the inodes at the front here is also more efficient
in terms of space (32 bytes/512 in a block is a lot of wasted space), and reduces the amount
of work it will be to do cleaning on the disk at a later date. The downside to this
implementation is the hard cap at the number of inodes able to be put in the iMap, and
therefore the number of files on the disk. A way around that which could be implemented
could be a indirection for the iMap just like the iNodes have.
    The first test file test01.c inits a new disk, does the setup on the first few blocks,
then starts to add data.
    The second test file test02.c will take the already initialized disk from test01, and
remove and modify some files and consequently their inodes.

-added 11/04/2019.
Problems encountered: 
    Through a combonation of other more immediately due projects and midterms, and constant
seg faults this project took a lot more time than expected. My makefile never wanted to
cooperate, the freads seem to return addresses instead of values, and unfortunately I ran
out of time to complete the assignment.
    While I feel like my logic is sound in my submission, my lack of experience with C was
a real obstacle to being able to make a strong showing in this assignment.

Things Left to Add:
    I never got past the seg faults so I was unable to get to the point of implementing
directories. I feel the system is robust enough in terms of crash contingencies, but would
have liked to have added the memory buffer if I had had the time.