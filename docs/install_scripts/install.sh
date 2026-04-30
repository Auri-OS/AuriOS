#!/bin/sh
set -e
readonly BINUTILS_VERSION="2.41"
readonly GCC_VERSION="13.2.0"
readonly THREADS=$(nproc)
readonly PREFIX="$PWD/.environment"

echo "[!] Installing cross compiler for i686-elf"
mkdir -p "$PREFIX/opt"
cd "$PREFIX/opt"
if [ ! -d binutils ]; then
	wget -O binutils.tar.gz "https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz"
	tar -xf binutils.tar.gz
	rm binutils.tar.gz
	mv binutils* binutils
fi
if [ ! -d gcc ]; then
	wget -O gcc.tar.gz "https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-$GCC_VERSION.tar.gz"
	tar -xf gcc.tar.gz
	rm gcc.tar.gz
	mv gcc* gcc
fi

echo "[!] Building and installing binutils"
cd binutils
mkdir -p build
cd build
if [ ! -f Makefile ]; then
	../configure --target=i686-elf --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
fi
make -j$THREADS
make install

cd "$PREFIX/opt"

echo "[!] Building and installing GCC"
cd gcc
mkdir -p build
cd build
if [ ! -f Makefile ]; then
	../configure --target=i686-elf --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
fi
make all-gcc -j$THREADS
make all-target-libgcc -j$THREADS
make install-gcc
make install-target-libgcc

cd "$PREFIX"

echo "[!] Cross compiler installed successfully"

cat > ./activate << EOF
export PATH="$PREFIX/bin:\$PATH"
EOF

printf "\nactivate with: \n\t. $PREFIX/activate\n"
