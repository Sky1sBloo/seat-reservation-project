cmake_minimum_required(VERSION 3.0)
project (seat-reservation)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
add_executable(seat-reservation main.c plane.c session.c account.c input-handler.c)
