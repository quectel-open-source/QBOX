cmake_minimum_required(VERSION 3.5)

project(QBOX LANGUAGES CXX)
set(CMAKE_C_COMPILER /usr/local/rb-debug-x86_64/sysroots/x86_64-qtisdk-linux/usr/bin/aarch64-oe-linux/aarch64-oe-linux-gcc)
set(CMAKE_CXX_COMPILER /usr/local/rb-debug-x86_64/sysroots/x86_64-qtisdk-linux/usr/bin/aarch64-oe-linux/aarch64-oe-linux-g++)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions("-Wall -g")

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


include_directories(.)
include_directories(./include)
include_directories(/home/ol/snpe/aarch_ffmpeg/include)

# json
include_directories(/home/ol/snpe/3rdparty/json/include)
set(JSON_LIB_DIR /home/ol/snpe/3rdparty/json/lib)
if(EXISTS ${JSON_LIB_DIR})
    link_directories(${JSON_LIB_DIR})
    set(JSON_LIBS jsoncpp)
endif()


# opencv
include_directories(/home/ol/snpe/3rdparty/opencv3.4/include)
set(OpenCV_LIB_DIR /home/ol/snpe/3rdparty/opencv3.4/lib)
if(EXISTS ${OpenCV_LIB_DIR})
    link_directories(${OpenCV_LIB_DIR})
    set(OpenCV_LIBS webp opencv_core opencv_imgcodecs opencv_imgproc opencv_dnn)
endif()

#perception
link_directories(/home/ol/snpe/3rdparty)
set(Perception_LIBS perception)

#ffmpeg
link_directories(/home/ol/snpe/aarch_ffmpeg/lib)
set(Ffmpeg_LIBS avcodec avdevice avfilter avformat avutil swresample swscale.so)

# snpe_2.12
include_directories(/home/ol/snpe/3rdparty/snpe-2.12/include)
set(SNPE_LIB_DIR /home/ol/snpe/3rdparty/snpe-2.12/lib/aarch64_Linux)
if(EXISTS ${SNPE_LIB_DIR})
    link_directories(${SNPE_LIB_DIR})
    set(SNPE_LIBS SNPE)
endif()

add_executable(QBOX
    main.cpp
    frmmain.cpp
    frmhost.cpp
    frmdata.cpp
    frmlogin.cpp
    frmlogout.cpp
    mydefencebutton.cpp
    frmdefence.cpp
    myswitchbutton.cpp
     frmselect.cpp
    frmpwd.cpp
    myapp.cpp
    frmconfig.cpp
    qffmpeg.cpp
    ffthread.cpp
    algothread.cpp
    imageWin.cpp
)

target_link_libraries(QBOX Qt${QT_VERSION_MAJOR}::Widgets
        ${OpenCV_LIBS}
        ${SNPE_LIBS}
        ${Perception_LIBS}
        ${JSON_LIBS}
        ${Ffmpeg_LIBS}) #

