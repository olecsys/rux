#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=cc
CCC=CC
CXX=CC
FC=f95
AS=yasm

# Macros
CND_PLATFORM=OracleSolarisStudio-Solaris-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include rux.basis

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lib/xan_malloc_thread.o \
	${OBJECTDIR}/lib/xan_interlocked.o \
	${OBJECTDIR}/lib/xan_type.o \
	${OBJECTDIR}/lib/xan_base64.o \
	${OBJECTDIR}/lib/xan_event.o \
	${OBJECTDIR}/lib/xan_typemanager_defines.o \
	${OBJECTDIR}/lib/xan_float.o \
	${OBJECTDIR}/lib/xan_methodinfo.o \
	${OBJECTDIR}/lib/xan_math.o \
	${OBJECTDIR}/lib/xan_gcref.o \
	${OBJECTDIR}/lib/xan_char.o \
	${OBJECTDIR}/lib/xan_thread.o \
	${OBJECTDIR}/lib/xan_randomnumbergenerator.o \
	${OBJECTDIR}/lib/xan_rawdata.o \
	${OBJECTDIR}/lib/xan_boolean.o \
	${OBJECTDIR}/lib/xan_double.o \
	${OBJECTDIR}/lib/xan_string.o \
	${OBJECTDIR}/lib/xan_malloc_event.o \
	${OBJECTDIR}/lib/xan_int32.o \
	${OBJECTDIR}/lib/xan_security.o \
	${OBJECTDIR}/lib/xan_linux_asm64.o \
	${OBJECTDIR}/lib/xan_int64.o \
	${OBJECTDIR}/lib/xan_log.o \
	${OBJECTDIR}/lib/xan_object.o \
	${OBJECTDIR}/lib/xan_uint64.o \
	${OBJECTDIR}/lib/xan_null.o \
	${OBJECTDIR}/lib/xan_console.o \
	${OBJECTDIR}/lib/xan_pipe.o \
	${OBJECTDIR}/lib/xan_time.o \
	${OBJECTDIR}/lib/xan_uint32.o \
	${OBJECTDIR}/lib/xan_debug.o \
	${OBJECTDIR}/lib/xan_methodparameter.o \
	${OBJECTDIR}/lib/xan_error.o \
	${OBJECTDIR}/lib/xan_memorymanager_defines.o \
	${OBJECTDIR}/lib/xan_gc_defines.o \
	${OBJECTDIR}/lib/xan_crtsect.o \
	${OBJECTDIR}/lib/xan_registry.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64 -xldscope=hidden -Kpic
CXXFLAGS=-m64 -xldscope=hidden -Kpic

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-a x86 -f elf64 -m amd64 -p nasm

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk ./../build/librux.basis.a

./../build/librux.basis.a: ${OBJECTFILES}
	${MKDIR} -p ./../build
	${RM} ./../build/librux.basis.a
	${AR} -rv ./../build/librux.basis.a ${OBJECTFILES} 
	$(RANLIB) ./../build/librux.basis.a

${OBJECTDIR}/lib/xan_malloc_thread.o: lib/xan_malloc_thread.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_malloc_thread.o lib/xan_malloc_thread.cpp

${OBJECTDIR}/lib/xan_interlocked.o: lib/xan_interlocked.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_interlocked.o lib/xan_interlocked.cpp

${OBJECTDIR}/lib/xan_type.o: lib/xan_type.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_type.o lib/xan_type.cpp

${OBJECTDIR}/lib/xan_base64.o: lib/xan_base64.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_base64.o lib/xan_base64.cpp

${OBJECTDIR}/lib/xan_event.o: lib/xan_event.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_event.o lib/xan_event.cpp

${OBJECTDIR}/lib/xan_typemanager_defines.o: lib/xan_typemanager_defines.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_typemanager_defines.o lib/xan_typemanager_defines.cpp

${OBJECTDIR}/lib/xan_float.o: lib/xan_float.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_float.o lib/xan_float.cpp

${OBJECTDIR}/lib/xan_methodinfo.o: lib/xan_methodinfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_methodinfo.o lib/xan_methodinfo.cpp

