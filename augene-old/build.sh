#/bin/sh

PROJUCER=../external/tracktion_engine/modules/juce/extras/Projucer
PROJUCER_EXE=$PROJUCER/Builds/LinuxMakefile/build/Projucer
SEDCMDVST="s/JUCE_PLUGINHOST_LADSPA/JUCE_PLUGINHOST_VST='1' JUCE_PLUGINHOST_LADSPA/"
# workaround for https://github.com/WeAreROLI/JUCE/issues/602
SETCMDNTB="s/setUsingNativeTitleBar/\/\/setUsingNativeTitleBar/"

APH=../external/tracktion_engine/modules/juce/extras/AudioPluginHost
PBD=../external/tracktion_engine/examples/projects/PlaybackDemo

git submodule update --init --recursive

make -C $PROJUCER/Builds/LinuxMakefile

sed -e "$SEDCMDVST" $APH/AudioPluginHost.jucer > tmpout || exit 1
mv tmpout $APH/AudioPluginHost.jucer
$PROJUCER_EXE --resave $APH/AudioPluginHost.jucer
make -C $APH/Builds/LinuxMakefile

$PROJUCER_EXE --create-project-from-pip ../external/tracktion_engine/examples/PlaybackDemo.h ../external/tracktion_engine/examples/projects ../external/tracktion_engine/modules/juce/modules ../external/tracktion_engine/modules/
sed -e "$SEDCMDVST" $PBD/PlaybackDemo.jucer > tmpout || exit 1
mv tmpout $PBD/PlaybackDemo.jucer
sed -e "$SETCMDNTB" $PBD/Source/Main.cpp > tmpout || exit 1
mv tmpout $PBD/Source/Main.cpp
$PROJUCER_EXE --resave $PBD/PlaybackDemo.jucer
make -C $PBD/Builds/LinuxMakefile

# $PROJUCER_EXE --create-project-from-pip Source/AugeneMainComponent.h .. ../external/tracktion_engine/modules/juce/modules ../external/tracktion_engine/modules/
sed -e "$SEDCMDVST" augene.jucer > tmpout || exit 1
mv tmpout augene.jucer
# sed -e "$SETCMDNTB" Source/Main.cpp > tmpout || exit 1
# mv tmpout Source/Main.cpp
$PROJUCER_EXE --resave augene.jucer
make -C $PBD/Builds/LinuxMakefile
