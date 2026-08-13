# Download and extract release
cd /tmp
wget https://github.com/cryptopp-modern/cryptopp-modern/releases/download/2026.8.1/cryptopp-modern-2026.8.1.zip
unzip -q cryptopp-modern-2026.8.1.zip -d cryptopp
cd cryptopp

# Configure with default preset (Release, Ninja)
cmake --preset=default

# Build
cmake --build build/default -j$(nproc)

# Run tests
./build/default/cryptest.exe v

# Install
sudo cmake --install build/default --prefix /usr/local