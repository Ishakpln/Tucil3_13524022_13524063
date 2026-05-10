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

## Board Editor GUI

The Board Editor scene can create or edit files in the same format as `test/1.txt`.

Controls:
- Click a component button (`X`, `*`, `L`, `Z`, `O`, `0`-`9`) and left-click a board cell to paint it.
- Right-click a board cell to select it without painting.
- Click a cell in the cost grid to select the tile cost.
- `X` obstacle costs are fixed to `999`; other tile costs can be edited in the cost value box.
- Use the row/column value boxes and `APPLY SIZE` to resize the editable board.
- `SAVE BOARD` opens a popup. Press `OK` to write `./test/<board-name>.txt` and return to board selection.

Validation before saving:
- Exactly one `Z` start and one `O` goal.
- Checkpoint digits must start from `0` and be contiguous.
- Costs must be non-negative.
