cmake_minimum_required(VERSION 3.0)

# Policy to use VERSION option in project()
cmake_policy(SET CMP0048 NEW)

# Project is in C++ but SDL is in C
project(ITD++ VERSION 0.9.2 LANGUAGES CXX C)

# Require SDL
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)

### Compilation options. ###

if(WIN32)
  add_definitions(-DOS_WINDOWS)
elseif(UNIX)
  add_definitions(-DOS_UNIX)
endif()

# Always check for warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

###

# Disable absolute path install destinations
set(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION ON)

# Install paths
# Default prefix is /usr/local or C:\Program Files based on OS.
# To change prefix use 'make DESTDIR=prefix_before_default'

# set(CMAKE_INSTALL_PREFIX ${ITD++_SOURCE_DIR}/into-the-dungeon++)

if(WIN32)
  # C:\Program Files\${PROJECT_NAME}
  set(BIN_DIR "bin")
  set(DATA_DIR ".")
  set(USER_DATA_DIR ${DATA_DIR})
  # For Windows users dll files need to be installed with the executable
  set(DLL_DIR "" CACHE PATH "Directory with all the dll files that need to be
    installed with the executable.")
  if(NOT IS_DIRECTORY ${DLL_DIR})
    message(FATAL_ERROR "DLL_DIR must point to directory with all dll files
      which need to be installed with the executable.")
  endif()
  file(GLOB ALL_DLLS ${DLL_DIR}/*.dll)
  install(FILES ${ALL_DLLS} DESTINATION ${BIN_DIR})
elseif(UNIX)
  # /usr/local/bin
  set(BIN_DIR "bin")
  # /usr/local/share/into-the-dungeon++
  set(DATA_DIR "share/into-the-dungeon++")
  # Install icons and .desktop files
  # After installing icons gtk-update-icon-cache needs to be called
  set(ICON_DIR "share/icons/hicolor")
  set(DESKTOP_DIR "share/applications")
  install(FILES share/icons/itd++-32.png DESTINATION ${ICON_DIR}/32x32/apps
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ RENAME itd++.png)
  install(FILES share/icons/itd++-48.png DESTINATION ${ICON_DIR}/48x48/apps
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ RENAME itd++.png)
  install(FILES share/icons/into-the-dungeon.desktop DESTINATION ${DESKTOP_DIR}
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
endif()

install(DIRECTORY share/gfx/ DESTINATION ${DATA_DIR}/gfx
  PATTERN Thumbs.db EXCLUDE)
install(DIRECTORY share/audio/ DESTINATION ${DATA_DIR}/audio)
install(DIRECTORY share/data/ DESTINATION ${DATA_DIR}/data
  PATTERN highscore.dat EXCLUDE)
# Save should be created from the game
# install(DIRECTORY DESTINATION ${USER_DATA_DIR}save)

include(InstallRequiredSystemLibraries)

include_directories("include")
add_subdirectory(src)

# CPack settings
set(CPACK_PACKAGE_NAME into-the-dungeon++)
set(CPACK_PACKAGE_VENDOR "Teon Banek")
set(CPACK_DESCRIPTION_SUMMARY "Simple graphical roguelike.")
set(CPACK_RESOURCE_FILE_LICENSE ${ITD++_SOURCE_DIR}/COPYING.txt)
set(CPACK_RESOURCE_FILE_README ${ITD++_SOURCE_DIR}/README.txt)
set(CPACK_PACKAGE_VERSION_MAJOR ${ITD++_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${ITD++_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${ITD++_VERSION_PATCH})
set(CPACK_PACKAGE_EXECUTABLES "itd++;Into The Dungeon++")
set(CPACK_CREATE_DESKTOP_LINKS itd++)
# CPack settings for installation on Windows
if(WIN32)
  set(CPACK_PACKAGE_NAME "Into The Dungeon++")
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
  set(CPACK_NSIS_MUI_ICON ${ITD++_SOURCE_DIR}/share/res/itd++.ico)
endif()
# CPack settings for source package
set(CPACK_SOURCE_IGNORE_FILES .git* Thumbs.db build .swp$ highscore.dat)
include(CPack)

if(UNIX)
  # Check if possible to build Arch packages for AUR
  execute_process(COMMAND which makepkg RESULT_VARIABLE MAKEPKG_EXISTS
    OUTPUT_QUIET ERROR_QUIET)
  if(MAKEPKG_EXISTS EQUAL 0)
    option(PACK_SOURCE_ARCH "Create source packaging for ArchLinux AUR" ON)
    if (PACK_SOURCE_ARCH)
      add_custom_target(package_arch COMMAND sh -c
        ". ${ITD++_SOURCE_DIR}/utils/package_arch.sh \
        ${ITD++_SOURCE_DIR}/utils/abs/into-the-dungeon++ \
        ${ITD++_SOURCE_DIR}/Version.txt")
    endif()
  endif()
endif()

enable_testing()
add_subdirectory(tests)
