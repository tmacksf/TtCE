
# Thomas the Chess Engine
A UCI compliant chess engine written in C++ using a bitboard based board representation, evaluation function taken from PeSTO, and alpha-beta search.
Not all UCI functionality has been implemented, but the core functionality including position, go, and wtime/btime are functioning.

### Compiling
In order to compile this engine, CMake 3.1 and C++ 17 are required.
```
mkdir build
cd build
cmake ..
cmake --build .
```
This will compile the project.

### Running
A UCI compliant GUI is required to play against the engine. Examples of these include [Arena](http://www.playwitharena.de/) and [Cutechess](https://cutechess.com/).  

