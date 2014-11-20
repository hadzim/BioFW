cmake_minimum_required(VERSION 3.0)
project(BioFW.Core)


file(GLOB_RECURSE BioFW.Core.Sources "src/*.cpp")


add_library(${PROJECT_NAME} SHARED ${BioFW.Core.Sources})


target_include_directories(${PROJECT_NAME} 
	PUBLIC include
)

target_link_libraries(${PROJECT_NAME}
	PocoFoundation
	PocoUtil
	PocoData
)

INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)