# xv6-riscv

This repository contains MIT 6.1810 xv6 for RISC-V, plus a Docker-based cross-toolchain helper.


## Prerequisites (Host Machine)

You still need host tools for anything that is not the cross-compiler itself:

- Docker (daemon running)
- `make`
- `qemu-system-riscv` (for `make qemu`)
- `bc` (used by the Makefile's QEMU version check)

Optional:

- `clang-format` (for `make fmt`)

## Sanity Check Before Building

From `rv-toolchain-docker/`:

```bash
qemu-system-riscv64 --version
riscv64-unknown-linux-gnu-gcc --version
```

## Quick Start (Docker Toolchain)

```bash
cd xv6-riscv
make qemu
```
xv6 has no ps command, but, if you type `Ctrl-p`, the kernel will print information about each process

To quit qemu type: `Ctrl-a x` 

## Running Without Docker

If you have a local RISC-V cross-toolchain installed already, you can skip `rv-toolchain-docker/` and build xv6 directly.

## Useful Commands

Build and run:

```bash
cd xv6-riscv
make
make qemu
```

Debug:

```bash
cd xv6-riscv
make qemu-gdb
# in another terminal
gdb
```

Tests:

```bash
cd xv6-riscv
./test-xv6.py usertests
./test-xv6.py -q usertests
./test-xv6.py crash
```

Format:

```bash
cd xv6-riscv
make fmt
```

