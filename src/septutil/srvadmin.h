/***************************************************************************
    NWNX SrvAdmin - NWN2Server Administrative GUI Functions
    Copyright (C) 2008 Skywing (skywing@valhallalegends.com) 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 ***************************************************************************/
#if !defined(SRVADMIN_H_INCLUDED)
#define SRVADMIN_H_INCLUDED

#define DLLEXPORT extern "C" __declspec(dllexport)

#include <windows.h>
#include <windowsx.h>
#include <specstrings.h>
#include "../plugin.h"
#include "../../misc/log.h"

class TstVersionPlg
{
public:
	TstVersionPlg(std::string nwnxHome, std::string version, std::string plugin);
	~TstVersionPlg();

	std::string nwnxHome;
	HWND     mainGuiWindow;
	std::string version;
	std::string plugin;

	static
	BOOL
	CALLBACK
	FindServerGuiWindowEnumProc(
		__in HWND hwnd,
		__in LPARAM lParam
		);

	void TestVersionPlugin();
	void LaunchRestrict(bool forced, int iTime);
	HWND
	FindServerGuiWindow(
		);

	void prepareMessage(std::vector<char>* message);

private:
	unsigned char key[16];
};



#endif
