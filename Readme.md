This program is taking a HEX file, parse it and simulate in.
It includes 2 program, HexParse and AvrSimulator.
AvrSimulator is the submodule of HexParse.
To use it, copy the commands below

git clone https://github.com/Bennyaw/HexParse.git
git submodule init
git submodule update

(then enter AvrSimulator project file to compile)

cd AvrSimulator
ceedling

(back to HexParse project file, need to make changes in project.yml)

ceedling release

(open command prompt)

C:\Bennyaw\HexParse>\build\release\simulate.exe
