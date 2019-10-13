#pragma once

#include "JuceHeader.h"
#include "AugeneProject.h"
#include "MainMenuModel.h"
#include "ApplicationCommands.h"

class AugeneModel
{
public:
    AugeneModel()
    {
        applicationCommands.reset(new ApplicationCommands(this));
        mainMenu.reset(new MainMenuModel(applicationCommands.get()));
    }

    class ApplicationEventListener
    {
    public:
        virtual void onProjectLoaded(AugeneProject* project) = 0;
    };

    class GenericApplicationEventListener : public ApplicationEventListener
    {
    public:
        void onProjectLoaded(AugeneProject* project) override {}
    };

    MainMenuModel* getMainMenuModel() { return mainMenu.get(); }

    ApplicationCommands* getApplicationCommands() { return applicationCommands.get(); }

    void loadProject(AugeneProject *project) {
        if (project != nullptr)
            unloadProject();
        this->project = project;
        for (auto l : listeners)
            l->onProjectLoaded(project);
    }

    void unloadProject()
    {
        if (project->hasUnsavedChanges())
            // FIXME: remove this CommandID hack
            applicationCommands->requestGenericYesNoDialog(2, "Unsaved changes",
                    String::formatted("Project '%s' has unsaved changes. Would you like to save it?", project->edit->filename));
    }

    AugeneProject* getProject() { return project; }

private:
    AugeneProject* project;
    std::unique_ptr<MainMenuModel> mainMenu;
    std::unique_ptr<ApplicationCommands> applicationCommands;

    Array<ApplicationEventListener*> listeners;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AugeneModel)
};

