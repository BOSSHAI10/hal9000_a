# Install script for directory: /home/alexandrusb/hal9000/HAL/User-mode/Applications/Tests/UserProg/BadActions

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
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadJumpKernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadJumpNoncanonical/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadJumpNull/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadReadIoPort/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadReadKernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadReadMsr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadReadNonCanonical/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadReadNull/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadWriteIoPort/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadWriteKernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadWriteMsr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadWriteNonCanonical/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/alexandrusb/hal9000/HAL/build/User-mode/Applications/Tests/UserProg/BadActions/BadWriteNull/cmake_install.cmake")
endif()

