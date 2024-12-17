#include <iostream>
#include <cstddef>
#include <cstring>

struct FreeBlock {
    size_t size;
    FreeBlock* next;
};

class MemoryManager {
    public:
        MemoryManager(size_t poolSize) {
            pool = new unsigned char[poolSize];
            totalSize = poolSize;

            freeList = reinterpret_cast<FreeBlock*>(pool);
            freeList->size = poolSize - sizeof(FreeBlock); 
            freeList->next = nullptr;
        }
        ~MemoryManager() {
            delete[] pool;
        }

        void* allocate(size_t size) {
            size = align(size);
            FreeBlock* prev = nullptr;
            FreeBlock* current = freeList;

            while (current) {
                if (current->size >= size) {
                    size_t remaining = current->size - size;

                    if (remaining > sizeof(FreeBlock)) {
                        unsigned char* splitAddr = reinterpret_cast<unsigned char*>(current) + sizeof(FreeBlock) + size;
                        FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(splitAddr);
                        newBlock->size = remaining - sizeof(FreeBlock);
                        newBlock->next = current->next;

                        current->size = size;
                        current->next = nullptr;

                        if (prev) {
                            prev->next = newBlock;
                        }
                        else {
                            freeList = newBlock;
                        }
                    }
                    else {
                        if (prev) {
                            prev->next = current->next;
                        }
                        else {
                            freeList = current->next;
                        }
                    }
                    return reinterpret_cast<unsigned char*>(current) + sizeof(FreeBlock);
                }
                prev = current;
                current = current->next;
            }
            return nullptr;
        }

        void deallocate(void* ptr) {
            if (!ptr) return;
            
            unsigned char* blockStart = reinterpret_cast<unsigned char*>(ptr) - sizeof(FreeBlock);
            FreeBlock* block = reinterpret_cast<FreeBlock*>(blockStart);

            FreeBlock* prev = nullptr;
            FreeBlock* current = freeList;

            while (current && current < block) {
                prev = current;
                current = current->next;
            }

            block->next = current;
            if (prev) {
                prev->next = block;
            }
            else {
                freeList = block;
            }
            mergeBlocks(block, current);
            if (prev) {
                mergeBlocks(prev, block);
            }
        }   
    private:
        unsigned char* pool;
        size_t totalSize;
        FreeBlock* freeList;
        
        size_t align(size_t size) {
            const size_t alignment = 8;
            return (size + alignment - 1) & ~(alignment - 1);
        }
        
        void mergeBlocks(FreeBlock* blockA, FreeBlock* blockB) {
            if (!blockA || !blockB) return;

            unsigned char*blockAEnd = reinterpret_cast<unsigned char*>(blockA) + sizeof(FreeBlock) + blockA->size;
            unsigned char*blockBStart = reinterpret_cast<unsigned char*>(blockB);
            if (blockAEnd == blockBStart) {
                blockA->size += sizeof(FreeBlock) + blockB->size;
                blockA->next = blockB->next;
            }
        }
};

int main() {
    MemoryManager mm(1024);
    void* a = mm.allocate(100);
    void* b = mm.allocate(200);
    void* c = mm.allocate(50);

    if (a) std::memset(a, 0, 100);
    if (b) std::memset(b, 0xFF, 200);
    if (c) std::memset(c, 0xAA, 50);

    mm.deallocate(b);
    mm.deallocate(a);
    mm.deallocate(c);

    return 0;
}