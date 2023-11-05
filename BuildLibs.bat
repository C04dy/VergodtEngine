SET Dir=%cd%

mkdir build
robocopy Assets %dir%\build\Assets

mkdir libs
cd libs
mkdir SDL
cd SDL
mkdir include
mkdir lib
cd include
mkdir SDL3
cd ../..
mkdir Box2D
cd Box2D
mkdir include
mkdir lib
cd include
mkdir box2d
cd ../../..

cd vendor/SDL
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DSDL_STATIC=ON -DSDL_TEST_LIBRARY=OFF
mingw32-make
mingw32-make install

copy libSDL3.a %dir%\libs\SDL\lib
copy libSDL3.dll.a %dir%\libs\SDL\lib
copy SDL3.dll %dir%\build
cd ../include
robocopy SDL3 %dir%\libs\SDL\include\SDL3

cd %dir%

cd vendor/SDL_image
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DSDL_STATIC=ON
mingw32-make
cmake .. -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF
mingw32-make

copy libSDL3_image.a %dir%\libs\SDL\lib
copy libSDL3_image.dll.a %dir%\libs\SDL\lib
copy SDL3_image.dll %dir%\build
cd ../include
robocopy SDL3_image %dir%\libs\SDL\include\SDL3

cd %dir%

cd vendor/box2d
mkdir build
cd build 
cmake .. -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF
mingw32-make

cd bin
copy libbox2d.a %dir%\libs\Box2D\lib
cd ../../include
robocopy box2d %dir%\libs\Box2D\include\box2d

cd %dir%