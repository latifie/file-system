#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define MAX_FILENAME 32
#define MAX_PATH 256
#define MAX_FILES 1000
#define BLOCK_SIZE 512
#define MAX_BLOCKS 10000
#define MAX_FILE_SIZE (BLOCK_SIZE * 100)
#define JOURNAL_SIZE 1000

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
} file_entry;

typedef struct {
    int type; // 0: create, 1: write, 2: delete
    char path[MAX_PATH];
    time_t timestamp;
} journal_entry;

typedef struct {
    file_entry file_table[MAX_FILES];
    char blocks[MAX_BLOCKS][BLOCK_SIZE];
    int free_blocks[MAX_BLOCKS];
    int num_files;
    journal_entry journal[JOURNAL_SIZE];
    int journal_count;
    char fs_file[MAX_PATH]; // Filesystem file path
} filesystem;

// Functions
filesystem* fs_create(const char* fs_path);
filesystem* fs_load(const char* fs_path);
int fs_save(filesystem* fs);
void fs_destroy(filesystem* fs);
int fs_create_directory(filesystem* fs, const char* path);
int fs_create_file(filesystem* fs, const char* path);
int fs_write(filesystem* fs, const char* path, const char* data, int size);
int fs_read(filesystem* fs, const char* path, char* buffer, int size);
int fs_list_directory(filesystem* fs, const char* path);
int fs_chmod(filesystem* fs, const char* path, mode_t mode);

int fs_defrag(filesystem* fs);
char* fs_get_error(void);

int fs_export(filesystem* fs, const char* virtual_path, const char* real_path);

#endif

