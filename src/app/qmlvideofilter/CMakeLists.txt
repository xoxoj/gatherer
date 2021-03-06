# Copyright (c) 2015, Ruslan Baratov
# All rights reserved.

set(CMAKE_INCLUDE_CURRENT_DIR ON) # for main.moc

find_package(Qt5Quick REQUIRED) # Qt5::Quick
find_package(Qt5Multimedia REQUIRED) # Qt5::QMultimediaDeclarativeModule

set(
  sources
  main.cpp
  FrameHandler.h
  FrameHandler.cpp
  TextureBuffer.hpp
  VideoFilter.hpp
  VideoFilter.cpp
  VideoFilterRunnable.hpp
  VideoFilterRunnable.cpp
  InfoFilter.hpp
  InfoFilter.cpp
  InfoFilterResult.hpp
  InfoFilterRunnable.hpp
  InfoFilterRunnable.cpp
  FPSCalculator.hpp
  FPSCalculator.cpp
  qmlvideofilter.qrc
  main.qml
  QTRenderGL.hpp
  QTRenderGL.cpp
  QVideoFrameScopeMap.h
  )

if(ANDROID)
  add_library(qmlvideofilter SHARED ${sources})

  target_link_libraries(qmlvideofilter Qt5::Quick Qt5::Multimedia)
  target_link_libraries(qmlvideofilter EGL GLESv2)

  set_target_properties(
    qmlvideofilter
    PROPERTIES
    VERSION 1.0
    DEBUG_POSTFIX "" # Fix Android load
    )

  hunter_add_package(QtAndroidCMake)
  list(APPEND CMAKE_MODULE_PATH "${QTANDROIDCMAKE_ROOT}")
  include(AddQtAndroidApk)

  add_qt_android_apk(
    TARGET qmlvideofilter-apk
    BASE_TARGET qmlvideofilter
    LAUNCH_TARGET qmlvideofilter-launch
    PACKAGE_NAME qmlvideofiler.qt
    MANIFEST "${CMAKE_CURRENT_LIST_DIR}/AndroidManifest.xml"
    QML_ROOT "${CMAKE_CURRENT_LIST_DIR}"
    )

  hunter_add_package(Android-SDK)
  message("Path to `android`: ${ANDROID-SDK_ROOT}/android-sdk/tools/android")
  message("Path to `emulator`: ${ANDROID-SDK_ROOT}/android-sdk/tools/emulator")
  message("Path to `adb`: ${ANDROID-SDK_ROOT}/android-sdk/platform-tools/adb")

else()
  find_package(Qt5Quick REQUIRED) # Qt5::Quick
  find_package(Qt5Qml REQUIRED) # Qt5::QmlQtQuick2Plugin

  # We should load this manually because of cyclic dependencies in Qt
  find_package(Qt5MultimediaWidgets REQUIRED)

  if(IOS)
    set(qt_conf "${CMAKE_CURRENT_LIST_DIR}/qt.conf")
    set(qml_dir "${CMAKE_CURRENT_BINARY_DIR}/qt_qml")

    hunter_add_package(QtQmlManager)
    list(APPEND CMAKE_MODULE_PATH "${QTQMLMANAGER_ROOT}/cmake")
    include(QtCopyQmlTo)
    QtCopyQmlTo(${qml_dir})
  else()
    set(qt_conf "")
    set(qml_dir "")
  endif()

  add_executable(qmlvideofilter MACOSX_BUNDLE ${sources} ${qt_conf} ${qml_dir})

  set_target_properties(
    qmlvideofilter
    PROPERTIES
    RESOURCE "${qt_conf};${qml_dir}"
    )

  if(IOS)
    set_target_properties(
      qmlvideofilter
      PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/plist.in"
      XCODE_ATTRIBUTE_PRODUCT_NAME
      "QmlVideoFilterExample"
      XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER
      "com.github.headupinclouds.gatherer.qmlvideofilter"
      XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2" # iPhone/iPad
      )
  endif()
  target_link_libraries(qmlvideofilter Qt5::Quick)
  target_link_libraries(qmlvideofilter Qt5::Multimedia)
  target_link_libraries(qmlvideofilter Qt5::OpenGL)

  ### Plugins
  target_link_libraries(qmlvideofilter Qt5::QmlQtQuick2Plugin)
  target_link_libraries(qmlvideofilter ${Qt5Multimedia_PLUGINS})
  target_link_libraries(qmlvideofilter Qt5::QMultimediaDeclarativeModule)
  if(IOS)
    target_link_libraries(qmlvideofilter ${Qt5Gui_PLUGINS})
  else()
    find_package(Qt5PrintSupport REQUIRED)
    target_link_libraries(qmlvideofilter Qt5::PrintSupport)
  endif()
endif()

target_link_libraries(qmlvideofilter gatherer_graphics ogles_gpgpu libyuv::yuv OGLESGPGPUTest)
