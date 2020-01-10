cmake_minimum_required(VERSION 3.5)

project(audiocodec)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)

include(ExternalProject)

# requires nasm or yasm installed

ExternalProject_Add(ex_ffmpeg
#    INSTALL_DIR ${EX_INSTALL_DIR}
    GIT_REPOSITORY "https://github.com/FFmpeg/FFmpeg.git"
    GIT_TAG "ee66e04bc9dbbcf95114a103f174ed54b2260758" # this is tag n4.0.4
    CONFIGURE_COMMAND <SOURCE_DIR>/configure
        # Standard options:
        --prefix=<INSTALL_DIR>
        # Configuraion optiosn:
        --enable-static
        --disable-shared
        --disable-autodetect
        # Documentation options:
        --disable-doc
        --disable-htmlpages
        --disable-manpages
        --disable-podpages
        --disable-txtpages # disable documentation build # is only --disable-doc enough?
        # Component options:
        --disable-avdevice
        --disable-swscale
        --disable-postproc
        --disable-avfilter
#        --disable-pthreads
#        --disable-w32threads
#        --disable-os2threads
        --disable-network
        --disable-pixelutils
        # Individual component options:
#        --disable-parsers
#        --disable-bsfs
#        --disable-protocols
#        --disable-devices
#        --disable-filters

        ## TODO check if there are more codecs/muxers we don't need

#        --disable-decoder=bmp
#        --disable-decoder=bmv_video
#        --disable-decoder=clearvideo
#        --disable-decoder=dsicinvideo
#        --disable-decoder=dvvideo
#        --disable-decoder=h261
#        --disable-decoder=h263_v4l2m2m
#        --disable-decoder=h263
#        --disable-decoder=h263i
#        --disable-decoder=h263p
#        --disable-decoder=h264_v4l2m2m
#        --disable-decoder=h264
#        --disable-decoder=hnm4_video
#        --disable-decoder=interplay_video
#        --disable-decoder=jpeg2000
#        --disable-decoder=mjpeg
#        --disable-decoder=mjpegb
#        --disable-decoder=mmvideo
#        --disable-decoder=mpeg1_v4l2m2m
#        --disable-decoder=mpeg1video
#        --disable-decoder=mpeg2_v4l2m2m
#        --disable-decoder=mpeg2video
#        --disable-decoder=mpeg4_v4l2m2m
#        --disable-decoder=mpeg4
#        --disable-decoder=mpegvideo
#        --disable-decoder=msmpeg4v1
#        --disable-decoder=msmpeg4v2
#        --disable-decoder=msmpeg4v3
#        --disable-decoder=msvideo1
#        --disable-decoder=paf_video
#        --disable-decoder=rawvideo
#        --disable-decoder=sheervideo
#        --disable-decoder=smvjpeg
#        --disable-decoder=tiertexseqvideo
#        --disable-decoder=utvideo
#        --disable-decoder=vc1_v4l2m2m
#        --disable-decoder=vmdvideo
#        --disable-decoder=vp8_v4l2m2m

#        --disable-encoder=bmp
#        --disable-encoder=dvvideo
#        --disable-encoder=h261
#        --disable-encoder=h263
#        --disable-encoder=h263_v4l2m2m
#        --disable-encoder=h263p
#        --disable-encoder=h264_v4l2m2m
#        --disable-encoder=jpeg2000
#        --disable-encoder=mjpeg
#        --disable-encoder=mpeg1video
#        --disable-encoder=mpeg2video
#        --disable-encoder=mpeg4
#        --disable-encoder=mpeg4_v4l2m2m
#        --disable-encoder=msmpeg4v2
#        --disable-encoder=msmpeg4v3
#        --disable-encoder=msvideo1
#        --disable-encoder=rawvideo
#        --disable-encoder=utvideo
#        --disable-encoder=vp8_v4l2m2m

#        --disable-demuxer=avi
#        --disable-demuxer=cavsvideo
#        --disable-demuxer=h261
#        --disable-demuxer=h263
#        --disable-demuxer=h264
#        --disable-demuxer=image2
#        --disable-demuxer=image2_alias_pix
#        --disable-demuxer=image2pipe
#        --disable-demuxer=image_bmp_pipe
#        --disable-demuxer=image_dds_pipe
#        --disable-demuxer=image_dpx_pipe
#        --disable-demuxer=image_exr_pipe
#        --disable-demuxer=image_j2k_pipe
#        --disable-demuxer=image_jpeg_pipe
#        --disable-demuxer=image_jpegls_pipe
#        --disable-demuxer=image_pam_pipe
#        --disable-demuxer=image_pbm_pipe
#        --disable-demuxer=image_pcx_pipe
#        --disable-demuxer=image_pgm_pipe
#        --disable-demuxer=image_pgmyuv_pipe
#        --disable-demuxer=image_pictor_pipe
#        --disable-demuxer=image_png_pipe
#        --disable-demuxer=image_psd_pipe
#        --disable-demuxer=image_qdraw_pipe
#        --disable-demuxer=image_sgi_pipe
#        --disable-demuxer=image_sunrast_pipe
#        --disable-demuxer=image_svg_pipe
#        --disable-demuxer=image_tiff_pipe
#        --disable-demuxer=image_webp_pipe
#        --disable-demuxer=image_xpm_pipe
#        --disable-demuxer=ipmovie
#        --disable-demuxer=mjpeg
#        --disable-demuxer=mjpeg_2000
#        --disable-demuxer=mov
#        --disable-demuxer=mpegvideo
#        --disable-demuxer=mpjpeg
#        --disable-demuxer=rawvideo

#        --disable-muxer=apng
#        --disable-muxer=avi
#        --disable-muxer=cavsvideo
#        --disable-muxer=gif
#        --disable-muxer=h261
#        --disable-muxer=h263
#        --disable-muxer=h264
#        --disable-muxer=image2
#        --disable-muxer=image2pipe
#        --disable-muxer=mjpeg
#        --disable-muxer=mov
#        --disable-muxer=mpeg1vcd
#        --disable-muxer=mpeg1video
#        --disable-muxer=mpeg2dvd
#        --disable-muxer=mpeg2svcd
#        --disable-muxer=mpeg2video
#        --disable-muxer=mpeg2vob
#        --disable-muxer=mpegts
#        --disable-muxer=mpjpeg
#        --disable-muxer=rawvideo
#        --disable-muxer=singlejpeg

        # External library support:
        --disable-alsa
        --disable-appkit
        --disable-avfoundation
        --disable-bzlib
        --disable-coreimage
        --disable-iconv
        --disable-libxcb
        --disable-libxcb-shm
        --disable-libxcb-xfixes
        --disable-libxcb-shape
        --disable-lzma
        --disable-sndio
        --disable-schannel
        --disable-sdl2
        --disable-securetransport
        --disable-xlib
        --disable-zlib

        # Hardware acceleration
        --disable-amf
        --disable-audiotoolbox
        --disable-cuvid
        --disable-d3d11va
        --disable-dxva2
        --disable-ffnvcodec
        --disable-nvdec
        --disable-nvenc
        --disable-v4l2-m2m
        --disable-vaapi
        --disable-vdpau
        --disable-videotoolbox

        # Toolchain options:
        --enable-pic
    BUILD_COMMAND make -j4
    INSTALL_COMMAND make install
    )

