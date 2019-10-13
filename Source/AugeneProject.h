#pragma once

#include "JuceHeader.h"

class AugeneProject
{
public:
    AugeneProject()
    {
        edit.reset(new Edit());
    }
    ~AugeneProject()
    {
        edit.reset(nullptr);
    }

    class Edit
    {
    public:
        String filename;
    };

    std::unique_ptr<Edit> edit;

    void loadFile(String string) {
        // FIXME: implement
    }

    bool hasUnsavedChanges() {
        // FIXME: implement
        return false;
    }

    void save() {
        // FIXME: implement
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AugeneProject)
};
