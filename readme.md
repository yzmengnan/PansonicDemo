# Requirements:
请满足CmakeList内的package要求。

# advice:

## 1、Download msys2 
https://github.com/msys2/msys2-installer/releases/download/2024-05-07/msys2-x86_64-20240507.exe
## 2、Run msys2
按下windows键，搜索ucrt 运行 msys2 ucrt64，进入ucrt运行环境
## 3、Update msys2
运行: pacman -Syu
## 4、Get mingw toolchains
运行：pacman -S mingw-w64-ucrt-x86_64-toolchain
运行：pacman -S mingw-w64-ucrt-x86_64-cmake
运行：pacman -S mingw-w64-ucrt-x86_64-ninja
## 5、Use mingw-ucrt-toolchains as default compile tools
打开Clion，找到设置，选择构建、运行、部署，选择工具链
点击‘+’号，创建一个工具链，细节均如图所示
![img.png](resources/img.png)

选择Cmake，细节如图所示
![img_1.png](resources/img_1.png)

## 6、Finish






