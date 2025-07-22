
cmake_minimum_required(VERSION 3.16)
project(presenter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick Multimedia)

find_path(POPPLER_INCLUDE_DIRS poppler-qt6.h HINTS /usr/include/poppler/qt6)
find_library(POPPLER_LIBRARIES poppler-qt6)

include_directories(${POPPLER_INCLUDE_DIRS})

qt_standard_project_setup(REQUIRES 6.5)

# qt5_wrap_cpp(MOC_SRCS
# 	rendering_pool.h
# 	image_view.h
# 	view_controller.h
# 	video_object.h
# )

qt_add_executable(presenter
	${MOC_SRCS}
	presenter.cpp
	rendering_pool.cpp
	image_view.cpp
	view_controller.cpp
	video_object.cpp
)
target_link_libraries(presenter PRIVATE
	Qt6::Widgets
	Qt6::Quick
	Qt6::Multimedia
	${POPPLER_LIBRARIES}
)
