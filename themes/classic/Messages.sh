#! /usr/bin/env bash
$EXTRACTRC `find . -name \*.rc -a ! -name theme.rc -o -name \*.ui -o -name \*.kcfg` >> rc.cpp
$XGETTEXT `find . -name \*.qml -o -name \*.cpp` -o $podir/lightdm_theme_classic.pot
rm -f rc.cpp
