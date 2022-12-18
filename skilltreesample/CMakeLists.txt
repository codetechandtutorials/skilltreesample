# CMakeList.txt : CMake project for skilltreesample, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (skilltreesample "skilltreesample.cpp" "skilltreesample.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET skilltreesample PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
