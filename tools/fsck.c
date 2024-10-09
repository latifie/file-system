#include <fcntl.h>
#include <stdio.h>
#include "filesystem.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <disk_image>\n", argv[0]);
        return 1;
    }
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Erreur lors de l'ouverture du fichier de disque");
        return 1;
    }

    mount_filesystem(fd);
    printf("Vérification du système de fichiers réussie\n");
    close(fd);
    return 0;
}
