#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/joao/Documents/Universidade/Github/CG/Projecto/xcode
  make -f /Users/joao/Documents/Universidade/Github/CG/Projecto/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/joao/Documents/Universidade/Github/CG/Projecto/xcode
  make -f /Users/joao/Documents/Universidade/Github/CG/Projecto/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/joao/Documents/Universidade/Github/CG/Projecto/xcode
  make -f /Users/joao/Documents/Universidade/Github/CG/Projecto/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/joao/Documents/Universidade/Github/CG/Projecto/xcode
  make -f /Users/joao/Documents/Universidade/Github/CG/Projecto/xcode/CMakeScripts/ReRunCMake.make
fi

