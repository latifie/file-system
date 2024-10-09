#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "block.h"

// Alloue un nouveau bloc de données (retourne le numéro de bloc)
int alloc_block(int fd, struct superblock *sb, struct bitmap *block_bitmap) {
    for (int i = 0; i < sb->block_count; i++) {
        if (!block_bitmap->blocks[i]) {
            block_bitmap->blocks[i] = 1; // Marque le bloc comme alloué
            sb->free_blocks--; // Diminue le nombre de blocs libres
            return i;
        }
    }
    return -1; // Aucun bloc libre
}

// Lecture d'un bloc de données
void read_block(int fd, int block_number, void *buffer) {
    lseek(fd, BLOCK_OFFSET(block_number), SEEK_SET);
    if (read(fd, buffer, BLOCK_SIZE) != BLOCK_SIZE) {
        perror("Erreur lors de la lecture du bloc");
        exit(1);
    }
}

// Écriture d'un bloc de données
void write_block(int fd, int block_number, const void *buffer) {
    lseek(fd, BLOCK_OFFSET(block_number), SEEK_SET);
    if (write(fd, buffer, BLOCK_SIZE) != BLOCK_SIZE) {
        perror("Erreur lors de l'écriture du bloc");
        exit(1);
    }
}
