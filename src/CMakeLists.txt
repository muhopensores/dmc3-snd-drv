
set(SND_SRC
main.cpp
VoxObj.hpp
VoxObj.cpp
)

add_library(${CMAKE_PROJECT_NAME} SHARED ${SND_SRC})

target_link_libraries (${CMAKE_PROJECT_NAME} soloud_static)
