#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "superblock.h"

// Initialisation du superbloc avec des valeurs par défaut
void init_superblock(struct superblock *sb) {
    sb->magic = FS_MAGIC;
    sb->block_size = DEFAULT_BLOCK_SIZE;
    sb->inode_count = DEFAULT_INODE_COUNT;
    sb->block_count = DEFAULT_BLOCK_COUNT;
    sb->free_blocks = sb->block_count;
    sb->free_inodes = sb->inode_count;
}

// Lecture du superbloc depuis un fichier
void read_superblock(int fd, struct superblock *sb) {
    lseek(fd, 0, SEEK_SET);
    if (read(fd, sb, sizeof(struct superblock)) != sizeof(struct superblock)) {
        perror("Erreur lors de la lecture du superbloc");
        exit(1);
    }
}

// Écriture du superbloc dans un fichier
void write_superblock(int fd, const struct superblock *sb) {
    lseek(fd, 0, SEEK_SET);
    if (write(fd, sb, sizeof(struct superblock)) != sizeof(struct superblock)) {
        perror("Erreur lors de l'écriture du superbloc");
        exit(1);
    }
}
