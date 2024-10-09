#include <assert.h>
#include "superblock.h"

void test_init_superblock() {
    struct superblock sb;
    init_superblock(&sb);
    
    assert(sb.magic == FS_MAGIC);
    assert(sb.block_size == DEFAULT_BLOCK_SIZE);
    printf("test_init_superblock: OK\n");
}

void test_read_write_superblock() {
    struct superblock sb, sb_read;
    init_superblock(&sb);

    int fd = open("test_fs.img", O_RDWR | O_CREAT, 0666);
    write_superblock(fd, &sb);
    read_superblock(fd, &sb_read);

    assert(sb.magic == sb_read.magic);
    printf("test_read_write_superblock: OK\n");
    close(fd);
}

int main() {
    test_init_superblock();
    test_read_write_superblock();
    return 0;
}
