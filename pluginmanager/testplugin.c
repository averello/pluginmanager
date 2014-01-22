//
//  testplugin.c
//  pluginmanager
//
//  Created by George Boumis on 22/1/14.
//  Copyright (c) 2014 George Boumis <developer.george.boumis@gmail.com>. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "plugin.h"

#define ComGeorgeBoumisPluginTest "com.georgeboumis.plugin.test"
#define ComGeorgeBoumisPluginTestUUID "33c0c3fc-f053-4b81-986d-4ed09d6d4d4d"

int ComGeorgeBoumisPluginTestStartFunction(PMPluginManager *manager) {
	printf("%s(): starting\n", __FUNCTION__);
	uuid_s string;
	uuid_get_string(UUIDFromString(ComGeorgeBoumisPluginTestUUID), string);
	puts(string);
	return 0;
}

void ComGeorgeBoumisPluginTestStopFunction(PMPluginManager *manager) {
	printf("%s(): stopping\n", __FUNCTION__);
};

int PMPluginRegistrationFunction(PMPluginRegistration *registration) {
	if (NULL == registration) return -1;
	registration->pluginID = ComGeorgeBoumisPluginTest;
	registration->uuid = UUIDFromString(ComGeorgeBoumisPluginTestUUID);
	registration->start = ComGeorgeBoumisPluginTestStartFunction;
	registration->stop = ComGeorgeBoumisPluginTestStopFunction;
	return 0;
}
