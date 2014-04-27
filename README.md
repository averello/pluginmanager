pluginmanager
=============

An exercice in plugin management

What is it?
===========

A little history
----------------
Well, I was wondering if it is possible to load some code dynamically in C. A 
program that gets over the network some compiled code for the same architecture,
is it possible to execute directly this code?
After a lot of search the answer was yes. And it is similar to loading plugins,
well unidentified plugins. That's how this little project was born.

It is a an attempt to create a generic library written in C, that handles loading
and unloading code, thereafter plugins, dynamically. This is done by compiling
some code that will be considered the payload of the plugin in the form of a 
dynamic library .so in Linux and .dylib in OS X.

This manager is not security oriented yet. It doesn't use any mechanism of 
sandboxing as well as any mechanism of verification on the loaded code.

This project is to be considered as **work in progress** or a **proof of concept**.

How does it work?
=================
A plugin is a library that has a function with this specific name 
"PMPluginRegistrationFunction". This function is called with an argument that is 
a structure the plugin uses to register two callbacks, its 'start' function
and its 'stop' function, as well as its ID and UUID.

The same manager will not load more than once a plugin. The actual 
implementation does not prevent this at all cost. 

Future work
-----------
A plugin should be a bundle with the payload and at least a  file (xml,json,etc)
that describes the necessary information like the 'start' and 'stop' function 
the ID and the UUID. The bundle could contain other resources as well. Some
code signing could take place also.

Example
=======
The code that loads the plugin:
```c
		PMPluginManager *manager = PMPluginManagerCreate();

		PMPlugin *plugin = PMPluginManagerLoad(manager, "libtestplugin.dylib");
		if (plugin) {
			PMPluginManagerStartPlugin(manager, plugin);
			PMPluginManagerStopPlugin(manager, plugin);
		}

		PMPluginManagerRelease(manager);
```

The plugin code:
```c
#include <stdio.h>
#include <string.h>
#include "plugin.h"

#define ComExamplePlugin "com.example.plugin"
#define ComExamplePluginUUID "33c0c3fc-f053-4b81-986d-4ed09d6d4d4d"

int ComExamplePluginStartFunction(PMPluginManager *manager) {
	printf("%s(): starting\n", __FUNCTION__);
	uuid_s string;
	uuid_get_string(UUIDFromString(ComExamplePluginUUID), string);
	puts(string);
	return 0;
}

void ComExamplePluginStopFunction(PMPluginManager *manager) {
	printf("%s(): stopping\n", __FUNCTION__);
};

int PMPluginRegistrationFunction(PMPluginRegistration *registration) {
	if (NULL == registration) return -1;
	registration->pluginID = ComExamplePlugin;
	registration->uuid = UUIDFromString(ComExamplePluginUUID);
	registration->start = ComExamplePluginStartFunction;
	registration->stop = ComExamplePluginStopFunction;
	return 0;
}
````
