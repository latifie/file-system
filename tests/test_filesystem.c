#include <assert.h>
#include <fcntl.h>
#include "filesystem.h"
#include "superblock.h"

void test_format_filesystem() {
    int fd = open("test_fs.img", O_RDWR | O_CREAT, 0666);
    assert(fd >= 0);

    format_filesystem(fd);

    struct superblock sb;
    read_superblock(fd, &sb);
    assert(sb.magic == FS_MAGIC);  // Vérification que le superbloc a été initialisé
    printf("test_format_filesystem: OK\n");

    close(fd);
}

void test_mount_filesystem() {
    int fd = open("test_fs.img", O_RDONLY);
    assert(fd >= 0);

    mount_filesystem(fd);
    printf("test_mount_filesystem: OK\n");

    close(fd);
}

int main() {
    test_format_filesystem();
    test_mount_filesystem();
    return 0;
}
