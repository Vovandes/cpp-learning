add_executable(numeric_words
    src/main.cpp
    src/words.cpp
)
target_include_directories(numeric_words PRIVATE src)
target_compile_features(numeric_words PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(numeric_words PRIVATE /W4 /WX)
else()
    target_compile_options(numeric_words PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()