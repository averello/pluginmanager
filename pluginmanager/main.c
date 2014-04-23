//
//  main.c
//  pluginmanager
//
//  Created by George Boumis on 22/1/14.
//  Copyright (c) 2014 George Boumis <developer.george.boumis@gmail.com>. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "plugin.h"
#include <memory_management/memory_management.h>

int main(int argc, const char * argv[])
{
	{
		uint8_t bytes[16] = { 0x48, 0x56, 0x45, 0xFA, 0xA5, 0x13, 0x4E, 0xE6, 0xAA, 0x88, 0x06, 0x48, 0xDB, 0x48, 0x18, 0xC6 };
		uuid_s output;
		UUID uuid = UUIDWithBytes(0x48, 0x56, 0x45, 0xFA, 0xA5, 0x13, 0x4E, 0xE6, 0xAA, 0x88, 0x06, 0x48, 0xDB, 0x48, 0x18, 0xC6);
		uuid_get_string(uuid, output);
		for (uint8_t i=0; i<16; i++)
			assert( uuid.bytes[i] == bytes[i] );
	}
	{
		uint8_t bytes[16] = { 0x48, 0x56, 0x45, 0xFA, 0xA5, 0x13, 0x4E, 0xE6, 0xAA, 0x88, 0x06, 0x48, 0xDB, 0x48, 0x18, 0xC6 };
		uuid_s output;
		UUID uuid = uuid_from_string("485645FA-A513-4EE6-AA88-0648DB4818C6");
		uuid_get_string(uuid, output);
		assert( strncasecmp("485645FA-A513-4EE6-AA88-0648DB4818C6", output, strlen("485645FA-A513-4EE6-AA88-0648DB4818C6")) == 0);
		for (uint8_t i=0; i<16; i++)
			assert( uuid.bytes[i] == bytes[i] );
	}
	{
		PMPluginManager *manager = PMPluginManagerCreate();
		assert( manager != NULL );
		
		PMPlugin *plugin = PMPluginManagerLoad(manager, "libtestplugin.dylib");
		assert( plugin != NULL );
		if (plugin) {
			PMPluginManagerStartPlugin(manager, plugin);
//			PMPluginManagerStopPlugin(manager, plugin);
		}

		
		PMPlugin *plugin2 = PMPluginManagerLoad(manager, "libtestplugin2.dylib");
		assert( plugin2 != NULL );
		if (plugin2) {
			PMPluginManagerStartPlugin(manager, plugin2);
//			PMPluginManagerStopPlugin(manager, plugin2);
		}
		
		
		PMPluginManagerRelease(manager);
	}
	memory_management_print_stats();
    return 0;
}