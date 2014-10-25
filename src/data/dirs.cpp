/*
 * Into the Dungeon++
 *
 * Copyright (C) 2014 Teon Banek
 *
 * This file is part of Into the Dungeon++.
 * Into the Dungeon++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Into the Dungeon++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Into the Dungeon++. If not, see <http://www.gnu.org/licenses/>
 *
 * Author(s): Teon Banek <intothedungeon@gmail.com>
 */
#include "data/dirs.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>

#ifdef OS_UNIX
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef OS_WINDOWS
#include <windows.h>
#endif

namespace data
{

typedef std::list<std::string> DirList;

static DirList data_dirs;
static DirList user_data_dirs;
static std::string exe_dir;

static void buildDataDirs();
static void buildUserDataDirs();
static void setExeDir();
static bool fileExists(const std::string &filepath);
static bool createDir(const std::string &path);

// TODO: Consider using _dupenv_s instead of getenv for safety.

std::string findFile(DirList &dirs, const std::string &filename)
{
  // If absolute path.
  if (fileExists(filename)) {
    return filename;
  }

  for (DirList::iterator it = dirs.begin(); it != dirs.end(); ++it) {
    std::string path = *it + "/" + filename;
    if (fileExists(path)) {
      return path;
    }
  }
  std::cerr << "Unable to find file " << filename << std::endl;
  return std::string();
}

std::string FindFile(const std::string &filename)
{
  buildDataDirs();
  return findFile(data_dirs, filename);
}

std::string FindUserFile(const std::string &filename)
{
  buildUserDataDirs();
  return findFile(user_data_dirs, filename);
}

std::string CreateUserDir(const std::string &dirname)
{
  buildUserDataDirs();
  std::string fullpath = user_data_dirs.front() + "/" + dirname;
  // Create the root dir first and then the given dir
  if (createDir(user_data_dirs.front()) && createDir(fullpath)) {
    std::cerr << "Unable to create dir " << fullpath << std::endl;
    return fullpath;
  } else {
    return "";
  }
}

std::string CreateUserFile(const std::string &file)
{
  buildUserDataDirs();
  std::string &user_dir = user_data_dirs.front();
  if (!createDir(user_dir)) {
    std::cerr << "Unable to create dir " << user_dir << std::endl;
  }
  return user_dir + "/" + file;
}

static void dumpDirs(DirList &dirs) {
  DirList::iterator it;
  std::cout << "Directories:" << std::endl;
  for (it = dirs.begin(); it != dirs.end(); ++it) {
    std::cout << *it << std::endl;
  }
  std::cout << std::endl;
}

static void buildDataDirs()
{
  if (!data_dirs.empty()) {
    return;
  }
  // Environment variable has highest priority.
  char *itd_data_dir = getenv("ITD_DATA_DIR");
  if (itd_data_dir) {
    data_dirs.push_back(itd_data_dir);
  }
  setExeDir();
  data_dirs.push_back(".");
  data_dirs.push_back(exe_dir);
  data_dirs.push_back(exe_dir + "/..");
#ifdef OS_UNIX
  data_dirs.push_back(exe_dir + "/../share/into-the-dungeon++");
#endif
  dumpDirs(data_dirs);
}

static void buildUserDataDirs()
{
  if (!user_data_dirs.empty()) {
    return;
  }
  // Environment variable has highest priority.
  char *itd_data_home = getenv("ITD_DATA_HOME");
  if (itd_data_home) {
    user_data_dirs.push_back(itd_data_home);
  }
#ifdef OS_UNIX
  char *xdg_data_home = getenv("XDG_DATA_HOME");
  if (!xdg_data_home) {
    char *home = getenv("HOME");
    user_data_dirs.push_back(
        std::string(home) + "/.local/share/into-the-dungeon++");
  } else {
    user_data_dirs.push_back(std::string(xdg_data_home) + "/into-the-dungeon++");
  }
#endif // OS_UNIX
  user_data_dirs.push_back(exe_dir + "/..");
  dumpDirs(user_data_dirs);
}

#ifdef OS_UNIX
static void setExeDir()
{
  const size_t BUFSIZE = 256;
  char buf[BUFSIZE + 1] = {0};
  ssize_t size = readlink("/proc/self/exe", buf, BUFSIZE);
  if (size == -1) {
    perror("data::setExeDir()");
    exe_dir = ".";
    return;
  }
  exe_dir = buf;
  size_t last_sep = exe_dir.find_last_of('/');
  if (last_sep != std::string::npos) {
    exe_dir.erase(last_sep);
  } else {
    exe_dir = ".";
  }
}

static bool fileExists(const std::string &filepath)
{
  struct stat buf;
  int res = stat(filepath.c_str(), &buf);
  if (res == -1) {
    if (errno == EACCES) {
        std::cerr << "data::fileExists(): Permission denied " << filepath
          << std::endl;
    }
    return false;
  }
  if (S_ISREG(buf.st_mode)) {
    return true;
  }
  return false;
}

static bool isDirectory(const std::string &path)
{
  struct stat buf;
  int res = stat(path.c_str(), &buf);
  if (res == -1) {
    perror("data::isDirectory");
    return false;
  }
  if (S_ISDIR(buf.st_mode)) {
    return true;
  }
  return false;
}

static bool createDir(const std::string &path)
{
  int status = mkdir(path.c_str(), S_IRWXU);
  if (status == -1) {
    if (errno == EEXIST && isDirectory(path)) {
      return true;
    }
    perror("data::createDir()");
    return false;
  }
  return true;
}
#endif // OS_UNIX

#if OS_WINDOWS
static void setExeDir()
{
  DWORD BUFSIZE = 256;
  LPTSTR buf = new CHAR[BUFSIZE + 1];
  DWORD size = GetModuleFileName(NULL, buf, BUFSIZE);
  // This probably means the buffer was too small
  if (size == BUFSIZE) {
    DWORD err = GetLastError();
    // Windows coders are weird: WindowsXP might set ERROR_SUCCESS
    if (err == ERROR_INSUFFICIENT_BUFFER || err == ERROR_SUCCESS) {
      std::cerr << "data::setExeDir(): insufficient buffer"<< std::endl;
      exe_dir = ".";
    }
  } else if (size == 0) {
    std::cerr << "data::setExeDir(): failed " << GetLastError() << std::endl;
    exe_dir = ".";
  } else {
    exe_dir = buf;
    size_t last_sep = exe_dir.find_last_of('\\');
    if (last_sep != std::string::npos) {
      exe_dir.erase(last_sep);
    } else {
      exe_dir = ".";
    }
  }
  delete[] buf;
}

static bool fileExists(const std::string &filepath)
{
  DWORD res = GetFileAttributes(filepath.c_str());
  if (res == INVALID_FILE_ATTRIBUTES) {
    //std::cerr << "data::fileExists(): failed " << GetLastError() << std::endl;
    return false;
  }
  if (res & FILE_ATTRIBUTE_DIRECTORY) {
    return false;
  }
  return true;
}

static bool isDirectory(const std::string &path)
{
  DWORD res = GetFileAttributes(path.c_str());
  if (res != INVALID_FILE_ATTRIBUTES && res & FILE_ATTRIBUTE_DIRECTORY) {
    return true;
  }
  return false;
}

static bool createDir(const std::string &path)
{
  BOOL res = CreateDirectory(path.c_str(), NULL);
  if (res == 0) {
    DWORD err = GetLastError();
    if (err == ERROR_ALREADY_EXISTS && isDirectory(path)) {
      return true;
    }
    std::cerr << "data::createDir(): failed " << GetLastError() << std::endl;
    return false;
  }
  return true;
}
#endif // OS_WINDOWS
} // namespace data
