#pragma once

#include "JuceHeader.h"
#include "AugeneProject.h"

class AugeneModel;

class ApplicationCommands
{
public:
    class Command
    {
    public:
        Command() {}

        virtual String getDescription() { return ""; }
    };

    class DialogRequestListener
    {
    public:
        virtual void openFileRequested(Command* command, String dialogTitle, String fileExtensions, String description, int flags) = 0;
        virtual void saveFileRequested(Command* command, String dialogTitle, String fileExtensions, String description, int flags) = 0;
        virtual void confirmationRequested(Command* command, String dialogTitle, String message) = 0;
    };

    class FileCommand : public Command
    {
    public:
        FileCommand() {}

        void perform(File file) {
            Array<File> files{file};
            perform(files);
        }

        virtual void perform(Array<File>& files) = 0;
    };

    class OpenProjectCommand : public FileCommand
    {
    public:
        OpenProjectCommand(AugeneModel* model) : model(model) {}

        void perform(Array<File>& files) override;
    private:
        AugeneModel* model;
    };

    class ConfirmedCommand : public Command
    {
    public:
        virtual void perform() = 0;
    };

    class TriggerSaveProjectCommand : public ConfirmedCommand
    {
    public:
        TriggerSaveProjectCommand(AugeneModel* model, std::unique_ptr<Command> thenCommand)
            : model(model), thenCommand(thenCommand.release()) {}
        void perform() override;

    private:
        AugeneModel* model;
        std::unique_ptr<Command> thenCommand;
    };

    class SaveProjectCommand : public FileCommand
    {
    public:
        SaveProjectCommand(AugeneModel* model) : model(model) {}

        void perform(Array<File>& files) override;
    private:
        AugeneModel* model;
    };

    ApplicationCommands(AugeneModel* model) : model(model)
    {
    }

    AugeneModel* getModel() { return model; }

    void addDialogListener(DialogRequestListener* listener) { listeners.add(listener); }

    void startFileOpenInteraction(Command* command, String dialogTitle, String fileExtensions, String description, int flags);
    void startFileSaveInteraction(Command* command, String dialogTitle, String fileExtensions, String description, int flags);
    void startConfirmationInteraction(Command* command, String dialogTitle, String message);
    void endFileOpenInteraction(Array<File> filenames);
    void endFileSaveInteraction(Array<File> filenames);
    void endConfirmationInteraction(bool result);

    void handleOpenProjectRequest();
    void handleSaveProjectRequest();
    void openProject(String filename);

    void saveProject();

private:
    AugeneModel* model;
    OwnedArray<Command> queuedCommands;
    OwnedArray<Command> ongoingInteractions;
    Array<DialogRequestListener*> listeners;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationCommands)
};
