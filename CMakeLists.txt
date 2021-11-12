cmake_minimum_required(VERSION 3.12)
project(main LANGUAGES C CXX)

#openMP
find_package(OpenMP)

add_executable(main "src/NEE.cpp")
target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE "include")

if(OpenMP_CXX_FOUND)
  target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
else()
  message(WARNING "failed to find OpenMP")
endif()