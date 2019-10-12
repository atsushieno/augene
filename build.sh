#/bin/sh

PROJUCER=external/tracktion_engine/modules/juce/extras/Projucer
PROJUCER_EXE=$PROJUCER/Builds/LinuxMakefile/build/Projucer
SEDCMD="s/JUCE_PLUGINHOST_LADSPA/JUCE_PLUGINHOST_VST='1' JUCE_PLUGINHOST_LADSPA/"

APH=external/tracktion_engine/modules/juce/extras/AudioPluginHost
PBD=external/tracktion_engine/examples/projects/PlaybackDemo

git submodule update --init --recursive

make -C $PROJUCER/Builds/LinuxMakefile

sed -e "$SEDCMD" $APH/AudioPluginHost.jucer > tmpout || exit 1
mv tmpout $APH/AudioPluginHost.jucer
$PROJUCER_EXE --resave $APH/AudioPluginHost.jucer
make -C $APH/Builds/LinuxMakefile

$PROJUCER_EXE --create-project-from-pip external/tracktion_engine/examples/PlaybackDemo.h external/tracktion_engine/examples/projects external/tracktion_engine/modules/juce/modules external/tracktion_engine/modules/
sed -e "$SEDCMD" $PBD/PlaybackDemo.jucer > tmpout || exit 1
mv tmpout $PBD/PlaybackDemo.jucer
$PROJUCER_EXE --resave $PBD/PlaynackDemo.jucer
make -C $PBD/Builds/LinuxMakefile
