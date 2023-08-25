cmake_minimum_required(VERSION 3.26)
project(towncrier LANGUAGES C)

set(CMAKE_C_STANDARD 99)

add_executable(pub src/pub.c src/publisher.c src/utils.c)
target_link_libraries(pub PUBLIC hiredis)

add_executable(sub src/sub.c src/subscriber.c src/utils.c)
target_link_libraries(sub PUBLIC hiredis)
