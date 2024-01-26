### Download NDI SDK 
* Create a sub-folder and name it `thirdparty`.
* Within the `thirdparty` folder create a sub-folder and name it `ndi`.
* Copy the `bin`, `include`, and `lib` folders from the NDI SDK download within the `thirdparty/ndi/` folder. 

### OpenCV
* Make sure you have downloaded binaries or built the binaries for OpenCV. 
* Point CMake to the appropriate folder where the OpenCV CMake config files can be found. 
```
cmake -DCMAKE_PREFIX_PATH="C:\OpenCV" ...
```