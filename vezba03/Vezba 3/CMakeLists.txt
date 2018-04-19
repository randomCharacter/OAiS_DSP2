cmake_minimum_required(VERSION 3.9)
project(Vezba_3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Vezba_3
        fft_filter.cpp
        fft_tables.h
        obrada.cpp
        obrada.h
        sr_fft.cpp
        sr_fft.h)
