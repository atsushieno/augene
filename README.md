
# augene: MML compiler for generic sequencer engine

augene is an experimental compound music authoring toolchain that brings old-fashion MML (music macro language) compiler integrated into modern sequencer that is also used in DAWs. It is nothing but a proof of concept. There isn't even a room for improvement - it is just living in a wild.

You can also have a quick glance at the project by [my slides for lightening talk at ADC 2019](https://speakerdeck.com/atsushieno/create-music-in-199x-language-for-2019-sequencer)

It uses the following software and libraries behind:

- The project model in a sample in [ntracktive](https://github.com/atsushieno/ntracktive) - defines a "project" which contains a set of MML sources and associated audio plugin filter graphs, converts SMF to audio plugin based songs (.tracktionedit)
- MML compiler [mugene](https://github.com/atsushieno/mugene) - compiles MML into SMF.
- [JUCE](https://github.com/WeAreROLI/JUCE) AudioPluginHost for editing audio graph.
- [tracktion_engine](https://github.com/Tracktion/tracktion_engine/) - music playback engine.
- [Xwt](https://github.com/mono/xwt/) cross-platform widget toolkit.

The most difficult part for anyone but me is to learn how to use MML and compiler.



# Usage

NOTE: before using augene, you most likely have to build things (explained in the next section).

launch `Augene.exe` application. It is a cross-platform .NET desktop GUI application (actually not in this repository; it's `ntractive` submodule).

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

- go to juce-modules and run `rewrite.sh` (TODO: Windows not supported yet, you can run the script on WSL though.)
- launch Projucer (in JUCE), open `augene-player.jucer`, and save projects.
- Build the project for your platform. Project files are under `Build/*` e.g. `Build/LinuxMakefile`.

For Linux environment there is a shorthand script `build.sh` which would be useful for other platforms (changes are needed e.g. use `Projucer.app/Content/MacOS/Projucer`).

(It was planned to be a console tool at first, but ended up to be a GUI app because we need audio and plugin configuration settings.)

### Disabling VST2

If you don't have VST2 SDK and would like to remove support for VST2, open augene.jucer (and probably AudioPluginHost.jucer if you once tried to build it from build.sh) on Projucer and select `juce_audio_processors` module and disable VST(2) there, then save project.

## Augene.exe

Another one is .NET/Xwt based GUI app/tool in [ntractive](https://github.com/atsushieno/ntracktive) repository.

Build `Augene.exe` with `external/ntracktive` submodule, by:

```
nuget restore external/ntractive/ntracktive.sln 
msbuild external/ntractive/ntracktive.sln
```

You need Mono locally installed. .NET Core does not work there, because Xwt does not support .NET Core.

## Android support

AugenePlayer is being supported on Android (most likely ongoing work). It is done via [aap-juce](https://github.com/atsushieno/aap-juce/) project. Audio plugins must be indicated as AAP (there is no VST 2/3 or other plugin formats that can run on Android).

# Augene project data format

An augene project is a simple set of XML described in a project file which looks like this:

```
<AugeneProject xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <Includes>
    <Include Bank="1" Source="Banks/SfzBanks.augene" />
    <Include Bank="2" Source="Banks/SF2Banks.augene" />
    <Include Bank="3" Source="Banks/SurgeBanks.augene" />
  </Includes>
  <MasterPlugins>
    <MasterPlugin>MasterPlugin1.filtergraph</MasterPlugin>
  </MasterPlugins>
  <AudioGraphs>
    <AudioGraph Id="GrandPiano1" Source="sfizz_city_piano_1.filtergraph" />
  </AudioGraphs>
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

Here is a list of elements:

| Element | feature |
|-|-|
| AugeneProject | the root element |
| Includes | container of `Include` elements. |
| Include | include other project files. They can also be a bank list of AudioGraph. See description below. |
| AudioGraphs | container of `AudioGraph` elements. |
| AudioGraph | gives a filtergraph a name so that it can be referenced by `AudioGraph` element within `AugeneTrack` element. |
| MasterPlugins | holds a list of master plugins |
| MasterPlugin | specifies an AudioGraph file that is used as a master plugin |
| Tracks | holds a list of tracks |
| AugeneTrack | a track definition specifier which holds an Id and an AudioGraph file (so far only one plugin is specified. Rooms for improvements. |
| MmlFiles | holds a list of MML files |
| MmlFile | specifies an MML source file to be compiled and converted to the edit file. |
| MmlStrings | holds a list of MML strings |
| MmlString | specifies an MML string to be compiled and converted to the edit file. |

An Augene project can include other Augene project files using `Include` element. It is useful to represent a bank of preset filtergraphs. On an `Include` element, `Bank` and `BankMsb` attributes indicate bank select MSB, `BankLsb` attribute indicates bank select LSB (`Bank` is equivalent to `BankMsb` here). `Source` attribute indicates the *included* file path, relative to the *including* file path.

An `AudioGraph` can be referenced by its `Id` attribute, by (1) mugene MIDI track with `INSTRUMENTNAME` meta event, or (2) `AudioGraph` attribute on `AugeneTrack` elements.

All tracks in either MML format (file or string) are converted into tracktionedit. Then audio graphs in the project are interpreted and converted to `PLUGIN` element in tracktionedit and then for each defined track by `Tracks` elements, if there is any graph whose `Id` is identical to the track's `AudioGraph` then the audio graph is attached to the track.

One thing to note is that while mugene supports track number in double (floating point number) SMF does not have "track numbers" and numbers are counted only by sequential index (0, 1, 2...),  the mappings could be totally different. It is always to indicate audio graph by INSTRUMENTNAME meta event in mugene MML, or supplementally use `AugeneTrack`'s mappings.


# Authoring Tips

If you are Tracktion Waveform user, you would like to examine the output `*.tracktionedit` file with Waveform. To do so, you will have to manually create a tracktion project (it is a binary file that Augene.exe does not support generation) and let it point to the edit file. To make it happen, you will most likely have to name your `*.augene` project file as `(projectname) Edit 1.augene`, or rename your edit in the track directly to match your project file.


# License

augene-player is released under the GPLv3 license.

ntractive is released under the MIT license.


## Dependencies

There are couple of dependencies in this application:

- [JUCE](https://juce.com/) - GPLv3 (or commercial).
- [tracktion_engine](https://github.com/Tracktion/tracktion_engine/) - ditto.
- [efsw](https://github.com/SpartanJ/efsw) - MIT.

