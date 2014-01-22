//
//  uuid.h
//  pluginloader
//
//  Created by George Boumis on 22/1/14.
//  Copyright (c) 2014 George Boumis <developer.george.boumis@gmail.com>. All rights reserved.
//

#ifndef pluginloader_uuid_h
#define pluginloader_uuid_h

#include <stdint.h>
#include <string.h>

//#ifdef __cplusplus
//extern "C" {
//#endif

typedef struct _uuid_t {
	uint8_t byte0;
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;
	uint8_t byte7;
	uint8_t byte8;
	uint8_t byte9;
	uint8_t byte10;
	uint8_t byte11;
	uint8_t byte12;
	uint8_t byte13;
	uint8_t byte14;
	uint8_t byte15;
} UUID;

#define UUIDWithBytes(byte0,\
byte1, \
byte2, \
byte3, \
byte4, \
byte5, \
byte6, \
byte7, \
byte8, \
byte9, \
byte10, \
byte11, \
byte12, \
byte13, \
byte14, \
byte15) uuid_with_bytes(byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11, byte12, byte13, byte14, byte15)

#define UUIDFromString(uuids) uuid_from_string((uuids))

#define UUID_S_LENGTH 40
typedef char uuid_s[UUID_S_LENGTH];

#define _UUIDWithBytes(byte0,\
byte1, \
byte2, \
byte3, \
byte4, \
byte5, \
byte6, \
byte7, \
byte8, \
byte9, \
byte10, \
byte11, \
byte12, \
byte13, \
byte14, \
byte15) \
{ byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11, byte12, byte13, byte14, byte15 }

inline static void uuid_get_string(UUID uuid, uuid_s iotstring) {
	snprintf(iotstring, UUID_S_LENGTH, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X", uuid.byte0, uuid.byte1, uuid.byte2, uuid.byte3, uuid.byte4, uuid.byte5, uuid.byte6, uuid.byte7, uuid.byte8, uuid.byte9, uuid.byte10, uuid.byte11, uuid.byte12, uuid.byte13, uuid.byte14, uuid.byte15);
}

inline static UUID uuid_with_bytes(uint8_t byte0, uint8_t byte1, uint8_t byte2,uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8, uint8_t byte9, uint8_t byte10, uint8_t byte11, uint8_t byte12, uint8_t byte13, uint8_t byte14, uint8_t byte15) {
	UUID uuid = _UUIDWithBytes(byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11, byte12, byte13, byte14, byte15);
	return uuid;
}

inline static UUID uuid_from_string(const char *uuids) {
	UUID uuid = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	if (NULL == uuids) return uuid;
	unsigned long length = strlen(uuids);
	if (length != 36) return uuid;
	unsigned int bytes[16];
	sscanf(uuids, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X", &bytes[0], &bytes[1], &bytes[2], &bytes[3], &bytes[4], &bytes[5], &bytes[6], &bytes[7], &bytes[8], &bytes[9], &bytes[10], &bytes[11], &bytes[12], &bytes[13], &bytes[14], &bytes[15]);
	
	uint8_t *bytePointer = &uuid.byte0;
	for (uint8_t i=0; i<16; i++)
		*bytePointer++ = bytes[i];
	return uuid;
}

inline static int uuid_cmp(UUID uuid, UUID other) {
	return memcmp(&uuid, &other, sizeof(UUID));
}

#undef _UUIDWithBytes
//#ifdef __cplusplus
//}
//#endif

#endif
