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
    if(filenames.size() == 0)
        Logger::writeToLog("canceled opening files.");
    Command* command = ongoingInteractions.removeAndReturn(ongoingInteractions.size() - 1);
    auto fc = dynamic_cast<FileCommand*>(command);
    jassert(fc);
    fc->perform(filenames);
}

void ApplicationCommands::endFileSaveInteraction(Array<File> filenames)
{
    if(filenames.size() == 0)
        Logger::writeToLog("canceled saving files.");
    Command* command = ongoingInteractions.removeAndReturn(ongoingInteractions.size() - 1);
    auto fc = dynamic_cast<FileCommand*>(command);
    jassert(fc);
    fc->perform(filenames);
}

void ApplicationCommands::endConfirmationInteraction(bool result)
{
    Command* command = ongoingInteractions.removeAndReturn(ongoingInteractions.size() - 1);
    if(result)
        reinterpret_cast<ConfirmedCommand*>(command)->perform();
    else
        Logger::writeToLog("canceled confirmation");
}


void ApplicationCommands::handleOpenProjectRequest()
{
    auto project = model->getProject();
    std::function<void()> func = [this]() {
        startFileOpenInteraction(new ApplicationCommands::OpenProjectCommand(model),
                                 "Open Augene project", "*.augene|*.*", "Select an augene project file", 0);
    };
    if (project->hasUnsavedChanges()) {
        queuedTasks.add(func);
        std::unique_ptr<Command> p{new SaveProjectCommand(model)};
        startConfirmationInteraction(new ApplicationCommands::TriggerSaveProjectCommand(model, std::move(p)),
                "Unsaved changes",
                String::formatted("Project '%s' has unsaved changes. Would you like to save it?",
                                  project->edit->filename));
    }
    else
        func();

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
    auto& queuedCommands = model->getApplicationCommands()->queuedTasks;
    auto nextFunc = queuedCommands.size() > 0 ? queuedCommands.removeAndReturn(0) : []{};
    if (files.size() == 0 || files[0].getFileName() == "")
        return;
    model->getApplicationCommands()->openProject(files[0].getFullPathName());
    nextFunc();
}

void ApplicationCommands::SaveProjectCommand::perform(Array<File> &files) {
    auto& queuedCommands = model->getApplicationCommands()->queuedTasks;
    auto nextFunc = queuedCommands.size() > 0 ? queuedCommands.removeAndReturn(0) : []{};
    if (files.size() == 0 || files[0].getFileName() == "")
        return;
    auto project = model->getProject();
    project->edit->filename = files[0].getFullPathName();
    project->save();
    nextFunc();
}

void ApplicationCommands::TriggerSaveProjectCommand::perform()
{
    model->getApplicationCommands()->handleSaveProjectRequest();
}
