#!/bin/bash

cd external/tracktion_engine/modules/juce/extras/AudioPluginHost
patch -i ../../../../../../audiopluginhost-lv2.patch -p1
cd ../../../../../..
mkdir build-pluginhost
cd build-pluginhost
pwd
cmake -DJUCE_BUILD_EXTRAS=ON ../external/tracktion_engine/modules/juce/
make
cd ..
