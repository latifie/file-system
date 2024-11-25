#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

// Current working directory context
char current_dir[MAX_PATH] = "/";

// Function to list directory contents
void handle_ls(filesystem* fs) {
    printf("Contents of '%s':\n", current_dir);
    fs_list_directory(fs, current_dir);
}

// Function to change directory
void handle_cd(filesystem* fs, const char* path) {
    char new_dir[MAX_PATH];
    if (path[0] == '/') {
        strncpy(new_dir, path, MAX_PATH); // Absolute path
    } else {
        snprintf(new_dir, MAX_PATH, "%s/%s", current_dir, path); // Relative path
    }

    int dir_index = fs_find_file_by_path(fs, new_dir);
    if (dir_index == -1 || !fs->file_table[dir_index].is_directory) {
        printf("Error: Directory '%s' not found\n", path);
        return;
    }

    strncpy(current_dir, new_dir, MAX_PATH);
    printf("Changed directory to '%s'\n", current_dir);
}

// Function to create a directory
void handle_mkdir(filesystem* fs, const char* dirname) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/%s", current_dir, dirname);

    if (fs_create_directory(fs, path) == 0) {
        printf("Directory '%s' created\n", path);
    } else {
        printf("Error: %s\n", fs_get_error());
    }
}

// Function to create a file
void handle_touch(filesystem* fs, const char* filename) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/%s", current_dir, filename);

    if (fs_create_file(fs, path) == 0) {
        printf("File '%s' created\n", path);
    } else {
        printf("Error: %s\n", fs_get_error());
    }
}

// Function to write to a file
void handle_write(filesystem* fs, const char* filename, const char* data) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/%s", current_dir, filename);

    if (fs_write(fs, path, data, strlen(data)) == 0) {
        printf("Written to '%s'\n", path);
    } else {
        printf("Error: %s\n", fs_get_error());
    }
}

// Function to read a file
void handle_cat(filesystem* fs, const char* filename) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/%s", current_dir, filename);

    char buffer[1024];
    if (fs_read(fs, path, buffer, sizeof(buffer)) == 0) {
        printf("Contents of '%s':\n%s\n", path, buffer);
    } else {
        printf("Error: %s\n", fs_get_error());
    }
}

// Function to change file permissions
void handle_chmod(filesystem* fs, const char* filename, int permissions) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/%s", current_dir, filename);

    if (fs_chmod(fs, path, permissions) == 0) {
        printf("Permissions for '%s' changed to %o\n", path, permissions);
    } else {
        printf("Error: %s\n", fs_get_error());
    }
}

int main() {
    filesystem* fs = fs_load("myfs.dat");
    if (!fs) {
        printf("Creating new filesystem...\n");
        fs = fs_create("myfs.dat");
        if (!fs) {
            printf("Failed to create filesystem: %s\n", fs_get_error());
            return 1;
        }
    }

    char command[256];
    while (1) {
        printf("%s$ ", current_dir);
        if (!fgets(command, sizeof(command), stdin)) break;

        char* cmd = strtok(command, " \n");
        if (!cmd) continue;

        if (strcmp(cmd, "ls") == 0) {
            handle_ls(fs);
        } else if (strcmp(cmd, "cd") == 0) {
            char* path = strtok(NULL, " \n");
            if (path) handle_cd(fs, path);
            else printf("Usage: cd <directory>\n");
        } else if (strcmp(cmd, "mkdir") == 0) {
            char* dirname = strtok(NULL, " \n");
            if (dirname) handle_mkdir(fs, dirname);
            else printf("Usage: mkdir <directory>\n");
        } else if (strcmp(cmd, "touch") == 0) {
            char* filename = strtok(NULL, " \n");
            if (filename) handle_touch(fs, filename);
            else printf("Usage: touch <file>\n");
        } else if (strcmp(cmd, "write") == 0) {
            char* filename = strtok(NULL, " \n");
            char* data = strtok(NULL, "\n");
            if (filename && data) handle_write(fs, filename, data);
            else printf("Usage: write <file> <data>\n");
        } else if (strcmp(cmd, "cat") == 0) {
            char* filename = strtok(NULL, " \n");
            if (filename) handle_cat(fs, filename);
            else printf("Usage: cat <file>\n");
        } else if (strcmp(cmd, "chmod") == 0) {
            char* filename = strtok(NULL, " \n");
            char* mode = strtok(NULL, " \n");
            if (filename && mode) handle_chmod(fs, filename, strtol(mode, NULL, 8));
            else printf("Usage: chmod <file> <permissions>\n");
        } else if (strcmp(cmd, "exit") == 0) {
            break;
        } else {
            printf("Unknown command: %s\n", cmd);
        }
    }

    fs_destroy(fs);
    return 0;
}
