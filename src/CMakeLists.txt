add_library(core STATIC
    scheduler.hpp
    implementation.hpp
    simulator.hpp
    simulator.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
