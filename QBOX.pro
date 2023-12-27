#-------------------------------------------------
#
# Project created by QtCreator 2013-12-30T11:05:41
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QBox
TEMPLATE = app

DESTDIR     = $$PWD/bin
#自动拷贝文件
src_file = $$PWD/host.json
dst_file = $$DESTDIR
win32 {
#转换斜杠 / 到 \\
src_file ~= s,/,\\,g
dst_file ~= s,/,\\,g
dst_file2 ~= s,/,\\,g
QMAKE_POST_LINK += copy $$src_file $$dst_file
}
unix {
QMAKE_POST_LINK += cp -r -f $$src_file $$dst_file
}

QT += multimedia
SOURCES += main.cpp\
        frmmain.cpp \
    frmhost.cpp \
    frmdata.cpp \
    frmlogin.cpp \
    frmlogout.cpp \
    mydefencebutton.cpp \
    frmdefence.cpp \
    myswitchbutton.cpp \
    frmselect.cpp \
    frmpwd.cpp \
    myapp.cpp \
    frmconfig.cpp \
    qffmpeg.cpp \
    ffthread.cpp \
    algothread.cpp \
    imageWin.cpp

LIBS += -L$$PWD/libs -L/home/quectel/zhangleo/3rdparty_x86/ -ljsoncpp -lSNPE -lperception
LIBS += -lopencv_core -lopencv_dnn -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lpthread

