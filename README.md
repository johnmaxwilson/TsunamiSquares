# TsunamiSquares
C++ library for modeling tsunamis from Virtual Quake simulated earthquakes using the Tsunami Squares method (Steven N. Ward)

Ramya: to compile each time, after any edit in main.cpp or whereever, you need totype into terminal:

cd TsunamiSquares
bash setup.sh
g++ main.cpp TsunamiSquares.cpp TsunamiSquaresUtil.cpp -o TsunamiSquares
./TsunamiSquares
python tsunamisquares.py