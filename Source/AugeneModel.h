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
        project.reset(new AugeneProject());
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

    void loadProject(std::unique_ptr<AugeneProject> project) {
        this->project = std::move(project);
        for (auto l : listeners)
            l->onProjectLoaded(project.get());
    }

    // anything required (e.g. saving existing project) must be handled beforehand.
    void unloadProject()
    {
        project.reset(new AugeneProject());
    }

    AugeneProject* getProject() { return project.get(); }

private:
    std::unique_ptr<AugeneProject> project;
    std::unique_ptr<MainMenuModel> mainMenu;
    std::unique_ptr<ApplicationCommands> applicationCommands;

    Array<ApplicationEventListener*> listeners;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AugeneModel)
};

