git clone https://github.com/cryptopp-modern/cryptopp-modern.git && \
apt install ninja-build -y && \
cd cryptopp-modern && \
cmake --preset=default && \
cmake --build build/default -j$(nproc) && \
cp build/default/libcryptopp.a /usr/local/lib/libcryptopp.a && \
cp -r include/cryptopp /usr/local/include && \
cd .. &&\
rm -r cryptopp-modern