ExternalProject_Get_Property(ex_ffmpeg INSTALL_DIR)
set(FFMPEG_INSTALL_DIR ${INSTALL_DIR})

set(CFLAGS_PARAM "CFLAGS=-fPIC")

ExternalProject_Add(ex_rnnoise
#    INSTALL_DIR ${EX_INSTALL_DIR}
    GIT_REPOSITORY "https://github.com/xiph/rnnoise.git"
    GIT_TAG "9acc1e5a633e0961a5895a73204df24744f199b6"
    CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh # 2> /dev/null
    # TODO autogen prints some staff to stderr (and it is not an error), should we suppress this output?
    COMMAND ${CFLAGS_PARAM} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --enable-static --disable-shared
    BUILD_COMMAND make
    INSTALL_COMMAND make install
    )

ExternalProject_Get_Property(ex_rnnoise INSTALL_DIR)
set(RNNOISE_INSTALL_DIR ${INSTALL_DIR})
include_directories(${INSTALL_DIR}/include)


## to build ffmpeg
# mkdir ffbuild
# cd ffbuild
## TODO there are a lot more parameters we might want to add there
# <SOURCE_DIR>/configure --prefix=<path> --enable-static --disable-shared --disable-doc --disable-htmlpages --disable-manpages --disable-podpages --disable-txtpages --enable-pic
# make
# make install
## to build rnnoise
# ./autogen.sh
# export CFLAGS="-fPIC"
# ./configure --prefix=<path> --disable-shared --enable-static
# make
# make install

include_directories(${FFMPEG_INSTALL_DIR}/include
    ${RNNOISE_INSTALL_DIR}/include)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-Bsymbolic")

#include_directories(${DEPS_INSTALL_DIR}/include)
#link_directories(${DEPS_INSTALL_DIR}/lib)


add_library(audiocodec SHARED
    audiocodec.h audiocodec.cpp
    synctask.h synctask.cpp
    ffmpegdecoder.h ffmpegdecoder.cpp
    ffmpegencodermodule.h ffmpegencodermodule.cpp
    ffmpegdecodermodule.h ffmpegdecodermodule.cpp
    ffmpegcodec.h ffmpegcodec.cpp
    ffmpegframe.h ffmpegframe.cpp
    ffmpegpacket.h ffmpegpacket.cpp
    ffmpegresampler.h ffmpegresampler.cpp
    outbuffermodule.h outbuffermodule.cpp
    taskappendmodule.h
    taskjoinmodule.h
    taskoutbuffer.h taskoutbuffer.cpp
    taskstartmodule.h rnnoisemodule.cpp
    rnnoisemodule.h
    results.h
    parameters.h
    parameters_rnnoise_off.h

    queue_buffer.h
    plain_queue_buffer.h plain_queue_buffer.cpp
    whole_plain_queue_buffer.h whole_plain_queue_buffer.cpp
    threadsafe_queue_buffer.h
    )

add_dependencies(audiocodec ex_ffmpeg ex_rnnoise)

target_include_directories(audiocodec PUBLIC ${DEPS_INSTALL_DIR}/include)

target_compile_options(audiocodec PRIVATE -Wall -pedantic -pedantic-errors) # -g

target_link_libraries(audiocodec
    PRIVATE

    ${FFMPEG_INSTALL_DIR}/lib/libavformat.a
    ${FFMPEG_INSTALL_DIR}/lib/libavcodec.a
    ${FFMPEG_INSTALL_DIR}/lib/libswresample.a
    ${FFMPEG_INSTALL_DIR}/lib/libavutil.a
    ${RNNOISE_INSTALL_DIR}/lib/librnnoise.a

    pthread m
)

add_executable(demo example.cpp)

target_link_libraries(demo audiocodec pthread m)
target_compile_options(demo PRIVATE -Wall -pedantic -pedantic-errors) # -g
