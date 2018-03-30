#ifndef IV_VERSION_H
#define IV_VERSION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifndef RC_INVOKED
#include "iv_common.h"
#endif
#include "iv_defines.h"

#ifndef IV_PACKAGE_NAME
#define IV_PACKAGE_NAME "noname"
#endif
#ifndef IV_PACKAGE_USER
#define IV_PACKAGE_USER "noname"
#endif
#ifndef IV_PACKAGE_CHANNEL
#define IV_PACKAGE_CHANNEL "stable"
#endif
#ifndef IV_JENKINS_JOB_URL
#define IV_JENKINS_JOB_URL ""
#endif
#ifndef IV_GIT_URL
#define IV_GIT_URL ""
#endif
#ifndef IV_GIT_COMMIT
#define IV_GIT_COMMIT ""
#endif
#ifndef IV_PACKAGE_DESCRIPTION
#define IV_PACKAGE_DESCRIPTION ""
#endif
#ifndef MAJOR_VERSION
#define MAJOR_VERSION    7
#endif
#ifndef MINOR_VERSION
#define MINOR_VERSION    0
#endif
#ifndef PATCH_VERSION
#define PATCH_VERSION 0
#endif
#ifndef SVN_BUILD_NUMBER
#define SVN_BUILD_NUMBER 0
#endif
#ifndef IV_BUILD_DATE
#define IV_BUILD_DATE "2017-03-17 18:23:35"
#endif

#define IV_VERSION_H_STR(n) IV_VERSION_H_STRINGIFY_HELPER(n)
#define IV_VERSION_H_STRINGIFY_HELPER(n) #n

#define IV_VERSION_STRING \
  IV_VERSION_H_STR(MAJOR_VERSION) "." \
  IV_VERSION_H_STR(MINOR_VERSION) "." \
  IV_VERSION_H_STR(PATCH_VERSION) "." \
  IV_VERSION_H_STR(SVN_BUILD_NUMBER)
typedef bool (*iv_version_dll_get_dependencies_callback_t)(void* udata
  , const char* dependency_package_name
  , const char* dependency_package_user
  , const char* dependency_package_channel
  , const char* dependency_version);
#define IVVERSION \
ivexport const char* dll_version(version_t* ver) {\
  if(ver)\
  {\
    ver->major = MAJOR_VERSION;\
    ver->minor = MINOR_VERSION;\
    ver->patch = PATCH_VERSION;\
    ver->build = SVN_BUILD_NUMBER;\
    ver->is_dev = 0;\
  }\
  return IV_VERSION_STRING;\
}\
ivexport const char* dll_build_date() {\
  return IV_BUILD_DATE;\
}\
ivexport const char* dll_package_name() {\
  return IV_PACKAGE_NAME;\
}\
ivexport const char* dll_package_user() {\
  return IV_PACKAGE_USER;\
}\
ivexport const char* dll_package_channel() {\
  return IV_PACKAGE_CHANNEL;\
}\
ivexport const char* dll_jenkins_job_url() {\
  return IV_JENKINS_JOB_URL;\
}\
ivexport const char* dll_git_url() {\
  return IV_GIT_URL;\
}\
ivexport const char* dll_git_commit() {\
  return IV_GIT_COMMIT;\
}\
ivexport const char* dll_package_description() {\
  return IV_PACKAGE_DESCRIPTION;\
}\
ivexport int dll_get_dependencies(void* udata\
  , iv_version_dll_get_dependencies_callback_t iv_version_dll_get_dependencies_callback) {\
  udata = udata;\
  iv_version_dll_get_dependencies_callback = iv_version_dll_get_dependencies_callback;\
  return 0;\
}
#endif
