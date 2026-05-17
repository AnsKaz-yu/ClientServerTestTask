# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "client\\CMakeFiles\\DeviceMonitorClient_autogen.dir\\AutogenUsed.txt"
  "client\\CMakeFiles\\DeviceMonitorClient_autogen.dir\\ParseCache.txt"
  "client\\DeviceMonitorClient_autogen"
  "server\\CMakeFiles\\DeviceMonitorServer_autogen.dir\\AutogenUsed.txt"
  "server\\CMakeFiles\\DeviceMonitorServer_autogen.dir\\ParseCache.txt"
  "server\\DeviceMonitorServer_autogen"
  )
endif()
