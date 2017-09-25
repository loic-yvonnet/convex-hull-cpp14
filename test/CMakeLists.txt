cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(hull_unit_tests)
add_executable(hull_unit_tests 
                    main.cpp
                    algorithms_test.cpp
                    angle_test.cpp
                    bounding_box_test.cpp
                    chan_test.cpp
                    graham_scan_test.cpp
                    jarvis_march_test.cpp
                    monotone_chain_test.cpp
                    point2d.hpp
                    point_concept_test.cpp
                    test_main.cpp
                    test_main.hpp
                    ../hull/algorithms.hpp
                    ../hull/angle.hpp
                    ../hull/static_assert.hpp
                    ../hull/bounding_box.hpp
                    ../hull/chan_algorithm.hpp
                    ../hull/graham_scan.hpp
                    ../hull/jarvis_march.hpp
                    ../hull/monotone_chain.hpp
                    ../hull/point_concept.hpp
                    ../hull/reflection.hpp
                    ../hull/math_utils.hpp
                    ../hull/point_math_utils.hpp
)
