add_executable(parse_errors
    src/main.cpp
    src/parse.cpp
)
target_include_directories(parse_errors PRIVATE src)
target_compile_features(parse_errors PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(parse_errors PRIVATE /W4)
else()
    target_compile_options(parse_errors PRIVATE -Wall -Wextra -Wpedantic)
endif()