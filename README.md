# xv6-riscv

A teaching RISC-V operating system implementing Unix Version 6, for MIT's 6.1810 course.

## Quick Start with Docker

### 1. Build the Toolchain Container
```bash
cd rv-toolchain-docker
./build.sh
```
This builds the `francisstokes/rv-toolchain:latest` Docker image and generates wrapper scripts in `./bin/`.

### 2. Add Toolchain to PATH
```bash
export PATH=$(pwd)/bin:$PATH
```
The wrapper scripts automatically run RISC-V tools inside the container.

### 3. Compile xv6
```bash
cd xv6-riscv
make
```

### 4. Run in QEMU
```bash
make qemu
```

### 5. Debug with GDB
**Terminal 1** (start QEMU with debugger):
```bash
make qemu-gdb
```

**Terminal 2** (connect debugger):
```bash
gdb
```

## How It Works

- **Docker image**: Ubuntu 18.04 with RISC-V 64-bit GCC/binutils toolchain at `/opt/riscv/`
- **Wrapper scripts**: Located in `rv-toolchain-docker/bin/`, each tool (e.g., `riscv64-unknown-linux-gnu-gcc`) is a shell script that runs the command inside the container with your working directory mounted
- **Direct host usage**: If you've installed the RISC-V toolchain natively on your host, skip Docker entirely and run `make` normally

## More Info

- **Kernel code**: `xv6-riscv/kernel/` – Core OS functionality
- **User programs**: `xv6-riscv/user/` – Utilities and tests
- **Build details**: See `.github/copilot-instructions.md` for comprehensive build/test/debug commands
- **Original README**: `xv6-riscv/README` – Acknowledgments and references

## Testing
```bash
cd xv6-riscv
./test-xv6.py usertests       # Run full user tests
./test-xv6.py -q usertests    # Quick tests
./test-xv6.py crash           # Crash tests
```

## Code Formatting
```bash
cd xv6-riscv
make fmt
```
