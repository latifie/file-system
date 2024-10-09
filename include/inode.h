#ifndef INODE_H
#define INODE_H

#include <sys/types.h>
#include "fs_simple.h"

#define INODE_TABLE_OFFSET 1024  // Offset où commence la table des inodes

struct inode {
    int inode_number;  // Numéro de l'inode
    mode_t mode;       // Mode (permissions, type de fichier)
    size_t size;       // Taille du fichier
    int blocks[12];    // Pointeurs vers les blocs de données
};

// Fonctions de gestion des inodes
struct inode *create_inode(int inode_number, mode_t mode);
void read_inode(int fd, int inode_number, struct inode *inode);
void write_inode(int fd, int inode_number, const struct inode *inode);

#endif
