# rv-toolchain-docker

Builds a Docker image with the RISC-V GNU toolchain and generates local wrapper scripts.

## Important Behavior

This setup does not run a persistent container.

- `build.sh` builds image `francisstokes/rv-toolchain:latest`
- `generate-bin-dir.sh` creates wrappers in `./bin`
- each wrapper starts a short-lived `docker run ...` command and exits

## Usage

```bash
./build.sh
export PATH="$(pwd)/bin:$PATH"
riscv64-unknown-linux-gnu-gcc --version
```

If the version command works, wrapper -> Docker -> toolchain is wired correctly.
