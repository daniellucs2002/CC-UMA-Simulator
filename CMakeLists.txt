cmake_minimum_required(VERSION 3.10)

project(CacheCoherenceSimulator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCES
    main.cpp
    config.cpp
    timer/Timer.cpp
    cpu/CPU.cpp
    traces/Trace.cpp
    bus/Bus.cpp
    cache/Cache.cpp
    cache/CacheSet.cpp
    cache/CacheLine.cpp
    cache/CacheController.cpp
)

add_executable(coherence ${SOURCES})
