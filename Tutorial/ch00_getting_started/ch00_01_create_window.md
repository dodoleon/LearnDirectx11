# 创建窗口

这部分是如何创建一个窗口。一般可以直接用Windows API来创建。但是这个方法，对于初学者非常不友好。笔者的对于这个有一个非常不愉快的经验，经常在这步被劝退。

这里选择用SDL2来创建窗口。

## SDL2

SDL2 库是一个开源的跨平台窗口创建库。可以几乎在任何平台上创建窗口。

### 初始化
```c++
if (SDL_Init(SDL_INIT_VIDEO) != 0)
{
    printf("Error: %s\n", SDL_GetError());
    return -1;
}
```
初始化SDL2库

### 创建窗口
```c++
int window_width = 1600;
int window_height = 900;
std::string title = "ch00_01_creating_a_window";
SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
SDL_Window* window = SDL_CreateWindow(
    title.c_str(),
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    window_width,
    window_height,
    window_flags);
if (window == nullptr)
{
    printf("SDL Create window failed");
    return -1;
}
```
```window_flags```是创建窗口的标签，这里设置了可以重新改变分辨率大小，和支持高分屏的支持。

### 主循环

```c++
    while (!close_window)
    {
        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event))
        {
            if (sdl_event.type == SDL_QUIT)
                close_window = true;
            if (sdl_event.type == SDL_WINDOWEVENT && sdl_event.window.event == SDL_WINDOWEVENT_CLOSE && sdl_event.window.windowID == SDL_GetWindowID(window))
                close_window = true;
            if (sdl_event.type == SDL_KEYDOWN && sdl_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                close_window = true;
        }
    }
```
这里就简单的获取SDL的事件，跳出主循环。

### 结束销毁
```c++
SDL_DestroyWindow(window);
SDL_Quit();
```
最后就需要销毁创建的窗口，以及SDL相关的上下文。

## 注意事项
SDL会自己重新定义一个SDL_main函数，这个作为用户的主函数。这个主要是为了解决不同平台函数入口不一样的问题。比如Windows API，程序入口是winmain，而不是大家非常熟悉的main。这里的用例也使用了这个方案。
```c++
#if defined(SDL_MAIN_NEEDED) || defined(SDL_MAIN_AVAILABLE)
#define main    SDL_main
#endif
```