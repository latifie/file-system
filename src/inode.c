#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "inode.h"

// Crée un nouvel inode avec les attributs par défaut
struct inode *create_inode(int inode_number, mode_t mode) {
    struct inode *inode = (struct inode *)malloc(sizeof(struct inode));
    if (!inode) {
        perror("Erreur d'allocation d'inode");
        exit(1);
    }
    
    inode->inode_number = inode_number;
    inode->mode = mode;
    inode->size = 0;
    memset(inode->blocks, 0, sizeof(inode->blocks));

    return inode;
}

// Lecture d'un inode depuis un fichier
void read_inode(int fd, int inode_number, struct inode *inode) {
    lseek(fd, INODE_TABLE_OFFSET + inode_number * sizeof(struct inode), SEEK_SET);
    if (read(fd, inode, sizeof(struct inode)) != sizeof(struct inode)) {
        perror("Erreur lors de la lecture de l'inode");
        exit(1);
    }
}

// Écriture d'un inode dans un fichier
void write_inode(int fd, int inode_number, const struct inode *inode) {
    lseek(fd, INODE_TABLE_OFFSET + inode_number * sizeof(struct inode), SEEK_SET);
    if (write(fd, inode, sizeof(struct inode)) != sizeof(struct inode)) {
        perror("Erreur lors de l'écriture de l'inode");
        exit(1);
    }
}
