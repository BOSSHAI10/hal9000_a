# Install script for directory: /home/alexandrusb/hal9000/HAL/User-mode/Applications/Tests/UserProg/FileSyscalls

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "../artifacts")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCloseBad/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCloseNormal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCloseStdout/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCloseTwice/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCreateBadPointer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCreateEmptyPath/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCreateExistent/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCreateMissing/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCreateNormal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCreateNull/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileCreateTwice/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileReadBadHandle/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileReadBadPointer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileReadKernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileReadNormal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileReadStdout/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/FileSyscalls/FileReadZero/cmake_install.cmake")
endif()

