#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#define MAX_FILENAME 32
#define MAX_PATH 256
#define MAX_FILES 1000
#define BLOCK_SIZE 512
#define MAX_BLOCKS 10000
#define MAX_FILE_SIZE (BLOCK_SIZE * 100)
#define JOURNAL_SIZE 1000
#define MAX_INODES 2000
#define NUM_BLOCK 12

typedef struct {
    char filename[MAX_FILENAME];
    size_t size;
    int start_block;
    int num_blocks;
    time_t created;
    time_t modified;
    time_t accessed;
    mode_t permissions;
    int is_directory;
    int parent_dir; // Parent directory index
    int in_use;
    int num_block;
} file_entry;

typedef struct {
    int type; // 0: create, 1: write, 2: delete
    char path[MAX_PATH];
    time_t timestamp;
} journal_entry;

// Define the `inode` structure before the `filesystem` structure
typedef struct inode {
    int is_file;                 // 1 for file, 0 for directory
    size_t size;                 // File size in bytes
    int blocks[NUM_BLOCK];       // Direct block indices
    int permissions;             // File permissions (e.g., 0777)
    time_t created;              // Creation time
    time_t modified;             // Last modification time
} inode;

// Define the `filesystem` structure
typedef struct filesystem {
    FILE* disk;                  // File pointer to the virtual disk
    inode inodes[MAX_INODES];    // Array of inodes
    int free_inode_bitmap[MAX_INODES]; // Bitmap for free/used inodes
    int free_block_bitmap[MAX_BLOCKS]; // Bitmap for free/used blocks
    file_entry file_table[MAX_FILES];
    char fs_file[MAX_PATH];
    char blocks[MAX_BLOCKS][BLOCK_SIZE];
    int num_files;
    int free_blocks[MAX_BLOCKS];
} filesystem;

// Function declarations (fs_create, fs_read, fs_write, etc.)
int fs_read(filesystem* fs, const char* path, char* buffer, size_t size);
int fs_create_file(filesystem* fs, const char* path);
int fs_create_directory(filesystem* fs, const char* path);
int fs_list_directory(filesystem* fs, const char* path);
int fs_find_file_by_path(filesystem* fs, const char* path);
const char* fs_get_error(void);
filesystem* fs_create(const char* fs_path);
filesystem* fs_load(const char* fs_path);
int fs_save(filesystem* fs);
void fs_destroy(filesystem* fs);
int fs_create_directory(filesystem* fs, const char* path);
int fs_create_file(filesystem* fs, const char* path);
 int fs_write(filesystem* fs, const char* path, const char* data, int size);
int fs_read(filesystem* fs, const char* path, char* buffer, size_t size);
int fs_list_directory(filesystem* fs, const char* path);
 int fs_chmod(filesystem* fs, const char* path, mode_t mode);

// New public function to find file by path
int fs_find_file_by_path(filesystem* fs, const char* path);

int fs_defrag(filesystem* fs);
// char* fs_get_error(void);

int fs_export(filesystem* fs, const char* virtual_path, const char* real_path);

#endif
