# Real-Time Memory Storage Management (C++)

## Overview

This project demonstrates a simple, custom memory manager implemented in C++. It provides a fixed-size memory pool and manually manages allocations and deallocations. Rather than relying on standard `new` and `delete`, this memory manager uses a single contiguous memory buffer and maintains a free list of available blocks. When you allocate memory, it finds a suitable block from the free list, splits it if necessary, and returns a pointer to the allocated space. When you free memory, it inserts the freed block back into the free list and attempts to merge it with any adjacent blocks to reduce fragmentation.

**Key Features:**
- A single contiguous memory pool allocated at startup.
- A free list data structure to track available memory.
- Ability to allocate and free memory in real-time without conventional heap usage.
- Demonstrates manual memory management concepts such as splitting blocks and merging freed blocks to minimize fragmentation.

**Note:** This code is a simplified educational example. It is not intended for production use and does not handle all edge cases or concurrency scenarios. No advanced optimization or error-checking is implemented.

## How it Works

1. **Initialisation:**
   - The memory manager is initialized with a fixed-size pool (e.g., 1024 bytes).
   - Initially, the entire pool is one large free block.

2. **Allocation (`allocate`):**
   - When you request memory of a certain size, the manager scans the free list for a block large enough to accommodate the request.
   - If found, it either uses the entire free block or splits it into a used portion and a smaller free block.
   - The function returns a pointer to the allocated region (just after the metadata).

3. **Deallocation (`deallocate`):**
   - When you free memory, the manager places that block back into the free list.
   - It then attempts to merge it with neighboring free blocks to reduce fragmentation.

4. **Data Structures:**
   - `FreeBlock` struct: Each free block contains a `size` and a pointer to the next free block.
   - The free list is a linked list of these `FreeBlock` structs, all stored within the same memory pool.

## How to Build and Run

1. **Prerequisites:**
   - A C++ compiler

2. **Compilation:**
   ```bash
   g++ -o memory_manager MemoryManager.cpp