${OBJECTDIR}/lib/xan_math.o: lib/xan_math.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_math.o lib/xan_math.cpp

${OBJECTDIR}/lib/xan_gcref.o: lib/xan_gcref.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_gcref.o lib/xan_gcref.cpp

${OBJECTDIR}/lib/xan_char.o: lib/xan_char.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_char.o lib/xan_char.cpp

${OBJECTDIR}/lib/xan_thread.o: lib/xan_thread.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_thread.o lib/xan_thread.cpp

${OBJECTDIR}/lib/xan_randomnumbergenerator.o: lib/xan_randomnumbergenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_randomnumbergenerator.o lib/xan_randomnumbergenerator.cpp

${OBJECTDIR}/lib/xan_rawdata.o: lib/xan_rawdata.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_rawdata.o lib/xan_rawdata.cpp

${OBJECTDIR}/lib/xan_boolean.o: lib/xan_boolean.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_boolean.o lib/xan_boolean.cpp

${OBJECTDIR}/lib/xan_double.o: lib/xan_double.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_double.o lib/xan_double.cpp

${OBJECTDIR}/lib/xan_string.o: lib/xan_string.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_string.o lib/xan_string.cpp

${OBJECTDIR}/lib/xan_malloc_event.o: lib/xan_malloc_event.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_malloc_event.o lib/xan_malloc_event.cpp

${OBJECTDIR}/lib/xan_int32.o: lib/xan_int32.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_int32.o lib/xan_int32.cpp

${OBJECTDIR}/lib/xan_security.o: lib/xan_security.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_security.o lib/xan_security.cpp

${OBJECTDIR}/lib/xan_linux_asm64.o: lib/xan_linux_asm64.S 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/lib/xan_linux_asm64.o lib/xan_linux_asm64.S

${OBJECTDIR}/lib/xan_int64.o: lib/xan_int64.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_int64.o lib/xan_int64.cpp

${OBJECTDIR}/lib/xan_log.o: lib/xan_log.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_log.o lib/xan_log.cpp

${OBJECTDIR}/lib/xan_object.o: lib/xan_object.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_object.o lib/xan_object.cpp

${OBJECTDIR}/lib/xan_uint64.o: lib/xan_uint64.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_uint64.o lib/xan_uint64.cpp

${OBJECTDIR}/lib/xan_null.o: lib/xan_null.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_null.o lib/xan_null.cpp

${OBJECTDIR}/lib/xan_console.o: lib/xan_console.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_console.o lib/xan_console.cpp

${OBJECTDIR}/lib/xan_pipe.o: lib/xan_pipe.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_pipe.o lib/xan_pipe.cpp

${OBJECTDIR}/lib/xan_time.o: lib/xan_time.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_time.o lib/xan_time.cpp

${OBJECTDIR}/lib/xan_uint32.o: lib/xan_uint32.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_uint32.o lib/xan_uint32.cpp

${OBJECTDIR}/lib/xan_debug.o: lib/xan_debug.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_debug.o lib/xan_debug.cpp

${OBJECTDIR}/lib/xan_methodparameter.o: lib/xan_methodparameter.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_methodparameter.o lib/xan_methodparameter.cpp

${OBJECTDIR}/lib/xan_error.o: lib/xan_error.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_error.o lib/xan_error.cpp

${OBJECTDIR}/lib/xan_memorymanager_defines.o: lib/xan_memorymanager_defines.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_memorymanager_defines.o lib/xan_memorymanager_defines.cpp

${OBJECTDIR}/lib/xan_gc_defines.o: lib/xan_gc_defines.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_gc_defines.o lib/xan_gc_defines.cpp

${OBJECTDIR}/lib/xan_crtsect.o: lib/xan_crtsect.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_crtsect.o lib/xan_crtsect.cpp

${OBJECTDIR}/lib/xan_registry.o: lib/xan_registry.cpp 
	${MKDIR} -p ${OBJECTDIR}/lib
	$(COMPILE.cc) -g -I./lib -o ${OBJECTDIR}/lib/xan_registry.o lib/xan_registry.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} ./../build/librux.basis.a
	${CCADMIN} -clean

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