HEADERS  += frmmain.h \
    myhelper.h \
    frmhost.h \
    frmdata.h \
    frmlogin.h \
    frmlogout.h \
    mydefencebutton.h \
    frmdefence.h \
    myswitchbutton.h \
    frmconfig.h \
    frmselect.h \
    frmpwd.h \
    myapp.h \
    json/allocator.h \
    json/assertions.h \
    json/config.h \
    json/forwards.h \
    json/json.h \
    json/json_features.h \
    json/reader.h \
    json/value.h \
    json/version.h \
    json/writer.h \
    ffmpeg/include/libavcodec/avcodec.h \
    ffmpeg/include/libavcodec/avfft.h \
    ffmpeg/include/libavcodec/dxva2.h \
    ffmpeg/include/libavcodec/old_codec_ids.h \
    ffmpeg/include/libavcodec/vaapi.h \
    ffmpeg/include/libavcodec/vda.h \
    ffmpeg/include/libavcodec/vdpau.h \
    ffmpeg/include/libavcodec/version.h \
    ffmpeg/include/libavcodec/xvmc.h \
    ffmpeg/include/libavfilter/asrc_abuffer.h \
    ffmpeg/include/libavfilter/avcodec.h \
    ffmpeg/include/libavfilter/avfilter.h \
    ffmpeg/include/libavfilter/avfiltergraph.h \
    ffmpeg/include/libavfilter/buffersink.h \
    ffmpeg/include/libavfilter/buffersrc.h \
    ffmpeg/include/libavfilter/version.h \
    ffmpeg/include/libavformat/avformat.h \
    ffmpeg/include/libavformat/avio.h \
    ffmpeg/include/libavformat/version.h \
    ffmpeg/include/libavutil/adler32.h \
    ffmpeg/include/libavutil/aes.h \
    ffmpeg/include/libavutil/attributes.h \
    ffmpeg/include/libavutil/audio_fifo.h \
    ffmpeg/include/libavutil/audioconvert.h \
    ffmpeg/include/libavutil/avassert.h \
    ffmpeg/include/libavutil/avconfig.h \
    ffmpeg/include/libavutil/avstring.h \
    ffmpeg/include/libavutil/avutil.h \
    ffmpeg/include/libavutil/base64.h \
    ffmpeg/include/libavutil/blowfish.h \
    ffmpeg/include/libavutil/bprint.h \
    ffmpeg/include/libavutil/bswap.h \
    ffmpeg/include/libavutil/buffer.h \
    ffmpeg/include/libavutil/channel_layout.h \
    ffmpeg/include/libavutil/common.h \
    ffmpeg/include/libavutil/cpu.h \
    ffmpeg/include/libavutil/crc.h \
    ffmpeg/include/libavutil/dict.h \
    ffmpeg/include/libavutil/downmix_info.h \
    ffmpeg/include/libavutil/error.h \
    ffmpeg/include/libavutil/eval.h \
    ffmpeg/include/libavutil/ffversion.h \
    ffmpeg/include/libavutil/fifo.h \
    ffmpeg/include/libavutil/file.h \
    ffmpeg/include/libavutil/frame.h \
    ffmpeg/include/libavutil/hmac.h \
    ffmpeg/include/libavutil/imgutils.h \
    ffmpeg/include/libavutil/intfloat.h \
    ffmpeg/include/libavutil/intfloat_readwrite.h \
    ffmpeg/include/libavutil/intreadwrite.h \
    ffmpeg/include/libavutil/lfg.h \
    ffmpeg/include/libavutil/log.h \
    ffmpeg/include/libavutil/lzo.h \
    ffmpeg/include/libavutil/macros.h \
    ffmpeg/include/libavutil/mathematics.h \
    ffmpeg/include/libavutil/md5.h \
    ffmpeg/include/libavutil/mem.h \
    ffmpeg/include/libavutil/murmur3.h \
    ffmpeg/include/libavutil/old_pix_fmts.h \
    ffmpeg/include/libavutil/opt.h \
    ffmpeg/include/libavutil/parseutils.h \
    ffmpeg/include/libavutil/pixdesc.h \
    ffmpeg/include/libavutil/pixfmt.h \
    ffmpeg/include/libavutil/random_seed.h \
    ffmpeg/include/libavutil/rational.h \
    ffmpeg/include/libavutil/replaygain.h \
    ffmpeg/include/libavutil/ripemd.h \
    ffmpeg/include/libavutil/samplefmt.h \
    ffmpeg/include/libavutil/sha.h \
    ffmpeg/include/libavutil/sha512.h \
    ffmpeg/include/libavutil/stereo3d.h \
    ffmpeg/include/libavutil/time.h \
    ffmpeg/include/libavutil/timecode.h \
    ffmpeg/include/libavutil/timestamp.h \
    ffmpeg/include/libavutil/version.h \
    ffmpeg/include/libavutil/xtea.h \
    ffmpeg/include/libswscale/swscale.h \
    ffmpeg/include/libswscale/version.h \
    qffmpeg.h \
    ffthread.h \
    algothread.h \
    imageWin.h \
    include/DeepPlate.hpp \
    include/Smoke.hpp \
    include/Masks.hpp \
    include/Helmet.hpp \
    include/ErrorCode.hpp \
    include/ObjectType.hpp \
    include/DeepFace.hpp \
    include/Pedestrian.hpp \
    include/FaceType.hpp

FORMS    += frmmain.ui \
    frmhost.ui \
    frmdata.ui \
    frmlogin.ui \
    frmlogout.ui \
    frmdefence.ui \
    frmconfig.ui \
    frmselect.ui \
    frmpwd.ui

FFMPEG_LIB      = /usr/local/ffmpeg/lib

LIBS += $$FFMPEG_LIB/libavcodec.so      \
        $$FFMPEG_LIB/libavdevice.so     \
        $$FFMPEG_LIB/libavfilter.so     \
        $$FFMPEG_LIB/libavformat.so     \
        $$FFMPEG_LIB/libavutil.so       \
        $$FFMPEG_LIB/libswresample.so   \
        $$FFMPEG_LIB/libswscale.so

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj

INCLUDEPATH +=  /home/quectel/zhangleo/3rdparty_x86/opencv3.4/include
INCLUDEPATH +=  /home/quectel/zhangleo/3rdparty_x86
INCLUDEPATH +=  /usr/local/ffmpeg/include

win32:RC_FILE=main.rc

RESOURCES += \
    main.qrc

CONFIG += warn_off

DISTFILES += \
    style.qss
