
cmake_minimum_required(VERSION 3.0)
project(presenter)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Multimedia REQUIRED)

find_path(POPPLER_INCLUDE_DIRS poppler-qt5.h HINTS /usr/include/poppler/qt5)
find_library(POPPLER_LIBRARIES poppler-qt5)

include_directories(${POPPLER_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp")

qt5_wrap_cpp(MOC_SRCS
	rendering_pool.h
	image_view.h
	view_controller.h
	video_object.h
)

add_executable(presenter
	${MOC_SRCS}
	presenter.cpp
	rendering_pool.cpp
	image_view.cpp
	view_controller.cpp
	video_object.cpp
)
target_link_libraries(presenter
	Qt5::Widgets
	Qt5::Quick
	Qt5::Multimedia
	${POPPLER_LIBRARIES}
)
