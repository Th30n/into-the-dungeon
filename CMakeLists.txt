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

# Disable absolute path install destinations
set(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION ON)

# Install paths
# Default prefix is /usr/local or C:\Program Files based on OS.
# To change prefix use 'make DESTDIR=prefix_before_default'

# set(CMAKE_INSTALL_PREFIX ${ITD++_SOURCE_DIR}/IntoTheDungeon++)

if(${WIN32})
  # C:\Program Files\${PROJECT_NAME}
  set(BIN_DIR "bin/")
  set(DATA_DIR "")
  set(USER_DATA_DIR ${DATA_DIR})
  # For Windows users dll files need to be installed with the executable
  set(SDL_DLL "" CACHE FILEPATH "Path to SDL.dll which will be installed.")
  if(NOT EXISTS ${SDL_DLL})
    message(FATAL_ERROR "SDL_DLL must point to SDL.dll file.")
  endif()
  set(SDL_IMAGE_DLL "" CACHE FILEPATH
    "Path to SDL_image.dll which will be installed.")
  if(NOT EXISTS ${SDL_IMAGE_DLL})
    message(FATAL_ERROR "SDL_IMAGE_DLL must point to SDL_image.dll file.")
  endif()
  set(SDL_MIXER_DLL CACHE FILEPATH
    "Path to SDL_mixer.dll which will be installed")
  if(NOT EXISTS ${SDL_MIXER_DLL})
    message(FATAL_ERROR "SDL_MIXER_DLL must point to SDL_mixer.dll file.")
  endif()
  install(FILES ${SDL_DLL} ${SDL_IMAGE_DLL} ${SDL_MIXER_DLL}
    DESTINATION ${BIN_DIR})
else()
  # /usr/local/bin
  set(BIN_DIR "bin/")
  # /usr/local/share/IntoTheDungeon++
  set(DATA_DIR "share/IntoTheDungeon++/")
endif()

install(DIRECTORY share/gfx/ DESTINATION ${DATA_DIR}gfx
  PATTERN Thumbs.db EXCLUDE)
install(DIRECTORY share/audio/ DESTINATION ${DATA_DIR}audio)
install(DIRECTORY share/data/ DESTINATION ${DATA_DIR}data
  PATTERN highscore.dat EXCLUDE)
# Save should be created from the game
# install(DIRECTORY DESTINATION ${USER_DATA_DIR}save)

include(InstallRequiredSystemLibraries)

include_directories("include")
add_subdirectory(src)

# CPack settings
set(CPACK_PACKAGE_NAME IntoTheDungeon++)
set(CPACK_PACKAGE_VENDOR "Teon Banek")
set(CPACK_DESCRIPTION_SUMMARY "Simple graphical roguelike.")
#set(CPACK_PACKAGE_ICON ${ITD++_SOURCE_DIR}/share/gfx/ITD++.bmp)
set(CPACK_RESOURCE_FILE_LICENSE ${ITD++_SOURCE_DIR}/COPYING.txt)
set(CPACK_RESOURCE_FILE_README ${ITD++_SOURCE_DIR}/README.txt)
set(CPACK_PACKAGE_VERSION_MAJOR ${ITD++_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${ITD++_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${ITD++_VERSION_PATCH})

# CPack settings for source package
set(CPACK_SOURCE_IGNORE_FILES .git* Thumbs.db build .swp$ save highscore.dat)
include(CPack)
