# xv6-riscv Copilot Instructions

xv6-riscv is a teaching RISC-V operating system kernel for MIT's 6.1810 course. It implements a Unix Version 6 clone in ANSI C for a 64-bit multiprocessor RISC-V architecture, running under QEMU emulation.

## Build, Test, and Debug

### Building
- `make` - Build everything (kernel, user programs, filesystem image)
- `make kernel/kernel` - Build just the kernel
- `make fs.img` - Rebuild the filesystem image
- `make clean` - Clean all build artifacts
- `make fmt` - Format code with clang-format (run before committing)

### Running and Testing
- `make qemu` - Boot xv6 in QEMU (Ctrl+A then X to exit)
- `make qemu CPUS=N` - Boot with N CPUs (default is 3)
- `./test-xv6.py usertests` - Run full user tests
- `./test-xv6.py -q usertests` - Run quick user tests
- `./test-xv6.py log` - Run log crash test
- `./test-xv6.py crash` - Run all crash tests
- `./test-xv6.py forphan` - Run specific test by name (grep matching supported)
- Test output saved to `test-xv6.out`

### Debugging
- `make qemu-gdb` - Start QEMU with GDB stub enabled; run `gdb` in another window
- `make print-gdbport` - Show the GDB port being used
- GDB configuration auto-generated in `.gdbinit`

## Architecture

### High-Level Structure
The kernel (in `kernel/`) implements core OS functionality:
- **Process management** (`proc.c`, `proc.h`) - Process scheduling, context switching, fork/exec
- **Memory management** (`vm.c`, `kalloc.c`) - Virtual memory, page tables, physical page allocation
- **Filesystem** (`fs.c`, `fs.h`, `bio.c`) - Simple filesystem with inode-based structure, buffer cache
- **Interrupts & traps** (`trap.c`, `trampoline.S`, `plic.c`) - Exception handling, PLIC timer/device interrupts
- **Synchronization** (`spinlock.c`, `sleeplock.c`) - Spinlocks and sleep locks for mutual exclusion
- **Devices** (`uart.c`, `console.c`, `virtio_disk.c`) - Serial I/O, console, block device drivers

User programs in `user/` include utilities like `cat`, `ls`, `sh` (shell), and test programs. All kernel/user interfaces go through syscalls defined in `syscall.c` and `sysproc.c`.

The `mkfs/` directory contains a utility to build the filesystem image from user programs.

### Key Data Structures
- **struct proc** - Per-process state (registers, page table, open files)
- **struct trapframe** - Saved registers during user/kernel transitions (offsets documented in struct)
- **struct inode** - Filesystem inode (in-memory representation)
- **struct buf** - Buffer cache entry for disk blocks
- **struct file** - Open file descriptor (references inode or pipe)

## Conventions

### Naming and Organization
- Function names are lowercase with underscores: `proc_mapstacks()`, `kfork()`, `push_off()`
- Kernel functions often prefixed with `k`: `kfork()`, `kexit()`, `kkill()` to distinguish from syscall names (`fork()`, `exit()`, `kill()`)
- Global state includes structs for per-CPU and per-process data: `struct cpu cpus[NCPU]`, `struct proc proc[NPROC]`
- Include files use forward declarations of structs to minimize dependencies

### Code Style and Formatting
- **Include order** (do not sort automatically):
  1. System types header: `#include "types.h"`
  2. Configuration headers: `#include "param.h"`, `#include "memlayout.h"`, `#include "riscv.h"`
  3. Synchronization primitives: `#include "spinlock.h"`
  4. Kernel-specific headers: `#include "proc.h"`, `#include "defs.h"`
  5. User programs separately include: `#include "kernel/types.h"`, `#include "kernel/fcntl.h"`, `#include "user/user.h"`
- **Brace style**: Opening brace on new line for function definitions (enforced by `.clang-format`)
- **Comments**: Use `//` style; document constraints above function definitions (e.g., "Interrupts must be disabled")
- **Struct member documentation**: Include byte offsets in comments for packed structs (e.g., `/* 8 */ uint64 kernel_sp;`)
- Run `make fmt` to apply clang-format before committing

### Important Implementation Notes
- **Synchronization**: Functions accessing shared state must document their locking requirements
- **Global state initialization**: Global data initialized at startup (see `main.c`), not inline
- **Assembly bridges**: Critical paths use hand-written RISC-V assembly (`.S` files) for context switching, trampoline code, and syscall entry
- **Memory model**: Xv6 uses simple linear kernel memory mapping; user processes have separate page tables
- **Filesystem layout**: Single disk image with superblock, inode blocks, bitmap, and data blocks

### Tools and Environment
- **Toolchain**: RISC-V 64-bit (riscv64-unknown-elf, riscv64-elf, riscv64-none-elf, or riscv64-linux-gnu prefixes auto-detected)
- **Emulator**: QEMU 7.2+ required (`qemu-system-riscv64`)
- **Compiler flags**: Flags disable standard library builtins and PIE, enable freestanding/baremetal compilation
- **GDB debugging**: Full symbol information included with `-ggdb -gdwarf-2`
