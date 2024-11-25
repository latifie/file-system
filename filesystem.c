#include "filesystem.h"

static char error_buffer[256];

char* fs_get_error(void) {
    return error_buffer;
}

static int split_path(const char* path, char* dir, char* name) {
    if (path[0] == '/') {
        // Path absolu
        path++;
    }

    char* last_slash = strrchr(path, '/');
    if (!last_slash) {
        strcpy(dir, "/");
        strcpy(name, path);
    } else {
        strncpy(dir, path, last_slash - path);
        dir[last_slash - path] = '\0';
        strcpy(name, last_slash + 1);
    }
    return 0;
}


static int find_file_by_path(filesystem* fs, const char* path) {
    char dir[MAX_PATH];
    char name[MAX_FILENAME];
    split_path(path, dir, name);

    // Si c'est un chemin racine
    if (strcmp(dir, "/") == 0) {
        // Chercher le fichier dans le système de fichiers
        for (int i = 0; i < MAX_FILES; i++) {
            if (fs->file_table[i].in_use && strcmp(fs->file_table[i].filename, name) == 0) {
                return i;  // Trouvé, retourner l'index
            }
        }
    } else {
        // Si ce n'est pas dans la racine, effectuer la recherche dans le sous-dossier
        for (int i = 0; i < MAX_FILES; i++) {
            if (fs->file_table[i].in_use && strcmp(fs->file_table[i].filename, dir) == 0) {
                // Chercher dans ce répertoire
                for (int j = 0; j < MAX_FILES; j++) {
                    if (fs->file_table[j].in_use && strcmp(fs->file_table[j].filename, name) == 0) {
                        return j;
                    }
                }
            }
        }
    }

    return -1;  // Si non trouvé
}



filesystem* fs_create(const char* fs_path) {
    filesystem* fs = (filesystem*)malloc(sizeof(filesystem));
    if (!fs) {
        sprintf(error_buffer, "Memory allocation failed");
        return NULL;
    }

    memset(fs, 0, sizeof(filesystem));
    for (int i = 0; i < MAX_BLOCKS; i++) {
        fs->free_blocks[i] = 1;
    }
    strncpy(fs->fs_file, fs_path, MAX_PATH);

    file_entry* root = &fs->file_table[0];
    strcpy(root->filename, "/");
    root->is_directory = 1;
    root->permissions = S_IFDIR | 0755;
    root->in_use = 1;
    root->parent_dir = 0;
    root->created = root->modified = root->accessed = time(NULL);

    fs->num_files = 1;
    fs_save(fs);
    return fs;
}

filesystem* fs_load(const char* fs_path) {
    FILE* file = fopen(fs_path, "rb");
    if (!file) {
        sprintf(error_buffer, "Filesystem not found");
        return NULL;
    }

    filesystem* fs = (filesystem*)malloc(sizeof(filesystem));
    if (!fs) {
        sprintf(error_buffer, "Memory allocation failed");
        fclose(file);
        return NULL;
    }

    fread(fs, sizeof(filesystem), 1, file);
    fclose(file);
    return fs;
}

int fs_save(filesystem* fs) {
    FILE* file = fopen(fs->fs_file, "wb");
    if (!file) {
        sprintf(error_buffer, "Filesystem save failed");
        return -1;
    }
    fwrite(fs, sizeof(filesystem), 1, file);
    fclose(file);
    return 0;
}

void fs_destroy(filesystem* fs) {
    if (fs) free(fs);
}

int fs_create_directory(filesystem* fs, const char* path) {
    char dir[MAX_PATH], name[MAX_FILENAME];
    split_path(path, dir, name);

    for (int i = 0; i < MAX_FILES; i++) {
        if (!fs->file_table[i].in_use) {
            file_entry* new_dir = &fs->file_table[i];
            strcpy(new_dir->filename, name);
            new_dir->is_directory = 1;
            new_dir->permissions = S_IFDIR | 0755;
            new_dir->parent_dir = 0; // Assume root
            new_dir->in_use = 1;
            fs->num_files++;
            fs_save(fs);
            return 0;
        }
    }
    sprintf(error_buffer, "No space for new directory");
    return -1;
}

