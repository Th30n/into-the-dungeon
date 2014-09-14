cmake_minimum_required(VERSION 2.8)

# Project is in C++ but SDL is in C
project(ITD++ CXX C)

# Require SDL
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)

set (ITD++_VERSION_MAJOR 0)
set (ITD++_VERSION_MINOR 9)
set (ITD++_VERSION_PATCH 1)

# Always check for warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Install in project root
set(CMAKE_INSTALL_PREFIX ${ITD++_SOURCE_DIR})

add_subdirectory(src)

include(InstallRequiredSystemLibraries)

# CPack settings
set(CPACK_PACKAGE_VERSION_MAJOR ${ITD++_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${ITD++_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${ITD++_VERSION_PATCH})
include(CPack)
