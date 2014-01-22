//
//  main.c
//  pluginmanager
//
//  Created by George Boumis on 22/1/14.
//  Copyright (c) 2014 George Boumis <developer.george.boumis@gmail.com>. All rights reserved.
//

#include <stdio.h>
#include "plugin.h"
#include <unistd.h>
#include <memory_management/memory_management.h>

int main(int argc, const char * argv[])
{
	{
		uuid_s output;
		UUID uuid = UUIDWithBytes(0x48, 0x56, 0x45, 0xFA, 0xA5, 0x13, 0x4E, 0xE6, 0xAA, 0x88, 0x06, 0x48, 0xDB, 0x48, 0x18, 0xC6);
		uuid_get_string(uuid, output);
		printf("output: %s\n", output);
	}
	{
		uuid_s output;
		UUID uuid = uuid_from_string("485645FA-A513-4EE6-AA88-0648DB4818C6");
		uuid_get_string(uuid, output);
		printf("output: %s\n", output);
	}
	{
		PMPluginManager *manager = PMPluginManagerCreate();
		
		PMPlugin *plugin = PMPluginManagerLoad(manager, "libtestplugin.dylib");
		if (plugin) {
			PMPluginManagerStartPlugin(manager, plugin);
//			PMPluginManagerStopPlugin(manager, plugin);
		}

		
		PMPlugin *plugin2 = PMPluginManagerLoad(manager, "libtestplugin2.dylib");
		if (plugin) {
			PMPluginManagerStartPlugin(manager, plugin2);
//			PMPluginManagerStopPlugin(manager, plugin2);
		}
		
		
		PMPluginManagerRelease(manager);
	}
	memory_management_print_stats();
    return 0;
}