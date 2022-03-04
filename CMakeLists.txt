# CMakeList.txt : CMake project for IPK, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("hinfosvc")

set(CMAKE_CXX_STANDARD 17)

# Add source to this project's executable.
add_executable (hinfosvc "IPK.cpp"  "Common.h" "Response.cpp" "Server.cpp" "Client.cpp" "Packet.cpp" "Packet.h" "Server.h" "Response.h" "Client.h")

# TODO: Add tests and install targets if needed.
