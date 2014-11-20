cmake_minimum_required(VERSION 3.0)
project(BioFW.Standard)

file(GLOB_RECURSE BioFW.Standard.Sources "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${BioFW.Standard.Sources})

target_include_directories(${PROJECT_NAME} 
	PUBLIC include
)

target_link_libraries(${PROJECT_NAME}
	PocoFoundation
	PocoUtil
	PocoData
	BioFW.Core
)

INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)