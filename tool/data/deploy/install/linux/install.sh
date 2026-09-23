root=$(pwd)
cd tool/data/deploy/install/linux/debian
apt install build-essential -y &&\
chmod +x cmake.sh cryptopp.sh git.sh spdlog.sh &&\
./git.sh &&\
./cmake.sh &&\
./cryptopp.sh &&\
./spdlog.sh &&\
cd $root
