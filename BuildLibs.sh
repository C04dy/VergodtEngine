mkdir build

mkdir libs
cd ./libs
mkdir SDL
cd ./SDL
mkdir include
mkdir lib
cd ..
mkdir Box2D
cd ./Box2D
mkdir include
mkdir lib
cd ../..

cd ./vendor/SDL
mkdir build
cd ./build
sudo cmake .. -DSDL_STATIC=ON -DSDL_TEST_LIBRARY=OFF
sudo make
sudo make install

cp -i libSDL3.a ../../../libs/SDL/lib
cd ../include
cp -R SDL3 ../../../libs/SDL/include

cd ../../SDL_image
mkdir build
cd ./build
sudo cmake .. -DBUILD_SHARED_LIBS=OFF -DBOX2D_BUILD_UNIT_TESTS=OFF -DBOX2D_BUILD_TESTBED=OFF
sudo make

cp -i libSDL3_image.a ../../../libs/SDL/lib
cd ../include/SDL3_image
cp -i SDL_image.h ../../../../libs/SDL/include/SDL3

cd ../../../box2d
mkdir build
cd ./build 
sudo cmake .. -DBUILD_SHARED_LIBS=OFF
sudo make

cd ./bin
cp -i libbox2d.a ../../../../libs/Box2D/lib
cd ../../include
cp -R box2d ../../../libs/Box2D/include/box2d