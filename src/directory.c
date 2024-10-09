#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory.h"
#include "inode.h"

// Crée un répertoire vide
int create_directory(int inode_number) {
    struct directory dir;
    memset(dir.entries, 0, sizeof(dir.entries));  // Initialise les entrées à 0
    dir.inode_number = inode_number;
    return inode_number;
}

// Ajoute une nouvelle entrée dans un répertoire
int add_directory_entry(struct directory *dir, int inode_number, const char *filename) {
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (dir->entries[i].inode_number == 0) {
            dir->entries[i].inode_number = inode_number;
            strncpy(dir->entries[i].filename, filename, MAX_FILENAME_LEN);
            return 0;  // Ajout réussi
        }
    }
    return -1;  // Aucun espace disponible
}

// Trouve une entrée de répertoire par son nom
int find_directory_entry(const struct directory *dir, const char *filename) {
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (strcmp(dir->entries[i].filename, filename) == 0) {
            return dir->entries[i].inode_number;  // Retourne l'inode du fichier trouvé
        }
    }
    return -1;  // Fichier non trouvé
}
