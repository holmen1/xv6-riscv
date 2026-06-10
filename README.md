# xv6-riscv

This repository contains MIT 6.1810 xv6 for RISC-V, plus a Docker-based cross-toolchain helper.

## What Is Actually Running in Docker?

Important: there is no long-running "toolchain container" process.

- `rv-toolchain-docker/build.sh` builds an image named `francisstokes/rv-toolchain:latest`.
- `rv-toolchain-docker/generate-bin-dir.sh` creates wrapper scripts in `rv-toolchain-docker/bin/`.
- Each wrapper runs one command in a short-lived container, then exits.

So if you expected to see a container "staying up", that is not how this setup works.

## Prerequisites (Host Machine)

You still need host tools for anything that is not the cross-compiler itself:

- Docker (daemon running)
- `make`
- `qemu-system-riscv64` (for `make qemu`)
- `bc` (used by the Makefile's QEMU version check)

Optional:

- `clang-format` (for `make fmt`)

## Quick Start (Docker Toolchain)

From repository root:

```bash
cd rv-toolchain-docker
./build.sh
export PATH="$(pwd)/bin:$PATH"

cd ../xv6-riscv
make
make qemu
```

## Sanity Check Before Building

From `rv-toolchain-docker/`:

```bash
which riscv64-unknown-linux-gnu-gcc
riscv64-unknown-linux-gnu-gcc --version
docker images | grep rv-toolchain
```

Expected behavior:

- `which` points to `rv-toolchain-docker/bin/riscv64-unknown-linux-gnu-gcc`
- `--version` works (this proves Docker wrapper execution works)

## Why "Container Never Started" Happens

Typical causes:

1. Docker daemon is not running.
2. `PATH` does not include `rv-toolchain-docker/bin`.
3. You exported `PATH` in one shell but built in another.
4. QEMU is missing, so compile succeeds but `make qemu` fails.
5. Non-interactive environments may dislike wrapper scripts using `docker run -it`.

## Wrapper Script Model (No Magic)

Each generated tool script is basically:

```sh
docker run -v "$(pwd)":/project -w /project -it francisstokes/rv-toolchain:latest <tool> "$@"
```

That means:

- your current directory is mounted into `/project` in the container
- command runs inside container
- output appears in your shell
- container exits immediately after command finishes

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

