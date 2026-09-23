git clone https://github.com/gabime/spdlog.git &&\
cd spdlog &&\
mkdir spdlog &&\
cmake -B build -S . && cmake --build build -j$(nproc) &&\
cp build/libspdlog.a /usr/local/lib/libspdlog.a &&\
cp -r include/spdlog /usr/local/include &&\
cd .. &&\
rm -r spdlog