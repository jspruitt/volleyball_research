cmake_minimum_required(VERSION 2.8)
project( cpp_court_line )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(BOOST_ROOT  "${BOOST_ROOT} /usr/local/Cellar/boost/1.69.0_2")
set(BOOST_INCLUDEDIR  "${BOOST_INCLUDEDIR} /usr/local/Cellar/boost/1.69.0_2/include")

find_package( OpenCV REQUIRED )

find_package(Boost 1.69.0 COMPONENTS filesystem system iostreams REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/externals/gnuplot-iostream")

add_executable( cpp_court_line
        main.cpp
        Line.cpp
        LineComparator.cpp
        Line.h
        DrawDebug.cpp
        DrawDebug.h
        VolleyballCourtFitter.cpp
        VolleyballCourtFitter.h
        VolleyballCourtModel.cpp
        VolleyballCourtModel.h
        PixelDetection.cpp
        PixelDetection.h
        LineDetection.cpp
        LineDetection.h
        InitialBallTrack.cpp
        InitialBallTrack.h
        Plotter.cpp
        Plotter.h ../castles/main.cpp ../castles/DominantColor.cpp ../castles/DominantColor.h)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( cpp_court_line ${OpenCV_LIBS} ${Boost_LIBRARIES})




