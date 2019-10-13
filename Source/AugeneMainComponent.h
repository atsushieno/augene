/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             augene
 version:          0.1.0
 vendor:           atsushieno
 website:          http://https://github.com/atsushieno/augene
 description:      Augene: MML compiler for audio plugins

 dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra,
                   juce_audio_basics, juce_audio_processors
 exporters:        xcode_mac, vs2017, linux_make, xcode_iphone, androidstudio, clion

 type:             Component
 mainClass:        AugeneMainComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

 Files to add afterwards:
   for f in Source/*.h ; do echo "      <FILE name='$(basename $f)' compile='0' resource='0' file='$f' />" ; done
   for f in Source/*.cpp ; do echo "      <FILE name='$(basename $f)' compile='1' resource='0' file='$f' />" ; done

*******************************************************************************/

#pragma once

#include "JuceHeader.h"
#include "MainMenuModel.h"
#include "AugeneModel.h"
#include "ApplicationDialogManager.h"

class AugeneMainComponent : public Component
{
    //==============================================================================
public:
	AugeneMainComponent()
	{
	    // it feels weird but instantiate model here.
	    model.reset(new AugeneModel());
	    dialogManager.reset(new ApplicationDialogManager(model.get()));
        model->getApplicationCommands()->addDialogListener(dialogManager.get());

		setSize(400, 400);

		menubar.reset(new MenuBarComponent(model->getMainMenuModel()));
		menubar->setBounds(0, 0, getWidth(), 20);
		addAndMakeVisible(menubar.get());

		button.reset(new TextButton("test"));
		button->setBounds(20, 20, 100, 20);
		addAndMakeVisible(button.get());
		setVisible(true);
	}

	~AugeneMainComponent() override
	{
	    menubar.reset(nullptr);
	    button.reset(nullptr);
	}
	
private:
    std::unique_ptr<AugeneModel> model;
	std::unique_ptr<MenuBarComponent> menubar;
	std::unique_ptr<TextButton> button;
	std::unique_ptr<ApplicationDialogManager> dialogManager;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AugeneMainComponent)
};
