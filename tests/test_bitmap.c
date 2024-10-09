#include <assert.h>
#include "bitmap.h"
#include "superblock.h"

void test_alloc_inode() {
    struct bitmap inode_bitmap = {0};
    struct superblock sb = {0};
    sb.inode_count = DEFAULT_INODE_COUNT;
    
    int inode_number = alloc_inode(&inode_bitmap, &sb);
    assert(inode_number == 0);  // Vérification que le premier inode est alloué
    printf("test_alloc_inode: OK\n");
}

void test_free_inode() {
    struct bitmap inode_bitmap = {0};
    struct superblock sb = {0};
    sb.inode_count = DEFAULT_INODE_COUNT;

    int inode_number = alloc_inode(&inode_bitmap, &sb);
    free_inode(inode_number, &inode_bitmap, &sb);
    assert(inode_bitmap.inodes[inode_number] == 0);  // Vérification que l'inode est libéré
    printf("test_free_inode: OK\n");
}

int main() {
    test_alloc_inode();
    test_free_inode();
    return 0;
}
