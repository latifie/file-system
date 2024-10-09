#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "filesystem.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <disk_image>\n", argv[0]);
        return 1;
    }
    
    int fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("Erreur lors de l'ouverture du fichier de disque");
        return 1;
    }

    format_filesystem(fd);
    printf("Système de fichiers créé avec succès sur %s\n", argv[1]);
    close(fd);
    return 0;
}
