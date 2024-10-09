#ifndef BITMAP_H
#define BITMAP_H

#define INODE_BITMAP_OFFSET 2048  // Offset où commence le bitmap des inodes
#define BLOCK_BITMAP_OFFSET 4096  // Offset où commence le bitmap des blocs

struct bitmap {
    unsigned char inodes[DEFAULT_INODE_COUNT];  // Bitmap des inodes
    unsigned char blocks[DEFAULT_BLOCK_COUNT];  // Bitmap des blocs
};

// Fonctions de gestion des bitmaps
int alloc_inode(struct bitmap *inode_bitmap, struct superblock *sb);
void free_inode(int inode_number, struct bitmap *inode_bitmap, struct superblock *sb);
void free_block(int block_number, struct bitmap *block_bitmap, struct superblock *sb);

#endif
