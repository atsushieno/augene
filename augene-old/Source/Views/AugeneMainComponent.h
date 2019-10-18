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

*******************************************************************************/
//   for f in `find Source -name *.h` ; do echo "      <FILE name='$(basename $f)' compile='0' resource='0' file='$f' />" ; done
//   for f in `find Source -name *.cpp` ; do echo "      <FILE name='$(basename $f)' compile='1' resource='0' file='$f' />" ; done

#pragma once

#include "JuceHeader.h"
#include "../Model/MainMenuModel.h"
#include "../Model/AugeneModel.h"
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

		trackList.reset(new TableListBox("track list", model->getTrackTableModel()));
        trackList->setBounds(0, 0, getWidth(), getHeight() - 20);
		addAndMakeVisible(trackList.get());
		setVisible(true);
		FlexBox fb;

	}

	~AugeneMainComponent() override
	{
	}
	
private:
    std::unique_ptr<AugeneModel> model;
	std::unique_ptr<MenuBarComponent> menubar;
	std::unique_ptr<TableListBox> trackList;
	std::unique_ptr<ApplicationDialogManager> dialogManager;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AugeneMainComponent)
};
