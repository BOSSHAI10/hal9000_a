# Install script for directory: /home/alexandrusb/hal9000/HAL/User-mode/Applications/Tests/UserProg/ProcessSyscalls

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
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessCloseFile/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessCloseNormal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessCloseParentHandle/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessCloseTwice/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessCreateBadPointer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessCreateMissingFile/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessCreateMultiple/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessCreateOnce/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessCreateWithArguments/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessExit/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessGetPid/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessWaitBadHandle/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessWaitClosedHandle/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessWaitNormal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/ProcessSyscalls/ProcessWaitTerminated/cmake_install.cmake")
endif()

