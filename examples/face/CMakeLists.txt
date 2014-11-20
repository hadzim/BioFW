cmake_minimum_required(VERSION 3.0)
project(BioFW.Example.Face)

file(GLOB_RECURSE BioFW.Example.Face.Sources "src/*.cpp")

add_executable(${PROJECT_NAME} ${BioFW.Example.Face.Sources})

target_link_libraries(${PROJECT_NAME}
	BioFW.Core
	BioFW.Standard
	BioFW.OpenCV
)

INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)