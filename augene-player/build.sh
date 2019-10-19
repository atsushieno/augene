#/bin/sh

PROJUCER=../external/tracktion_engine/modules/juce/extras/Projucer
PROJUCER_EXE=$PROJUCER/Builds/LinuxMakefile/build/Projucer
SEDCMDVST="s/JUCE_PLUGINHOST_LADSPA/JUCE_PLUGINHOST_VST='1' JUCE_PLUGINHOST_LADSPA/"
# workaround for https://github.com/WeAreROLI/JUCE/issues/602
SETCMDNTB="s/setUsingNativeTitleBar/\/\/setUsingNativeTitleBar/"

APH=../external/tracktion_engine/modules/juce/extras/AudioPluginHost

git submodule update --init --recursive

make -C $PROJUCER/Builds/LinuxMakefile

sed -e "$SEDCMDVST" $APH/AudioPluginHost.jucer > tmpout || exit 1
mv tmpout $APH/AudioPluginHost.jucer
$PROJUCER_EXE --resave $APH/AudioPluginHost.jucer
make -C $APH/Builds/LinuxMakefile

sed -e "$SEDCMDVST" augene-player.jucer > tmpout || exit 1
mv tmpout augene-player.jucer
# sed -e "$SETCMDNTB" Source/Main.cpp > tmpout || exit 1
# mv tmpout Source/Main.cpp
$PROJUCER_EXE --resave augene-player.jucer
make -C $PBD/Builds/LinuxMakefile
