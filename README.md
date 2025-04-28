```
mkdir build
cmake -S . -B build -DCMAKE_PREFIX_PATH="/home/fgao/code/anari-superbuild/build/deps/install"
cmake --build build --config Release --parallel

export LD_LIBRARY_PATH=/home/fgao/code/anari-superbuild/build/deps/install/lib:$LD_LIBRARY_PATH
ANARI_LIBRARY=helide /home/fgao/code/brain_visualization/build/demoViewer

helide
ospray
barney
visrtx


```