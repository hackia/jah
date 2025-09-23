cmake_minimum_required(VERSION 4.1)
project(jah)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
add_library(ji Ji.cpp Ji.hpp)
add_executable(jah main.cpp)
target_link_libraries(jah ji)
install(TARGETS jah DESTINATION bin)
install(FILES Ji.hpp DESTINATION include)
install(TARGETS ji DESTINATION lib)

