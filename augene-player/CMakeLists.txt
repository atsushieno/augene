
cmake_minimum_required(VERSION 3.12)

project(AUGENE_PLAYER VERSION 0.1.0)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 gtk+-x11-3.0)

add_subdirectory(juce-symlink)

juce_add_gui_app(augene-player
	PRODUCT_NAME "augene-player")

juce_add_module(../external/tracktion_engine/modules/tracktion_engine)

target_include_directories(augene-player PRIVATE
	${GTK3_INCLUDEDIR}
	../external/tracktion_engine/modules/
	../external/efsw/include/
	../external/efsw/src/)

target_sources(augene-player PRIVATE
../external/efsw/src/efsw/platform/posix/SystemImpl.cpp
../external/efsw/src/efsw/platform/posix/ThreadImpl.cpp
../external/efsw/src/efsw/platform/posix/FileSystemImpl.cpp
../external/efsw/src/efsw/platform/posix/MutexImpl.cpp
../external/efsw/src/efsw/platform/win/SystemImpl.cpp
../external/efsw/src/efsw/platform/win/ThreadImpl.cpp
../external/efsw/src/efsw/platform/win/FileSystemImpl.cpp
../external/efsw/src/efsw/platform/win/MutexImpl.cpp
../external/efsw/src/efsw/DirWatcherGeneric.cpp
../external/efsw/src/efsw/WatcherGeneric.cpp
../external/efsw/src/efsw/FileWatcherCWrapper.cpp
../external/efsw/src/efsw/Debug.cpp
../external/efsw/src/efsw/FileInfo.cpp
../external/efsw/src/efsw/Log.cpp
../external/efsw/src/efsw/DirectorySnapshot.cpp
../external/efsw/src/efsw/Watcher.cpp
../external/efsw/src/efsw/FileSystem.cpp
../external/efsw/src/efsw/DirectorySnapshotDiff.cpp
../external/efsw/src/efsw/WatcherInotify.cpp
../external/efsw/src/efsw/System.cpp
../external/efsw/src/efsw/FileWatcherGeneric.cpp
../external/efsw/src/efsw/FileWatcherKqueue.cpp
../external/efsw/src/efsw/Thread.cpp
../external/efsw/src/efsw/FileWatcherFSEvents.cpp
../external/efsw/src/efsw/FileWatcherImpl.cpp
../external/efsw/src/efsw/WatcherKqueue.cpp
../external/efsw/src/efsw/FileWatcherInotify.cpp
../external/efsw/src/efsw/FileWatcher.cpp
../external/efsw/src/efsw/WatcherFSEvents.cpp
../external/efsw/src/efsw/FileWatcherWin32.cpp
../external/efsw/src/efsw/WatcherWin32.cpp
../external/efsw/src/efsw/Mutex.cpp
../external/efsw/src/efsw/String.cpp
	Source/Main.cpp
	Source/MainComponent.cpp)

target_compile_definitions(augene-player PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:augene-player,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:augene-player,JUCE_VERSION>")

target_link_libraries(augene-player PRIVATE
	juce::juce_gui_extra
	tracktion_engine)

