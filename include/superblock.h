#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include "fs_simple.h"

struct superblock {
    int magic;         // Identifiant du système de fichiers
    int block_size;    // Taille des blocs en octets
    int inode_count;   // Nombre total d'inodes
    int block_count;   // Nombre total de blocs
    int free_blocks;   // Nombre de blocs libres
    int free_inodes;   // Nombre d'inodes libres
};

// Fonctions de gestion du superbloc
void init_superblock(struct superblock *sb);
void read_superblock(int fd, struct superblock *sb);
void write_superblock(int fd, const struct superblock *sb);

#endif
