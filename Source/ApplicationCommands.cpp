//
// Created by atsushi on 2019/10/13.
//

#include "ApplicationCommands.h"
#include "AugeneModel.h"

void ApplicationCommands::openProject(String filename) {
    auto p = new AugeneProject();
    p->loadFile(filename);
    model->loadProject(p);
}

void ApplicationCommands::requestGenericOpenFileDialog(CommandID commandID, String dialogTitle, String fileExtensions, String description, int flags)
{
    for (auto l : listeners)
        l->openFileRequested(commandID, dialogTitle, fileExtensions, description, flags);
}

void ApplicationCommands::requestGenericYesNoDialog(CommandID commandID, String dialogTitle, String message)
{
  for (auto l : listeners)
      l->yesNoDialogRequested(commandID, dialogTitle, message);
}

void ApplicationCommands::saveProject()
{
    model->getProject()->save();
}
