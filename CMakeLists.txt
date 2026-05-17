cmake_minimum_required(VERSION 3.16)
project(DeviceMonitor VERSION 1.0.0)

# Build both sub-projects from the same root.
add_subdirectory(server)
add_subdirectory(client)
