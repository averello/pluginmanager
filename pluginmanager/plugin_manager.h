//
//  plugin_manager.h
//  pluginloader
//
//  Created by George Boumis on 22/1/14.
//  Copyright (c) 2014 George Boumis <developer.george.boumis@gmail.com>. All rights reserved.
//

#ifndef pluginloader_plugin_manager_h
#define pluginloader_plugin_manager_h

//#ifdef _cplusplus
//extern "C" {
//#endif

#include "uuid.h"

typedef struct pm_plugin_t PMPlugin;
typedef struct pm_plugin_registration_t PMPluginRegistration;
typedef struct pm_plugin_manager_t PMPluginManager;

PMPluginManager *PMPluginManagerCreate() __attribute__((malloc));
PMPluginManager *PMPluginManagerRetain(PMPluginManager *manager) __attribute__((nonnull));
void PMPluginManagerRelease(PMPluginManager *manager) __attribute__((nonnull));


PMPlugin *PMPluginManagerLoad(PMPluginManager *manager, const char *path) __attribute__((nonnull));
int PMPluginManagerUnload(PMPluginManager *manager, PMPlugin *plugin) __attribute__((nonnull));

int PMPluginManagerStartPlugin(PMPluginManager *manager, PMPlugin *plugin) __attribute__((nonnull));
void PMPluginManagerStopPlugin(PMPluginManager *manager, PMPlugin *plugin) __attribute__((nonnull));


const char *PMPluginGetID(PMPlugin *plugin) __attribute__((nonnull));
UUID PMPluginGetUUID(PMPlugin *plugin);

//#ifdef _cplusplus
//}
//#endif


#endif
