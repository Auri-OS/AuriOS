echo "[!] Installing cross compiler for i686-elf"
mkdir -p ~/opt/cross
cd ~/opt/cross
wget https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz
tar -xf binutils-2.41.tar.gz
tar -xf gcc-13.2.0.tar.gz
mkdir build-binutils build-gcc
echo "[!] Building and installing binutils"
cd build-binutils
../binutils-2.41/configure --target=i686-elf --prefix=$HOME/opt/cross --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install
cd ..
echo "[!] Building and installing GCC"
cd build-gcc
../gcc-13.2.0/configure --target=i686-elf --prefix=$HOME/opt/cross --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make install-gcc
make install-target-libgcc
cd ..
echo "[!] Cross compiler installed successfully"
echo 'export PATH="$HOME/opt/cross/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc