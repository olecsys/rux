#ifndef VERSION_H
#define VERSION_H

#define MAJOR_VERSION    1
#define MINOR_VERSION    0
#define PATCH_VERSION    0
#define SVN_BUILD_NUMBER    1
#define VER_INTERNAL_NAME "rux.configure.shellext"
#define VER_FILE_DESCRIPTION "RUX configure Shell Extension"
#define VERSION_H_STR(n) VERSION_H_STRINGIFY_HELPER(n)
#define VERSION_H_STRINGIFY_HELPER(n) #n

#define VERSION_STRING \
	VERSION_H_STR(MAJOR_VERSION) "." \
	VERSION_H_STR(MINOR_VERSION) "." \
	VERSION_H_STR(PATCH_VERSION) "." \
	VERSION_H_STR(SVN_BUILD_NUMBER)
#endif
