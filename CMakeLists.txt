cmake_minimum_required(VERSION 3.5.0)
project(Proyecto-2 VERSION 1.0.0 LANGUAGES C CXX)

#include(CTest)
#enable_testing()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
#set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++2a")
endif()

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${INCLUDE_DIR}/*.h")

include_directories(${INCLUDE_DIR})

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${PUGIXML_INCLUDE_DIRS})

add_compile_options(-Winconsistent-missing-override)
target_link_libraries(${PROJECT_NAME} PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> 
                                              $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static> 
                                              $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
                                              $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
                                              $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
                                              pugixml::pugixml
                                              box2d::box2d)

#target_compile_definitions(Proyecto-2 PRIVATE uint32=unsigned\ __int32 uint64=unsigned\ __int64)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
include(InstallRequiredSystemLibraries)
include(CPack)
