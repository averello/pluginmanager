//
//  plugin_manager.c
//  pluginmanager
//
//  Created by George Boumis on 22/1/14.
//  Copyright (c) 2014 George Boumis <developer.george.boumis@gmail.com>. All rights reserved.
//

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdint.h>
#include <sys/queue.h>
#include <errno.h>
#include <dlfcn.h>

#include "plugin_manager.h"
#include "plugin.h"
#include <memory_management/memory_management.h>


#ifndef TAILQ_FOREACH_SAFE
#define TAILQ_FOREACH_SAFE(var, head, field, tvar)          \
for ((var) = TAILQ_FIRST((head));               \
(var) && ((tvar) = TAILQ_NEXT((var), field), 1);        \
(var) = (tvar))
#endif

////////////////////
/*  Plugin Types  */
////////////////////
typedef enum _pm_plugin_event_t {
	PMPluginEventRegister,
	PMPluginEventStart,
	PMPluginEventStop,
	PMPluginEventUnregister
} pm_plugin_event_t;

typedef void (*pm_plugin_state_f)(PMPlugin *, pm_plugin_event_t);
struct pm_plugin_t {
	pm_plugin_state_f state;
	PMPluginRegistration registrationInfo;
	void *handle;
};


void pm_plugin_state_registered(PMPlugin *plugin, pm_plugin_event_t event);
void pm_plugin_state_started(PMPlugin *plugin, pm_plugin_event_t event);
void pm_plugin_state_stoped(PMPlugin *plugin, pm_plugin_event_t event);

void pm_plugin_state_registered(PMPlugin *plugin, pm_plugin_event_t event) {
	if (NULL==plugin) return;
	if ( event == PMPluginEventStart ) {
		plugin->state = pm_plugin_state_started;
	}
	else if ( event == PMPluginEventUnregister ) {
		plugin->state = NULL;
	}
}

void pm_plugin_state_started(PMPlugin *plugin, pm_plugin_event_t event) {
	if (NULL==plugin) return;
	if ( event == PMPluginEventStop ) {
		plugin->state = pm_plugin_state_registered;
	}
}

void pm_plugin_state_stoped(PMPlugin *plugin, pm_plugin_event_t event) {
	if (NULL==plugin) return;
	if ( event == PMPluginEventStart ) {
		plugin->state = pm_plugin_state_started;
	}
}
////////////////////
/*                */
////////////////////




////////////////////
/* Plugin Manager */
////////////////////
struct _list_item {
	PMPlugin *plugin;
	TAILQ_ENTRY(_list_item) items;
};

struct pm_plugin_manager_t {
	TAILQ_HEAD(ListHead, _list_item) plugins; /*!< the plugins list */
};

static void PMPluginManagerDealloc();
////////////////////
/*                */
////////////////////

PMPluginManager *PMPluginManagerCreate() {
	PMPluginManager *manager = MEMORY_MANAGEMENT_ALLOC(sizeof(PMPluginManager));
	if (NULL==manager) return errno = ENOMEM, NULL;
	MEMORY_MANAGEMENT_ATTRIBUTE_SET_DEALLOC_FUNCTION(manager, PMPluginManagerDealloc);
	TAILQ_INIT(&manager->plugins);
	return manager;
}

static void PMPluginManagerDealloc(void *_manager) {
	if (NULL == _manager) return;
	PMPluginManager *manager = _manager;
	struct _list_item *item, *tmp;
	
	TAILQ_FOREACH_SAFE(item, &manager->plugins, items, tmp) {
		PMPluginManagerUnload(manager, item->plugin);
	}
}

PMPluginManager *PMPluginManagerRetain(PMPluginManager *manager) {
	return retain(manager);
}
void PMPluginManagerRelease(PMPluginManager *manager) {
	release(manager);
}

PMPlugin *PMPluginCreate() {
	PMPlugin *plugin = MEMORY_MANAGEMENT_ALLOC(sizeof(PMPlugin));
	if (NULL == plugin) return errno = ENOMEM, NULL;
	return plugin;
}

