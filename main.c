#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to recursively create parent directories
int create_parent_directories(filesystem* fs, const char* path) {
    char dir[MAX_PATH];
    char* last_slash = strrchr(path, '/');
    
    // If no slash or root directory, no parent to create
    if (!last_slash || last_slash == path) return 0;

    strncpy(dir, path, last_slash - path);
    dir[last_slash - path] = '\0';

    // If directory already exists, return
    int existing = fs_find_file_by_path(fs, dir);
    if (existing != -1) return 0;

    // Recursively create parent directories
    create_parent_directories(fs, dir);

    // Create this directory
    if (fs_create_directory(fs, dir) != 0) {
        printf("Failed to create directory: %s\n", dir);
        return -1;
    }

    return 0;
}

// (Rest of the main.c remains the same as in previous version)

int main() {
    // Load or create the filesystem
    filesystem* fs = fs_load("myfs.dat");
    if (!fs) {
        printf("Creating new filesystem...\n");
        fs = fs_create("myfs.dat");
        if (!fs) {
            printf("Failed to create filesystem: %s\n", fs_get_error());
            return 1;
        }
    }

    // Entrée utilisateur pour le fichier
    char filename[MAX_PATH];
    char export_filename[MAX_PATH];
    int permissions;
    char content[1024];

    printf("Enter the name of the file to create (e.g., /documents/myfile.txt): ");
    scanf("%s", filename);

    printf("Enter the permissions for the file (e.g., 0644): ");
    scanf("%o", &permissions);

    printf("Enter the content of the file: ");
    getchar(); // Consume the newline character from previous input
    fgets(content, sizeof(content), stdin);

    // Create parent directories
    if (create_parent_directories(fs, filename) != 0) {
        printf("Failed to create parent directories\n");
        fs_destroy(fs);
        return 1;
    }

    // Créer le fichier
    if (fs_create_file(fs, filename) != 0) {
        printf("Failed to create file: %s\n", fs_get_error());
        fs_destroy(fs);
        return 1;
    }

    // Écrire dans le fichier
    if (fs_write(fs, filename, content, strlen(content)) != 0) {
        printf("Failed to write to file: %s\n", fs_get_error());
        fs_destroy(fs);
        return 1;
    }

    // Modifier les permissions
    if (fs_chmod(fs, filename, permissions) != 0) {
        printf("Failed to change permissions: %s\n", fs_get_error());
        fs_destroy(fs);
        return 1;
    }

    printf("File '%s' created successfully with permissions %o.\n", filename, permissions);

    // Exporter le fichier
    printf("Enter the name for the exported file (e.g., exported_file.txt): ");
    scanf("%s", export_filename);

    if (fs_export(fs, filename, export_filename) != 0) {
        printf("Failed to export file: %s\n", fs_get_error());
        fs_destroy(fs);
        return 1;
    }

    printf("File exported to '%s'.\n", export_filename);

    // Nettoyage
    fs_destroy(fs);
    return 0;
}
