cmake toolchain
https://github.com/LaurentGomila/qt-android-cmake

docker container with build environment
https://hub.docker.com/r/rabits/qt/

---

docker run -itv /home/nehlsen/Projects/VenueShotClient/:/source androidbuildenv
g++

sudo ln -s /opt/android-ndk/sysroot/usr/include/ /opt/android-ndk/platforms/android-16/arch-arm/usr/include


cmake -DCMAKE_TOOLCHAIN_FILE=../android/qt-android-cmake/toolchain/android.toolchain.cmake -DANDROID_NDK=/opt/android-ndk/ -DANDROID_NATIVE_API_LEVEL=android-16 ..
