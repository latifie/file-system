#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "filesystem.h"
#include "superblock.h"
#include "inode.h"
#include "block.h"
#include "bitmap.h"

// Formate le système de fichiers
void format_filesystem(int fd) {
    struct superblock sb;
    init_superblock(&sb);
    
    // Initialisation des bitmaps
    struct bitmap inode_bitmap, block_bitmap;
    memset(inode_bitmap.inodes, 0, sizeof(inode_bitmap.inodes));
    memset(block_bitmap.blocks, 0, sizeof(block_bitmap.blocks));

    // Écriture du superbloc
    write_superblock(fd, &sb);

    // Écriture des bitmaps dans le fichier
    lseek(fd, INODE_BITMAP_OFFSET, SEEK_SET);
    write(fd, inode_bitmap.inodes, sizeof(inode_bitmap.inodes));
    
    lseek(fd, BLOCK_BITMAP_OFFSET, SEEK_SET);
    write(fd, block_bitmap.blocks, sizeof(block_bitmap.blocks));
}

// Monte le système de fichiers
void mount_filesystem(int fd) {
    struct superblock sb;
    read_superblock(fd, &sb);
    printf("Système de fichiers monté. Taille des blocs : %d, Nombre de blocs : %d\n", sb.block_size, sb.block_count);
}

// Démontage du système de fichiers
void unmount_filesystem(int fd) {
    close(fd);
    printf("Système de fichiers démonté\n");
}
