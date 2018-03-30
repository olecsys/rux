#ifndef IV_COMMON_H
#define IV_COMMON_H

#include <stdlib.h>
#include <stdarg.h>

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#ifndef iv_int8
//#define iv_int8 signed char
#define iv_int8 int8_t
#endif
#ifndef iv_int16
//#define iv_int16 short
#define iv_int16 int16_t
#endif
#ifndef iv_int32
//#define iv_int32 int
#define iv_int32 int32_t
#endif
#ifndef iv_int64
//#define iv_int64 long long
#define iv_int64 int64_t
#endif
#ifndef iv_uint8
//#define iv_uint8 unsigned char
#define iv_uint8 uint8_t
#endif
#ifndef iv_uint16
//#define iv_uint16 unsigned short
#define iv_uint16 uint16_t
#endif
#ifndef iv_uint32
//#define iv_uint32 unsigned int
#define iv_uint32 uint32_t
#endif
#ifndef iv_uint64
//#define iv_uint64 unsigned long long
#define iv_uint64 uint64_t
#endif

//typedef iv_int8        int8;
//typedef iv_uint8      uint8;
//typedef iv_int16      int16;
//typedef iv_uint16    uint16;
//typedef iv_int32      int32;
//typedef iv_uint32    uint32;
//typedef iv_int64      int64;
//typedef iv_uint64    uint64;

#define COUNT(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#define ZERO( x ) memset( x, 0, sizeof( *x ) );


#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC extern
#endif


#if defined( _WIN32 ) || defined (__MINGW32__)
#else
#ifdef __cplusplus
#define EXTERNC extern "C"
#define CALLBACK
#undef  TRUE
#define TRUE 1
#undef  FALSE
#define FALSE 0
#endif
typedef void *HANDLE;
#endif

#if defined(_MSC_VER) || defined (__MINGW32__)
#define IV_EXPORT __declspec(dllexport)
#define IV_EXTERN extern
#elif defined(__sun) && !defined(__GNUC__)
#define IV_EXPORT __symbolic
#define IV_EXTERN extern __hidden
#else
#define IV_EXPORT __attribute__ ((visibility("default")))
#define IV_EXTERN extern __attribute__ ((visibility("hidden")))
#endif
#define IV_IMPORT

#define IV_EXPORTC EXTERNC IV_EXPORT
#define IV_IMPORTC EXTERNC IV_IMPORT


#define PARAM_NONE    0
#define PARAM_CONFIG  1
#define PARAM_PARAM   PARAM_CONFIG
#define PARAM_PCHAR   2
#define PARAM_PVOID   3
#define PARAM_PINT32  4
#define PARAM_PINT64  5
#define PARAM_FUNC    6
#define PARAM_CALLBACK  7  //using callback_t
#define PARAM_BUFFER    8  //using buffer_t
#define PARAM_PFLOAT    9
#define PARAM_UCALLBACK 10 //unregister callback
#define PARAM_PPCHAR 11
#define PARAM_INT32  12
#define PARAM_INT64  13
#define PARAM_UNKNOWN  14

#define param_iter() __p
#define param_foreach( var ) for(param_t* param_iter() = (param_t*)var; param_iter() != NULL && param_iter()->type != 0; ++param_iter() )
#define param_for_param( var ) param_t* param_iter() = (param_t*)var; param_iter() != NULL && param_iter()->type != 0; ++param_iter()
#define each_param(var) param_t* param_iter() = (param_t*)var; param_iter() != NULL && param_iter()->type != 0; ++param_iter()
//#define param_name(n) !strcmp(param_iter()->name, n)
//#define param_type(t) (param_iter()->type == t)
//#define param_val(type) (type*)param_iter()->val
//#define param_is(n, t) param_name(n) && param_type(t)
//#define param_get(t, v, n, pt) if ( (param_iter()->type == pt) && !strcmp(param_iter()->name, n) && param_iter()->val) { v = (t*)param_iter()->val; continue; }
//#define param_get_val(t, v, n, pt) if ( (param_iter()->type == pt) && !strcmp(param_iter()->name, n) && param_iter()->val) { v = *(t*)param_iter()->val; continue; }

