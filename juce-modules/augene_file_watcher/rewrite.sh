#!/bin/sh

# SEDSCRIPT="s/\#include <FileWatcher\//#include \<..\/include\/FileWatcher\//" 
SEDSCRIPT="s/\#([ \t]*)include([ \t]+)<FileWatcher\/(.+).h>/#include \"FileWatcher\/\3.h\"/" 

mkdir -p simplefilewatcher-modified/

cp -R ../../external/simplefilewatcher/include/* simplefilewatcher-modified

for f in ../../external/simplefilewatcher/source/*.cpp ; do
  sed -E "$SEDSCRIPT" $f > simplefilewatcher-modified/$(basename $f)
  # cp $f simplefilewatcher-modified/
done
