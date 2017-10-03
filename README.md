# AntTracking
Repository for the ant tracking software.

# Building for Windows with Visual Studio
 - download and build OpenCV with all contrib modules
 - download and build libzmq with additional C++ bindings (see [this](https://stackoverflow.com/a/37535196) manual)
 - create a folder `build` and run cmake from within making sure that...
 	- a path to a video file is set
 	- a path to a mask file is set
 	- all OpenCV directories are set
 	- all ZeroMQ directories are set
 - build (currently `release` is working only)

# Notes
The default externals directories are `externals/libzmq` and `externals/OpenCV`.
CMake will set the provided video and mask file paths as global `#DEFINE FILE_VIDEO` and `#DEFINE FILE_MASK` strings in C++.