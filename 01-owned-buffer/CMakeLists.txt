add_executable(owned_buffer
    src/main.cpp
    src/owned_buffer.cpp
)
target_include_directories(owned_buffer PRIVATE src)
target_compile_options(owned_buffer PRIVATE /W4)