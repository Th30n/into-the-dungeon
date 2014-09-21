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

#include <cstdlib>
#include <iostream>
#include <list>

#ifdef __unix__
#include <sys/stat.h>
#endif

namespace data
{

typedef std::list<std::string> DataDirList;

static DataDirList data_dirs;
static std::string exe_dir;

static void dumpDataDirs() {
  DataDirList::iterator it;
  for (it = data_dirs.begin(); it != data_dirs.end(); ++it) {
    std::cout << *it << std::endl;
  }
}

static void buildDataDirs() {
  if (!data_dirs.empty()) {
    return;
  }
  data_dirs.push_back("./");
  data_dirs.push_back(exe_dir + "../");
#ifdef __unix__
  data_dirs.push_back(exe_dir + "../share/IntoTheDungeon++/");
#endif
  char *env_data_dir = getenv("ITD_DATA_DIR");
  if (env_data_dir) {
    data_dirs.push_front(env_data_dir);
  }
  dumpDataDirs();
}

void SetExeName(const char *exe_name)
{
  exe_dir = exe_name;
  size_t last_sep = exe_dir.find_last_of('/');
  if (last_sep != std::string::npos) {
    exe_dir.erase(last_sep + 1);
  } else {
    exe_dir = "./";
  }
}

#ifdef __unix__
static bool fileExists(const char *filepath)
{
  struct stat buf;
  int res = stat(filepath, &buf);
  if (res == -1) {
      //std::cerr << "data::fileExists: Failed 'stat'" << std::endl;
      return false;
  }
  if (S_ISREG(buf.st_mode)) {
    return true;
  }
  return false;
}
#endif

std::string FindFile(const char *filename)
{
  buildDataDirs();
  DataDirList::iterator it;

  // If absolute path.
  if (fileExists(filename)) {
    return filename;
  }

  for (it = data_dirs.begin(); it != data_dirs.end(); ++it) {
    std::string path = *it + filename;
    if (fileExists(path.c_str())) {
      return path;
    }
  }
  std::cerr << "Unable to find file " << filename << std::endl;
  return std::string();
}

} // namespace data