int fs_create_file(filesystem* fs, const char* path) {
    char dir[MAX_PATH];
    char name[MAX_FILENAME];
    split_path(path, dir, name);

    // Chercher le répertoire parent
    int parent_index = find_file_by_path(fs, dir);
    if (parent_index == -1) {
        sprintf(error_buffer, "Parent directory not found");
        return -1;
    }

    // Chercher un espace libre pour créer un nouveau fichier
    int file_index = -1;
    for (int i = 0; i < MAX_FILES; i++) {
        if (!fs->file_table[i].in_use) {
            file_index = i;
            break;
        }
    }

    if (file_index == -1) {
        sprintf(error_buffer, "No free file table entries");
        return -1;
    }

    // Initialiser l'entrée du fichier
    file_entry* new_file = &fs->file_table[file_index];
    strcpy(new_file->filename, name);
    new_file->is_directory = 0;  // Ce n'est pas un répertoire
    new_file->permissions = 0;  // Permissions initiales
    new_file->created = new_file->modified = new_file->accessed = time(NULL);
    new_file->in_use = 1;
    new_file->parent_dir = parent_index;

    fs->num_files++;
    fs_save(fs);

    return 0;
}

int fs_write(filesystem* fs, const char* path, const char* data, int size) {
    int file_index = find_file_by_path(fs, path);
    if (file_index == -1) {
        sprintf(error_buffer, "File not found");
        return -1;
    }
    file_entry* file = &fs->file_table[file_index];

    if (size > BLOCK_SIZE) size = BLOCK_SIZE; // Simplify block allocation
    file->start_block = 0;
    file->num_blocks = 1;
    memcpy(fs->blocks[0], data, size);
    file->size = size;
    file->modified = time(NULL);
    fs_save(fs);
    return 0;
}
int fs_export(filesystem* fs, const char* virtual_path, const char* real_path) {
    int file_index = find_file_by_path(fs, virtual_path);
    if (file_index == -1) {
        sprintf(error_buffer, "File not found: %s", virtual_path);
        return -1;
    }

    file_entry* file = &fs->file_table[file_index];
    if (file->is_directory) {
        sprintf(error_buffer, "Cannot export a directory");
        return -1;
    }

    FILE* output = fopen(real_path, "wb");
    if (!output) {
        sprintf(error_buffer, "Failed to open output file: %s", strerror(errno));
        return -1;
    }

    fwrite(fs->blocks[file->start_block], 1, file->size, output);
    fclose(output);
    return 0;
}


int fs_list_directory(filesystem* fs, const char* path) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs->file_table[i].in_use) {
            printf("%s\n", fs->file_table[i].filename);
        }
    }
    return 0;
}
int fs_chmod(filesystem* fs, const char* path, mode_t mode) {
    int index = find_file_by_path(fs, path);
    if (index == -1) {
        sprintf(error_buffer, "File or directory not found");
        return -1;
    }

    fs->file_table[index].permissions = mode;
    fs->file_table[index].modified = time(NULL); // Mise à jour de la date de modification
    fs_save(fs);

    return 0;
}
int fs_find_file_by_path(filesystem* fs, const char* path) {
    char dir[MAX_PATH];
    char name[MAX_FILENAME];
    split_path(path, dir, name);

    // Si c'est un chemin racine
    if (strcmp(dir, "/") == 0) {
        // Chercher le fichier dans le système de fichiers
        for (int i = 0; i < MAX_FILES; i++) {
            if (fs->file_table[i].in_use && strcmp(fs->file_table[i].filename, name) == 0) {
                return i;  // Trouvé, retourner l'index
            }
        }
    } else {
        // Si ce n'est pas dans la racine, effectuer la recherche dans le sous-dossier
        for (int i = 0; i < MAX_FILES; i++) {
            if (fs->file_table[i].in_use && strcmp(fs->file_table[i].filename, dir) == 0) {
                // Chercher dans ce répertoire
                for (int j = 0; j < MAX_FILES; j++) {
                    if (fs->file_table[j].in_use && strcmp(fs->file_table[j].filename, name) == 0) {
                        return j;
                    }
                }
            }
        }
    }

    return -1;  // Si non trouvé
}
