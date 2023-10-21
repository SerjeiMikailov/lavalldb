cd src
clang++ main.cpp io.cpp -o ../build/app -lsqlite3
cd ..
cd build
./app
