# LearnDirectx11
Tutorial of Directx11

这是一个仿照[LearnOpenGL](https://learnopengl.com/)写的学习DirectX 11的教程。在写这个教程的初衷是因为，一开始在学习OpenGL的时候LearnOpenGL是一个非常好的step by step的教程。但是当我想要学习D3D的时候发现几乎有这样简单易懂的教程。要么是类似龙书这样，需要一定的window编程基础和点发懵的各种Windows概念。缺少一个非常容易入门的学习教程。

出于这个原因开始编写这份教程。

第一版的教程中，不会涉及到一些矩阵或者库相关的问题，有问题可以自行到[LearnOpenGL](https://learnopengl.com/)中查看相关的教程。后续会补充上，以及我在做图形开发的过程中遇到的经验。比如ReverseZ DepthMap之类的方法。

## 需要的编程环境
- Windows 7/10(作者用的版本)/11 
- visual studio 17/19(作者用的版本)
- cmake v3.12以上

教程会按照Windows 10和 visual studio 19 社区版开始。也有可能会有读者是在Windows 7上开发，出现了错误可以在issue中提。

## 需要的前置学习背景
需要一定的C++语言能力，如果还没有学习过c++的话，推荐去网易云课堂或者别的地方提前学习一下C++。代码基本上是按照C++17的标准来写的。

构建工具使用了CMake，如果还没有学习过，可以上[modern-cmake](https://cliutils.gitlab.io/modern-cmake/) 学习一下。教程中我也会简单的step by step教学CMake怎么写以及为什么这么写。
## 你可以学习到什么
- 如何从零开始编写DirectX 11的图形程序。
- 图形API的概念。
- 图形学几种效果的简单实现。
- CMake构建代码。

## 目录
- Code 代码文件夹。
- Tutorial 教程的文件夹。

## 如何获取源代码以及编译
源代码在Code文件夹中，因为DirectX一定是需要Windows SDK的支持。这里直接默认是使用visual studio以及它的编译器。不支持mingw，gcc等其他的编译器，可能可以，但是会比较麻烦。
```powershell
git clone https://github.com/dodoleon/LearnDirectx11.git
cd LearnDirectx11
git submodule update --init
mkdir Code/build
cd Code/build
cmake ..
```

## More Thing
大多数的DirectX的教程会使用DirectXMath作为数学库。但是考虑到，DirectXMath是左手系，以及向量左乘矩阵这种形式，这与我们平时在学习的线性代数不一致。所以选用来GLM作为我们的数学库，以及在shader中都会选择向量右乘矩阵的方式来写。这和我们平时学习的线性代数的习惯是一致的。