/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================

void showAudioDeviceSettings (tracktion_engine::Engine& engine)
{
    DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Audio Settings");
    o.dialogBackgroundColour = LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId);
    engine.getDeviceManager().initialise(0, 2);
    o.content.setOwned (new AudioDeviceSelectorComponent (engine.getDeviceManager().deviceManager,
                                                          0, 2, 1, 2, false, false, true, true));
    o.content->setSize (400, 600);
    o.launchAsync();
}

MainComponent::MainComponent()
{
    setSize (600, 400);

    editFilePath = JUCEApplication::getCommandLineParameters().replace ("-NSDocumentRevisionsDebugMode YES", "").unquoted().trim();

    #if !JUCE_ANDROID
    selectFileButton.onClick = [this] {
        FileChooser fc{"Open tracktionedit File", File{}, "*.tracktionedit"};
        if (fc.browseForFileToOpen()) {
            editFilePath = fc.getResult().getFullPathName();
            loadEditFile();
        }
    };
    #endif

    playPauseButton.onClick = [this] { togglePlay (*edit); };

    // Show the plugin scan dialog
    // If you're loading an Edit with plugins in, you'll need to perform a scan first
    pluginsButton.onClick = [this]
    {
        DialogWindow::LaunchOptions o;
        o.dialogTitle                   = TRANS("Plugins");
        o.dialogBackgroundColour        = Colours::black;
        o.escapeKeyTriggersCloseButton  = true;
        o.useNativeTitleBar             = true;
        o.resizable                     = true;
        o.useBottomRightCornerResizer   = true;

        auto v = new PluginListComponent (engine.getPluginManager().pluginFormatManager,
                                          engine.getPluginManager().knownPluginList,
                                          engine.getTemporaryFileManager().getTempFile ("PluginScanDeadMansPedal"),
                                          tracktion_engine::getApplicationSettings());
        v->setSize (800, 600);
        o.content.setOwned (v);
        o.launchAsync();
    };

    settingsButton.onClick  = [this] { showAudioDeviceSettings (engine); };
    updatePlayButtonText();
    editNameLabel.setJustificationType (Justification::centred);
    addAndMakeVisible(&selectFileButton);
    addAndMakeVisible(&pluginsButton);
    addAndMakeVisible(&settingsButton);
    addAndMakeVisible(&playPauseButton);
    addAndMakeVisible(&editNameLabel);

    const File editFile (editFilePath);
    if (editFile.existsAsFile())
        loadEditFile();
}

MainComponent::~MainComponent()
{
    if (edit->getTransport().isPlaying())
        edit->getTransport().stop(true, true);
    engine.getTemporaryFileManager().getTempDirectory().deleteRecursively();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto r = getLocalBounds();
    auto topR = r.removeFromTop (30);
    selectFileButton.setBounds (topR.removeFromLeft (topR.getWidth() / 3).reduced (2));
    pluginsButton.setBounds (topR.removeFromLeft (topR.getWidth() / 3).reduced (2));
    settingsButton.setBounds (topR.removeFromLeft (topR.getWidth() / 2).reduced (2));
    playPauseButton.setBounds (topR.reduced (2));
    editNameLabel.setBounds (r);
}
