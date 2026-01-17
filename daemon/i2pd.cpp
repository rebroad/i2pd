/*
* Copyright (c) 2013-2020, The PurpleI2P Project
*
* This file is part of Purple i2pd project and licensed under BSD3
*
* See full license text in LICENSE file at top of project tree
*/

#include <stdlib.h>
#include "Daemon.h"
#include "Log.h"

#if defined(QT_GUI_LIB)
namespace i2p
{
namespace qt
{
	int RunQT (int argc, char* argv[]);
}
}

int main( int argc, char* argv[] )
{
	return i2p::qt::RunQT (argc, argv);
}
#else
int main( int argc, char* argv[] )
{
	if (Daemon.init(argc, argv))
	{
		if (Daemon.start())
		{
			Daemon.run ();
			LogPrint(eLogInfo, "i2pd: Main loop exited, stopping daemon...");
		}
		else
		{
			LogPrint(eLogError, "i2pd: Failed to start daemon");
			return EXIT_FAILURE;
		}
		Daemon.stop();
		LogPrint(eLogInfo, "i2pd: Daemon stopped, exiting with code 0");
	}
	else
	{
		LogPrint(eLogError, "i2pd: Failed to initialize daemon");
	}
	return EXIT_SUCCESS;
}
#endif

#ifdef _WIN32
#include <windows.h>

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
	)
{
	return main(__argc, __argv);
}
#endif
