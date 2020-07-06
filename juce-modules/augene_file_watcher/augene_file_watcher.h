/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.txt file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:               augene_file_watcher
  vendor:           atsushieno
  version:          0.1.0
  name:             auegne file watcher
  description:      Classes for file system watcher/notifier
  website:          https://github.com/atsushieno/augene
  license:          MIT

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#include <juce_core/juce_core.h>
#include "simplefilewatcher-modified/FileWatcher/FileWatcher.h"

using namespace juce;

class AugeneFileWatcher : public FW::FileWatchListener
{
public:
    class Listener
    {
    public:
        virtual void fileUpdated(String fullPath) = 0;
    };

    class FileWatcherTimer : public juce::HighResolutionTimer
    {
    public:
        FileWatcherTimer(FW::FileWatcher* watcher) : watcher(watcher) {}

        void hiResTimerCallback() override { watcher->update(); }

    private:
        FW::FileWatcher* watcher;
    };

    AugeneFileWatcher() {
        timer.reset(new FileWatcherTimer(&watcher));
        timer->startTimer(1000);
    }
    ~AugeneFileWatcher() {
        timer->stopTimer();
        watcher.update();
    }

    bool startWatchingFile(const juce::String& fullPath)
    {
        return toggleWatch(fullPath, true);
    }

    bool stopWatchingFile(juce::String& fullPath)
    {
        return toggleWatch(fullPath, false);
    }

    void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action) override
    {
        String dirJ = String{dir};
        String filenameJ = String{filename};
        String fullPath = File{dirJ}.getChildFile(filenameJ).getFullPathName();
        if (action == FW::Actions::Modified || action == FW::Actions::Delete)
            for (auto l : listeners)
                l->fileUpdated(fullPath);
    }

    void addListener(Listener* listener)
    {
        listeners.add(listener);
    }

    void removeListener(Listener* listener)
    {
        auto at = std::find(listeners.begin(), listeners.end(), listener);
        if(at != listeners.end())
            listeners.remove(at);
        else
            jassertfalse;
    }

private:
    FW::FileWatcher watcher{};
    Array<String> targets{};
    Array<Listener*> listeners{};
    std::unique_ptr<FileWatcherTimer> timer;

    bool toggleWatch(String fullPath, bool startWatch)
    {
        File file{fullPath};
        if(!file.existsAsFile())
            return false;
        File dir = file.getParentDirectory();
        FW::String dirName{dir.getFullPathName().toRawUTF8()};
        if (startWatch) {
            targets.add(fullPath);
            watcher.addWatch(dirName, this, false);
        } else {
            bool stillWatching = false;
            for (int i = 0; i < targets.size(); i++) {
                if (targets[i] == fullPath)
                    targets.remove(i);
                else if (File{targets[i]}.getParentDirectory().getFullPathName() == dir.getFullPathName())
                    stillWatching = true;
            }
            if (!stillWatching)
                watcher.removeWatch(dirName);
        }
        return true;
    }
};