#define param_start if (param_iter()->name == NULL) continue
#define param_get_int32(x) else if( (param_iter()->type == PARAM_PINT32) && (param_iter()->val != NULL) && !strcmp(param_iter()->name, #x) ) x = *((iv_int32*)param_iter()->val)
#define param_get_pint32(x) else if( (param_iter()->type == PARAM_PINT32) && !strcmp(param_iter()->name, #x) ) x = (iv_int32*)param_iter()->val
#define param_get_named_pint32(x,n) else if( (param_iter()->type == PARAM_PINT32) && !strcmp(param_iter()->name, n) ) x = (iv_int32*)param_iter()->val
#define param_get_named_int32(x,n) else if( (param_iter()->type == PARAM_PINT32) && !strcmp(param_iter()->name, n) ) x = *(iv_int32*)param_iter()->val
#define param_get_uint32(x) else if( (param_iter()->type == PARAM_PINT32) && (param_iter()->val != NULL) && !strcmp(param_iter()->name, #x) ) x = *((iv_uint32*)param_iter()->val)
#define param_get_puint32(x) else if( (param_iter()->type == PARAM_PINT32) && !strcmp(param_iter()->name, #x) ) x = (iv_uint32*)param_iter()->val
#define param_get_named_puint32(x,n) else if( (param_iter()->type == PARAM_PINT32) && !strcmp(param_iter()->name, n) ) x = (iv_uint32*)param_iter()->val
#define param_get_int64(x) else if( (param_iter()->type == PARAM_PINT64) && (param_iter()->val != NULL) && !strcmp(param_iter()->name, #x) ) x = *((iv_int64*)param_iter()->val)
#define param_get_pint64(x) else if( (param_iter()->type == PARAM_PINT64) && !strcmp(param_iter()->name, #x) ) x = (iv_int64*)param_iter()->val
#define param_get_named_pint64(x,n) else if( (param_iter()->type == PARAM_PINT64) && !strcmp(param_iter()->name, n) ) x = (iv_int64*)param_iter()->val
#define param_get_uint64(x) else if( (param_iter()->type == PARAM_PINT64) && (param_iter()->val != NULL) && !strcmp(param_iter()->name, #x) ) x = *((iv_uint64*)param_iter()->val)
#define param_get_puint64(x) else if( (param_iter()->type == PARAM_PINT64) && !strcmp(param_iter()->name, #x) ) x = (iv_uint64*)param_iter()->val
#define param_get_named_puint64(x,n) else if( (param_iter()->type == PARAM_PINT64) && !strcmp(param_iter()->name, n) ) x = (iv_uint64*)param_iter()->val
#define param_get_float(x) else if( (param_iter()->type == PARAM_PFLOAT) && (param_iter()->val != NULL) && !strcmp(param_iter()->name, #x) ) x = *(float*)param_iter()->val
#define param_get_pfloat(x) else if( (param_iter()->type == PARAM_PFLOAT) && !strcmp(param_iter()->name, #x) ) x = (float*)param_iter()->val
#define param_get_pbuffer(x) else if( (param_iter()->type == PARAM_BUFFER) && !strcmp(param_iter()->name, #x) ) x = ((buffer_t*)param_iter()->val)
#define param_get_buffer(x) else if( (param_iter()->type == PARAM_BUFFER) && (param_iter()->val != NULL) && !strcmp(param_iter()->name, #x) ) x = *((buffer_t*)param_iter()->val)
#define param_get_pvoid(x) else if( (param_iter()->type == PARAM_PVOID ) && !strcmp(param_iter()->name, #x) ) x = (void*)param_iter()->val
#define param_get_pointer(x,t) else if( (param_iter()->type == PARAM_PVOID ) && !strcmp(param_iter()->name, #x) ) x = (t)param_iter()->val
#define param_get_pchar(x) else if( (param_iter()->type == PARAM_PCHAR) && !strcmp(param_iter()->name, #x) ) x = (char*)param_iter()->val
#define param_get_named_pchar(x,n) else if( (param_iter()->type == PARAM_PCHAR) && !strcmp(param_iter()->name, n) ) x = (char*)param_iter()->val
#define param_get_named_pvoid(x,n) else if( (param_iter()->type == PARAM_PVOID) && !strcmp(param_iter()->name, n) ) x = (char*)param_iter()->val
#define param_get_config(x) else if( (param_iter()->type == PARAM_CONFIG) && !strcmp(param_iter()->name, #x) ) x = (param_t*)param_iter()->val
#define param_get_param(x,n) else if( (param_iter()->type == PARAM_PARAM) && !strcmp(param_iter()->name, n) ) x = (param_t*)param_iter()->val
#define param_get_event(x) else if( !strcmp(param_iter()->name, #x) ) x = 1 
#define param_get_event_by_name(x,n) else if( !strcmp(param_iter()->name, n) ) x = 1 
#define param_get_callback(x) else if( (param_iter()->type == PARAM_CALLBACK) && !strcmp(param_iter()->name, #x) ) x = (callback_t*)param_iter()->val
#define param_unknown_pchar(x,y) else if( (param_iter()->type == PARAM_PCHAR) ){ x = (char*)param_iter()->name; y = (char*)param_iter()->val;}

typedef struct param_t 
{
  int         type;  //PARAM_xxx
  const char* name;
  const void* val;
} param_t;

typedef void (* callback_proc_t)(const void* udata, const param_t* p);

typedef struct callback_t 
{
  const void* data;
  callback_proc_t func; //function callback_proc_t
} callback_t;

// data
// size
typedef struct buffer_t 
{
  void* data;
  iv_uint32 size; 
} buffer_t;

typedef struct univ_t
{
  iv_uint32 Size;
  iv_uint32 Type;
}univ_t;

typedef struct version_t
{
  int major;      ///< major number
  int minor;      ///< minor number
  int patch;      ///< patch number
  int build;      ///< build number from svn
  int is_dev;     ///< is development (1 or 0)
} version_t;

#endif // COMMON_H
