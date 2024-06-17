cmake_minimum_required(VERSION 3.27)
project(HSE)

set(CMAKE_CXX_STANDARD 17)

add_executable(HSE
        Matrix/matrix.h
        "Rational/ rational.cpp"
        Array/array.h
        Vector/vector.h
        BigInteger/big_integer.cpp
        BigInteger/big_integer.h
)
