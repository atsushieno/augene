
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MainComponent
    : public Component, public ChangeListener, public AugeneFileWatcher::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void togglePlay (tracktion_engine::Edit& edit)
    {
        auto& transport = edit.getTransport();

        if (transport.isPlaying())
            transport.stop (false, false);
        else
            transport.play (false);
    }

    void fileUpdated(String fullPath)
    {
        if(edit == nullptr)
            return;
        MessageManager::callAsync([this](){
            auto& transport = edit->getTransport();
            bool wasPlaying = transport.isPlaying();
            if(wasPlaying)
                togglePlay(*edit.get()); // note that this "edit" is another instance than below
            unloadEditFile();
            loadEditFile();
            if (wasPlaying)
                togglePlay(*edit.get()); // note that this "edit" is another instance than above.
        });
    }

private:
    //==============================================================================
    tracktion_engine::Engine engine { ProjectInfo::projectName };
    std::unique_ptr<tracktion_engine::Edit> edit;
    String editFilePath;
    AugeneFileWatcher fileWatcher;

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

    void unloadEditFile()
    {
        auto& transport = edit->getTransport();
        if (transport.isPlaying())
            transport.stop (true, false);
        fileWatcher.stopWatchingFile(editFilePath);
    }

    void loadEditFile()
    {
        File editFile{editFilePath};
        auto itemId = tracktion_engine::ProjectItemID::createNewID(1);
        edit = std::make_unique<tracktion_engine::Edit> (engine, tracktion_engine::loadEditFromFile (engine, editFile, itemId), tracktion_engine::Edit::forEditing, nullptr, 0);
        auto& transport = edit->getTransport();
        transport.addChangeListener (this);

        fileWatcher.startWatchingFile(editFilePath);

        editNameLabel.setText (editFile.getFileNameWithoutExtension(), dontSendNotification);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
