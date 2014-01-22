//
//  plugin.h
//  pluginloader
//
//  Created by George Boumis on 22/1/14.
//  Copyright (c) 2014 George Boumis <developer.george.boumis@gmail.com>. All rights reserved.
//

#ifndef pluginloader_plugin_h
#define pluginloader_plugin_h

#include <stdint.h>
#include "uuid.h"
#include "plugin_manager.h"

typedef UUID pm_uuid_t;

typedef int (*PMPluginRegistrationF)(PMPluginRegistration *);
typedef int  (*PMPluginStartF)(PMPluginManager *);
typedef void (*PMPluginStopF)(PMPluginManager *);

struct pm_plugin_registration_t {
	pm_uuid_t uuid;
	const char *pluginID;
	PMPluginStartF start;
	PMPluginStopF stop;
};


#endif
