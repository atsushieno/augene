#pragma once

#include "JuceHeader.h"
#include "ApplicationCommands.h"

class MainMenuModel : public MenuBarModel
{
    //==============================================================================
public:
    MainMenuModel(ApplicationCommands* commands) : commands(commands)
    {
        menuBarNames = StringArray{"File"};
    }

    StringArray getMenuBarNames() override
    {
        return menuBarNames;
    }

    PopupMenu getMenuForIndex(int topLevelMenuIndex, const String &menuName) override
    {
        PopupMenu menu;
        if (menuName == "File") {
            menu.addItem("Open Augene project", [this]{
                commands->handleOpenProjectRequest();
            });
            menu.addItem("Quit", [this]{ JUCEApplication::getInstance()->quit(); });
        }
        return menu;
    }

    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override
    {
    }

private:
    ApplicationCommands* commands;
    StringArray menuBarNames;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainMenuModel)
};
