mkdir build
cp -r Assets ./build

mkdir libs
cd ./libs
mkdir SDL
cd ./SDL
mkdir include
mkdir lib
cd ./include
mkdir SDL3
cd ../..
mkdir Box2D
cd ./Box2D
mkdir include
mkdir lib
cd ./include
mkdir box2d
cd ../../..

cd ./vendor/SDL
mkdir build
cd ./build
cmake .. -DSDL_STATIC=ON -DSDL_TEST_LIBRARY=OFF
make
make install

cp -i libSDL3.a ../../../libs/SDL/lib
cd ../include
cp -R SDL3 ../../../libs/SDL/include/SDL3