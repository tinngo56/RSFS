/*
    implementation of API
*/

#include "def.h"

pthread_mutex_t mutex_for_fs_stat;

//initialize file system - should be called as the first thing before accessing this file system 
int RSFS_init(){

    //initialize data blocks
    for(int i=0; i<NUM_DBLOCKS; i++){
      void *block = malloc(BLOCK_SIZE); //a data block is allocated from memory
      if(block==NULL){
        printf("[init] fails to init data_blocks\n");
        return -1;
      }
      data_blocks[i] = block;  
    } 

    //initialize bitmaps
    for(int i=0; i<NUM_DBLOCKS; i++) data_bitmap[i]=0;
    pthread_mutex_init(&data_bitmap_mutex,NULL);
    for(int i=0; i<NUM_INODES; i++) inode_bitmap[i]=0;
    pthread_mutex_init(&inode_bitmap_mutex,NULL);    

    //initialize inodes
    for(int i=0; i<NUM_INODES; i++){
        inodes[i].length=0;
        for(int j=0; j<NUM_POINTER; j++) 
            inodes[i].block[j]=-1; //pointer value -1 means the pointer is not used
        inodes[i].num_current_reader=0;
        pthread_mutex_init(&inodes[i].rw_mutex,NULL);
        pthread_mutex_init(&inodes[i].read_mutex,NULL);
    }
    pthread_mutex_init(&inodes_mutex,NULL); 

    //initialize open file table
    for(int i=0; i<NUM_OPEN_FILE; i++){
        struct open_file_entry entry=open_file_table[i];
        entry.used=0; //each entry is not used initially
        pthread_mutex_init(&entry.entry_mutex,NULL);
        entry.position=0;
        entry.access_flag=-1;
    }
    pthread_mutex_init(&open_file_table_mutex,NULL); 

    //initialize root directory
    root_dir.head = root_dir.tail = NULL;

    //initialize mutex_for_fs_stat
    pthread_mutex_init(&mutex_for_fs_stat,NULL);

    //return 0 means success
    return 0;
}


//create file
//if file does not exist, create the file and return 0;
//if file_name already exists, return -1; 
//otherwise, return -2.
int RSFS_create(char *file_name){

    //search root_dir for dir_entry matching provided file_name
    struct dir_entry *dir_entry = search_dir(file_name);

    if(dir_entry){//already exists
        printf("[create] file (%s) already exists.\n", file_name);
        return -1;
    }else{

        if(DEBUG) printf("[create] file (%s) does not exist.\n", file_name);

        //construct and insert a new dir_entry with given file_name
        dir_entry = insert_dir(file_name);
        if(DEBUG) printf("[create] insert a dir_entry with file_name:%s.\n", dir_entry->name);
        
        //access inode-bitmap to get a free inode 
        int inode_number = allocate_inode();
        if(inode_number<0){
            printf("[create] fail to allocate an inode.\n");
            return -2;
        } 
        if(DEBUG) printf("[create] allocate inode with number:%d.\n", inode_number);

        //save inode-number to dir-entry
        dir_entry->inode_number = inode_number;
        
        return 0;
    }
}



//open a file with RSFS_RDONLY or RSFS_RDWR flags
//When flag=RSFS_RDONLY: 
//  if the file is currently opened with RSFS_RDWR (by a process/thread)=> the caller should be blocked (wait); 
//  otherwise, the file is opened and the descriptor (i.e., index of the open_file_entry in the open_file_table) is returned
//When flag=RSFS_RDWR:
//  if the file is currently opened with RSFS_RDWR (by a process/thread) or RSFS_RDONLY (by one or multiple processes/threads) 
//      => the caller should be blocked (i.e. wait);
//  otherwise, the file is opened and the desrcriptor is returned
int RSFS_open(char *file_name, int access_flag){

    //to do: check to make sure access_flag is either RSFS_RDONLY or RSFS_RDWR
    
    //to do: find dir_entry matching file_name
    
    //to do: find the corresponding inode 
    
    
    //to do: base on the requested access_flag and the current "open" status of this file to block the caller if needed
    //(refer to solution to reader/writer problem) 
    

    //to do: find an unused open-file-entry in open-file-table and fill the fields of the entry properly
    
    
    //to do: return the index of the open-file-entry in open-file-table as file descriptor
    return 0; //placeholder
}



