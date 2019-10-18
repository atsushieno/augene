#pragma once

#include "JuceHeader.h"

class AugeneProject
{
public:
    class Track
    {
    public:
        int32_t id;
        String audioGraph;
    };

    Array<Track> tracks;
    Array<String> mmlFiles;
    Array<String> mmlStrings;

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

    void loadFile(String filename) {
        // FIXME: implement
        Logger::writeToLog("Not Implemented: loadFile()");
    }

    bool hasUnsavedChanges() {
        // FIXME: implement
        Logger::writeToLog("Not Implemented: hasUnsavedChanges()");
        return true;
    }

    void save() {
        Logger::writeToLog("Not Implemented: save()");
        jassert(edit->filename != "");
        // FIXME: implement
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AugeneProject)
};
