#pragma once

// UI code that implements MainMenuModel::DialogRequestListener

#include "JuceHeader.h"
#include "../Model/MainMenuModel.h"
#include "../Model/AugeneModel.h"

class ApplicationDialogManager : public ApplicationCommands::DialogRequestListener
{
public:
    ApplicationDialogManager(AugeneModel* model) : model(model) {}

    void openFileRequested(ApplicationCommands::Command* command, String dialogTitle, String fileExtensions, String description, int flags) override
    {
        FileChooser fileChooser{dialogTitle, File(), fileExtensions};
        fileChooser.showDialog(FileBrowserComponent::saveMode, nullptr);
        model->getApplicationCommands()->endFileOpenInteraction(fileChooser.getResults());
    }

    void saveFileRequested(ApplicationCommands::Command* command, String dialogTitle, String fileExtensions, String description, int flags) override
    {
        FileChooser fileChooser{dialogTitle, File(), fileExtensions};
        fileChooser.showDialog(FileBrowserComponent::saveMode, nullptr);
        model->getApplicationCommands()->endFileSaveInteraction(fileChooser.getResults());
    }

    void confirmationRequested(ApplicationCommands::Command* command, String dialogTitle, String message) override
    {
        model->getApplicationCommands()->endConfirmationInteraction(
            AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::QuestionIcon, dialogTitle, message));
    }

private:
    AugeneModel* model;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationDialogManager)
};

