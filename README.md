# 🐉 DragonHeap

A custom memory allocator implementation in C that manages memory like a dragon hoards treasure.

## Overview

DragonHeap is a learning-focused memory allocator that provides an alternative to the standard library's `malloc`, `realloc`, and `free` functions. It uses a linked list architecture to track allocated memory chunks and maintains a free list for efficient memory reuse.

> ⚠️ **Note:** This is an educational project built for learning purposes and portfolio demonstration. It is not intended for production use.

<img width="1077" height="763" alt="Screenshot 2025-07-24 210138" src="https://github.com/user-attachments/assets/501f973d-27a2-4af2-8c87-a82d64d725f9" />

## Features

- **Custom Memory Management**: Three core functions that mirror standard library behavior
- **Automatic Zero Initialization**: All allocated memory is automatically cleared, so no realloc
- **Free List Optimization**: Reuses freed memory chunks when possible
- **Metadata Tracking**: Maintains allocation information alongside user data
- **Debug Utilities**: Built-in functions to inspect memory state

## API Reference

### Core Functions

```c
void* dragon_hoard(size_t requested_size);
```
```malloc```: Allocates memory of the specified size. Returns a pointer to the allocated memory region, automatically initialized to zero.

```c
void* dragon_reforge(void* hoard, size_t revised_size);
```
```realloc```: Resizes an existing memory allocation. The `revised_size` must be greater than the original size, otherwise returns an error.

```c
void dragon_release(void* hoard);
```
```free```: Frees the memory pointed to by `hoard` and adds it to the free list for potential reuse.

### Debug Functions

```c
void print_dragonheap(void);
void print_free_list(void);
```
Utility functions to print the current state of all memory chunks and the free list respectively.

## Architecture

<img width="1005" height="537" alt="Screenshot 2025-07-24 205558" src="https://github.com/user-attachments/assets/8c123f2d-7da1-4540-96fa-cf3a1b586574" />

DragonHeap maintains two primary data structures:

1. **Memory Chunks List**: A linked list tracking all memory regions requested from the system
2. **Free List**: A separate linked list of freed chunks available for reuse

Each memory chunk contains metadata stored before the user-accessible region. When memory is requested, DragonHeap first searches the free list for suitable chunks before requesting new memory from the system.

## Building

### Prerequisites
- GCC compiler
- Make
- Linux/WSL environment

### Build Commands

```bash
# Build static library only
make lib

# Build library and example program
make
```

This will generate:
- `libdragonheap.a` - Static library file
- `example` - Test executable (when using `make`)

## Project Structure

```
DragonHeap/
├── src/
│   ├── dragonheap.c      # Main allocator implementation
│   └── [other .c files]  # Additional source files
├── dragonheap.h          # Public API header
├── example.c             # Usage examples and tests
├── Makefile              # Build configuration
└── README.md             # This file
```

## Usage Example

```c
#include "dragonheap.h"

int main(void) {
    // Allocate memory
    void* ptr1 = dragon_hoard(100);
    
    // Resize allocation
    void* ptr2 = dragon_reforge(ptr1, 200);
    
    // Free memory
    dragon_release(ptr2);
    
    // Debug: print current state
    print_dragonheap();
    print_free_list();
    
    return 0;
}
```

## Learning Objectives

This project demonstrates:
- Custom memory management implementation
- Linked list data structures
- Metadata management in memory allocators
- Static library creation and linking
- C programming best practices

---

*Built for educational purposes and portfolio demonstration* 🎓
