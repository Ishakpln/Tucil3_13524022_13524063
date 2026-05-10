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
GUI asset rendering notes
- Runtime working directory should be the repository root, so paths like ./assets/components and ./assets/players are valid.
- GUI assets are loaded through RAII wrappers in include/view/assets and src/view/assets, not inside the frame loop.
- Board logic still uses X, *, L, Z, O, and 0-9. Asset classes only affect rendering.
- Build GUI with raylib installed: make clean && make rung
- Build CLI only: make clean && make cli
