#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bitmap.h"

// Alloue un inode libre
int alloc_inode(struct bitmap *inode_bitmap, struct superblock *sb) {
    for (int i = 0; i < sb->inode_count; i++) {
        if (!inode_bitmap->inodes[i]) {
            inode_bitmap->inodes[i] = 1; // Marque l'inode comme alloué
            sb->free_inodes--;
            return i;
        }
    }
    return -1; // Aucun inode libre
}

// Libère un inode
void free_inode(int inode_number, struct bitmap *inode_bitmap, struct superblock *sb) {
    if (inode_bitmap->inodes[inode_number]) {
        inode_bitmap->inodes[inode_number] = 0; // Libère l'inode
        sb->free_inodes++;
    }
}

// Libère un bloc de données
void free_block(int block_number, struct bitmap *block_bitmap, struct superblock *sb) {
    if (block_bitmap->blocks[block_number]) {
        block_bitmap->blocks[block_number] = 0; // Libère le bloc
        sb->free_blocks++;
    }
}
