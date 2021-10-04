cmake_minimum_required (VERSION 3.12)
project ("ch00_02_hello_window")

set(target ch00_02_hello_window)

add_executable (${target} "main.cpp")

target_link_libraries(${target}
PRIVATE
    SDL2-static
    SDL2main
    d3d11.lib
    dxgi.lib
)

set_target_properties(${target}
PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)