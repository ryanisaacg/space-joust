cmake_minimum_required(VERSION 2.4.0)
project (space-joust)
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0015 NEW)

#SET WARNINGS
if(MSVC)
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wfatal-errors -pedantic -std=c99 -Wno-unused-parameter")
endif()

file(GLOB SOURCES *.c)
include_directories(. au-engine)

add_subdirectory(au-engine)

add_executable(game ${SOURCES})
set_property(TARGET game PROPERTY C_STANDARD 99)
set_target_properties(game PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(game au-engine m SDL2_gpu SDL2)
if(WIN32)
	target_link_libraries(game opengl32)
else()
	target_link_libraries(game GL)
endif()

