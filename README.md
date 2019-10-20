
# augene: MML compiler for generic sequencer engine

augene is a compound music authoring toolchain that brings old-fashion MML (music macro language) compiler integrated into modern sequencer that is also used in DAWs.

It uses the following software and libraries behind:

- The project model in a sample in [ntracktive](https://github.com/atsushieno/ntracktive) - defines a "project" which contains a set of MML sources and associated audio plugin filter graphs, converts SMF to audio plugin based songs (.tracktionedit)
- MML compiler [mugene](https://github.com/atsushieno/mugene) - compiles MML into SMF.
- [JUCE](https://github.com/WeAreROLI/JUCE) AudioPluginHost for editing audio graph.
- [tracktion_engine](https://github.com/Tracktion/tracktion_engine/) - music playback engine.
- [Xwt](https://github.com/mono/xwt/) cross-platform widget toolkit.

The most difficult part for anyone but me is learn how to use MML and compiler

# Usage

launch`Augene.exe` application. It is a cross-platform .NET desktop GUI application (actually not in this repository).

![augene.exe sshot](https://photos.app.goo.gl/6vDaZrKecVtbeZEb6)

By default those lists are actually empty. It's a screenshot of the app that has loaded sample data that makes use of Collective (bundled with Tracktion [Waveform](https://www.tracktion.com/products/waveform)).

To use this app, there are couple of things to do - Configure the app. Namely paths to two external tools are needed:

- augene-player (JUCE app in this repository, which is mostly based on PlaybackDemo in tracktion_engine repository)
- AudioPluginHost (can be found in JUCE extras)

The next step is to build a list of locally installed audio plugins. Begin with "Plugins" button to start the process.

![build audio plugin list](https://picasaweb.google.com/110788028571466287583/6749376733834561841#6749376739265845362)
Once you are done with above, then you're ready to use the app. You can open a `*.tracktionedit` file and play it. Note that if you don't have the audio plugins specified in the edit file, you are unable to play it.

To compose your own music, create new audiograph and new MML for each list, which can be performed via each context menu. Then use "Compile" command from the main menu.


# Building

## augene-player

There are two repositories for this "augene" application. This repository only contains "augene-player" part of the entire system, which is a JUCE based C++ application. It is a typical JUCE application project so you can build it with the following steps:

- launch Projucer (in JUCE), open `augene-player.jucer`, and save projects.
- Build the project for your platform. Project files are under `Build/*` e.g. `Build/LinuxMakefile`.

For Linux environment there is a shorthand script `build.sh` which would be useful for other platforms (changes are needed e.g. use `Projucer.app/Content/MacOS/Projucer`).

(It was planned to be a console tool at first, but ended up to be a GUI app because we need audio and plugin configuration settings.)

## Augene.exe

Another one is .NET/Xwt based GUI app/tool in [ntractive](https://github.com/atsushieno/ntracktive) repository. Use nuget restore and msbuild, or simply open `ntractive.sln` with Rider, MonoDevelop, Visual Studio (for Mac / for Win) etc.


## Disabling VST2

If you don't have VST2 SDK and would like to remove support for VST2, open augene.jucer (and probably AudioPluginHost.jucer if you once tried to build it from build.sh) on Projucer and select `juce_audio_processors` module and disable VST(2) there, then save project.



## Augene project data format

An augene project is a simple set of XML described in a project file which looks like this:

```
<AugeneProject xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <Tracks>
    <AugeneTrack>
      <Id>1</Id>
      <AudioGraph>Unnamed.filtergraph</AudioGraph>
    </AugeneTrack>
  </Tracks>
  <MmlFiles>
    <MmlFile>foobar.mugene</MmlFile>
  </MmlFiles>
  <MmlStrings>
    <MmlString>![CDATA[ 1 @0 V110 v100 o5 l8 cegcegeg  > c1 ]]></MmlString>
  </MmlStrings>
</AugeneProject>
```

We would give more details in the future but the contents are not very complicated.

One thing to note is that we cannot control track number so the Id at AugeneTrack is mapped only in the order in the output SMF from MML. While mugene supports track number in double (floating point number) SMF does not have "track numbers" and numbers are counted only by sequential index (0, 1, 2...),  the mappings could be totally different.

# License

augene-player is released under the GPLv3 license.

ntractive is released under the MIT license.
