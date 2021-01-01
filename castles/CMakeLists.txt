cmake_minimum_required(VERSION 2.8)
project( castles )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(BOOST_ROOT  "${BOOST_ROOT} /usr/local/Cellar/boost/1.69.0_2")
set(BOOST_INCLUDEDIR  "${BOOST_INCLUDEDIR} /usr/local/Cellar/boost/1.69.0_2/include")

find_package( OpenCV REQUIRED )

find_package(Boost 1.69.0 COMPONENTS filesystem system iostreams REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/externals/gnuplot-iostream")

add_executable( castles
        main.cpp
        Line.cpp
        LineComparator.cpp
        Line.h
        DrawDebug.cpp
        DrawDebug.h
        DominantColor.cpp
        DominantColor.h
        VolleyballCourtFitter.cpp
        VolleyballCourtFitter.h
        VolleyballCourtModel.cpp
        VolleyballCourtModel.h
        PixelDetection.cpp
        PixelDetection.h
        LineDetection.cpp
        LineDetection.h
        BallCandidateProducing.cpp
        BallCandidateProducing.h
        Plotter.cpp
        Plotter.h )
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( castles ${OpenCV_LIBS} ${Boost_LIBRARIES})




