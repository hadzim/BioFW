cmake_minimum_required(VERSION 3.0)
project(BioFW.OpenCV)

file(GLOB_RECURSE BioFW.OpenCV.Sources "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${BioFW.OpenCV.Sources})

find_package( OpenCV REQUIRED )

target_include_directories(${PROJECT_NAME} 
	PUBLIC include
)

target_link_libraries(${PROJECT_NAME}
	PocoFoundation
	PocoUtil
	PocoData
	BioFW.Core
	BioFW.Standard
	${OpenCV_LIBS}
)

INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)