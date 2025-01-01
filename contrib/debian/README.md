
Debian
====================
This directory contains files used to package tenebrixcoind/tenebrixcoin-qt
for Debian-based Linux systems. If you compile tenebrixcoind/tenebrixcoin-qt yourself, there are some useful files here.

## tenebrixcoin: URI support ##


tenebrixcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install tenebrixcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your tenebrixcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/tenebrixcoin128.png` to `/usr/share/pixmaps`

tenebrixcoin-qt.protocol (KDE)

