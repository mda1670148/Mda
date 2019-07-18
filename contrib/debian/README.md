
Debian
====================
This directory contains files used to package aratritond/aratriton-qt
for Debian-based Linux systems. If you compile aratritond/aratriton-qt yourself, there are some useful files here.

## aratriton: URI support ##


aratriton-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install aratriton-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your aratritonqt binary to `/usr/bin`
and the `../../share/pixmaps/aratriton128.png` to `/usr/share/pixmaps`

aratriton-qt.protocol (KDE)

