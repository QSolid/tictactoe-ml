**Overview**

- A GTK-based Tic‑Tac‑Toe game in C with two AI modes — Minimax and a simple ML mode that uses linear regression. 

- Designed to help kids explore strategy and pattern recognition by comparing how different AIs play.

## Project Structure

- src/ — C sources for the desktop app
	- main.c, game_logic.c/.h, minimax.c/.h, ML.c, ML-config.h
- assets/ — UI resources used at runtime
	- tictactoeUI.glade, style.css, images (if any)
- models/ — Final trained weights used by the app
	- MLWeights.txt
- training/ — Training program and resources (CLI)
	- ML-Training.c, Training-config.h
	- data/ — dataset docs and references
	- models/ — weights produced by the trainer (MLWeights.txt)
- scripts/ — Build scripts
	- 1-click-installation-for-windows.bat

## How to Build & Run

- Prerequisites: Install GTK3 via MSYS2 and ensure gcc is available.
- Build the app (PowerShell):

	".\scripts\1-click Installation for windows.bat"

- Run from the repository root so the app can load assets/ and models/:

	.\TTT.exe

## GTK3 Installation for Windows
1. Open MSYS2 Mingw64
2. Execute this command  `pacman -S mingw-w64-x86_64-gtk3` and `pacman -S mingw-w64-x86_64-toolchain base-devel`
3. Edit Windows Enviroment Path
4. Add `C:\msys64\mingw64\bin` to Environment Path

## Easy Installation for Windows PC
Execute `1-click Installation for windows.bat` to compile TTT.exe\
Run `TTT.exe`

## If easy installation does not work:

### Manual Steps (Windows)
1. Open MSYS2 MinGW64 terminal.
2. `cd` to the repository root.
3. Build with GTK flags via pkg-config and link WinMM for sound:

	gcc src/main.c src/minimax.c src/ML.c src/game_logic.c -o TTT.exe $(pkg-config --cflags --libs gtk+-3.0) -lwinmm -Wl,--export-all-symbols -mwindows

4. Run `TTT.exe` from the repo root.

## Training (CLI)

- The trainer reads the dataset referenced by `DATA_FILE` in training/Training-config.h (tic-tac-toe.data).
- Recommended (PowerShell, repo root):

	& '.\\training\\1-click-compiler.bat'

- MSYS2 alternative:

	gcc -o training/ml training/ML-Training.c
	./training/ml

- Weights output: the trainer writes to models/MLWeights.txt in the repo root so the app uses the latest weights.

## Algorithms by Difficulty
- **Easy:** Random move selection among all empty squares.
- **Medium (Minimax):** First move random; then Minimax with shallow search (depth = 1).
- **Hard (Minimax):** First move random; then full-depth Minimax (depth = 9).
- **ML:** Linear evaluation over 7 features (constant, two-in-a-row own, two-in-a-row opponent, center control, corners, one-own-two-empty, three-in-a-row own). The model scores from the computer’s perspective (O = −1) and chooses the highest-scoring move.

## References
- UCI ML Repository: Tic-Tac-Toe Endgame Data Set.
- Feature inspiration: "TicTacToeRegressionBot" — player feature definitions (https://github.com/Skyy93/TicTacToeRegressionBot/blob/master/player.py).