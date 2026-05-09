WINDOWS install raylib
using mysys2-ucrt64
1. Masuk ke terminal ucrt64, lakukan update 
pacman -Syu
2. Ketikkan perintah ini untuk instalasi 
pacman -S mingw-w64-ucrt-x86_64-raylib


WSL to raylib static linking

sudo apt update
sudo apt install build-essential git cmake \
    libasound2-dev mesa-common-dev libx11-dev libxrandr-dev \
    libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake .. -DPLATFORM=Desktop
make
sudo make install


run gui from wsl
make clean && make rung
*make run untuk cli, rung untuk gui