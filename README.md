# Ice Sliding Puzzle Solver

## Penjelasan Singkat Program

Program ini adalah penyelesai permainan **Ice Sliding Puzzle** berbasis C++.
Pemain berada pada papan es dan akan terus meluncur ke arah yang dipilih sampai menabrak obstacle, keluar papan, terkena lava, atau mencapai tujuan tertentu.

Program memiliki dua mode utama:

1. **CLI**
   - Menjalankan solver melalui terminal.
   - Digunakan untuk memilih algoritma, membaca file papan, dan menampilkan hasil pencarian.

2. **GUI**
   - Dibuat menggunakan **raylib** dan **raygui**.
   - Menyediakan menu utama, pemilihan player, pemilihan board, mode manual play, mode algorithm play, serta board editor.
   - Board tetap direpresentasikan secara logis menggunakan karakter:
     - `X` = obstacle
     - `*` = path/jalur
     - `L` = lava
     - `Z` = posisi awal player
     - `O` = goal
     - `0-9` = checkpoint berurutan

Algoritma pencarian yang tersedia:

- A*
- UCS
- GBFS
- BFS
- DFS

## Requirement Program

Program membutuhkan:

- C++ compiler dengan dukungan C++17
- `make` atau `mingw32-make`
- raylib
- raygui sudah tersedia di dalam repository pada folder `src/library` dan `include/library`

### Instalasi raylib di Windows

Disarankan menggunakan **MSYS2 UCRT64**.

1. Buka terminal **MSYS2 UCRT64**.
2. Update package:

```bash
pacman -Syu
```

3. Install raylib:

```bash
pacman -S mingw-w64-ucrt-x86_64-raylib
```

### Instalasi raylib di Linux/WSL

```bash
sudo apt update
sudo apt install build-essential git cmake \
    libasound2-dev mesa-common-dev libx11-dev libxrandr-dev \
    libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```

Jika raylib belum tersedia dari package manager, raylib dapat dikompilasi dari source:

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build
cd build
cmake .. -DPLATFORM=Desktop
make
sudo make install
```

## Cara Mengkompilasi Program

Pastikan terminal berada pada root repository, yaitu folder yang berisi `makefile`.

### Compile CLI

Windows:

```powershell
mingw32-make cli
```

Linux/WSL:

```bash
make cli
```

### Compile GUI

Windows:

```powershell
mingw32-make gui
```

Linux/WSL:

```bash
make gui
```

### Compile Semua Target

Windows:

```powershell
mingw32-make all
```

Linux/WSL:

```bash
make all
```

### Membersihkan Hasil Build

Windows:

```powershell
mingw32-make clean
```

Linux/WSL:

```bash
make clean
```

## Cara Menjalankan Program

### Menjalankan CLI

Windows:

```powershell
bin\mainCLI.exe
```

Linux/WSL:

```bash
./bin/mainCLI.exe
```

Atau melalui make:

```bash
make run
```

Pada mode CLI, pengguna memilih file board, algoritma, dan heuristic jika algoritma membutuhkan heuristic.

### Menjalankan GUI

Windows:

```powershell
bin\mainGUI.exe
```

Linux/WSL:

```bash
./bin/mainGUI.exe
```

Atau melalui make:

```bash
make rung
```

Catatan:

- Jalankan program dari root repository agar path asset seperti `./assets/components` dan `./assets/players` dapat ditemukan.
- File board contoh berada di folder `test`.
- Asset GUI berada di folder `assets`.

## Cara Menggunakan GUI

1. Pada **Main Menu**, pilih salah satu mode:
   - `Play Manual`
   - `Play Algorithm`
   - `Create Board`

2. Jika memilih **Play Manual** atau **Play Algorithm**:
   - Pilih player pada halaman **Choose Player**.
   - Klik `OK`.
   - Pilih board pada halaman **Select Board**.
   - Klik `OK`.

3. Pada **Manual Play**:
   - Gunakan tombol `W`, `A`, `S`, `D` untuk menggerakkan player.
   - Player akan meluncur sesuai aturan Ice Sliding Puzzle.

4. Pada **Algorithm Play**:
   - Pilih algoritma: A*, UCS, GBFS, BFS, atau DFS.
   - Klik `Run`.
   - Jika solusi ditemukan, gunakan tombol `Play`, `Pause`, `Next`, dan `Prev` untuk melihat pergerakan solusi.

5. Pada **Board Editor**:
   - Pilih komponen board seperti `X`, `*`, `L`, `Z`, `O`, atau `0-9`.
   - Klik kiri pada board untuk menggambar tile.
   - Klik kanan pada board untuk memilih tile tanpa mengubahnya.
   - Atur cost pada grid cost.
   - Tile `X` otomatis memiliki cost `999`.
   - Klik `SAVE BOARD`, masukkan nama board, lalu klik `OK`.

## Format File Board

File board disimpan dalam format teks.
Contoh struktur umum:

```txt
<jumlah_baris> <jumlah_kolom>
<baris_board_1>
<baris_board_2>
...
<baris_board_n>
<cost_baris_1>
<cost_baris_2>
...
<cost_baris_n>
```

Contoh karakter board:

```txt
XXXXXXX
X*****X
X*Z*O*X
X*****X
XXXXXXX
```

## Author

| NIM | Nama |
| :---: | :---: |
| 13524022 | Ishak Palentino Napitupulu |
| 13524063| Marcel Luther Sitorus |
