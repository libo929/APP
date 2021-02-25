# cmake file for building ExampleContent
#-------------------------------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

PROJECT( CaloRec )

FOREACH( _path ${CMAKE_PREFIX_PATH} )                                                                                    
   FIND_PATH(pandora_cmake_path "PandoraCMakeSettings.cmake" ${_path}/cmakemodules)
   IF (pandora_cmake_path)
      BREAK()
   ENDIF()
ENDFOREACH() 

IF (pandora_cmake_path)
   LIST(APPEND CMAKE_MODULE_PATH ${pandora_cmake_path})
ENDIF()

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "ExampleContent requires an out-of-source build.")
endif()

# project version
set(${PROJECT_NAME}_VERSION_MAJOR 03)
set(${PROJECT_NAME}_VERSION_MINOR 00)
set(${PROJECT_NAME}_VERSION_PATCH 03)
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")

#-------------------------------------------------------------------------------------------------------------------------------------------

# Dependencies
include(PandoraCMakeSettings)

# Prefer local include directory to any paths to installed header files
include_directories(include)

find_package(PandoraSDK 03.00.00 REQUIRED)
include_directories(${PandoraSDK_INCLUDE_DIRS})
link_libraries(${PandoraSDK_LIBRARIES})
add_definitions(${PandoraSDK_DEFINITIONS})

find_package( ROOT REQUIRED )
include_directories(${ROOT_INCLUDE_DIRS})
link_libraries(${ROOT_LIBRARIES})
add_definitions(${ROOT_DEFINITIONS})

find_package( Boost REQUIRED )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

find_package( Armadillo REQUIRED )
include_directories( SYSTEM ${ARMADILLO_INCLUDE_DIRS} )

find_package( APRILContent REQUIRED )
if (APRILContent_FOUND)
	list(APPEND CMAKE_MODULE_PATH ${APRILContent_ROOT}/cmake)
else()
	message(FATAL_ERROR "APRILContent is not found.")
endif()

find_package( mlpack REQUIRED )
include_directories( SYSTEM ${mlpack_INCLUDE_DIRS} )
link_libraries( ${mlpack_LIBRARIES} )

include_directories(${APRILContent_INCLUDE_DIRS})
link_libraries(${APRILContent_LIBRARIES})
add_definitions(${APRILContent_DEFINITIONS})

if(PANDORA_MONITORING)
    find_package(PandoraMonitoring 03.00.00 REQUIRED)
    include_directories(${PandoraMonitoring_INCLUDE_DIRS})
    link_libraries(${PandoraMonitoring_LIBRARIES})
    add_definitions(${PandoraMonitoring_DEFINITIONS})
    add_definitions("-DMONITORING")

    list(APPEND CMAKE_MODULE_PATH "$ENV{ROOTSYS}/etc/cmake/")
    find_package(ROOT 5.26.00 REQUIRED COMPONENTS Eve Geom RGL EG)
endif()

message(${CMAKE_CURRENT_SOURCE_DIR})

#-------------------------------------------------------------------------------------------------------------------------------------------
# Low level settings - compiler etc
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -Wno-long-long -Wno-sign-compare -fno-strict-aliasing ${CMAKE_CXX_FLAGS}")

option(BUILD_WITH_OPENMP "." ON)

if(BUILD_WITH_OPENMP)
	set(CMAKE_CXX_FLAGS "-fopenmp ${CMAKE_CXX_FLAGS}")
endif()

include(CheckCXXCompilerFlag)
unset(COMPILER_SUPPORTS_CXX_FLAGS CACHE)
CHECK_CXX_COMPILER_FLAG(${CMAKE_CXX_FLAGS} COMPILER_SUPPORTS_CXX_FLAGS)

if(NOT COMPILER_SUPPORTS_CXX_FLAGS)
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support cxx flags ${CMAKE_CXX_FLAGS}")
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Build products

# - Collect sources - not ideal because you have to keep running CMake to pick up changes
file(GLOB_RECURSE EXAMPLE_CONTENT_SRCS RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cc")

# - Add library and properties
add_library(${PROJECT_NAME} SHARED ${EXAMPLE_CONTENT_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_SOVERSION})

# - Executable
add_executable(runRec ${PROJECT_SOURCE_DIR}/test/runRec.cc)
if(PANDORA_MONITORING)
    include_directories(${ROOT_INCLUDE_DIRS})
    target_link_libraries(runRec ${ROOT_LIBRARIES})
endif()
target_link_libraries(runRec ${PROJECT_NAME})

#-------------------------------------------------------------------------------------------------------------------------------------------
# - library
install(TARGETS ${PROJECT_NAME} DESTINATION lib COMPONENT Runtime)

# - headers
#install(DIRECTORY include/ DESTINATION include COMPONENT Development FILES_MATCHING PATTERN "*.h")

# - executable
install(TARGETS runRec DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

#-------------------------------------------------------------------------------------------------------------------------------------------
# display some variables and write them to cache
PANDORA_DISPLAY_STD_VARIABLES()