//append the content in buf to the end of the file of descriptor fd
int RSFS_append(int fd, void *buf, int size){

    //to do: check the sanity of the arguments: fd should be in [0,NUM_OPEN_FILE] and size>0.
    
    //to do: get the open file entry corresponding to fd
    
    //to do: check if the file is opened with RSFS_RDWR mode; otherwise return -1
    
    //get the current position
    
    //to do: get the corresponding directory entry
    
    //to do: get the corresponding inode 
    

    //to do: append the content in buf to the data blocks of the file from the end of the file; 
    //allocate new block(s) when needed - (refer to lecture L22 on how)
    
     
    //to do: update the current position in open file entry
    
    
    //to do: return the number of bytes appended to the file
    return 0; //placeholder
}







//update current position of the file (which is in the open_file_entry) to offset
int RSFS_fseek(int fd, int offset){

    //to do: sanity test of fd    
    
    //to do: get the correspondng open file entry
    
    //to do: get the current position
    
    //to do: get the corresponding dir entry
    
    //to do: get the corresponding inode and file length
    
    //to do: check if argument offset is not within 0...length, do not proceed and return current position
    
    //to do: update the current position to offset, and return the new current position
    
    //to do: return the current poisiton
    return 0; //placeholder
}






//read from file from the current position for up to size bytes
int RSFS_read(int fd, void *buf, int size){

    //to do: sanity test of fd and size    
    
    //to do: get the corresponding open file entry

    //to do: get the current position
    
    //to do: get the corresponding directory entry
    
    //to do: get the corresponding inode 
    
    //to do: read the content of the file from current position for up to size bytes 
    
    //to do: update the current position in open file entry
    
    //to do: return the actual number of bytes read
    return 0; //placeholder 
}


//close file: return 0 if succeed
int RSFS_close(int fd){

    //to do: sanity test of fd and whence    
    
    //to do: get the corresponding open file entry
    
    //to do: get the corresponding dir entry
    
    //to do: get the corresponding inode 

    //to do: depending on the way that the file was open (RSFS_RDONLY or RSFS_RDWR), update the corresponding mutex and/or count 
    //(refer to the solution to the readers/writers problem)
    
    //to do: release this open file entry in the open file table
    
    return 0;
}









//delete file
int RSFS_delete(char *file_name){

    //to do: find the corresponding dir_entry

    //to do: find the corresponding inode
    
    //to do: free the inode in inode-bitmap
    
    return 0;
}


//print status of the file system
void RSFS_stat(){

    pthread_mutex_lock(&mutex_for_fs_stat);

    printf("\nCurrent status of the file system:\n\n %16s%10s%10s\n", "File Name", "Length", "iNode #");

    //list files
    struct dir_entry *dir_entry = root_dir.head;
    while(dir_entry!=NULL){

        int inode_number = dir_entry->inode_number;
        struct inode *inode = &inodes[inode_number];
        
        printf("%16s%10d%10d\n", dir_entry->name, inode->length, inode_number);
        dir_entry = dir_entry->next;
    }
    
    //data blocks
    int db_used=0;
    for(int i=0; i<NUM_DBLOCKS; i++) db_used+=data_bitmap[i];
    printf("\nTotal Data Blocks: %4d,  Used: %d,  Unused: %d\n", NUM_DBLOCKS, db_used, NUM_DBLOCKS-db_used);

    //inodes
    int inodes_used=0;
    for(int i=0; i<NUM_INODES; i++) inodes_used+=inode_bitmap[i];
    printf("Total iNode Blocks: %3d,  Used: %d,  Unused: %d\n", NUM_INODES, inodes_used, NUM_INODES-inodes_used);

    //open files
    int of_num=0;
    for(int i=0; i<NUM_OPEN_FILE; i++) of_num+=open_file_table[i].used;
    printf("Total Opened Files: %3d\n\n", of_num);

    pthread_mutex_unlock(&mutex_for_fs_stat);
}



//write the content of size (bytes) in buf to the file (of descripter fd) from current position for up to size bytes 
int RSFS_write(int fd, void *buf, int size){
    return 0; //placeholder
}




//cut the content from the current position for up to size (bytes) from the file of descriptor fd
int RSFS_cut(int fd, int size){
    return 0; //placeholder
}



