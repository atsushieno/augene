
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MainComponent   : public Component, public ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    tracktion_engine::Engine engine { ProjectInfo::projectName };
    std::unique_ptr<tracktion_engine::Edit> edit;
    String editFilePath;

    TextButton selectFileButton { "Open File" }, pluginsButton { "Plugins" }, settingsButton { "Settings" }, playPauseButton { "Play" };
    Label editNameLabel { "No Edit Loaded" };

    //==============================================================================
    void updatePlayButtonText()
    {
        if (edit != nullptr)
            playPauseButton.setButtonText (edit->getTransport().isPlaying() ? "Pause" : "Play");
    }

    void changeListenerCallback(ChangeBroadcaster*) override
    {
        updatePlayButtonText();
    }

    void loadEditFile()
    {
        File editFile{editFilePath};
        auto itemId = tracktion_engine::ProjectItemID::createNewID(1);
        edit = std::make_unique<tracktion_engine::Edit> (engine, tracktion_engine::loadEditFromFile (editFile, itemId), tracktion_engine::Edit::forEditing, nullptr, 0);
        auto& transport = edit->getTransport();
        transport.setLoopRange ({ 0.0, edit->getLength() });
        transport.looping = true;
        transport.addChangeListener (this);

        editNameLabel.setText (editFile.getFileNameWithoutExtension(), dontSendNotification);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