PMPlugin *PMPluginManagerLoad(PMPluginManager *manager, const char *path) {
	if (NULL == manager) return errno = EINVAL, NULL;
	if (NULL == path) return errno = EINVAL, NULL;
	PMPlugin *plugin;
	struct _list_item *item;
	char *error;
	PMPluginRegistrationF registrationFunction;
	void *handle;
	
	plugin = PMPluginCreate();
	if (NULL == plugin) return NULL;
	item = MEMORY_MANAGEMENT_ALLOC(sizeof(struct _list_item));
	if (NULL == item) return release(plugin), errno = ENOMEM, NULL;
	
	handle = dlopen(path, RTLD_LAZY|RTLD_LOCAL);
	if (NULL == handle) {
		fprintf(stderr, "%s\n", dlerror());
		release(plugin), release(item);
		return NULL;
	}
	
	*(void **) (&registrationFunction) = dlsym(handle, "PMPluginRegistrationFunction");
	error = dlerror();
	if (NULL!=error || NULL == registrationFunction) {
		fprintf(stderr, "%s\n", dlerror());
		release(plugin), release(item);
		dlclose(handle);
		return NULL;
	}
	
	if ( (*registrationFunction)(&plugin->registrationInfo) != 0) {
		dlclose(handle);
		release(plugin), release(item);
		return NULL;
	}
	// check if already registered
	struct _list_item *it;
	TAILQ_FOREACH(it, &manager->plugins, items) {
		if ( uuid_cmp(it->plugin->registrationInfo.uuid, plugin->registrationInfo.uuid) == 0) {
			// Plugin already loaded
			release(plugin);
			release(item);
			dlclose(handle);
			return NULL;
		}
	}
	plugin->handle = handle;
	item->plugin = retain(plugin);
	TAILQ_INSERT_TAIL(&manager->plugins, item, items);
	release(plugin);
	plugin->state = pm_plugin_state_registered;
	return plugin;
}

int PMPluginManagerUnload(PMPluginManager *manager, PMPlugin *plugin) {
	if (NULL == manager) return errno = EINVAL, -1;
	if (NULL == plugin) return errno = EINVAL, -1;
	struct _list_item *item;
	TAILQ_FOREACH(item, &manager->plugins, items) {
		if (item->plugin == plugin) {
			if (plugin->state == pm_plugin_state_started ) {
				(*plugin->state)(plugin, PMPluginEventStop);
				(*plugin->registrationInfo.stop)(manager);
			}
			(*plugin->state)(plugin, PMPluginEventUnregister);
			TAILQ_REMOVE(&manager->plugins, item, items);
			release(item->plugin), release(item);
			return 0;
		}
	}
	return -1;
}

int PMPluginManagerStartPlugin(PMPluginManager *manager, PMPlugin *plugin) {
	if (NULL == manager) return errno = EINVAL, -1;
	if (NULL == plugin) return  errno = EINVAL, -1;
	if (plugin->state == pm_plugin_state_registered ) {
		(*plugin->state)(plugin, PMPluginEventStart);
		return (*plugin->registrationInfo.start)(manager);
	}
	return -1;
}

void PMPluginManagerStopPlugin(PMPluginManager *manager, PMPlugin *plugin) {
	if (NULL == manager) return;
	if (NULL == plugin) return;
	if (plugin->state == pm_plugin_state_started ) {
		(*plugin->state)(plugin, PMPluginEventStop);
		(*plugin->registrationInfo.stop)(manager);
	}
}




////////////////////
/*  Plugin Calls  */
////////////////////
PMPlugin *PMPluginManagerRegisterPlugin(PMPluginManager *manager, PMPluginRegistration pluginRegistration) {
	if (NULL==manager) return NULL;
	PMPlugin *plugin = PMPluginCreate();
	if (NULL == plugin) return NULL;
	struct _list_item *item = MEMORY_MANAGEMENT_ALLOC(sizeof(struct _list_item));
	if (NULL == item) return release(plugin), NULL;
	
	// check if already registered
	struct _list_item *it;
	TAILQ_FOREACH(it, &manager->plugins, items) {
		if ( uuid_cmp(it->plugin->registrationInfo.uuid, pluginRegistration.uuid) == 0) {
			// Plugin already loaded
			release(plugin);
			release(item);
			return NULL;
		}
	}

	plugin->state = pm_plugin_state_registered;
	plugin->registrationInfo = pluginRegistration;

	item->plugin = retain(plugin);
	TAILQ_INSERT_TAIL(&manager->plugins, item, items);
	release(plugin);
	
	return plugin;
}
////////////////////
/*                */
////////////////////

