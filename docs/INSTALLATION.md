# Installation tutorial :

## Step 1:

for fedora :
```bash
sudo dnf install gcc gcc-c++ binutils make wget tar
```

for debian based :
```bash
sudo apt install gcc gcc-c++ binutils make wget tar
```

for arch based :
```bash
sudo pacman -S gcc gcc-c++ binutils make wget tar
```

## Step 2 :

install cross compiler

```bash
mkdir -p ~/opt/cross
cd ~/opt/cross
wget https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz
tar -xf binutils-2.41.tar.gz
tar -xf gcc-13.2.0.tar.gz
mkdir build-binutils build-gcc
```

it can take several minutes, be patient...
## Step 3:
Dependancies : 

```bash
sudo dnf install texinfo gmp-devel mpfr-devel libmpc-devel
```
## Step 4:
Build Binutils :

```bash
cd build-binutils
../binutils-2.41/configure --target=i686-elf --prefix=$HOME/opt/cross --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install
cd ..
```

Build GCC:
```bash
cd build-gcc
../gcc-13.2.0/configure --target=i686-elf --prefix=$HOME/opt/cross --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make install-gcc
    make install-target-libgcc
cd ..
```
## Step 5:
Adding Env Var :

```bash
echo 'export PATH="$HOME/opt/cross/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```
