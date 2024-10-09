#include <assert.h>
#include "inode.h"

void test_create_inode() {
    struct inode *inode = create_inode(1, S_IFREG);
    assert(inode->mode == S_IFREG);
    assert(inode->size == 0);
    free(inode);
    printf("test_create_inode: OK\n");
}

void test_read_write_inode() {
    struct inode inode, read_inode;
    create_inode(1, S_IFREG);

    int fd = open("test_fs.img", O_RDWR | O_CREAT, 0666);
    write_inode(fd, 1, &inode);
    read_inode(fd, 1, &read_inode);

    assert(inode.size == read_inode.size);
    printf("test_read_write_inode: OK\n");
    close(fd);
}

int main() {
    test_create_inode();
    test_read_write_inode();
    return 0;
}
