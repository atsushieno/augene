#pragma once

#include "JuceHeader.h"
#include "AugeneProject.h"

class AugeneModel;

class ApplicationCommands
{
public:
    class DialogRequestListener
    {
    public:
        virtual void openFileRequested(CommandID id, String dialogTitle, String fileExtensions, String description, int flags) = 0;
        virtual void saveFileRequested(CommandID id, String dialogTitle, String fileExtensions, String description, int flags) = 0;
        virtual void yesNoDialogRequested(CommandID commandID, String dialogTitle, String message) = 0;
    };

    ApplicationCommands(AugeneModel* model) : model(model)
    {
    }

    void addDialogListener(DialogRequestListener* listener) { listeners.add(listener); }

    void requestGenericOpenFileDialog(CommandID commandID, String dialogTitle, String fileExtensions, String description, int flags);

    void openProject(String filename);

    void requestGenericYesNoDialog(CommandID commandID, String dialogTitle, String message);

    void saveProject();

private:
    AugeneModel* model;
    Array<DialogRequestListener*> listeners;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationCommands)
};
