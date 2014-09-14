cmake_minimum_required(VERSION 3.0)

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

add_subdirectory(src)

# Install paths
# Default prefix is /usr/local or C:\Program Files based on OS.
set(CMAKE_INSTALL_PREFIX ${ITD++_SOURCE_DIR}/IntoTheDungeon++)

install(DIRECTORY gfx/ DESTINATION gfx
  PATTERN Thumbs.db EXCLUDE)
install(DIRECTORY audio/ DESTINATION audio)
install(DIRECTORY data/ DESTINATION data PATTERN highscore.dat EXCLUDE)
install(DIRECTORY DESTINATION save)

include(InstallRequiredSystemLibraries)

# CPack settings
set(CPACK_PACKAGE_NAME IntoTheDungeon++)
set(CPACK_PACKAGE_VENDOR "Teon Banek")
set(CPACK_DESCRIPTION_SUMMARY "Simple graphical roguelike.")
set(CPACK_PACKAGE_ICON ${ITD++_SOURCE_DIR}/gfx/ITD++.bmp)
set(CPACK_RESOURCE_FILE_LICENSE ${ITD++_SOURCE_DIR}/COPYING.txt)
set(CPACK_RESOURCE_FILE_README ${ITD++_SOURCE_DIR}/README.txt)
set(CPACK_PACKAGE_VERSION_MAJOR ${ITD++_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${ITD++_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${ITD++_VERSION_PATCH})

# CPack settings for source package
set(CPACK_SOURCE_IGNORE_FILES .git* build clang-build .swp$ save highscore.dat)
include(CPack)
