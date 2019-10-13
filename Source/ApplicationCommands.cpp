//
// Created by atsushi on 2019/10/13.
//

#include "ApplicationCommands.h"
#include "AugeneModel.h"

void ApplicationCommands::openProject(String filename) {
    auto p = new AugeneProject();
    p->loadFile(filename);
    model->loadProject(std::unique_ptr<AugeneProject>(p));
}

void ApplicationCommands::startFileOpenInteraction(Command* command, String dialogTitle, String fileExtensions, String description, int flags)
{
    ongoingInteractions.add(command);
    for (auto l : listeners)
        l->openFileRequested(command, dialogTitle, fileExtensions, description, flags);
}

void ApplicationCommands::startFileSaveInteraction(Command* command, String dialogTitle, String fileExtensions, String description, int flags)
{
    ongoingInteractions.add(command);
    for (auto l : listeners)
        l->saveFileRequested(command, dialogTitle, fileExtensions, description, flags);
}

void ApplicationCommands::startConfirmationInteraction(Command* command, String dialogTitle, String message)
{
    ongoingInteractions.add(command);
    for (auto l : listeners)
        l->confirmationRequested(command, dialogTitle, message);
}

void ApplicationCommands::endFileOpenInteraction(Array<File> filenames)
{
    Command* command = ongoingInteractions.removeAndReturn(ongoingInteractions.size() - 1);
    auto fc = dynamic_cast<FileCommand*>(command);
    jassert(fc);
    fc->perform(filenames);
}

void ApplicationCommands::endFileSaveInteraction(Array<File> filenames)
{
    Command* command = ongoingInteractions.removeAndReturn(ongoingInteractions.size() - 1);
    auto fc = dynamic_cast<FileCommand*>(command);
    jassert(fc);
    fc->perform(filenames);
}

void ApplicationCommands::endConfirmationInteraction(bool result)
{
    Command* command = ongoingInteractions.removeAndReturn(ongoingInteractions.size() - 1);
    reinterpret_cast<ConfirmedCommand*>(command)->perform();
}


void ApplicationCommands::handleOpenProjectRequest()
{
    auto project = model->getProject();
    if (project->hasUnsavedChanges()) {
        queuedCommands.add(new OpenProjectCommand(model));
        std::unique_ptr<Command> p{new SaveProjectCommand(model)};
        startConfirmationInteraction(new ApplicationCommands::TriggerSaveProjectCommand(model, std::move(p)),
                "Unsaved changes",
                String::formatted("Project '%s' has unsaved changes. Would you like to save it?",
                                  project->edit->filename));
    }
    else
        startFileOpenInteraction(new ApplicationCommands::OpenProjectCommand(model),
                "Open Augene project", "*.augene|*.*", "Select an augene project file", 0);

}

void ApplicationCommands::handleSaveProjectRequest()
{
    auto project = model->getProject();
    if (project->edit->filename == "")
        startFileSaveInteraction(new SaveProjectCommand(model),
                                     "Save Augene project", "*.augene|*.*", "Specify an augene project file", 0);
    else
        model->getProject()->save();
}

void ApplicationCommands::OpenProjectCommand::perform(Array<File>& files) {
    if (files.size() == 0 || files[0].getFileName() == "")
        return;
    model->getApplicationCommands()->openProject(files[0].getFullPathName());
    // FIXME: dequeue and process next command.
}

void ApplicationCommands::SaveProjectCommand::perform(Array<File> &files) {
    if (files.size() == 0 || files[0].getFileName() == "")
        return;
    auto project = model->getProject();
    project->edit->filename = files[0].getFullPathName();
    project->save();
    // FIXME: dequeue and process next command.
}

void ApplicationCommands::TriggerSaveProjectCommand::perform()
{
    model->getApplicationCommands()->handleSaveProjectRequest();
}
