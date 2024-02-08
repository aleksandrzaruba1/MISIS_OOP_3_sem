# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/crzy/Downloads/MISIS_OOP_3_sem/prj.cw/build/_deps/googletest-src"
  "/Users/crzy/Downloads/MISIS_OOP_3_sem/prj.cw/build/_deps/googletest-build"
  "/Users/crzy/Downloads/MISIS_OOP_3_sem/prj.cw/build/_deps/googletest-subbuild/googletest-populate-prefix"
  "/Users/crzy/Downloads/MISIS_OOP_3_sem/prj.cw/build/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "/Users/crzy/Downloads/MISIS_OOP_3_sem/prj.cw/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "/Users/crzy/Downloads/MISIS_OOP_3_sem/prj.cw/build/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "/Users/crzy/Downloads/MISIS_OOP_3_sem/prj.cw/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/crzy/Downloads/MISIS_OOP_3_sem/prj.cw/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
