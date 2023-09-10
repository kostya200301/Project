sudo apt update
sudo apt-get install libgflags-dev
sudo apt-get install nlohmann-json3-dev
wget https://download.libsodium.org/libsodium/releases/LATEST.tar.gz
tar -xzvf LATEST.tar.gz
cd libsodium-stable
./configure
make
sudo make install
cd ..
mkdir build
cd build
cmake ..
make
