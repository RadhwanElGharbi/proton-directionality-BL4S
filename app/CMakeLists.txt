## ---------------------------------------------------------
##  G4_QPIX | app/CMakeLists.txt
##
##  CMake build script.
##   * Author: Justo Martin-Albo
##   * Creation date: 14 Aug 2019
## ---------------------------------------------------------


add_executable(directionality01 directionality01.cpp)
target_include_directories(directionality01 PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(directionality01 ${CMAKE_PROJECT_NAME} ${Geant4_LIBRARIES})

install(TARGETS directionality01 RUNTIME DESTINATION bin)
