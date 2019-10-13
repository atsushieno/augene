#pragma once

// UI code that implements MainMenuModel::DialogRequestListener

#include "JuceHeader.h"
#include "MainMenuModel.h"
#include "AugeneModel.h"

class ApplicationDialogManager : public ApplicationCommands::DialogRequestListener
{
public:
    ApplicationDialogManager(AugeneModel* model)
    {}

    void openFileRequested(CommandID id, String dialogTitle, String fileExtensions, String description, int flags) override
    {
        if (id == 1) { // FIXME: remove this hack
            FileChooser fileChooser{dialogTitle, File(), fileExtensions};
            if (fileChooser.showDialog(flags, nullptr))
                model->getApplicationCommands()->openProject(fileChooser.getResult().getFullPathName());
        }
    }

    void saveFileRequested(CommandID id, String dialogTitle, String fileExtensions, String description, int flags) override
    {

    }

    void yesNoDialogRequested(CommandID commandID, String dialogTitle, String message) override
    {
        if (commandID == 2) { // FIXME: remove this hack
            if (AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::QuestionIcon, dialogTitle, message))
                model->getApplicationCommands()->saveProject();
        }
    }


private:
    AugeneModel* model;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationDialogManager)
};

