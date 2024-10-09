#ifndef BLOCK_H
#define BLOCK_H

#include "fs_simple.h"

#define BLOCK_OFFSET(block_number) (BLOCK_SIZE * block_number)
#define BLOCK_SIZE 1024  // Taille des blocs de données

// Fonctions de gestion des blocs de données
int alloc_block(int fd, struct superblock *sb, struct bitmap *block_bitmap);
void read_block(int fd, int block_number, void *buffer);
void write_block(int fd, int block_number, const void *buffer);

#endif
