# CMakeList.txt : CMake project for IPK, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("IPK")

# Add source to this project's executable.
add_executable (IPK "IPK.cpp"  "Common.h" "Response.cpp" "Server.cpp" "Client.cpp" "Packet.cpp" "Packet.h" "Server.h" "Response.h" "Client.h")

# TODO: Add tests and install targets if needed.
