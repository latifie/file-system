#include <assert.h>
#include "block.h"
#include "bitmap.h"
#include "superblock.h"

void test_alloc_block() {
    struct superblock sb = {0};
    struct bitmap block_bitmap = {0};
    sb.block_count = DEFAULT_BLOCK_COUNT;

    int block_number = alloc_block(0, &sb, &block_bitmap);
    assert(block_number == 0);  // Vérification que le premier bloc est alloué
    printf("test_alloc_block: OK\n");
}

void test_free_block() {
    struct superblock sb = {0};
    struct bitmap block_bitmap = {0};
    sb.block_count = DEFAULT_BLOCK_COUNT;

    int block_number = alloc_block(0, &sb, &block_bitmap);
    free_block(block_number, &block_bitmap, &sb);
    assert(block_bitmap.blocks[block_number] == 0);  // Vérification que le bloc est libéré
    printf("test_free_block: OK\n");
}

int main() {
    test_alloc_block();
    test_free_block();
    return 0;
}
