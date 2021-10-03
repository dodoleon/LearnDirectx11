# LearnDirectx11
Tutorial of Directx11

这是一个仿照[LearnOpenGL](https://learnopengl.com/)写的学习DirectX 11的教程。在写这个教程的初衷是因为，一开始在学习OpenGL的时候LearnOpenGL是一个非常好的step by step的教程。但是当我想要学习D3D的时候发现几乎有这样简单易懂的教程。要么是类似龙书这样，需要一定的window编程基础和点发懵的各种Windows概念。缺少一个非常容易入门的学习教程。

出于这个原因开始编写这份教程。

## 需要的编程环境
- Windows 7/10/11 
- visual studio 15/17/19
- cmake

教程会按照Windows 10和 visual studio 19 社区版开始。

## 需要的前置学习背景
需要一定的C++语言能力，如果还没有学习过c++的话，推荐去网易云课堂翁恺老师开设的免费课程[程序设计入门—C语言](https://mooc.study.163.com/course/1000002011?tid=2403041004&_trace_c_p_k2_=9c151c4bd24f4164b48e91f30f3e4d15#/info)。虽然我会使用C++作为教程的编程语言。但是基本上是C with class 的形式。不会涉及到很复杂的C++特性，所以学习C语言，基本就可以满足大部分的要求了。

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
```bash
git clone https://github.com/dodoleon/LearnDirectx11.git
cd LearnDirectx11/Code
mkdir build
cd build
cmake ..
```

## More Thing
大多数的DirectX的教程会使用DirectXMath作为数学库。但是考虑到，DirectXMath是左手系，以及向量左乘矩阵这种形式，这与我们平时在学习的线性代数不一致。所以选用来GLM作为我们的数学库，以及在shader中都会选择向量右乘矩阵的方式来写。这和我们平时学习的线性代数的习惯是一致的。