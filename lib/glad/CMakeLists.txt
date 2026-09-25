cmake_minimum_required(VERSION 3.20)

project(glad VERSION 2.0.8)

add_library(glad glad.c)
target_include_directories(glad PUBLIC include/)
