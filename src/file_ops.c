#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "inode.h"
#include "block.h"

// Lit les données d'un fichier à partir de son inode
ssize_t read_file(int fd, int inode_number, void *buffer, size_t count) {
    struct inode inode;
    read_inode(fd, inode_number, &inode);
    
    if (count > inode.size) {
        count = inode.size;  // Limite la lecture à la taille du fichier
    }

    int block_number = inode.blocks[0];  // Simplification : on lit un seul bloc
    read_block(fd, block_number, buffer);

    return count;
}

// Écrit des données dans un fichier à partir de son inode
ssize_t write_file(int fd, int inode_number, const void *buffer, size_t count) {
    struct inode inode;
    read_inode(fd, inode_number, &inode);

    int block_number = inode.blocks[0];  // Simplification : on écrit un seul bloc
    write_block(fd, block_number, buffer);

    inode.size = count;  // Mise à jour de la taille du fichier
    write_inode(fd, inode_number, &inode);

    return count;
}

// Supprime un fichier à partir de son inode
int delete_file(int fd, int inode_number, struct bitmap *inode_bitmap, struct bitmap *block_bitmap, struct superblock *sb) {
    struct inode inode;
    read_inode(fd, inode_number, &inode);

    // Libère les blocs alloués à ce fichier
    for (int i = 0; i < 12; i++) {
        if (inode.blocks[i] != 0) {
            free_block(inode.blocks[i], block_bitmap, sb);
        }
    }

    // Libère l'inode
    free_inode(inode_number, inode_bitmap, sb);

    return 0;
}
