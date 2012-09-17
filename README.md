# Testing

## Installation

You need to build and install lightdm and lightdm-kde in the same prefix. We are
going to assume both are installed in $PREFIX

## Set lightdm-kde-greeter as lightdm greeter

You can either do:

    cd $PREFIX/share/xgreeters/
    ln -s lightdm-kde-greeter.desktop default.desktop

or:

- edit $PREFIX/etc/lightdm.conf
- find the group "[SeatDefaults]"
- set the "greeter-session" key to "lightdm-kde-greeter"

But your changes will get overwritten next time you run `make install` in
lightdm directory.

## Fix session lists

Lightdm needs to know which sessions are available:

    cd $PREFIX/share
    ln -s /usr/share/xsessions

## Running

Start lightdm like this:

    $PREFIX/sbin/lightdm --test-mode --debug

Output of the greeter is sent to `$HOME/.cache/lightdm/log/x-$N-greeter.log`,
where $N starts at 1 and increase each time you close the window. $N does not
increase if you stop lightdm with Ctrl+C, so using Ctrl+C is the recommended
way to stop it.

Note: the test X server is using a QWERTY layout. Keep this in mind if you are
using a different layout when you type your password.
