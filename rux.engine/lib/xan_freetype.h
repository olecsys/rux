#ifndef XAN_FREETYPE_H
#define XAN_FREETYPE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE  1
#endif
#define TT_CONFIG_CMAP_FORMAT_0
#define TT_CONFIG_CMAP_FORMAT_2
#define  TT_CONFIG_CMAP_FORMAT_4
#define  TT_CONFIG_CMAP_FORMAT_6
#define  TT_CONFIG_CMAP_FORMAT_8
#define  TT_CONFIG_CMAP_FORMAT_10
#define  TT_CONFIG_CMAP_FORMAT_12
#define  TT_CONFIG_CMAP_FORMAT_13
#define  TT_CONFIG_CMAP_FORMAT_14
namespace ft_error_h
{

#undef  FT_NEED_EXTERN_C

#undef  FT_ERR_XCAT
#undef  FT_ERR_CAT

#define FT_ERR_XCAT( x, y )  x ## y
#define FT_ERR_CAT( x, y )   FT_ERR_XCAT( x, y )


  /* FT_ERR_PREFIX is used as a prefix for error identifiers. */
  /* By default, we use `FT_Err_'.                            */
  /*                                                          */
#ifndef FT_ERR_PREFIX
#define FT_ERR_PREFIX  FT_Err_
#endif


  /* FT_ERR_BASE is used as the base for module-specific errors. */
  /*                                                             */
#ifdef FT_CONFIG_OPTION_USE_MODULE_ERRORS

#ifndef FT_ERR_BASE
#define FT_ERR_BASE  FT_Mod_Err_Base
#endif

#else

#undef FT_ERR_BASE
#define FT_ERR_BASE  0

#endif /* FT_CONFIG_OPTION_USE_MODULE_ERRORS */


  /* If FT_ERRORDEF is not defined, we need to define a simple */
  /* enumeration type.                                         */
  /*                                                           */
#ifndef FT_ERRORDEF

#define FT_ERRORDEF( e, v, s )  e = v,
#define FT_ERROR_START_LIST     enum {
#define FT_ERROR_END_LIST       FT_ERR_CAT( FT_ERR_PREFIX, Max ) };

#ifdef __cplusplus
#define FT_NEED_EXTERN_C
  extern "C" {
#endif

#endif /* !FT_ERRORDEF */


  /* this macro is used to define an error */
#define FT_ERRORDEF_( e, v, s )   \
		  FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v + FT_ERR_BASE, s )

  /* this is only used for <module>_Err_Ok, which must be 0! */
#define FT_NOERRORDEF_( e, v, s ) \
		  FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v, s )


#ifdef FT_ERROR_START_LIST
  FT_ERROR_START_LIST
#endif


  /* now include the error codes */
FT_NOERRORDEF_( Ok,                                        0x00, \
	"no error" )
	FT_ERRORDEF_( Cannot_Open_Resource,                        0x01, \
	"cannot open resource" )
	FT_ERRORDEF_( Unknown_File_Format,                         0x02, \
	"unknown file format" )
	FT_ERRORDEF_( Invalid_File_Format,                         0x03, \
	"broken file" )
	FT_ERRORDEF_( Invalid_Version,                             0x04, \
	"invalid FreeType version" )
	FT_ERRORDEF_( Lower_Module_Version,                        0x05, \
	"module version is too low" )
	FT_ERRORDEF_( Invalid_Argument,                            0x06, \
	"invalid argument" )
	FT_ERRORDEF_( Unimplemented_Feature,                       0x07, \
	"unimplemented feature" )
	FT_ERRORDEF_( Invalid_Table,                               0x08, \
	"broken table" )
	FT_ERRORDEF_( Invalid_Offset,                              0x09, \
	"broken offset within table" )
	FT_ERRORDEF_( Array_Too_Large,                             0x0A, \
	"array allocation size too large" )
	FT_ERRORDEF_( Missing_Module,                              0x0B, \
	"missing module" )
	/* glyph/character errors */
	FT_ERRORDEF_( Invalid_Glyph_Index,                         0x10, \
	"invalid glyph index" )
	FT_ERRORDEF_( Invalid_Character_Code,                      0x11, \
	"invalid character code" )
	FT_ERRORDEF_( Invalid_Glyph_Format,                        0x12, \
	"unsupported glyph image format" )
	FT_ERRORDEF_( Cannot_Render_Glyph,                         0x13, \
	"cannot render this glyph format" )
	FT_ERRORDEF_( Invalid_Outline,                             0x14, \
	"invalid outline" )
	FT_ERRORDEF_( Invalid_Composite,                           0x15, \
	"invalid composite glyph" )
	FT_ERRORDEF_( Too_Many_Hints,                              0x16, \
	"too many hints" )
	FT_ERRORDEF_( Invalid_Pixel_Size,                          0x17, \
	"invalid pixel size" )
	/* handle errors */
	FT_ERRORDEF_( Invalid_Handle,                              0x20, \
	"invalid object handle" )
	FT_ERRORDEF_( Invalid_Library_Handle,                      0x21, \
	"invalid library handle" )
	FT_ERRORDEF_( Invalid_Driver_Handle,                       0x22, \
	"invalid module handle" )
	FT_ERRORDEF_( Invalid_Face_Handle,                         0x23, \
	"invalid face handle" )
	FT_ERRORDEF_( Invalid_Size_Handle,                         0x24, \
	"invalid size handle" )
	FT_ERRORDEF_( Invalid_Slot_Handle,                         0x25, \
	"invalid glyph slot handle" )
	FT_ERRORDEF_( Invalid_CharMap_Handle,                      0x26, \
	"invalid charmap handle" )
	FT_ERRORDEF_( Invalid_Cache_Handle,                        0x27, \
	"invalid cache manager handle" )
	FT_ERRORDEF_( Invalid_Stream_Handle,                       0x28, \
	"invalid stream handle" )
	/* driver errors */
	FT_ERRORDEF_( Too_Many_Drivers,                            0x30, \
	"too many modules" )
	FT_ERRORDEF_( Too_Many_Extensions,                         0x31, \
	"too many extensions" )
	/* memory errors */
	FT_ERRORDEF_( Out_Of_Memory,                               0x40, \
	"out of memory" )
	FT_ERRORDEF_( Unlisted_Object,                             0x41, \
	"unlisted object" )
	/* stream errors */
	FT_ERRORDEF_( Cannot_Open_Stream,                          0x51, \
	"cannot open stream" )
	FT_ERRORDEF_( Invalid_Stream_Seek,                         0x52, \
	"invalid stream seek" )
	FT_ERRORDEF_( Invalid_Stream_Skip,                         0x53, \
	"invalid stream skip" )
	FT_ERRORDEF_( Invalid_Stream_Read,                         0x54, \
	"invalid stream read" )
	FT_ERRORDEF_( Invalid_Stream_Operation,                    0x55, \
	"invalid stream operation" )
	FT_ERRORDEF_( Invalid_Frame_Operation,                     0x56, \
	"invalid frame operation" )
	FT_ERRORDEF_( Nested_Frame_Access,                         0x57, \
	"nested frame access" )
	FT_ERRORDEF_( Invalid_Frame_Read,                          0x58, \
	"invalid frame read" )
	/* raster errors */
	FT_ERRORDEF_( Raster_Uninitialized,                        0x60, \
	"raster uninitialized" )
	FT_ERRORDEF_( Raster_Corrupted,                            0x61, \
	"raster corrupted" )
	FT_ERRORDEF_( Raster_Overflow,                             0x62, \
	"raster overflow" )
	FT_ERRORDEF_( Raster_Negative_Height,                      0x63, \
	"negative height while rastering" )
	/* cache errors */
	FT_ERRORDEF_( Too_Many_Caches,                             0x70, \
	"too many registered caches" )
	/* TrueType and SFNT errors */
	FT_ERRORDEF_( Invalid_Opcode,                              0x80, \
	"invalid opcode" )
	FT_ERRORDEF_( Too_Few_Arguments,                           0x81, \
	"too few arguments" )
	FT_ERRORDEF_( Stack_Overflow,                              0x82, \
	"stack overflow" )
	FT_ERRORDEF_( Code_Overflow,                               0x83, \
	"code overflow" )
	FT_ERRORDEF_( Bad_Argument,                                0x84, \
	"bad argument" )
	FT_ERRORDEF_( Divide_By_Zero,                              0x85, \
	"division by zero" )
	FT_ERRORDEF_( Invalid_Reference,                           0x86, \
	"invalid reference" )
	FT_ERRORDEF_( Debug_OpCode,                                0x87, \
	"found debug opcode" )
	FT_ERRORDEF_( ENDF_In_Exec_Stream,                         0x88, \
	"found ENDF opcode in execution stream" )
	FT_ERRORDEF_( Nested_DEFS,                                 0x89, \
	"nested DEFS" )
	FT_ERRORDEF_( Invalid_CodeRange,                           0x8A, \
	"invalid code range" )
	FT_ERRORDEF_( Execution_Too_Long,                          0x8B, \
	"execution context too long" )
	FT_ERRORDEF_( Too_Many_Function_Defs,                      0x8C, \
	"too many function definitions" )
	FT_ERRORDEF_( Too_Many_Instruction_Defs,                   0x8D, \
	"too many instruction definitions" )
	FT_ERRORDEF_( Table_Missing,                               0x8E, \
	"SFNT font table missing" )
	FT_ERRORDEF_( Horiz_Header_Missing,                        0x8F, \
	"horizontal header (hhea) table missing" )
	FT_ERRORDEF_( Locations_Missing,                           0x90, \
	"locations (loca) table missing" )
	FT_ERRORDEF_( Name_Table_Missing,                          0x91, \
	"name table missing" )
	FT_ERRORDEF_( CMap_Table_Missing,                          0x92, \
	"character map (cmap) table missing" )
	FT_ERRORDEF_( Hmtx_Table_Missing,                          0x93, \
	"horizontal metrics (hmtx) table missing" )
	FT_ERRORDEF_( Post_Table_Missing,                          0x94, \
	"PostScript (post) table missing" )
	FT_ERRORDEF_( Invalid_Horiz_Metrics,                       0x95, \
	"invalid horizontal metrics" )
	FT_ERRORDEF_( Invalid_CharMap_Format,                      0x96, \
	"invalid character map (cmap) format" )
	FT_ERRORDEF_( Invalid_PPem,                                0x97, \
	"invalid ppem value" )
	FT_ERRORDEF_( Invalid_Vert_Metrics,                        0x98, \
	"invalid vertical metrics" )
	FT_ERRORDEF_( Could_Not_Find_Context,                      0x99, \
	"could not find context" )
	FT_ERRORDEF_( Invalid_Post_Table_Format,                   0x9A, \
	"invalid PostScript (post) table format" )
	FT_ERRORDEF_( Invalid_Post_Table,                          0x9B, \
	"invalid PostScript (post) table" )
	/* CFF, CID, and Type 1 errors */
	FT_ERRORDEF_( Syntax_Error,                                0xA0, \
	"opcode syntax error" )
	FT_ERRORDEF_( Stack_Underflow,                             0xA1, \
	"argument stack underflow" )
	FT_ERRORDEF_( Ignore,                                      0xA2, \
	"ignore" )
	FT_ERRORDEF_( No_Unicode_Glyph_Name,                       0xA3, \
	"no Unicode glyph name found" )
	/* BDF errors */
	FT_ERRORDEF_( Missing_Startfont_Field,                     0xB0, \
	"`STARTFONT' field missing" )
	FT_ERRORDEF_( Missing_Font_Field,                          0xB1, \
	"`FONT' field missing" )
	FT_ERRORDEF_( Missing_Size_Field,                          0xB2, \
	"`SIZE' field missing" )
	FT_ERRORDEF_( Missing_Fontboundingbox_Field,               0xB3, \
	"`FONTBOUNDINGBOX' field missing" )
	FT_ERRORDEF_( Missing_Chars_Field,                         0xB4, \
	"`CHARS' field missing" )
	FT_ERRORDEF_( Missing_Startchar_Field,                     0xB5, \
	"`STARTCHAR' field missing" )
	FT_ERRORDEF_( Missing_Encoding_Field,                      0xB6, \
	"`ENCODING' field missing" )
	FT_ERRORDEF_( Missing_Bbx_Field,                           0xB7, \
	"`BBX' field missing" )
	FT_ERRORDEF_( Bbx_Too_Big,                                 0xB8, \
	"`BBX' too big" )
	FT_ERRORDEF_( Corrupted_Font_Header,                       0xB9, \
	"Font header corrupted or missing fields" )
	FT_ERRORDEF_( Corrupted_Font_Glyphs,                       0xBA, \
	"Font glyphs corrupted or missing fields" )


#ifdef FT_ERROR_END_LIST
  FT_ERROR_END_LIST
#endif

#ifdef FT_NEED_EXTERN_C
  }
#endif

#undef FT_ERROR_START_LIST
#undef FT_ERROR_END_LIST

#undef FT_ERRORDEF
#undef FT_ERRORDEF_
#undef FT_NOERRORDEF_

#undef FT_NEED_EXTERN_C
#undef FT_ERR_BASE

  /* FT_KEEP_ERR_PREFIX is needed for ftvalid.h */
#ifndef FT_KEEP_ERR_PREFIX
#undef FT_ERR_PREFIX
#else
#undef FT_KEEP_ERR_PREFIX
#endif





	
#undef  FT_NEED_EXTERN_C
#ifndef FT_MODERRDEF
#ifdef FT_CONFIG_OPTION_USE_MODULE_ERRORS
#define FT_MODERRDEF( e, v, s )  FT_Mod_Err_ ## e = v,
#else
#define FT_MODERRDEF( e, v, s )  FT_Mod_Err_ ## e = 0,
#endif
#define FT_MODERR_START_LIST  enum {
#define FT_MODERR_END_LIST    FT_Mod_Err_Max };
#ifdef __cplusplus
#define FT_NEED_EXTERN_C
	extern "C" {
#endif
#endif
#ifdef FT_MODERR_START_LIST
  FT_MODERR_START_LIST
#endif
  FT_MODERRDEF( Base,      0x000, "base module" )
  FT_MODERRDEF( Autofit,   0x100, "autofitter module" )
  FT_MODERRDEF( BDF,       0x200, "BDF module" )
  FT_MODERRDEF( Bzip2,     0x300, "Bzip2 module" )
  FT_MODERRDEF( Cache,     0x400, "cache module" )
  FT_MODERRDEF( CFF,       0x500, "CFF module" )
  FT_MODERRDEF( CID,       0x600, "CID module" )
  FT_MODERRDEF( Gzip,      0x700, "Gzip module" )
  FT_MODERRDEF( LZW,       0x800, "LZW module" )
  FT_MODERRDEF( OTvalid,   0x900, "OpenType validation module" )
  FT_MODERRDEF( PCF,       0xA00, "PCF module" )
  FT_MODERRDEF( PFR,       0xB00, "PFR module" )
  FT_MODERRDEF( PSaux,     0xC00, "PS auxiliary module" )
  FT_MODERRDEF( PShinter,  0xD00, "PS hinter module" )
  FT_MODERRDEF( PSnames,   0xE00, "PS names module" )
  FT_MODERRDEF( Raster,    0xF00, "raster module" )
  FT_MODERRDEF( SFNT,     0x1000, "SFNT module" )
  FT_MODERRDEF( Smooth,   0x1100, "smooth raster module" )
  FT_MODERRDEF( TrueType, 0x1200, "TrueType module" )
  FT_MODERRDEF( Type1,    0x1300, "Type 1 module" )
  FT_MODERRDEF( Type42,   0x1400, "Type 42 module" )
  FT_MODERRDEF( Winfonts, 0x1500, "Windows FON/FNT module" )
#ifdef FT_MODERR_END_LIST
  FT_MODERR_END_LIST
#endif
#ifdef FT_NEED_EXTERN_C
	}
#endif

#undef FT_MODERR_START_LIST
#undef FT_MODERR_END_LIST
#undef FT_MODERRDEF
#undef FT_NEED_EXTERN_C
	
#undef  FT_ERR_PREFIX
#define FT_ERR_PREFIX  FNT_Err_
#define FT_ERR_BASE    FT_Mod_Err_Winfonts
#undef  FT_NEED_EXTERN_C
#undef  FT_ERR_XCAT
#undef  FT_ERR_CAT
#define FT_ERR_XCAT( x, y )  x ## y
#define FT_ERR_CAT( x, y )   FT_ERR_XCAT( x, y )
#ifndef FT_ERR_PREFIX
#define FT_ERR_PREFIX  FT_Err_
#endif
#ifdef FT_CONFIG_OPTION_USE_MODULE_ERRORS
#ifndef FT_ERR_BASE
#define FT_ERR_BASE  FT_Mod_Err_Base
#endif
#else
#undef FT_ERR_BASE
#define FT_ERR_BASE  0
#endif /* FT_CONFIG_OPTION_USE_MODULE_ERRORS */
#ifndef FT_ERRORDEF
#define FT_ERRORDEF( e, v, s )  e = v,
#define FT_ERROR_START_LIST     enum {
#define FT_ERROR_END_LIST       FT_ERR_CAT( FT_ERR_PREFIX, Max ) };
#ifdef __cplusplus
#define FT_NEED_EXTERN_C
  extern "C" {
#endif
#endif /* !FT_ERRORDEF */
#define FT_ERRORDEF_( e, v, s )   \
	FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v + FT_ERR_BASE, s )
#define FT_NOERRORDEF_( e, v, s ) \
	FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v, s )
#ifdef FT_ERROR_START_LIST
		FT_ERROR_START_LIST
#endif
			/* now include the error codes */
			FT_NOERRORDEF_( Ok,                                        0x00, \
			"no error" )
			FT_ERRORDEF_( Cannot_Open_Resource,                        0x01, \
			"cannot open resource" )
			FT_ERRORDEF_( Unknown_File_Format,                         0x02, \
			"unknown file format" )
			FT_ERRORDEF_( Invalid_File_Format,                         0x03, \
			"broken file" )
			FT_ERRORDEF_( Invalid_Version,                             0x04, \
			"invalid FreeType version" )
			FT_ERRORDEF_( Lower_Module_Version,                        0x05, \
			"module version is too low" )
			FT_ERRORDEF_( Invalid_Argument,                            0x06, \
			"invalid argument" )
			FT_ERRORDEF_( Unimplemented_Feature,                       0x07, \
			"unimplemented feature" )
			FT_ERRORDEF_( Invalid_Table,                               0x08, \
			"broken table" )
			FT_ERRORDEF_( Invalid_Offset,                              0x09, \
			"broken offset within table" )
			FT_ERRORDEF_( Array_Too_Large,                             0x0A, \
			"array allocation size too large" )
			FT_ERRORDEF_( Missing_Module,                              0x0B, \
			"missing module" )
			/* glyph/character errors */
			FT_ERRORDEF_( Invalid_Glyph_Index,                         0x10, \
			"invalid glyph index" )
			FT_ERRORDEF_( Invalid_Character_Code,                      0x11, \
			"invalid character code" )
			FT_ERRORDEF_( Invalid_Glyph_Format,                        0x12, \
			"unsupported glyph image format" )
			FT_ERRORDEF_( Cannot_Render_Glyph,                         0x13, \
			"cannot render this glyph format" )
			FT_ERRORDEF_( Invalid_Outline,                             0x14, \
			"invalid outline" )
			FT_ERRORDEF_( Invalid_Composite,                           0x15, \
			"invalid composite glyph" )
			FT_ERRORDEF_( Too_Many_Hints,                              0x16, \
			"too many hints" )
			FT_ERRORDEF_( Invalid_Pixel_Size,                          0x17, \
			"invalid pixel size" )
			/* handle errors */
			FT_ERRORDEF_( Invalid_Handle,                              0x20, \
			"invalid object handle" )
			FT_ERRORDEF_( Invalid_Library_Handle,                      0x21, \
			"invalid library handle" )
			FT_ERRORDEF_( Invalid_Driver_Handle,                       0x22, \
			"invalid module handle" )
			FT_ERRORDEF_( Invalid_Face_Handle,                         0x23, \
			"invalid face handle" )
			FT_ERRORDEF_( Invalid_Size_Handle,                         0x24, \
			"invalid size handle" )
			FT_ERRORDEF_( Invalid_Slot_Handle,                         0x25, \
			"invalid glyph slot handle" )
			FT_ERRORDEF_( Invalid_CharMap_Handle,                      0x26, \
			"invalid charmap handle" )
			FT_ERRORDEF_( Invalid_Cache_Handle,                        0x27, \
			"invalid cache manager handle" )
			FT_ERRORDEF_( Invalid_Stream_Handle,                       0x28, \
			"invalid stream handle" )
			/* driver errors */
			FT_ERRORDEF_( Too_Many_Drivers,                            0x30, \
			"too many modules" )
			FT_ERRORDEF_( Too_Many_Extensions,                         0x31, \
			"too many extensions" )
			/* memory errors */
			FT_ERRORDEF_( Out_Of_Memory,                               0x40, \
			"out of memory" )
			FT_ERRORDEF_( Unlisted_Object,                             0x41, \
			"unlisted object" )
			/* stream errors */
			FT_ERRORDEF_( Cannot_Open_Stream,                          0x51, \
			"cannot open stream" )
			FT_ERRORDEF_( Invalid_Stream_Seek,                         0x52, \
			"invalid stream seek" )
			FT_ERRORDEF_( Invalid_Stream_Skip,                         0x53, \
			"invalid stream skip" )
			FT_ERRORDEF_( Invalid_Stream_Read,                         0x54, \
			"invalid stream read" )
			FT_ERRORDEF_( Invalid_Stream_Operation,                    0x55, \
			"invalid stream operation" )
			FT_ERRORDEF_( Invalid_Frame_Operation,                     0x56, \
			"invalid frame operation" )
			FT_ERRORDEF_( Nested_Frame_Access,                         0x57, \
			"nested frame access" )
			FT_ERRORDEF_( Invalid_Frame_Read,                          0x58, \
			"invalid frame read" )
			/* raster errors */
			FT_ERRORDEF_( Raster_Uninitialized,                        0x60, \
			"raster uninitialized" )
			FT_ERRORDEF_( Raster_Corrupted,                            0x61, \
			"raster corrupted" )
			FT_ERRORDEF_( Raster_Overflow,                             0x62, \
			"raster overflow" )
			FT_ERRORDEF_( Raster_Negative_Height,                      0x63, \
			"negative height while rastering" )
			/* cache errors */
			FT_ERRORDEF_( Too_Many_Caches,                             0x70, \
			"too many registered caches" )
			/* TrueType and SFNT errors */
			FT_ERRORDEF_( Invalid_Opcode,                              0x80, \
			"invalid opcode" )
			FT_ERRORDEF_( Too_Few_Arguments,                           0x81, \
			"too few arguments" )
			FT_ERRORDEF_( Stack_Overflow,                              0x82, \
			"stack overflow" )
			FT_ERRORDEF_( Code_Overflow,                               0x83, \
			"code overflow" )
			FT_ERRORDEF_( Bad_Argument,                                0x84, \
			"bad argument" )
			FT_ERRORDEF_( Divide_By_Zero,                              0x85, \
			"division by zero" )
			FT_ERRORDEF_( Invalid_Reference,                           0x86, \
			"invalid reference" )
			FT_ERRORDEF_( Debug_OpCode,                                0x87, \
			"found debug opcode" )
			FT_ERRORDEF_( ENDF_In_Exec_Stream,                         0x88, \
			"found ENDF opcode in execution stream" )
			FT_ERRORDEF_( Nested_DEFS,                                 0x89, \
			"nested DEFS" )
			FT_ERRORDEF_( Invalid_CodeRange,                           0x8A, \
			"invalid code range" )
			FT_ERRORDEF_( Execution_Too_Long,                          0x8B, \
			"execution context too long" )
			FT_ERRORDEF_( Too_Many_Function_Defs,                      0x8C, \
			"too many function definitions" )
			FT_ERRORDEF_( Too_Many_Instruction_Defs,                   0x8D, \
			"too many instruction definitions" )
			FT_ERRORDEF_( Table_Missing,                               0x8E, \
			"SFNT font table missing" )
			FT_ERRORDEF_( Horiz_Header_Missing,                        0x8F, \
			"horizontal header (hhea) table missing" )
			FT_ERRORDEF_( Locations_Missing,                           0x90, \
			"locations (loca) table missing" )
			FT_ERRORDEF_( Name_Table_Missing,                          0x91, \
			"name table missing" )
			FT_ERRORDEF_( CMap_Table_Missing,                          0x92, \
			"character map (cmap) table missing" )
			FT_ERRORDEF_( Hmtx_Table_Missing,                          0x93, \
			"horizontal metrics (hmtx) table missing" )
			FT_ERRORDEF_( Post_Table_Missing,                          0x94, \
			"PostScript (post) table missing" )
			FT_ERRORDEF_( Invalid_Horiz_Metrics,                       0x95, \
			"invalid horizontal metrics" )
			FT_ERRORDEF_( Invalid_CharMap_Format,                      0x96, \
			"invalid character map (cmap) format" )
			FT_ERRORDEF_( Invalid_PPem,                                0x97, \
			"invalid ppem value" )
			FT_ERRORDEF_( Invalid_Vert_Metrics,                        0x98, \
			"invalid vertical metrics" )
			FT_ERRORDEF_( Could_Not_Find_Context,                      0x99, \
			"could not find context" )
			FT_ERRORDEF_( Invalid_Post_Table_Format,                   0x9A, \
			"invalid PostScript (post) table format" )
			FT_ERRORDEF_( Invalid_Post_Table,                          0x9B, \
			"invalid PostScript (post) table" )
			/* CFF, CID, and Type 1 errors */
			FT_ERRORDEF_( Syntax_Error,                                0xA0, \
			"opcode syntax error" )
			FT_ERRORDEF_( Stack_Underflow,                             0xA1, \
			"argument stack underflow" )
			FT_ERRORDEF_( Ignore,                                      0xA2, \
			"ignore" )
			FT_ERRORDEF_( No_Unicode_Glyph_Name,                       0xA3, \
			"no Unicode glyph name found" )
			/* BDF errors */
			FT_ERRORDEF_( Missing_Startfont_Field,                     0xB0, \
			"`STARTFONT' field missing" )
			FT_ERRORDEF_( Missing_Font_Field,                          0xB1, \
			"`FONT' field missing" )
			FT_ERRORDEF_( Missing_Size_Field,                          0xB2, \
			"`SIZE' field missing" )
			FT_ERRORDEF_( Missing_Fontboundingbox_Field,               0xB3, \
			"`FONTBOUNDINGBOX' field missing" )
			FT_ERRORDEF_( Missing_Chars_Field,                         0xB4, \
			"`CHARS' field missing" )
			FT_ERRORDEF_( Missing_Startchar_Field,                     0xB5, \
			"`STARTCHAR' field missing" )
			FT_ERRORDEF_( Missing_Encoding_Field,                      0xB6, \
			"`ENCODING' field missing" )
			FT_ERRORDEF_( Missing_Bbx_Field,                           0xB7, \
			"`BBX' field missing" )
			FT_ERRORDEF_( Bbx_Too_Big,                                 0xB8, \
			"`BBX' too big" )
			FT_ERRORDEF_( Corrupted_Font_Header,                       0xB9, \
			"Font header corrupted or missing fields" )
			FT_ERRORDEF_( Corrupted_Font_Glyphs,                       0xBA, \
			"Font glyphs corrupted or missing fields" )
#ifdef FT_ERROR_END_LIST
  FT_ERROR_END_LIST
#endif
#ifdef FT_NEED_EXTERN_C
  }
#endif
#undef FT_ERROR_START_LIST
#undef FT_ERROR_END_LIST
#undef FT_ERRORDEF
#undef FT_ERRORDEF_
#undef FT_NOERRORDEF_
#undef FT_NEED_EXTERN_C
#undef FT_ERR_BASE
	/* FT_KEEP_ERR_PREFIX is needed for ftvalid.h */
#ifndef FT_KEEP_ERR_PREFIX
#undef FT_ERR_PREFIX
#else
#undef FT_KEEP_ERR_PREFIX
#endif


#undef  FT_ERR_PREFIX
#define FT_ERR_PREFIX  TT_Err_
#define FT_ERR_BASE    FT_Mod_Err_TrueType
#undef  FT_NEED_EXTERN_C
#undef  FT_ERR_XCAT
#undef  FT_ERR_CAT
#define FT_ERR_XCAT( x, y )  x ## y
#define FT_ERR_CAT( x, y )   FT_ERR_XCAT( x, y )
#ifndef FT_ERR_PREFIX
#define FT_ERR_PREFIX  FT_Err_
#endif
#ifdef FT_CONFIG_OPTION_USE_MODULE_ERRORS
#ifndef FT_ERR_BASE
#define FT_ERR_BASE  FT_Mod_Err_Base
#endif
#else
#undef FT_ERR_BASE
#define FT_ERR_BASE  0
#endif /* FT_CONFIG_OPTION_USE_MODULE_ERRORS */
#ifndef FT_ERRORDEF
#define FT_ERRORDEF( e, v, s )  e = v,
#define FT_ERROR_START_LIST     enum {
#define FT_ERROR_END_LIST       FT_ERR_CAT( FT_ERR_PREFIX, Max ) };
#ifdef __cplusplus
#define FT_NEED_EXTERN_C
  extern "C" {
#endif
#endif /* !FT_ERRORDEF */
#define FT_ERRORDEF_( e, v, s )   \
	FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v + FT_ERR_BASE, s )
#define FT_NOERRORDEF_( e, v, s ) \
	FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v, s )
#ifdef FT_ERROR_START_LIST
		FT_ERROR_START_LIST
#endif
			/* now include the error codes */
			FT_NOERRORDEF_( Ok,                                        0x00, \
			"no error" )
			FT_ERRORDEF_( Cannot_Open_Resource,                        0x01, \
			"cannot open resource" )
			FT_ERRORDEF_( Unknown_File_Format,                         0x02, \
			"unknown file format" )
			FT_ERRORDEF_( Invalid_File_Format,                         0x03, \
			"broken file" )
			FT_ERRORDEF_( Invalid_Version,                             0x04, \
			"invalid FreeType version" )
			FT_ERRORDEF_( Lower_Module_Version,                        0x05, \
			"module version is too low" )
			FT_ERRORDEF_( Invalid_Argument,                            0x06, \
			"invalid argument" )
			FT_ERRORDEF_( Unimplemented_Feature,                       0x07, \
			"unimplemented feature" )
			FT_ERRORDEF_( Invalid_Table,                               0x08, \
			"broken table" )
			FT_ERRORDEF_( Invalid_Offset,                              0x09, \
			"broken offset within table" )
			FT_ERRORDEF_( Array_Too_Large,                             0x0A, \
			"array allocation size too large" )
			FT_ERRORDEF_( Missing_Module,                              0x0B, \
			"missing module" )
			/* glyph/character errors */
			FT_ERRORDEF_( Invalid_Glyph_Index,                         0x10, \
			"invalid glyph index" )
			FT_ERRORDEF_( Invalid_Character_Code,                      0x11, \
			"invalid character code" )
			FT_ERRORDEF_( Invalid_Glyph_Format,                        0x12, \
			"unsupported glyph image format" )
			FT_ERRORDEF_( Cannot_Render_Glyph,                         0x13, \
			"cannot render this glyph format" )
			FT_ERRORDEF_( Invalid_Outline,                             0x14, \
			"invalid outline" )
			FT_ERRORDEF_( Invalid_Composite,                           0x15, \
			"invalid composite glyph" )
			FT_ERRORDEF_( Too_Many_Hints,                              0x16, \
			"too many hints" )
			FT_ERRORDEF_( Invalid_Pixel_Size,                          0x17, \
			"invalid pixel size" )
			/* handle errors */
			FT_ERRORDEF_( Invalid_Handle,                              0x20, \
			"invalid object handle" )
			FT_ERRORDEF_( Invalid_Library_Handle,                      0x21, \
			"invalid library handle" )
			FT_ERRORDEF_( Invalid_Driver_Handle,                       0x22, \
			"invalid module handle" )
			FT_ERRORDEF_( Invalid_Face_Handle,                         0x23, \
			"invalid face handle" )
			FT_ERRORDEF_( Invalid_Size_Handle,                         0x24, \
			"invalid size handle" )
			FT_ERRORDEF_( Invalid_Slot_Handle,                         0x25, \
			"invalid glyph slot handle" )
			FT_ERRORDEF_( Invalid_CharMap_Handle,                      0x26, \
			"invalid charmap handle" )
			FT_ERRORDEF_( Invalid_Cache_Handle,                        0x27, \
			"invalid cache manager handle" )
			FT_ERRORDEF_( Invalid_Stream_Handle,                       0x28, \
			"invalid stream handle" )
			/* driver errors */
			FT_ERRORDEF_( Too_Many_Drivers,                            0x30, \
			"too many modules" )
			FT_ERRORDEF_( Too_Many_Extensions,                         0x31, \
			"too many extensions" )
			/* memory errors */
			FT_ERRORDEF_( Out_Of_Memory,                               0x40, \
			"out of memory" )
			FT_ERRORDEF_( Unlisted_Object,                             0x41, \
			"unlisted object" )
			/* stream errors */
			FT_ERRORDEF_( Cannot_Open_Stream,                          0x51, \
			"cannot open stream" )
			FT_ERRORDEF_( Invalid_Stream_Seek,                         0x52, \
			"invalid stream seek" )
			FT_ERRORDEF_( Invalid_Stream_Skip,                         0x53, \
			"invalid stream skip" )
			FT_ERRORDEF_( Invalid_Stream_Read,                         0x54, \
			"invalid stream read" )
			FT_ERRORDEF_( Invalid_Stream_Operation,                    0x55, \
			"invalid stream operation" )
			FT_ERRORDEF_( Invalid_Frame_Operation,                     0x56, \
			"invalid frame operation" )
			FT_ERRORDEF_( Nested_Frame_Access,                         0x57, \
			"nested frame access" )
			FT_ERRORDEF_( Invalid_Frame_Read,                          0x58, \
			"invalid frame read" )
			/* raster errors */
			FT_ERRORDEF_( Raster_Uninitialized,                        0x60, \
			"raster uninitialized" )
			FT_ERRORDEF_( Raster_Corrupted,                            0x61, \
			"raster corrupted" )
			FT_ERRORDEF_( Raster_Overflow,                             0x62, \
			"raster overflow" )
			FT_ERRORDEF_( Raster_Negative_Height,                      0x63, \
			"negative height while rastering" )
			/* cache errors */
			FT_ERRORDEF_( Too_Many_Caches,                             0x70, \
			"too many registered caches" )
			/* TrueType and SFNT errors */
			FT_ERRORDEF_( Invalid_Opcode,                              0x80, \
			"invalid opcode" )
			FT_ERRORDEF_( Too_Few_Arguments,                           0x81, \
			"too few arguments" )
			FT_ERRORDEF_( Stack_Overflow,                              0x82, \
			"stack overflow" )
			FT_ERRORDEF_( Code_Overflow,                               0x83, \
			"code overflow" )
			FT_ERRORDEF_( Bad_Argument,                                0x84, \
			"bad argument" )
			FT_ERRORDEF_( Divide_By_Zero,                              0x85, \
			"division by zero" )
			FT_ERRORDEF_( Invalid_Reference,                           0x86, \
			"invalid reference" )
			FT_ERRORDEF_( Debug_OpCode,                                0x87, \
			"found debug opcode" )
			FT_ERRORDEF_( ENDF_In_Exec_Stream,                         0x88, \
			"found ENDF opcode in execution stream" )
			FT_ERRORDEF_( Nested_DEFS,                                 0x89, \
			"nested DEFS" )
			FT_ERRORDEF_( Invalid_CodeRange,                           0x8A, \
			"invalid code range" )
			FT_ERRORDEF_( Execution_Too_Long,                          0x8B, \
			"execution context too long" )
			FT_ERRORDEF_( Too_Many_Function_Defs,                      0x8C, \
			"too many function definitions" )
			FT_ERRORDEF_( Too_Many_Instruction_Defs,                   0x8D, \
			"too many instruction definitions" )
			FT_ERRORDEF_( Table_Missing,                               0x8E, \
			"SFNT font table missing" )
			FT_ERRORDEF_( Horiz_Header_Missing,                        0x8F, \
			"horizontal header (hhea) table missing" )
			FT_ERRORDEF_( Locations_Missing,                           0x90, \
			"locations (loca) table missing" )
			FT_ERRORDEF_( Name_Table_Missing,                          0x91, \
			"name table missing" )
			FT_ERRORDEF_( CMap_Table_Missing,                          0x92, \
			"character map (cmap) table missing" )
			FT_ERRORDEF_( Hmtx_Table_Missing,                          0x93, \
			"horizontal metrics (hmtx) table missing" )
			FT_ERRORDEF_( Post_Table_Missing,                          0x94, \
			"PostScript (post) table missing" )
			FT_ERRORDEF_( Invalid_Horiz_Metrics,                       0x95, \
			"invalid horizontal metrics" )
			FT_ERRORDEF_( Invalid_CharMap_Format,                      0x96, \
			"invalid character map (cmap) format" )
			FT_ERRORDEF_( Invalid_PPem,                                0x97, \
			"invalid ppem value" )
			FT_ERRORDEF_( Invalid_Vert_Metrics,                        0x98, \
			"invalid vertical metrics" )
			FT_ERRORDEF_( Could_Not_Find_Context,                      0x99, \
			"could not find context" )
			FT_ERRORDEF_( Invalid_Post_Table_Format,                   0x9A, \
			"invalid PostScript (post) table format" )
			FT_ERRORDEF_( Invalid_Post_Table,                          0x9B, \
			"invalid PostScript (post) table" )
			/* CFF, CID, and Type 1 errors */
			FT_ERRORDEF_( Syntax_Error,                                0xA0, \
			"opcode syntax error" )
			FT_ERRORDEF_( Stack_Underflow,                             0xA1, \
			"argument stack underflow" )
			FT_ERRORDEF_( Ignore,                                      0xA2, \
			"ignore" )
			FT_ERRORDEF_( No_Unicode_Glyph_Name,                       0xA3, \
			"no Unicode glyph name found" )
			/* BDF errors */
			FT_ERRORDEF_( Missing_Startfont_Field,                     0xB0, \
			"`STARTFONT' field missing" )
			FT_ERRORDEF_( Missing_Font_Field,                          0xB1, \
			"`FONT' field missing" )
			FT_ERRORDEF_( Missing_Size_Field,                          0xB2, \
			"`SIZE' field missing" )
			FT_ERRORDEF_( Missing_Fontboundingbox_Field,               0xB3, \
			"`FONTBOUNDINGBOX' field missing" )
			FT_ERRORDEF_( Missing_Chars_Field,                         0xB4, \
			"`CHARS' field missing" )
			FT_ERRORDEF_( Missing_Startchar_Field,                     0xB5, \
			"`STARTCHAR' field missing" )
			FT_ERRORDEF_( Missing_Encoding_Field,                      0xB6, \
			"`ENCODING' field missing" )
			FT_ERRORDEF_( Missing_Bbx_Field,                           0xB7, \
			"`BBX' field missing" )
			FT_ERRORDEF_( Bbx_Too_Big,                                 0xB8, \
			"`BBX' too big" )
			FT_ERRORDEF_( Corrupted_Font_Header,                       0xB9, \
			"Font header corrupted or missing fields" )
			FT_ERRORDEF_( Corrupted_Font_Glyphs,                       0xBA, \
			"Font glyphs corrupted or missing fields" )
#ifdef FT_ERROR_END_LIST
  FT_ERROR_END_LIST
#endif
#ifdef FT_NEED_EXTERN_C
  }
#endif
#undef FT_ERROR_START_LIST
#undef FT_ERROR_END_LIST
#undef FT_ERRORDEF
#undef FT_ERRORDEF_
#undef FT_NOERRORDEF_
#undef FT_NEED_EXTERN_C
#undef FT_ERR_BASE
	/* FT_KEEP_ERR_PREFIX is needed for ftvalid.h */
#ifndef FT_KEEP_ERR_PREFIX
#undef FT_ERR_PREFIX
#else
#undef FT_KEEP_ERR_PREFIX
#endif

	
#undef  FT_ERR_PREFIX
#define FT_ERR_PREFIX  SFNT_Err_
#define FT_ERR_BASE    FT_Mod_Err_SFNT
#define FT_KEEP_ERR_PREFIX
#undef  FT_NEED_EXTERN_C
#undef  FT_ERR_XCAT
#undef  FT_ERR_CAT
#define FT_ERR_XCAT( x, y )  x ## y
#define FT_ERR_CAT( x, y )   FT_ERR_XCAT( x, y )
#ifndef FT_ERR_PREFIX
#define FT_ERR_PREFIX  FT_Err_
#endif
#ifdef FT_CONFIG_OPTION_USE_MODULE_ERRORS
#ifndef FT_ERR_BASE
#define FT_ERR_BASE  FT_Mod_Err_Base
#endif
#else
#undef FT_ERR_BASE
#define FT_ERR_BASE  0
#endif /* FT_CONFIG_OPTION_USE_MODULE_ERRORS */
#ifndef FT_ERRORDEF
#define FT_ERRORDEF( e, v, s )  e = v,
#define FT_ERROR_START_LIST     enum {
#define FT_ERROR_END_LIST       FT_ERR_CAT( FT_ERR_PREFIX, Max ) };
#ifdef __cplusplus
#define FT_NEED_EXTERN_C
  extern "C" {
#endif
#endif /* !FT_ERRORDEF */
#define FT_ERRORDEF_( e, v, s )   \
	FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v + FT_ERR_BASE, s )
#define FT_NOERRORDEF_( e, v, s ) \
	FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v, s )
#ifdef FT_ERROR_START_LIST
		FT_ERROR_START_LIST
#endif
			/* now include the error codes */
			FT_NOERRORDEF_( Ok,                                        0x00, \
			"no error" )
			FT_ERRORDEF_( Cannot_Open_Resource,                        0x01, \
			"cannot open resource" )
			FT_ERRORDEF_( Unknown_File_Format,                         0x02, \
			"unknown file format" )
			FT_ERRORDEF_( Invalid_File_Format,                         0x03, \
			"broken file" )
			FT_ERRORDEF_( Invalid_Version,                             0x04, \
			"invalid FreeType version" )
			FT_ERRORDEF_( Lower_Module_Version,                        0x05, \
			"module version is too low" )
			FT_ERRORDEF_( Invalid_Argument,                            0x06, \
			"invalid argument" )
			FT_ERRORDEF_( Unimplemented_Feature,                       0x07, \
			"unimplemented feature" )
			FT_ERRORDEF_( Invalid_Table,                               0x08, \
			"broken table" )
			FT_ERRORDEF_( Invalid_Offset,                              0x09, \
			"broken offset within table" )
			FT_ERRORDEF_( Array_Too_Large,                             0x0A, \
			"array allocation size too large" )
			FT_ERRORDEF_( Missing_Module,                              0x0B, \
			"missing module" )
			/* glyph/character errors */
			FT_ERRORDEF_( Invalid_Glyph_Index,                         0x10, \
			"invalid glyph index" )
			FT_ERRORDEF_( Invalid_Character_Code,                      0x11, \
			"invalid character code" )
			FT_ERRORDEF_( Invalid_Glyph_Format,                        0x12, \
			"unsupported glyph image format" )
			FT_ERRORDEF_( Cannot_Render_Glyph,                         0x13, \
			"cannot render this glyph format" )
			FT_ERRORDEF_( Invalid_Outline,                             0x14, \
			"invalid outline" )
			FT_ERRORDEF_( Invalid_Composite,                           0x15, \
			"invalid composite glyph" )
			FT_ERRORDEF_( Too_Many_Hints,                              0x16, \
			"too many hints" )
			FT_ERRORDEF_( Invalid_Pixel_Size,                          0x17, \
			"invalid pixel size" )
			/* handle errors */
			FT_ERRORDEF_( Invalid_Handle,                              0x20, \
			"invalid object handle" )
			FT_ERRORDEF_( Invalid_Library_Handle,                      0x21, \
			"invalid library handle" )
			FT_ERRORDEF_( Invalid_Driver_Handle,                       0x22, \
			"invalid module handle" )
			FT_ERRORDEF_( Invalid_Face_Handle,                         0x23, \
			"invalid face handle" )
			FT_ERRORDEF_( Invalid_Size_Handle,                         0x24, \
			"invalid size handle" )
			FT_ERRORDEF_( Invalid_Slot_Handle,                         0x25, \
			"invalid glyph slot handle" )
			FT_ERRORDEF_( Invalid_CharMap_Handle,                      0x26, \
			"invalid charmap handle" )
			FT_ERRORDEF_( Invalid_Cache_Handle,                        0x27, \
			"invalid cache manager handle" )
			FT_ERRORDEF_( Invalid_Stream_Handle,                       0x28, \
			"invalid stream handle" )
			/* driver errors */
			FT_ERRORDEF_( Too_Many_Drivers,                            0x30, \
			"too many modules" )
			FT_ERRORDEF_( Too_Many_Extensions,                         0x31, \
			"too many extensions" )
			/* memory errors */
			FT_ERRORDEF_( Out_Of_Memory,                               0x40, \
			"out of memory" )
			FT_ERRORDEF_( Unlisted_Object,                             0x41, \
			"unlisted object" )
			/* stream errors */
			FT_ERRORDEF_( Cannot_Open_Stream,                          0x51, \
			"cannot open stream" )
			FT_ERRORDEF_( Invalid_Stream_Seek,                         0x52, \
			"invalid stream seek" )
			FT_ERRORDEF_( Invalid_Stream_Skip,                         0x53, \
			"invalid stream skip" )
			FT_ERRORDEF_( Invalid_Stream_Read,                         0x54, \
			"invalid stream read" )
			FT_ERRORDEF_( Invalid_Stream_Operation,                    0x55, \
			"invalid stream operation" )
			FT_ERRORDEF_( Invalid_Frame_Operation,                     0x56, \
			"invalid frame operation" )
			FT_ERRORDEF_( Nested_Frame_Access,                         0x57, \
			"nested frame access" )
			FT_ERRORDEF_( Invalid_Frame_Read,                          0x58, \
			"invalid frame read" )
			/* raster errors */
			FT_ERRORDEF_( Raster_Uninitialized,                        0x60, \
			"raster uninitialized" )
			FT_ERRORDEF_( Raster_Corrupted,                            0x61, \
			"raster corrupted" )
			FT_ERRORDEF_( Raster_Overflow,                             0x62, \
			"raster overflow" )
			FT_ERRORDEF_( Raster_Negative_Height,                      0x63, \
			"negative height while rastering" )
			/* cache errors */
			FT_ERRORDEF_( Too_Many_Caches,                             0x70, \
			"too many registered caches" )
			/* TrueType and SFNT errors */
			FT_ERRORDEF_( Invalid_Opcode,                              0x80, \
			"invalid opcode" )
			FT_ERRORDEF_( Too_Few_Arguments,                           0x81, \
			"too few arguments" )
			FT_ERRORDEF_( Stack_Overflow,                              0x82, \
			"stack overflow" )
			FT_ERRORDEF_( Code_Overflow,                               0x83, \
			"code overflow" )
			FT_ERRORDEF_( Bad_Argument,                                0x84, \
			"bad argument" )
			FT_ERRORDEF_( Divide_By_Zero,                              0x85, \
			"division by zero" )
			FT_ERRORDEF_( Invalid_Reference,                           0x86, \
			"invalid reference" )
			FT_ERRORDEF_( Debug_OpCode,                                0x87, \
			"found debug opcode" )
			FT_ERRORDEF_( ENDF_In_Exec_Stream,                         0x88, \
			"found ENDF opcode in execution stream" )
			FT_ERRORDEF_( Nested_DEFS,                                 0x89, \
			"nested DEFS" )
			FT_ERRORDEF_( Invalid_CodeRange,                           0x8A, \
			"invalid code range" )
			FT_ERRORDEF_( Execution_Too_Long,                          0x8B, \
			"execution context too long" )
			FT_ERRORDEF_( Too_Many_Function_Defs,                      0x8C, \
			"too many function definitions" )
			FT_ERRORDEF_( Too_Many_Instruction_Defs,                   0x8D, \
			"too many instruction definitions" )
			FT_ERRORDEF_( Table_Missing,                               0x8E, \
			"SFNT font table missing" )
			FT_ERRORDEF_( Horiz_Header_Missing,                        0x8F, \
			"horizontal header (hhea) table missing" )
			FT_ERRORDEF_( Locations_Missing,                           0x90, \
			"locations (loca) table missing" )
			FT_ERRORDEF_( Name_Table_Missing,                          0x91, \
			"name table missing" )
			FT_ERRORDEF_( CMap_Table_Missing,                          0x92, \
			"character map (cmap) table missing" )
			FT_ERRORDEF_( Hmtx_Table_Missing,                          0x93, \
			"horizontal metrics (hmtx) table missing" )
			FT_ERRORDEF_( Post_Table_Missing,                          0x94, \
			"PostScript (post) table missing" )
			FT_ERRORDEF_( Invalid_Horiz_Metrics,                       0x95, \
			"invalid horizontal metrics" )
			FT_ERRORDEF_( Invalid_CharMap_Format,                      0x96, \
			"invalid character map (cmap) format" )
			FT_ERRORDEF_( Invalid_PPem,                                0x97, \
			"invalid ppem value" )
			FT_ERRORDEF_( Invalid_Vert_Metrics,                        0x98, \
			"invalid vertical metrics" )
			FT_ERRORDEF_( Could_Not_Find_Context,                      0x99, \
			"could not find context" )
			FT_ERRORDEF_( Invalid_Post_Table_Format,                   0x9A, \
			"invalid PostScript (post) table format" )
			FT_ERRORDEF_( Invalid_Post_Table,                          0x9B, \
			"invalid PostScript (post) table" )
			/* CFF, CID, and Type 1 errors */
			FT_ERRORDEF_( Syntax_Error,                                0xA0, \
			"opcode syntax error" )
			FT_ERRORDEF_( Stack_Underflow,                             0xA1, \
			"argument stack underflow" )
			FT_ERRORDEF_( Ignore,                                      0xA2, \
			"ignore" )
			FT_ERRORDEF_( No_Unicode_Glyph_Name,                       0xA3, \
			"no Unicode glyph name found" )
			/* BDF errors */
			FT_ERRORDEF_( Missing_Startfont_Field,                     0xB0, \
			"`STARTFONT' field missing" )
			FT_ERRORDEF_( Missing_Font_Field,                          0xB1, \
			"`FONT' field missing" )
			FT_ERRORDEF_( Missing_Size_Field,                          0xB2, \
			"`SIZE' field missing" )
			FT_ERRORDEF_( Missing_Fontboundingbox_Field,               0xB3, \
			"`FONTBOUNDINGBOX' field missing" )
			FT_ERRORDEF_( Missing_Chars_Field,                         0xB4, \
			"`CHARS' field missing" )
			FT_ERRORDEF_( Missing_Startchar_Field,                     0xB5, \
			"`STARTCHAR' field missing" )
			FT_ERRORDEF_( Missing_Encoding_Field,                      0xB6, \
			"`ENCODING' field missing" )
			FT_ERRORDEF_( Missing_Bbx_Field,                           0xB7, \
			"`BBX' field missing" )
			FT_ERRORDEF_( Bbx_Too_Big,                                 0xB8, \
			"`BBX' too big" )
			FT_ERRORDEF_( Corrupted_Font_Header,                       0xB9, \
			"Font header corrupted or missing fields" )
			FT_ERRORDEF_( Corrupted_Font_Glyphs,                       0xBA, \
			"Font glyphs corrupted or missing fields" )
#ifdef FT_ERROR_END_LIST
  FT_ERROR_END_LIST
#endif
#ifdef FT_NEED_EXTERN_C
  }
#endif
#undef FT_ERROR_START_LIST
#undef FT_ERROR_END_LIST
#undef FT_ERRORDEF
#undef FT_ERRORDEF_
#undef FT_NOERRORDEF_
#undef FT_NEED_EXTERN_C
#undef FT_ERR_BASE
	/* FT_KEEP_ERR_PREFIX is needed for ftvalid.h */
#ifndef FT_KEEP_ERR_PREFIX
#undef FT_ERR_PREFIX
#else
#undef FT_KEEP_ERR_PREFIX
#endif
	
#undef  FT_ERR_PREFIX
#define FT_ERR_PREFIX  Smooth_Err_
#define FT_ERR_BASE    FT_Mod_Err_Smooth
#define FT_KEEP_ERR_PREFIX
#undef  FT_NEED_EXTERN_C
#undef  FT_ERR_XCAT
#undef  FT_ERR_CAT
#define FT_ERR_XCAT( x, y )  x ## y
#define FT_ERR_CAT( x, y )   FT_ERR_XCAT( x, y )
#ifndef FT_ERR_PREFIX
#define FT_ERR_PREFIX  FT_Err_
#endif
#ifdef FT_CONFIG_OPTION_USE_MODULE_ERRORS
#ifndef FT_ERR_BASE
#define FT_ERR_BASE  FT_Mod_Err_Base
#endif
#else
#undef FT_ERR_BASE
#define FT_ERR_BASE  0
#endif /* FT_CONFIG_OPTION_USE_MODULE_ERRORS */
#ifndef FT_ERRORDEF
#define FT_ERRORDEF( e, v, s )  e = v,
#define FT_ERROR_START_LIST     enum {
#define FT_ERROR_END_LIST       FT_ERR_CAT( FT_ERR_PREFIX, Max ) };
#ifdef __cplusplus
#define FT_NEED_EXTERN_C
  extern "C" {
#endif
#endif /* !FT_ERRORDEF */
#define FT_ERRORDEF_( e, v, s )   \
	FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v + FT_ERR_BASE, s )
#define FT_NOERRORDEF_( e, v, s ) \
	FT_ERRORDEF( FT_ERR_CAT( FT_ERR_PREFIX, e ), v, s )
#ifdef FT_ERROR_START_LIST
		FT_ERROR_START_LIST
#endif
			/* now include the error codes */
			FT_NOERRORDEF_( Ok,                                        0x00, \
			"no error" )
			FT_ERRORDEF_( Cannot_Open_Resource,                        0x01, \
			"cannot open resource" )
			FT_ERRORDEF_( Unknown_File_Format,                         0x02, \
			"unknown file format" )
			FT_ERRORDEF_( Invalid_File_Format,                         0x03, \
			"broken file" )
			FT_ERRORDEF_( Invalid_Version,                             0x04, \
			"invalid FreeType version" )
			FT_ERRORDEF_( Lower_Module_Version,                        0x05, \
			"module version is too low" )
			FT_ERRORDEF_( Invalid_Argument,                            0x06, \
			"invalid argument" )
			FT_ERRORDEF_( Unimplemented_Feature,                       0x07, \
			"unimplemented feature" )
			FT_ERRORDEF_( Invalid_Table,                               0x08, \
			"broken table" )
			FT_ERRORDEF_( Invalid_Offset,                              0x09, \
			"broken offset within table" )
			FT_ERRORDEF_( Array_Too_Large,                             0x0A, \
			"array allocation size too large" )
			FT_ERRORDEF_( Missing_Module,                              0x0B, \
			"missing module" )
			/* glyph/character errors */
			FT_ERRORDEF_( Invalid_Glyph_Index,                         0x10, \
			"invalid glyph index" )
			FT_ERRORDEF_( Invalid_Character_Code,                      0x11, \
			"invalid character code" )
			FT_ERRORDEF_( Invalid_Glyph_Format,                        0x12, \
			"unsupported glyph image format" )
			FT_ERRORDEF_( Cannot_Render_Glyph,                         0x13, \
			"cannot render this glyph format" )
			FT_ERRORDEF_( Invalid_Outline,                             0x14, \
			"invalid outline" )
			FT_ERRORDEF_( Invalid_Composite,                           0x15, \
			"invalid composite glyph" )
			FT_ERRORDEF_( Too_Many_Hints,                              0x16, \
			"too many hints" )
			FT_ERRORDEF_( Invalid_Pixel_Size,                          0x17, \
			"invalid pixel size" )
			/* handle errors */
			FT_ERRORDEF_( Invalid_Handle,                              0x20, \
			"invalid object handle" )
			FT_ERRORDEF_( Invalid_Library_Handle,                      0x21, \
			"invalid library handle" )
			FT_ERRORDEF_( Invalid_Driver_Handle,                       0x22, \
			"invalid module handle" )
			FT_ERRORDEF_( Invalid_Face_Handle,                         0x23, \
			"invalid face handle" )
			FT_ERRORDEF_( Invalid_Size_Handle,                         0x24, \
			"invalid size handle" )
			FT_ERRORDEF_( Invalid_Slot_Handle,                         0x25, \
			"invalid glyph slot handle" )
			FT_ERRORDEF_( Invalid_CharMap_Handle,                      0x26, \
			"invalid charmap handle" )
			FT_ERRORDEF_( Invalid_Cache_Handle,                        0x27, \
			"invalid cache manager handle" )
			FT_ERRORDEF_( Invalid_Stream_Handle,                       0x28, \
			"invalid stream handle" )
			/* driver errors */
			FT_ERRORDEF_( Too_Many_Drivers,                            0x30, \
			"too many modules" )
			FT_ERRORDEF_( Too_Many_Extensions,                         0x31, \
			"too many extensions" )
			/* memory errors */
			FT_ERRORDEF_( Out_Of_Memory,                               0x40, \
			"out of memory" )
			FT_ERRORDEF_( Unlisted_Object,                             0x41, \
			"unlisted object" )
			/* stream errors */
			FT_ERRORDEF_( Cannot_Open_Stream,                          0x51, \
			"cannot open stream" )
			FT_ERRORDEF_( Invalid_Stream_Seek,                         0x52, \
			"invalid stream seek" )
			FT_ERRORDEF_( Invalid_Stream_Skip,                         0x53, \
			"invalid stream skip" )
			FT_ERRORDEF_( Invalid_Stream_Read,                         0x54, \
			"invalid stream read" )
			FT_ERRORDEF_( Invalid_Stream_Operation,                    0x55, \
			"invalid stream operation" )
			FT_ERRORDEF_( Invalid_Frame_Operation,                     0x56, \
			"invalid frame operation" )
			FT_ERRORDEF_( Nested_Frame_Access,                         0x57, \
			"nested frame access" )
			FT_ERRORDEF_( Invalid_Frame_Read,                          0x58, \
			"invalid frame read" )
			/* raster errors */
			FT_ERRORDEF_( Raster_Uninitialized,                        0x60, \
			"raster uninitialized" )
			FT_ERRORDEF_( Raster_Corrupted,                            0x61, \
			"raster corrupted" )
			FT_ERRORDEF_( Raster_Overflow,                             0x62, \
			"raster overflow" )
			FT_ERRORDEF_( Raster_Negative_Height,                      0x63, \
			"negative height while rastering" )
			/* cache errors */
			FT_ERRORDEF_( Too_Many_Caches,                             0x70, \
			"too many registered caches" )
			/* TrueType and SFNT errors */
			FT_ERRORDEF_( Invalid_Opcode,                              0x80, \
			"invalid opcode" )
			FT_ERRORDEF_( Too_Few_Arguments,                           0x81, \
			"too few arguments" )
			FT_ERRORDEF_( Stack_Overflow,                              0x82, \
			"stack overflow" )
			FT_ERRORDEF_( Code_Overflow,                               0x83, \
			"code overflow" )
			FT_ERRORDEF_( Bad_Argument,                                0x84, \
			"bad argument" )
			FT_ERRORDEF_( Divide_By_Zero,                              0x85, \
			"division by zero" )
			FT_ERRORDEF_( Invalid_Reference,                           0x86, \
			"invalid reference" )
			FT_ERRORDEF_( Debug_OpCode,                                0x87, \
			"found debug opcode" )
			FT_ERRORDEF_( ENDF_In_Exec_Stream,                         0x88, \
			"found ENDF opcode in execution stream" )
			FT_ERRORDEF_( Nested_DEFS,                                 0x89, \
			"nested DEFS" )
			FT_ERRORDEF_( Invalid_CodeRange,                           0x8A, \
			"invalid code range" )
			FT_ERRORDEF_( Execution_Too_Long,                          0x8B, \
			"execution context too long" )
			FT_ERRORDEF_( Too_Many_Function_Defs,                      0x8C, \
			"too many function definitions" )
			FT_ERRORDEF_( Too_Many_Instruction_Defs,                   0x8D, \
			"too many instruction definitions" )
			FT_ERRORDEF_( Table_Missing,                               0x8E, \
			"SFNT font table missing" )
			FT_ERRORDEF_( Horiz_Header_Missing,                        0x8F, \
			"horizontal header (hhea) table missing" )
			FT_ERRORDEF_( Locations_Missing,                           0x90, \
			"locations (loca) table missing" )
			FT_ERRORDEF_( Name_Table_Missing,                          0x91, \
			"name table missing" )
			FT_ERRORDEF_( CMap_Table_Missing,                          0x92, \
			"character map (cmap) table missing" )
			FT_ERRORDEF_( Hmtx_Table_Missing,                          0x93, \
			"horizontal metrics (hmtx) table missing" )
			FT_ERRORDEF_( Post_Table_Missing,                          0x94, \
			"PostScript (post) table missing" )
			FT_ERRORDEF_( Invalid_Horiz_Metrics,                       0x95, \
			"invalid horizontal metrics" )
			FT_ERRORDEF_( Invalid_CharMap_Format,                      0x96, \
			"invalid character map (cmap) format" )
			FT_ERRORDEF_( Invalid_PPem,                                0x97, \
			"invalid ppem value" )
			FT_ERRORDEF_( Invalid_Vert_Metrics,                        0x98, \
			"invalid vertical metrics" )
			FT_ERRORDEF_( Could_Not_Find_Context,                      0x99, \
			"could not find context" )
			FT_ERRORDEF_( Invalid_Post_Table_Format,                   0x9A, \
			"invalid PostScript (post) table format" )
			FT_ERRORDEF_( Invalid_Post_Table,                          0x9B, \
			"invalid PostScript (post) table" )
			/* CFF, CID, and Type 1 errors */
			FT_ERRORDEF_( Syntax_Error,                                0xA0, \
			"opcode syntax error" )
			FT_ERRORDEF_( Stack_Underflow,                             0xA1, \
			"argument stack underflow" )
			FT_ERRORDEF_( Ignore,                                      0xA2, \
			"ignore" )
			FT_ERRORDEF_( No_Unicode_Glyph_Name,                       0xA3, \
			"no Unicode glyph name found" )
			/* BDF errors */
			FT_ERRORDEF_( Missing_Startfont_Field,                     0xB0, \
			"`STARTFONT' field missing" )
			FT_ERRORDEF_( Missing_Font_Field,                          0xB1, \
			"`FONT' field missing" )
			FT_ERRORDEF_( Missing_Size_Field,                          0xB2, \
			"`SIZE' field missing" )
			FT_ERRORDEF_( Missing_Fontboundingbox_Field,               0xB3, \
			"`FONTBOUNDINGBOX' field missing" )
			FT_ERRORDEF_( Missing_Chars_Field,                         0xB4, \
			"`CHARS' field missing" )
			FT_ERRORDEF_( Missing_Startchar_Field,                     0xB5, \
			"`STARTCHAR' field missing" )
			FT_ERRORDEF_( Missing_Encoding_Field,                      0xB6, \
			"`ENCODING' field missing" )
			FT_ERRORDEF_( Missing_Bbx_Field,                           0xB7, \
			"`BBX' field missing" )
			FT_ERRORDEF_( Bbx_Too_Big,                                 0xB8, \
			"`BBX' too big" )
			FT_ERRORDEF_( Corrupted_Font_Header,                       0xB9, \
			"Font header corrupted or missing fields" )
			FT_ERRORDEF_( Corrupted_Font_Glyphs,                       0xBA, \
			"Font glyphs corrupted or missing fields" )
#ifdef FT_ERROR_END_LIST
  FT_ERROR_END_LIST
#endif
#ifdef FT_NEED_EXTERN_C
  }
#endif
#undef FT_ERROR_START_LIST
#undef FT_ERROR_END_LIST
#undef FT_ERRORDEF
#undef FT_ERRORDEF_
#undef FT_NOERRORDEF_
#undef FT_NEED_EXTERN_C
#undef FT_ERR_BASE
	/* FT_KEEP_ERR_PREFIX is needed for ftvalid.h */
#ifndef FT_KEEP_ERR_PREFIX
#undef FT_ERR_PREFIX
#else
#undef FT_KEEP_ERR_PREFIX
#endif
};
	
namespace ft_defines_h
{
#define TT_CMAP_FLAG_UNSORTED     1
#define TT_CMAP_FLAG_OVERLAPPING  2
#define GRID_FIT_METRICS
#define FREETYPE_MAJOR  2
#define FREETYPE_MINOR  4
#define FREETYPE_PATCH  10
#define FT_RENDER_POOL_SIZE  16384L
#ifndef FT_EXPORT
#ifdef __cplusplus
#define FT_EXPORT( x )  extern "C"  x
#else
#define FT_EXPORT( x )  extern  x
#endif
#endif /* !FT_EXPORT */
#ifdef FT_DEBUG_LEVEL_ERROR
#define FT_ERROR( varformat )  FT_Message  varformat
#else  /* !FT_DEBUG_LEVEL_ERROR */
#define FT_ERROR( varformat )
#endif /* !FT_DEBUG_LEVEL_ERROR */
#ifndef FT_UNUSED
#define FT_UNUSED( arg )  ( (arg) = (arg) )
#endif
#ifndef FT_CALLBACK_DEF
#ifdef __cplusplus
#define FT_CALLBACK_DEF( x )  extern "C"  x
#else
#define FT_CALLBACK_DEF( x )  static  x
#endif
#endif /* FT_CALLBACK_DEF */
#ifndef FT_BASE
#ifdef __cplusplus
#define FT_BASE( x )  extern "C"  x
#else
#define FT_BASE( x )  extern  x
#endif
#endif /* !FT_BASE */
#ifdef FT_MAKE_OPTION_SINGLE_OBJECT
#define FT_LOCAL( x )      static  x
#define FT_LOCAL_DEF( x )  static  x
#else
#ifdef __cplusplus
#define FT_LOCAL( x )      extern "C"  x
#define FT_LOCAL_DEF( x )  extern "C"  x
#else
#define FT_LOCAL( x )      extern  x
#define FT_LOCAL_DEF( x )  x
#endif
#endif /* FT_MAKE_OPTION_SINGLE_OBJECT */
#ifndef FT_BASE_DEF
#ifdef __cplusplus
#define FT_BASE_DEF( x )  x
#else
#define FT_BASE_DEF( x )  x
#endif
#endif /* !FT_BASE_DEF */
#ifndef FT_EXPORT_DEF
#ifdef __cplusplus
#define FT_EXPORT_DEF( x )  extern "C"  x
#else
#define FT_EXPORT_DEF( x )  extern  x
#endif
#endif /* !FT_EXPORT_DEF */
#ifndef FT_IMAGE_TAG
#define FT_IMAGE_TAG( value, _x1, _x2, _x3, _x4 )  \
		  value = ( ( (unsigned long)_x1 << 24 ) | \
					( (unsigned long)_x2 << 16 ) | \
					( (unsigned long)_x3 << 8  ) | \
					  (unsigned long)_x4         )
#endif /* FT_IMAGE_TAG */
#ifndef FT_ENC_TAG
#define FT_ENC_TAG( value, a, b, c, d )         \
		  value = ( ( (FT_UInt32)(a) << 24 ) |  \
					( (FT_UInt32)(b) << 16 ) |  \
					( (FT_UInt32)(c) <<  8 ) |  \
					  (FT_UInt32)(d)         )

#endif /* FT_ENC_TAG */
#ifndef FT_CHAR_BIT
#define FT_CHAR_BIT  CHAR_BIT
#endif
#define FT_UINT_MAX    UINT_MAX
#define FT_ULONG_MAX   ULONG_MAX
#define FT_INT_MAX     INT_MAX
#ifdef __cplusplus
extern "C++"
	template <typename T> inline T*
	cplusplus_typeof(        T*,
	void  *v )
{
	return static_cast <T*> ( v );
}
#define FT_ASSIGNP( p, val )  (p) = ft_defines_h::cplusplus_typeof( (p), (val) )
#else
#define FT_ASSIGNP( p, val )  (p) = (val)
#endif

#ifdef FT_DEBUG_MEMORY
FT_BASE( const char* )  _ft_debug_file;
FT_BASE( long )         _ft_debug_lineno;
#define FT_DEBUG_INNER( exp )  ( _ft_debug_file   = __FILE__, \
	_ft_debug_lineno = __LINE__, \
	(exp) )
#define FT_ASSIGNP_INNER( p, exp )  ( _ft_debug_file   = __FILE__, \
	_ft_debug_lineno = __LINE__, \
	FT_ASSIGNP( p, exp ) )
#else /* !FT_DEBUG_MEMORY */
#define FT_DEBUG_INNER( exp )       (exp)
#define FT_ASSIGNP_INNER( p, exp )  FT_ASSIGNP( p, exp )
#endif /* !FT_DEBUG_MEMORY */
#define FT_BEGIN_STMNT  do {
#define FT_END_STMNT    } while ( 0 )
#define FT_MEM_FREE( ptr )                \
	FT_BEGIN_STMNT                  \
	ft_mem_free( memory, (ptr) ); \
	(ptr) = NULL;                 \
	FT_END_STMNT
#define FT_FREE( ptr )  FT_MEM_FREE( ptr )
#define FT_MEM_ALLOC( ptr, size )                                         \
	FT_ASSIGNP_INNER( ptr, ft_mem_alloc( memory, (size), &error ) )
#define FT_MEM_SET_ERROR( cond )  ( (cond), error != 0 )
#define FT_ALLOC( ptr, size )                           \
	FT_MEM_SET_ERROR( FT_MEM_ALLOC( ptr, size ) )
#define ft_memchr   memchr
#define ft_memcmp   memcmp
#define ft_memcpy   memcpy
#define ft_memmove  memmove
#define ft_memset   memset
#define ft_strcat   strcat
#define ft_strcmp   strcmp
#define ft_strcpy   strcpy
#define ft_strlen   strlen
#define ft_strncmp  strncmp
#define ft_strncpy  strncpy
#define ft_strrchr  strrchr
#define ft_strstr   strstr
#define FT_MEM_SET( dest, byte, count )     ft_memset( dest, byte, count )
#define FT_MEM_ZERO( dest, count )  FT_MEM_SET( dest, 0, count )
#define FT_MEM_NEW( ptr )                        \
	FT_MEM_ALLOC( ptr, sizeof ( *(ptr) ) )
#define FT_NEW( ptr )  FT_MEM_SET_ERROR( FT_MEM_NEW( ptr ) )
#define FT_ZERO( p )                FT_MEM_ZERO( p, sizeof ( *(p) ) )
#define FT_MEM_QALLOC( ptr, size )                                         \
	FT_ASSIGNP_INNER( ptr, ft_mem_qalloc( memory, (size), &error ) )
#define FT_QALLOC( ptr, size )                           \
	FT_MEM_SET_ERROR( FT_MEM_QALLOC( ptr, size ) )
#define FT_MEM_COPY( dest, source, count )  ft_memcpy( dest, source, count )
#define FT_MEM_NEW_ARRAY( ptr, count )                                      \
	FT_ASSIGNP_INNER( ptr, ft_mem_realloc( memory, sizeof ( *(ptr) ), \
	0, (count),                \
	NULL, &error ) )
#define FT_NEW_ARRAY( ptr, count )                           \
	FT_MEM_SET_ERROR( FT_MEM_NEW_ARRAY( ptr, count ) )
#define FT_MEM_RENEW_ARRAY( ptr, cursz, newsz )                             \
	FT_ASSIGNP_INNER( ptr, ft_mem_realloc( memory, sizeof ( *(ptr) ), \
	(cursz), (newsz),          \
	(ptr), &error ) )
#define FT_RENEW_ARRAY( ptr, curcnt, newcnt )                           \
	FT_MEM_SET_ERROR( FT_MEM_RENEW_ARRAY( ptr, curcnt, newcnt ) )
#define FT_MEM_REALLOC( ptr, cursz, newsz )                        \
	FT_ASSIGNP_INNER( ptr, ft_mem_realloc( memory, 1,        \
	(cursz), (newsz), \
	(ptr), &error ) )
#define FT_REALLOC( ptr, cursz, newsz )                           \
	FT_MEM_SET_ERROR( FT_MEM_REALLOC( ptr, cursz, newsz ) )
#define FT_MEM_ALLOC_MULT( ptr, count, item_size )                    \
	FT_ASSIGNP_INNER( ptr, ft_mem_realloc( memory, (item_size), \
	0, (count),          \
	NULL, &error ) )
#define FT_ALLOC_MULT( ptr, count, item_size )                           \
	FT_MEM_SET_ERROR( FT_MEM_ALLOC_MULT( ptr, count, item_size ) )
#define FT_ARRAY_COPY( dest, source, count )                        \
	FT_MEM_COPY( dest, source, (count) * sizeof ( *(dest) ) )
#define FT_MEM_MOVE( dest, source, count )  ft_memmove( dest, source, count )
#define FT_ARRAY_MOVE( dest, source, count )                        \
		  FT_MEM_MOVE( dest, source, (count) * sizeof ( *(dest) ) )
#define FT_QNEW_ARRAY( ptr, count )                          \
	FT_MEM_SET_ERROR( FT_MEM_NEW_ARRAY( ptr, count ) )
#define FT_QRENEW_ARRAY( ptr, curcnt, newcnt )                          \
	FT_MEM_SET_ERROR( FT_MEM_RENEW_ARRAY( ptr, curcnt, newcnt ) )
#define FT_MODULE_FONT_DRIVER         1  /* this module is a font driver  */
#define FT_MODULE_RENDERER            2  /* this module is a renderer     */
#define FT_MODULE_HINTER              4  /* this module is a glyph hinter */
#define FT_MODULE_STYLER              8  /* this module is a styler       */
#define FT_MODULE_DRIVER_SCALABLE     0x100   /* the driver supports      */
											  /* scalable fonts           */
#define FT_MODULE_DRIVER_NO_OUTLINES  0x200   /* the driver does not      */
											  /* support vector outlines  */
#define FT_MODULE_DRIVER_HAS_HINTER   0x400   /* the driver provides its  */
											  /* own hinter               */
#define FT_GLYPH_OWN_BITMAP  0x1
#define FT_FACE_FLAG_SCALABLE          ( 1L <<  0 )
#define FT_FACE_FLAG_FIXED_SIZES       ( 1L <<  1 )
#define FT_FACE_FLAG_FIXED_WIDTH       ( 1L <<  2 )
#define FT_FACE_FLAG_SFNT              ( 1L <<  3 )
#define FT_FACE_FLAG_HORIZONTAL        ( 1L <<  4 )
#define FT_FACE_FLAG_VERTICAL          ( 1L <<  5 )
#define FT_FACE_FLAG_KERNING           ( 1L <<  6 )
#define FT_FACE_FLAG_FAST_GLYPHS       ( 1L <<  7 )
#define FT_FACE_FLAG_MULTIPLE_MASTERS  ( 1L <<  8 )
#define FT_FACE_FLAG_GLYPH_NAMES       ( 1L <<  9 )
#define FT_FACE_FLAG_EXTERNAL_STREAM   ( 1L << 10 )
#define FT_FACE_FLAG_HINTER            ( 1L << 11 )
#define FT_FACE_FLAG_CID_KEYED         ( 1L << 12 )
#define FT_FACE_FLAG_TRICKY            ( 1L << 13 )
#define FT_MIN( a, b )  ( (a) < (b) ? (a) : (b) )
#define FT_MAX( a, b )  ( (a) > (b) ? (a) : (b) )
#define FT_ABS( a )     ( (a) < 0 ? -(a) : (a) )
#define FT_PAD_FLOOR( x, n )  ( (x) & ~((n)-1) )
#define FT_PAD_ROUND( x, n )  FT_PAD_FLOOR( (x) + ((n)/2), n )
#define FT_PAD_CEIL( x, n )   FT_PAD_FLOOR( (x) + ((n)-1), n )
#define FT_PIX_FLOOR( x )     ( (x) & ~63 )
#define FT_PIX_ROUND( x )     FT_PIX_FLOOR( (x) + 32 )
#define FT_PIX_CEIL( x )      FT_PIX_FLOOR( (x) + 63 )
#if                                  FT_ULONG_MAX == 0xFFFFFFFFUL
#define FT_SIZEOF_LONG  (32 / FT_CHAR_BIT)
#elif FT_ULONG_MAX > 0xFFFFFFFFUL && FT_ULONG_MAX == 0xFFFFFFFFFFUL
#define FT_SIZEOF_LONG  (32 / FT_CHAR_BIT)
#elif FT_ULONG_MAX > 0xFFFFFFFFUL && FT_ULONG_MAX == 0xFFFFFFFFFFFFFFFFUL
#define FT_SIZEOF_LONG  (64 / FT_CHAR_BIT)
#else
#error "Unsupported size of `long' type!"
#endif
#if FT_SIZEOF_LONG == (64 / FT_CHAR_BIT)
/* FT_LONG64 must be defined if a 64-bit type is available */
#define FT_LONG64
#define FT_INT64  long
#elif defined( _MSC_VER ) && _MSC_VER >= 900  /* Visual C++ (and Intel C++) */
/* this compiler provides the __int64 type */
#define FT_LONG64
#define FT_INT64  __int64
#elif defined( __BORLANDC__ )  /* Borland C++ */
/* XXXX: We should probably check the value of __BORLANDC__ in order */
/*       to test the compiler version.                               */
/* this compiler provides the __int64 type */
#define FT_LONG64
#define FT_INT64  __int64
#elif defined( __WATCOMC__ )   /* Watcom C++ */
/* Watcom doesn't provide 64-bit data types */
#elif defined( __MWERKS__ )    /* Metrowerks CodeWarrior */
#define FT_LONG64
#define FT_INT64  long long int
#elif defined( __GNUC__ )
/* GCC provides the `long long' type */
#define FT_LONG64
#define FT_INT64  long long int
#endif /* FT_SIZEOF_LONG == (64 / FT_CHAR_BIT) */
/*************************************************************************/
/*                                                                       */
/* A 64-bit data type will create compilation problems if you compile    */
/* in strict ANSI mode.  To avoid them, we disable its use if __STDC__   */
/* is defined.  You can however ignore this rule by defining the         */
/* FT_CONFIG_OPTION_FORCE_INT64 configuration macro.                     */
/*                                                                       */
#if defined( FT_LONG64 ) && !defined( FT_CONFIG_OPTION_FORCE_INT64 )
#ifdef __STDC__
/* undefine the 64-bit macros in strict ANSI compilation mode */
#undef FT_LONG64
#undef FT_INT64
#endif /* __STDC__ */
#endif /* FT_LONG64 && !FT_CONFIG_OPTION_FORCE_INT64 */
#define FT_OPEN_MEMORY    0x1
#define FT_OPEN_STREAM    0x2
#define FT_OPEN_PATHNAME  0x4
#define FT_OPEN_DRIVER    0x8
#define FT_OPEN_PARAMS    0x10
#ifdef FT_DEBUG_LEVEL_ERROR
#define FT_ASSERT( condition )                                      \
	do                                                        \
					{                                                         \
					if ( !( condition ) )                                   \
					FT_Panic( "assertion failed on line %d of file %s\n", \
					__LINE__, __FILE__ );                       \
					} while ( 0 )
#else /* !FT_DEBUG_LEVEL_ERROR */
#define FT_ASSERT( condition )//  do { } while ( 0 )
#endif /* !FT_DEBUG_LEVEL_ERROR */
#define FT_ERROR_BASE( x )    ( (x) & 0xFF )
#define TT_PLATFORM_APPLE_UNICODE  0
#define TT_PLATFORM_MACINTOSH      1
#define TT_PLATFORM_ISO            2 /* deprecated */
#define TT_PLATFORM_MICROSOFT      3
#define TT_PLATFORM_CUSTOM         4
#define TT_PLATFORM_ADOBE          7 /* artificial */
#define TT_MS_ID_SYMBOL_CS    0
#define TT_MS_ID_UNICODE_CS   1
#define TT_MS_ID_SJIS         2
#define TT_MS_ID_GB2312       3
#define TT_MS_ID_BIG_5        4
#define TT_MS_ID_WANSUNG      5
#define TT_MS_ID_JOHAB        6
#define TT_MS_ID_UCS_4       10
#define TT_APPLE_ID_DEFAULT           0 /* Unicode 1.0 */
#define TT_APPLE_ID_UNICODE_1_1       1 /* specify Hangul at U+34xx */
#define TT_APPLE_ID_ISO_10646         2 /* deprecated */
#define TT_APPLE_ID_UNICODE_2_0       3 /* or later */
#define TT_APPLE_ID_UNICODE_32        4 /* 2.0 or later, full repertoire */
#define TT_APPLE_ID_VARIANT_SELECTOR  5 /* variation selector data */
#ifndef FT_CALLBACK_DEF
#ifdef __cplusplus
#define FT_CALLBACK_DEF( x )  extern "C"  x
#else
#define FT_CALLBACK_DEF( x )  static  x
#endif
#endif /* FT_CALLBACK_DEF */
#ifndef FT_CALLBACK_TABLE
#ifdef __cplusplus
#define FT_CALLBACK_TABLE      extern "C"
#define FT_CALLBACK_TABLE_DEF  extern "C"
#else
#define FT_CALLBACK_TABLE      extern
#define FT_CALLBACK_TABLE_DEF  /* nothing */
#endif
#endif /* FT_CALLBACK_TABLE */
#define FT_FRAME_OP_SHIFT         2
#define FT_FRAME_OP_SIGNED        1
#define FT_FRAME_OP_LITTLE        2
#define FT_FRAME_OP_COMMAND( x )  ( x >> FT_FRAME_OP_SHIFT )
#define FT_MAKE_FRAME_OP( command, little, sign ) \
	( ( command << FT_FRAME_OP_SHIFT ) | ( little << 1 ) | sign )
#define FT_FRAME_OP_END    0
#define FT_FRAME_OP_START  1  /* start a new frame     */
#define FT_FRAME_OP_BYTE   2  /* read 1-byte value     */
#define FT_FRAME_OP_SHORT  3  /* read 2-byte value     */
#define FT_FRAME_OP_LONG   4  /* read 4-byte value     */
#define FT_FRAME_OP_OFF3   5  /* read 3-byte value     */
#define FT_FRAME_OP_BYTES  6  /* read a bytes sequence */
#define FT_DEFINE_SERVICE( name )            \
	typedef struct FT_Service_ ## name ## Rec_ \
	FT_Service_ ## name ## Rec ;             \
	typedef struct FT_Service_ ## name ## Rec_ \
	const * FT_Service_ ## name ;            \
struct FT_Service_ ## name ## Rec_
#undef  FT_SET_ERROR
#define FT_SET_ERROR( expression ) \
	( ( error = (expression) ) != 0 )
#define WINFNT_MZ_MAGIC  0x5A4D
#define WINFNT_NE_MAGIC  0x454E
#define WINFNT_PE_MAGIC  0x4550
#define FT_BYTE_( p, i )  ( ((const ft_structs_h::FT_Byte*)(p))[(i)] )
#define FT_INT8_( p, i )  ( ((const ft_structs_h::FT_Char*)(p))[(i)] )
#define FT_INT16( x )   ( (ft_structs_h::FT_Int16)(x)  )
#define FT_UINT16( x )  ( (ft_structs_h::FT_UInt16)(x) )
#define FT_INT32( x )   ( (ft_structs_h::FT_Int32)(x)  )
#define FT_UINT32( x )  ( (ft_structs_h::FT_UInt32)(x) )
#define FT_BYTE_I16( p, i, s )  ( FT_INT16(  FT_BYTE_( p, i ) ) << (s) )
#define FT_BYTE_U16( p, i, s )  ( FT_UINT16( FT_BYTE_( p, i ) ) << (s) )
#define FT_BYTE_I32( p, i, s )  ( FT_INT32(  FT_BYTE_( p, i ) ) << (s) )
#define FT_BYTE_U32( p, i, s )  ( FT_UINT32( FT_BYTE_( p, i ) ) << (s) )
#define FT_INT8_I16( p, i, s )  ( FT_INT16(  FT_INT8_( p, i ) ) << (s) )
#define FT_INT8_U16( p, i, s )  ( FT_UINT16( FT_INT8_( p, i ) ) << (s) )
#define FT_INT8_I32( p, i, s )  ( FT_INT32(  FT_INT8_( p, i ) ) << (s) )
#define FT_INT8_U32( p, i, s )  ( FT_UINT32( FT_INT8_( p, i ) ) << (s) )
#define FT_PEEK_SHORT( p )  FT_INT16( FT_INT8_I16( p, 0, 8) | \
	FT_BYTE_I16( p, 1, 0) )
#define FT_PEEK_USHORT( p )  FT_UINT16( FT_BYTE_U16( p, 0, 8 ) | \
	FT_BYTE_U16( p, 1, 0 ) )
#define FT_PEEK_LONG( p )  FT_INT32( FT_INT8_I32( p, 0, 24 ) | \
	FT_BYTE_I32( p, 1, 16 ) | \
	FT_BYTE_I32( p, 2,  8 ) | \
	FT_BYTE_I32( p, 3,  0 ) )
#define FT_PEEK_ULONG( p )  FT_UINT32( FT_BYTE_U32( p, 0, 24 ) | \
	FT_BYTE_U32( p, 1, 16 ) | \
	FT_BYTE_U32( p, 2,  8 ) | \
	FT_BYTE_U32( p, 3,  0 ) )
#define FT_PEEK_OFF3( p )  FT_INT32( FT_INT8_I32( p, 0, 16 ) | \
	FT_BYTE_I32( p, 1,  8 ) | \
	FT_BYTE_I32( p, 2,  0 ) )
#define FT_PEEK_UOFF3( p )  FT_UINT32( FT_BYTE_U32( p, 0, 16 ) | \
	FT_BYTE_U32( p, 1,  8 ) | \
	FT_BYTE_U32( p, 2,  0 ) )
#define FT_PEEK_SHORT_LE( p )  FT_INT16( FT_INT8_I16( p, 1, 8 ) | \
	FT_BYTE_I16( p, 0, 0 ) )
#define FT_PEEK_USHORT_LE( p )  FT_UINT16( FT_BYTE_U16( p, 1, 8 ) |  \
	FT_BYTE_U16( p, 0, 0 ) )
#define FT_PEEK_LONG_LE( p )  FT_INT32( FT_INT8_I32( p, 3, 24 ) | \
	FT_BYTE_I32( p, 2, 16 ) | \
	FT_BYTE_I32( p, 1,  8 ) | \
	FT_BYTE_I32( p, 0,  0 ) )
#define FT_PEEK_ULONG_LE( p )  FT_UINT32( FT_BYTE_U32( p, 3, 24 ) | \
	FT_BYTE_U32( p, 2, 16 ) | \
	FT_BYTE_U32( p, 1,  8 ) | \
	FT_BYTE_U32( p, 0,  0 ) )
#define FT_PEEK_OFF3_LE( p )  FT_INT32( FT_INT8_I32( p, 2, 16 ) | \
	FT_BYTE_I32( p, 1,  8 ) | \
	FT_BYTE_I32( p, 0,  0 ) )
#define FT_PEEK_UOFF3_LE( p )  FT_UINT32( FT_BYTE_U32( p, 2, 16 ) | \
	FT_BYTE_U32( p, 1,  8 ) | \
	FT_BYTE_U32( p, 0,  0 ) )
#define FT_NEXT_CHAR( buffer )       \
	( (signed char)*buffer++ )
#define FT_NEXT_BYTE( buffer )         \
	( (unsigned char)*buffer++ )
#define FT_NEXT_SHORT( buffer )                                   \
	( (short)( buffer += 2, FT_PEEK_SHORT( buffer - 2 ) ) )
#define FT_NEXT_USHORT( buffer )                                            \
	( (unsigned short)( buffer += 2, FT_PEEK_USHORT( buffer - 2 ) ) )
#define FT_NEXT_OFF3( buffer )                                  \
	( (long)( buffer += 3, FT_PEEK_OFF3( buffer - 3 ) ) )
#define FT_NEXT_UOFF3( buffer )                                           \
	( (unsigned long)( buffer += 3, FT_PEEK_UOFF3( buffer - 3 ) ) )
#define FT_NEXT_LONG( buffer )                                  \
	( (long)( buffer += 4, FT_PEEK_LONG( buffer - 4 ) ) )
#define FT_NEXT_ULONG( buffer )                                           \
	( (unsigned long)( buffer += 4, FT_PEEK_ULONG( buffer - 4 ) ) )
#define FT_NEXT_SHORT_LE( buffer )                                   \
	( (short)( buffer += 2, FT_PEEK_SHORT_LE( buffer - 2 ) ) )
#define FT_NEXT_USHORT_LE( buffer )                                            \
	( (unsigned short)( buffer += 2, FT_PEEK_USHORT_LE( buffer - 2 ) ) )
#define FT_NEXT_OFF3_LE( buffer )                                  \
	( (long)( buffer += 3, FT_PEEK_OFF3_LE( buffer - 3 ) ) )
#define FT_NEXT_UOFF3_LE( buffer )                                           \
	( (unsigned long)( buffer += 3, FT_PEEK_UOFF3_LE( buffer - 3 ) ) )
#define FT_NEXT_LONG_LE( buffer )                                  \
	( (long)( buffer += 4, FT_PEEK_LONG_LE( buffer - 4 ) ) )
#define FT_NEXT_ULONG_LE( buffer )                                           \
	( (unsigned long)( buffer += 4, FT_PEEK_ULONG_LE( buffer - 4 ) ) )
#define FT_STYLE_FLAG_ITALIC  ( 1 << 0 )
#define FT_STYLE_FLAG_BOLD    ( 1 << 1 )
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DECLARE_MODULE(class_)                                            \
	FT_CALLBACK_TABLE                                                          \
	const FT_Module_Class  class_;                                             \

#define FT_DEFINE_ROOT_MODULE(flags_, size_, name_, version_, requires_,     \
	interface_, init_, done_, get_interface_)      \
	{                                                                          \
	flags_,                                                                  \
	size_,                                                                   \
	\
	name_,                                                                   \
	version_,                                                                \
	requires_,                                                               \
	\
	interface_,                                                              \
	\
	init_,                                                                   \
	done_,                                                                   \
	get_interface_,                                                          \
	},
#define FT_DEFINE_MODULE(class_, flags_, size_, name_, version_, requires_,  \
	interface_, init_, done_, get_interface_)           \
	FT_CALLBACK_TABLE_DEF                                                      \
	const ft_structs_h::FT_Module_Class class_ =                                             \
	{                                                                          \
	flags_,                                                                  \
	size_,                                                                   \
	\
	name_,                                                                   \
	version_,                                                                \
	requires_,                                                               \
	\
	interface_,                                                              \
	\
	init_,                                                                   \
	done_,                                                                   \
	get_interface_,                                                          \
	};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DECLARE_MODULE(class_)                                            \
	FT_Error FT_Create_Class_##class_( FT_Library library,                     \
	FT_Module_Class** output_class );       \
	void     FT_Destroy_Class_##class_( FT_Library library,                    \
	FT_Module_Class*  clazz );
#define FT_DEFINE_ROOT_MODULE(flags_, size_, name_, version_, requires_,     \
	interface_, init_, done_, get_interface_)      \
	clazz->root.module_flags       = flags_;                                 \
	clazz->root.module_size        = size_;                                  \
	clazz->root.module_name        = name_;                                  \
	clazz->root.module_version     = version_;                               \
	clazz->root.module_requires    = requires_;                              \
	\
	clazz->root.module_interface   = interface_;                             \
	\
	clazz->root.module_init        = init_;                                  \
	clazz->root.module_done        = done_;                                  \
	clazz->root.get_interface      = get_interface_;
#define FT_DEFINE_MODULE(class_, flags_, size_, name_, version_, requires_,  \
	interface_, init_, done_, get_interface_)           \
	\
	void                                                                       \
	FT_Destroy_Class_##class_( FT_Library library,                             \
	FT_Module_Class*  clazz )                       \
	{                                                                          \
	FT_Memory memory = library->memory;                                      \
	class_##_pic_free( library );                                            \
	if ( clazz )                                                             \
	FT_FREE( clazz );                                                      \
	}                                                                          \
	\
	FT_Error                                                                   \
	FT_Create_Class_##class_( FT_Library library,                              \
	FT_Module_Class**  output_class )                \
	{                                                                          \
	FT_Memory memory = library->memory;                                      \
	FT_Module_Class*  clazz;                                                 \
	FT_Error          error;                                                 \
	\
	if ( FT_ALLOC( clazz, sizeof(*clazz) ) )                                 \
	return error;                                                          \
	error = class_##_pic_init( library );                                    \
	if(error)                                                                \
		{                                                                        \
		FT_FREE( clazz );                                                      \
		return error;                                                          \
		}                                                                        \
		\
		clazz->module_flags       = flags_;                                      \
		clazz->module_size        = size_;                                       \
		clazz->module_name        = name_;                                       \
		clazz->module_version     = version_;                                    \
		clazz->module_requires    = requires_;                                   \
		\
		clazz->module_interface   = interface_;                                  \
		\
		clazz->module_init        = init_;                                       \
		clazz->module_done        = done_;                                       \
		clazz->get_interface      = get_interface_;                              \
		\
		*output_class = clazz;                                                   \
		return FT_Err_Ok;                                                        \
	}
#endif /* FT_CONFIG_OPTION_PIC */
#ifndef FT_CONFIG_OPTION_PIC
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
#define FT_DEFINE_DRIVERS_OLD_INTERNALS(a_,b_) \
	a_, b_,
#else
#define FT_DEFINE_DRIVERS_OLD_INTERNALS(a_,b_)
#endif
#define FT_DECLARE_DRIVER(class_)    \
	FT_CALLBACK_TABLE                  \
	const ft_structs_h::FT_Driver_ClassRec  class_;
#define FT_DEFINE_DRIVER(class_,                                             \
	flags_, size_, name_, version_, requires_,          \
	interface_, init_, done_, get_interface_,           \
	face_object_size_, size_object_size_,               \
	slot_object_size_, init_face_, done_face_,          \
	init_size_, done_size_, init_slot_, done_slot_,     \
	old_set_char_sizes_, old_set_pixel_sizes_,          \
	load_glyph_, get_kerning_, attach_file_,            \
	get_advances_, request_size_, select_size_ )        \
	FT_CALLBACK_TABLE_DEF                                                      \
	const ft_structs_h::FT_Driver_ClassRec class_ =                                          \
	{                                                                          \
	FT_DEFINE_ROOT_MODULE(flags_,size_,name_,version_,requires_,interface_,  \
	init_,done_,get_interface_)                        \
	\
	face_object_size_,                                                       \
	size_object_size_,                                                       \
	slot_object_size_,                                                       \
	\
	init_face_,                                                              \
	done_face_,                                                              \
	\
	init_size_,                                                              \
	done_size_,                                                              \
	\
	init_slot_,                                                              \
	done_slot_,                                                              \
	\
	FT_DEFINE_DRIVERS_OLD_INTERNALS(old_set_char_sizes_, old_set_pixel_sizes_) \
	\
	load_glyph_,                                                             \
	\
	get_kerning_,                                                            \
	attach_file_,                                                            \
	get_advances_,                                                           \
	\
	request_size_,                                                           \
	select_size_                                                             \
	};
#else /* FT_CONFIG_OPTION_PIC */
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
#define FT_DEFINE_DRIVERS_OLD_INTERNALS(a_,b_) \
	clazz->set_char_sizes = a_; \
	clazz->set_pixel_sizes = b_;
#else
#define FT_DEFINE_DRIVERS_OLD_INTERNALS(a_,b_)
#endif
#define FT_DECLARE_DRIVER(class_)    FT_DECLARE_MODULE(class_)
#define FT_DEFINE_DRIVER(class_,                                             \
	flags_, size_, name_, version_, requires_,          \
	interface_, init_, done_, get_interface_,           \
	face_object_size_, size_object_size_,               \
	slot_object_size_, init_face_, done_face_,          \
	init_size_, done_size_, init_slot_, done_slot_,     \
	old_set_char_sizes_, old_set_pixel_sizes_,          \
	load_glyph_, get_kerning_, attach_file_,            \
	get_advances_, request_size_, select_size_ )        \
	\
	void                                                                       \
	FT_Destroy_Class_##class_( FT_Library        library,                      \
	FT_Module_Class*  clazz )                       \
	{                                                                          \
	FT_Memory       memory = library->memory;                                \
	FT_Driver_Class dclazz = (FT_Driver_Class)clazz;                         \
	class_##_pic_free( library );                                            \
	if ( dclazz )                                                            \
	FT_FREE( dclazz );                                                     \
	}                                                                          \
	\
	FT_Error                                                                   \
	FT_Create_Class_##class_( FT_Library        library,                       \
	FT_Module_Class**  output_class )                \
	{                                                                          \
	FT_Driver_Class  clazz;                                                  \
	FT_Error         error;                                                  \
	FT_Memory        memory = library->memory;                               \
	\
	if ( FT_ALLOC( clazz, sizeof(*clazz) ) )                                 \
	return error;                                                          \
	\
	error = class_##_pic_init( library );                                    \
	if(error)                                                                \
		{                                                                        \
		FT_FREE( clazz );                                                      \
		return error;                                                          \
		}                                                                        \
		\
		FT_DEFINE_ROOT_MODULE(flags_,size_,name_,version_,requires_,interface_,  \
		init_,done_,get_interface_)                        \
		\
		clazz->face_object_size    = face_object_size_;                          \
		clazz->size_object_size    = size_object_size_;                          \
		clazz->slot_object_size    = slot_object_size_;                          \
		\
		clazz->init_face           = init_face_;                                 \
		clazz->done_face           = done_face_;                                 \
		\
		clazz->init_size           = init_size_;                                 \
		clazz->done_size           = done_size_;                                 \
		\
		clazz->init_slot           = init_slot_;                                 \
		clazz->done_slot           = done_slot_;                                 \
		\
		FT_DEFINE_DRIVERS_OLD_INTERNALS(old_set_char_sizes_, old_set_pixel_sizes_) \
		\
		clazz->load_glyph          = load_glyph_;                                \
		\
		clazz->get_kerning         = get_kerning_;                               \
		clazz->attach_file         = attach_file_;                               \
		clazz->get_advances        = get_advances_;                              \
		\
		clazz->request_size        = request_size_;                              \
		clazz->select_size         = select_size_;                               \
		\
		*output_class = (FT_Module_Class*)clazz;                                 \
		return FT_Err_Ok;                                                        \
	}
#endif /* FT_CONFIG_OPTION_PIC */
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DECLARE_RENDERER(class_)                                          \
	FT_EXPORT_VAR( const ft_structs_h::FT_Renderer_Class ) class_;
#define FT_DEFINE_RENDERER(class_,                                           \
	flags_, size_, name_, version_, requires_,        \
	interface_, init_, done_, get_interface_,         \
	glyph_format_, render_glyph_, transform_glyph_,   \
	get_glyph_cbox_, set_mode_, raster_class_ )       \
	FT_CALLBACK_TABLE_DEF                                                      \
	const ft_structs_h::FT_Renderer_Class  class_ =                                          \
	{                                                                          \
	FT_DEFINE_ROOT_MODULE(flags_,size_,name_,version_,requires_,             \
	interface_,init_,done_,get_interface_)             \
	glyph_format_,                                                           \
	\
	render_glyph_,                                                           \
	transform_glyph_,                                                        \
	get_glyph_cbox_,                                                         \
	set_mode_,                                                               \
	\
	raster_class_                                                            \
	};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DECLARE_RENDERER(class_)  FT_DECLARE_MODULE(class_)
#define FT_DEFINE_RENDERER(class_, \
	flags_, size_, name_, version_, requires_,        \
	interface_, init_, done_, get_interface_,         \
	glyph_format_, render_glyph_, transform_glyph_,   \
	get_glyph_cbox_, set_mode_, raster_class_ )       \
	\
	void                                                                       \
	FT_Destroy_Class_##class_( FT_Library        library,                      \
	FT_Module_Class*  clazz )                            \
	{                                                                          \
	FT_Renderer_Class* rclazz = (FT_Renderer_Class*)clazz;                   \
	FT_Memory         memory = library->memory;                              \
	class_##_pic_free( library );                                            \
	if ( rclazz )                                                            \
	FT_FREE( rclazz );                                                     \
	}                                                                          \
	\
	FT_Error                                                                   \
	FT_Create_Class_##class_( FT_Library         library,                      \
	FT_Module_Class**  output_class )                \
	{                                                                          \
	FT_Renderer_Class*  clazz;                                               \
	FT_Error            error;                                               \
	FT_Memory           memory = library->memory;                            \
	\
	if ( FT_ALLOC( clazz, sizeof(*clazz) ) )                                 \
	return error;                                                          \
	\
	error = class_##_pic_init( library );                                    \
	if(error)                                                                \
		{                                                                        \
		FT_FREE( clazz );                                                      \
		return error;                                                          \
		}                                                                        \
		\
		FT_DEFINE_ROOT_MODULE(flags_,size_,name_,version_,requires_,             \
		interface_,init_,done_,get_interface_)             \
		\
		clazz->glyph_format       = glyph_format_;                               \
		\
		clazz->render_glyph       = render_glyph_;                               \
		clazz->transform_glyph    = transform_glyph_;                            \
		clazz->get_glyph_cbox     = get_glyph_cbox_;                             \
		clazz->set_mode           = set_mode_;                                   \
		\
		clazz->raster_class       = raster_class_;                               \
		\
		*output_class = (FT_Module_Class*)clazz;                                 \
		return FT_Err_Ok;                                                        \
	}
#endif /* FT_CONFIG_OPTION_PIC */
#ifdef TT_USE_BYTECODE_INTERPRETER
#define TT_HINTER_FLAG   FT_MODULE_DRIVER_HAS_HINTER
#else
#define TT_HINTER_FLAG   0
#endif
#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
#define TT_SIZE_SELECT    tt_size_select
#else
#define TT_SIZE_SELECT    0
#endif
#define FT_LOAD_DEFAULT                      0x0
#define FT_LOAD_NO_SCALE                     ( 1L << 0 )
#define FT_LOAD_NO_HINTING                   ( 1L << 1 )
#define FT_LOAD_RENDER                       ( 1L << 2 )
#define FT_LOAD_NO_BITMAP                    ( 1L << 3 )
#define FT_LOAD_VERTICAL_LAYOUT              ( 1L << 4 )
#define FT_LOAD_FORCE_AUTOHINT               ( 1L << 5 )
#define FT_LOAD_CROP_BITMAP                  ( 1L << 6 )
#define FT_LOAD_PEDANTIC                     ( 1L << 7 )
#define FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH  ( 1L << 9 )
#define FT_LOAD_NO_RECURSE                   ( 1L << 10 )
#define FT_LOAD_IGNORE_TRANSFORM             ( 1L << 11 )
#define FT_LOAD_MONOCHROME                   ( 1L << 12 )
#define FT_LOAD_LINEAR_DESIGN                ( 1L << 13 )
#define FT_LOAD_NO_AUTOHINT                  ( 1L << 15 )
#define FT_IS_TRICKY( face ) \
	( face->face_flags & FT_FACE_FLAG_TRICKY )
#define FT_MAKE_TAG( _x1, _x2, _x3, _x4 ) \
	(ft_structs_h::FT_Tag)                        \
	( ( (ft_structs_h::FT_ULong)_x1 << 24 ) |     \
	( (ft_structs_h::FT_ULong)_x2 << 16 ) |     \
	( (ft_structs_h::FT_ULong)_x3 <<  8 ) |     \
	(ft_structs_h::FT_ULong)_x4         )
#define TTAG_avar  FT_MAKE_TAG( 'a', 'v', 'a', 'r' )
#define TTAG_BASE  FT_MAKE_TAG( 'B', 'A', 'S', 'E' )
#define TTAG_bdat  FT_MAKE_TAG( 'b', 'd', 'a', 't' )
#define TTAG_BDF   FT_MAKE_TAG( 'B', 'D', 'F', ' ' )
#define TTAG_bhed  FT_MAKE_TAG( 'b', 'h', 'e', 'd' )
#define TTAG_bloc  FT_MAKE_TAG( 'b', 'l', 'o', 'c' )
#define TTAG_bsln  FT_MAKE_TAG( 'b', 's', 'l', 'n' )
#define TTAG_CFF   FT_MAKE_TAG( 'C', 'F', 'F', ' ' )
#define TTAG_CID   FT_MAKE_TAG( 'C', 'I', 'D', ' ' )
#define TTAG_cmap  FT_MAKE_TAG( 'c', 'm', 'a', 'p' )
#define TTAG_cvar  FT_MAKE_TAG( 'c', 'v', 'a', 'r' )
#define TTAG_cvt   FT_MAKE_TAG( 'c', 'v', 't', ' ' )
#define TTAG_DSIG  FT_MAKE_TAG( 'D', 'S', 'I', 'G' )
#define TTAG_EBDT  FT_MAKE_TAG( 'E', 'B', 'D', 'T' )
#define TTAG_EBLC  FT_MAKE_TAG( 'E', 'B', 'L', 'C' )
#define TTAG_EBSC  FT_MAKE_TAG( 'E', 'B', 'S', 'C' )
#define TTAG_feat  FT_MAKE_TAG( 'f', 'e', 'a', 't' )
#define TTAG_FOND  FT_MAKE_TAG( 'F', 'O', 'N', 'D' )
#define TTAG_fpgm  FT_MAKE_TAG( 'f', 'p', 'g', 'm' )
#define TTAG_fvar  FT_MAKE_TAG( 'f', 'v', 'a', 'r' )
#define TTAG_gasp  FT_MAKE_TAG( 'g', 'a', 's', 'p' )
#define TTAG_GDEF  FT_MAKE_TAG( 'G', 'D', 'E', 'F' )
#define TTAG_glyf  FT_MAKE_TAG( 'g', 'l', 'y', 'f' )
#define TTAG_GPOS  FT_MAKE_TAG( 'G', 'P', 'O', 'S' )
#define TTAG_GSUB  FT_MAKE_TAG( 'G', 'S', 'U', 'B' )
#define TTAG_gvar  FT_MAKE_TAG( 'g', 'v', 'a', 'r' )
#define TTAG_hdmx  FT_MAKE_TAG( 'h', 'd', 'm', 'x' )
#define TTAG_head  FT_MAKE_TAG( 'h', 'e', 'a', 'd' )
#define TTAG_hhea  FT_MAKE_TAG( 'h', 'h', 'e', 'a' )
#define TTAG_hmtx  FT_MAKE_TAG( 'h', 'm', 't', 'x' )
#define TTAG_JSTF  FT_MAKE_TAG( 'J', 'S', 'T', 'F' )
#define TTAG_just  FT_MAKE_TAG( 'j', 'u', 's', 't' )
#define TTAG_kern  FT_MAKE_TAG( 'k', 'e', 'r', 'n' )
#define TTAG_lcar  FT_MAKE_TAG( 'l', 'c', 'a', 'r' )
#define TTAG_loca  FT_MAKE_TAG( 'l', 'o', 'c', 'a' )
#define TTAG_LTSH  FT_MAKE_TAG( 'L', 'T', 'S', 'H' )
#define TTAG_LWFN  FT_MAKE_TAG( 'L', 'W', 'F', 'N' )
#define TTAG_MATH  FT_MAKE_TAG( 'M', 'A', 'T', 'H' )
#define TTAG_maxp  FT_MAKE_TAG( 'm', 'a', 'x', 'p' )
#define TTAG_META  FT_MAKE_TAG( 'M', 'E', 'T', 'A' )
#define TTAG_MMFX  FT_MAKE_TAG( 'M', 'M', 'F', 'X' )
#define TTAG_MMSD  FT_MAKE_TAG( 'M', 'M', 'S', 'D' )
#define TTAG_mort  FT_MAKE_TAG( 'm', 'o', 'r', 't' )
#define TTAG_morx  FT_MAKE_TAG( 'm', 'o', 'r', 'x' )
#define TTAG_name  FT_MAKE_TAG( 'n', 'a', 'm', 'e' )
#define TTAG_opbd  FT_MAKE_TAG( 'o', 'p', 'b', 'd' )
#define TTAG_OS2   FT_MAKE_TAG( 'O', 'S', '/', '2' )
#define TTAG_OTTO  FT_MAKE_TAG( 'O', 'T', 'T', 'O' )
#define TTAG_PCLT  FT_MAKE_TAG( 'P', 'C', 'L', 'T' )
#define TTAG_POST  FT_MAKE_TAG( 'P', 'O', 'S', 'T' )
#define TTAG_post  FT_MAKE_TAG( 'p', 'o', 's', 't' )
#define TTAG_prep  FT_MAKE_TAG( 'p', 'r', 'e', 'p' )
#define TTAG_prop  FT_MAKE_TAG( 'p', 'r', 'o', 'p' )
#define TTAG_sfnt  FT_MAKE_TAG( 's', 'f', 'n', 't' )
#define TTAG_SING  FT_MAKE_TAG( 'S', 'I', 'N', 'G' )
#define TTAG_trak  FT_MAKE_TAG( 't', 'r', 'a', 'k' )
#define TTAG_true  FT_MAKE_TAG( 't', 'r', 'u', 'e' )
#define TTAG_ttc   FT_MAKE_TAG( 't', 't', 'c', ' ' )
#define TTAG_ttcf  FT_MAKE_TAG( 't', 't', 'c', 'f' )
#define TTAG_TYP1  FT_MAKE_TAG( 'T', 'Y', 'P', '1' )
#define TTAG_typ1  FT_MAKE_TAG( 't', 'y', 'p', '1' )
#define TTAG_VDMX  FT_MAKE_TAG( 'V', 'D', 'M', 'X' )
#define TTAG_vhea  FT_MAKE_TAG( 'v', 'h', 'e', 'a' )
#define TTAG_vmtx  FT_MAKE_TAG( 'v', 'm', 't', 'x' )
#define FT_PARAM_TAG_IGNORE_PREFERRED_FAMILY  FT_MAKE_TAG( 'i', 'g', 'p', 'f' )
#define FT_PARAM_TAG_IGNORE_PREFERRED_SUBFAMILY  FT_MAKE_TAG( 'i', 'g', 'p', 's' )
#define FT_OUTLINE_NONE             0x0
#define FT_OUTLINE_OWNER            0x1
#define FT_OUTLINE_EVEN_ODD_FILL    0x2
#define FT_OUTLINE_REVERSE_FILL     0x4
#define FT_OUTLINE_IGNORE_DROPOUTS  0x8
#define FT_OUTLINE_SMART_DROPOUTS   0x10
#define FT_OUTLINE_INCLUDE_STUBS    0x20
#define FT_OUTLINE_HIGH_PRECISION   0x100
#define FT_OUTLINE_SINGLE_PASS      0x200
#define USE_MY_METRICS             0x0200
#define ARGS_ARE_WORDS             0x0001
#define ARGS_ARE_XY_VALUES         0x0002
#define ROUND_XY_TO_GRID           0x0004
#define WE_HAVE_A_SCALE            0x0008
/* reserved                        0x0010 */
#define MORE_COMPONENTS            0x0020
#define WE_HAVE_AN_XY_SCALE        0x0040
#define WE_HAVE_A_2X2              0x0080
#define WE_HAVE_INSTR              0x0100
#define USE_MY_METRICS             0x0200
#define OVERLAP_COMPOUND           0x0400
#define SCALED_COMPONENT_OFFSET    0x0800
#define UNSCALED_COMPONENT_OFFSET  0x1000
#define FT_GLYPHLOADER_CHECK_P( _loader, _count )                         \
	( (_count) == 0 || ((_loader)->base.outline.n_points    +              \
	(_loader)->current.outline.n_points +              \
	(unsigned long)(_count)) <= (_loader)->max_points )
#define FT_GLYPHLOADER_CHECK_C( _loader, _count )                          \
	( (_count) == 0 || ((_loader)->base.outline.n_contours    +              \
	(_loader)->current.outline.n_contours +              \
	(unsigned long)(_count)) <= (_loader)->max_contours )
#define FT_GLYPHLOADER_CHECK_POINTS( _loader, _points,_contours )      \
	( ( FT_GLYPHLOADER_CHECK_P( _loader, _points )   &&                  \
	FT_GLYPHLOADER_CHECK_C( _loader, _contours ) )                   \
	? 0                                                                \
	: FT_GlyphLoader_CheckPoints( (_loader), (_points), (_contours) ) )
#define FT_USHORT_MAX  USHRT_MAX
#define FT_OUTLINE_CONTOURS_MAX  SHRT_MAX
#define FT_OUTLINE_POINTS_MAX    SHRT_MAX
#define FT_CURVE_TAG_TOUCH_X       8  /* reserved for the TrueType hinter */
#define FT_CURVE_TAG_TOUCH_Y      16  /* reserved for the TrueType hinter */
#define FT_CURVE_TAG_TOUCH_BOTH    ( FT_CURVE_TAG_TOUCH_X | \
									 FT_CURVE_TAG_TOUCH_Y )
#define FT_LOAD_SBITS_ONLY                   ( 1L << 14 )
#define FT_CURVE_TAG_ON            1
#undef  TT_KERN_INDEX
#define TT_KERN_INDEX( g1, g2 )  ( ( (ft_structs_h::FT_ULong)(g1) << 16 ) | (g2) )
#define TT_MAC_LANGID_ENGLISH                       0
#define TT_MAC_LANGID_FRENCH                        1
#define TT_MAC_LANGID_GERMAN                        2
#define TT_MAC_LANGID_ITALIAN                       3
#define TT_MAC_LANGID_DUTCH                         4
#define TT_MAC_LANGID_SWEDISH                       5
#define TT_MAC_LANGID_SPANISH                       6
#define TT_MAC_LANGID_DANISH                        7
#define TT_MAC_LANGID_PORTUGUESE                    8
#define TT_MAC_LANGID_NORWEGIAN                     9
#define TT_MAC_LANGID_HEBREW                       10
#define TT_MAC_LANGID_JAPANESE                     11
#define TT_MAC_LANGID_ARABIC                       12
#define TT_MAC_LANGID_FINNISH                      13
#define TT_MAC_LANGID_GREEK                        14
#define TT_MAC_LANGID_ICELANDIC                    15
#define TT_MAC_LANGID_MALTESE                      16
#define TT_MAC_LANGID_TURKISH                      17
#define TT_MAC_LANGID_CROATIAN                     18
#define TT_MAC_LANGID_CHINESE_TRADITIONAL          19
#define TT_MAC_LANGID_URDU                         20
#define TT_MAC_LANGID_HINDI                        21
#define TT_MAC_LANGID_THAI                         22
#define TT_MAC_LANGID_KOREAN                       23
#define TT_MAC_LANGID_LITHUANIAN                   24
#define TT_MAC_LANGID_POLISH                       25
#define TT_MAC_LANGID_HUNGARIAN                    26
#define TT_MAC_LANGID_ESTONIAN                     27
#define TT_MAC_LANGID_LETTISH                      28
#define TT_MAC_LANGID_SAAMISK                      29
#define TT_MAC_LANGID_FAEROESE                     30
#define TT_MAC_LANGID_FARSI                        31
#define TT_MAC_LANGID_RUSSIAN                      32
#define TT_MAC_LANGID_CHINESE_SIMPLIFIED           33
#define TT_MAC_LANGID_FLEMISH                      34
#define TT_MAC_LANGID_IRISH                        35
#define TT_MAC_LANGID_ALBANIAN                     36
#define TT_MAC_LANGID_ROMANIAN                     37
#define TT_MAC_LANGID_CZECH                        38
#define TT_MAC_LANGID_SLOVAK                       39
#define TT_MAC_LANGID_SLOVENIAN                    40
#define TT_MAC_LANGID_YIDDISH                      41
#define TT_MAC_LANGID_SERBIAN                      42
#define TT_MAC_LANGID_MACEDONIAN                   43
#define TT_MAC_LANGID_BULGARIAN                    44
#define TT_MAC_LANGID_UKRAINIAN                    45
#define TT_MAC_LANGID_BYELORUSSIAN                 46
#define TT_MAC_LANGID_UZBEK                        47
#define TT_MAC_LANGID_KAZAKH                       48
#define TT_MAC_LANGID_AZERBAIJANI                  49
#define TT_MAC_LANGID_AZERBAIJANI_CYRILLIC_SCRIPT  49
#define TT_MAC_LANGID_AZERBAIJANI_ARABIC_SCRIPT    50
#define TT_MAC_LANGID_ARMENIAN                     51
#define TT_MAC_LANGID_GEORGIAN                     52
#define TT_MAC_LANGID_MOLDAVIAN                    53
#define TT_MAC_LANGID_KIRGHIZ                      54
#define TT_MAC_LANGID_TAJIKI                       55
#define TT_MAC_LANGID_TURKMEN                      56
#define TT_MAC_LANGID_MONGOLIAN                    57
#define TT_MAC_LANGID_MONGOLIAN_MONGOLIAN_SCRIPT   57
#define TT_MAC_LANGID_MONGOLIAN_CYRILLIC_SCRIPT    58
#define TT_MAC_LANGID_PASHTO                       59
#define TT_MAC_LANGID_KURDISH                      60
#define TT_MAC_LANGID_KASHMIRI                     61
#define TT_MAC_LANGID_SINDHI                       62
#define TT_MAC_LANGID_TIBETAN                      63
#define TT_MAC_LANGID_NEPALI                       64
#define TT_MAC_LANGID_SANSKRIT                     65
#define TT_MAC_LANGID_MARATHI                      66
#define TT_MAC_LANGID_BENGALI                      67
#define TT_MAC_LANGID_ASSAMESE                     68
#define TT_MAC_LANGID_GUJARATI                     69
#define TT_MAC_LANGID_PUNJABI                      70
#define TT_MAC_LANGID_ORIYA                        71
#define TT_MAC_LANGID_MALAYALAM                    72
#define TT_MAC_LANGID_KANNADA                      73
#define TT_MAC_LANGID_TAMIL                        74
#define TT_MAC_LANGID_TELUGU                       75
#define TT_MAC_LANGID_SINHALESE                    76
#define TT_MAC_LANGID_BURMESE                      77
#define TT_MAC_LANGID_KHMER                        78
#define TT_MAC_LANGID_LAO                          79
#define TT_MAC_LANGID_VIETNAMESE                   80
#define TT_MAC_LANGID_INDONESIAN                   81
#define TT_MAC_LANGID_TAGALOG                      82
#define TT_MAC_LANGID_MALAY_ROMAN_SCRIPT           83
#define TT_MAC_LANGID_MALAY_ARABIC_SCRIPT          84
#define TT_MAC_LANGID_AMHARIC                      85
#define TT_MAC_LANGID_TIGRINYA                     86
#define TT_MAC_LANGID_GALLA                        87
#define TT_MAC_LANGID_SOMALI                       88
#define TT_MAC_LANGID_SWAHILI                      89
#define TT_MAC_LANGID_RUANDA                       90
#define TT_MAC_LANGID_RUNDI                        91
#define TT_MAC_LANGID_CHEWA                        92
#define TT_MAC_LANGID_MALAGASY                     93
#define TT_MAC_LANGID_ESPERANTO                    94
#define TT_MAC_LANGID_WELSH                       128
#define TT_MAC_LANGID_BASQUE                      129
#define TT_MAC_LANGID_CATALAN                     130
#define TT_MAC_LANGID_LATIN                       131
#define TT_MAC_LANGID_QUECHUA                     132
#define TT_MAC_LANGID_GUARANI                     133
#define TT_MAC_LANGID_AYMARA                      134
#define TT_MAC_LANGID_TATAR                       135
#define TT_MAC_LANGID_UIGHUR                      136
#define TT_MAC_LANGID_DZONGKHA                    137
#define TT_MAC_LANGID_JAVANESE                    138
#define TT_MAC_LANGID_SUNDANESE                   139
#define TT_MAC_ID_ROMAN                 0
#define TT_MAC_ID_JAPANESE              1
#define TT_MAC_ID_TRADITIONAL_CHINESE   2
#define TT_MAC_ID_KOREAN                3
#define TT_MAC_ID_ARABIC                4
#define TT_MAC_ID_HEBREW                5
#define TT_MAC_ID_GREEK                 6
#define TT_MAC_ID_RUSSIAN               7
#define TT_MAC_ID_RSYMBOL               8
#define TT_MAC_ID_DEVANAGARI            9
#define TT_MAC_ID_GURMUKHI             10
#define TT_MAC_ID_GUJARATI             11
#define TT_MAC_ID_ORIYA                12
#define TT_MAC_ID_BENGALI              13
#define TT_MAC_ID_TAMIL                14
#define TT_MAC_ID_TELUGU               15
#define TT_MAC_ID_KANNADA              16
#define TT_MAC_ID_MALAYALAM            17
#define TT_MAC_ID_SINHALESE            18
#define TT_MAC_ID_BURMESE              19
#define TT_MAC_ID_KHMER                20
#define TT_MAC_ID_THAI                 21
#define TT_MAC_ID_LAOTIAN              22
#define TT_MAC_ID_GEORGIAN             23
#define TT_MAC_ID_ARMENIAN             24
#define TT_MAC_ID_MALDIVIAN            25
#define TT_MAC_ID_SIMPLIFIED_CHINESE   25
#define TT_MAC_ID_TIBETAN              26
#define TT_MAC_ID_MONGOLIAN            27
#define TT_MAC_ID_GEEZ                 28
#define TT_MAC_ID_SLAVIC               29
#define TT_MAC_ID_VIETNAMESE           30
#define TT_MAC_ID_SINDHI               31
#define TT_MAC_ID_UNINTERP             32
#define TT_NAME_ID_COPYRIGHT            0
#define TT_NAME_ID_FONT_FAMILY          1
#define TT_NAME_ID_FONT_SUBFAMILY       2
#define TT_NAME_ID_UNIQUE_ID            3
#define TT_NAME_ID_FULL_NAME            4
#define TT_NAME_ID_VERSION_STRING       5
#define TT_NAME_ID_PS_NAME              6
#define TT_NAME_ID_TRADEMARK            7
#define TT_NAME_ID_MANUFACTURER         8
#define TT_NAME_ID_DESIGNER             9
#define TT_NAME_ID_DESCRIPTION          10
#define TT_NAME_ID_VENDOR_URL           11
#define TT_NAME_ID_DESIGNER_URL         12
#define TT_NAME_ID_LICENSE              13
#define TT_NAME_ID_LICENSE_URL          14
#define TT_NAME_ID_PREFERRED_FAMILY     16
#define TT_NAME_ID_PREFERRED_SUBFAMILY  17
#define TT_NAME_ID_MAC_FULL_NAME        18
#define TT_NAME_ID_SAMPLE_TEXT          19
#define TT_NAME_ID_CID_FINDFONT_NAME    20
#define TT_NAME_ID_WWS_FAMILY           21
#define TT_NAME_ID_WWS_SUBFAMILY        22
#define TT_PEEK_SHORT   FT_PEEK_SHORT
#define TT_PEEK_USHORT  FT_PEEK_USHORT
#define TT_PEEK_UINT24  FT_PEEK_UOFF3
#define TT_PEEK_LONG    FT_PEEK_LONG
#define TT_PEEK_ULONG   FT_PEEK_ULONG
#define TT_NEXT_SHORT   FT_NEXT_SHORT
#define TT_NEXT_USHORT  FT_NEXT_USHORT
#define TT_NEXT_UINT24  FT_NEXT_UOFF3
#define TT_NEXT_LONG    FT_NEXT_LONG
#define TT_NEXT_ULONG   FT_NEXT_ULONG
#define FT_RASTER_FLAG_DEFAULT  0x0
#define FT_RASTER_FLAG_AA       0x1
#define FT_RASTER_FLAG_DIRECT   0x2
#define FT_RASTER_FLAG_CLIP     0x4
#define FT_CURVE_TAG( flag )  ( flag & 3 )
#define FT_CURVE_TAG_ON            1
#define FT_CURVE_TAG_CONIC         0
#define FT_CURVE_TAG_CUBIC         2
#define FT_CURVE_TAG_HAS_SCANMODE  4
};

namespace ft_structs_h
{
#if                                 FT_UINT_MAX == 0xFFFFUL
#define FT_SIZEOF_INT  (16 / FT_CHAR_BIT)
#elif                               FT_UINT_MAX == 0xFFFFFFFFUL
#define FT_SIZEOF_INT  (32 / FT_CHAR_BIT)
#elif FT_UINT_MAX > 0xFFFFFFFFUL && FT_UINT_MAX == 0xFFFFFFFFFFFFFFFFUL
#define FT_SIZEOF_INT  (64 / FT_CHAR_BIT)
#else
#error "Unsupported size of `int' type!"
#endif	
#if FT_SIZEOF_INT == (32 / FT_CHAR_BIT)
	typedef signed int      FT_Int32;
	typedef unsigned int    FT_UInt32;
#elif FT_SIZEOF_LONG == (32 / FT_CHAR_BIT)
	typedef signed long     FT_Int32;
	typedef unsigned long   FT_UInt32;
#else
#error "no 32bit type found -- please check your configuration files"
#endif	
#ifdef FT_LONG64
	typedef FT_INT64  FT_Int64;
#else
	typedef struct  FT_Int64_
	{
		FT_UInt32  lo;
		FT_UInt32  hi;
	} FT_Int64;
#endif /* FT_LONG64 */
	#define ft_longjmp     longjmp
	#define ft_jmp_buf     jmp_buf  /* note: this cannot be a typedef since */
	#define ft_setjmp( b ) setjmp( *(jmp_buf*) &(b) )    /* same thing here */
	typedef struct FT_ValidatorRec_ volatile*  FT_Validator;
	typedef signed long  FT_F26Dot6;
	typedef int  FT_Error;
	typedef signed int  FT_Int;
	typedef unsigned int  FT_UInt;
	typedef unsigned long  FT_ULong;
	typedef signed long  FT_Long;
	typedef char  FT_String;
	typedef signed long  FT_Fixed;
	typedef void*  FT_Pointer;
	typedef signed short  FT_Short;
	//typedef FT_Fixed  FT_Angle;
	typedef FT_Pointer  FT_Module_Interface;
	typedef signed long  FT_Pos;
	typedef unsigned short  FT_UShort;
	typedef unsigned char  FT_Bool;
	typedef unsigned char  FT_Byte;
	typedef unsigned short  FT_UInt16;
	typedef signed short    FT_Int16;
	typedef signed char  FT_Char;
	typedef signed short  FT_F2Dot14;
typedef FT_UInt32  FT_Tag;
#if defined( __WINDOWS__ ) && defined( __x86__ )
#define ft_sfree     ::free
#define ft_smalloc   ::malloc
#else
#define ft_sfree     free
#define ft_smalloc   malloc
#endif
#define ft_strcmp   strcmp
#define FT_FILE     FILE
#define ft_fclose   fclose
#define ft_fopen    fopen
#define ft_fread    fread
#define ft_fseek    fseek
#define ft_ftell    ftell
#define ft_sprintf  sprintf
#define STREAM_FILE( stream )  ( (FT_FILE*)stream->descriptor.pointer )
#define FT_MAX_MODULES  32
typedef enum  FT_Pixel_Mode_
{
	FT_PIXEL_MODE_NONE = 0,
	FT_PIXEL_MODE_MONO,
	FT_PIXEL_MODE_GRAY,
	FT_PIXEL_MODE_GRAY2,
	FT_PIXEL_MODE_GRAY4,
	FT_PIXEL_MODE_LCD,
	FT_PIXEL_MODE_LCD_V,
	FT_PIXEL_MODE_MAX      /* do not remove */
} FT_Pixel_Mode;
typedef enum  FT_Render_Mode_
{
	FT_RENDER_MODE_NORMAL = 0,
	FT_RENDER_MODE_LIGHT,
	FT_RENDER_MODE_MONO,
	FT_RENDER_MODE_LCD,
	FT_RENDER_MODE_LCD_V,
	FT_RENDER_MODE_MAX
} FT_Render_Mode;
typedef enum  FT_Glyph_Format_
{
	FT_IMAGE_TAG( FT_GLYPH_FORMAT_NONE, 0, 0, 0, 0 ),
	FT_IMAGE_TAG( FT_GLYPH_FORMAT_COMPOSITE, 'c', 'o', 'm', 'p' ),
	FT_IMAGE_TAG( FT_GLYPH_FORMAT_BITMAP,    'b', 'i', 't', 's' ),
	FT_IMAGE_TAG( FT_GLYPH_FORMAT_OUTLINE,   'o', 'u', 't', 'l' ),
	FT_IMAGE_TAG( FT_GLYPH_FORMAT_PLOTTER,   'p', 'l', 'o', 't' )
} FT_Glyph_Format;
typedef enum  FT_Encoding_
{
	FT_ENC_TAG( FT_ENCODING_NONE, 0, 0, 0, 0 ),
	FT_ENC_TAG( FT_ENCODING_MS_SYMBOL, 's', 'y', 'm', 'b' ),
	FT_ENC_TAG( FT_ENCODING_UNICODE,   'u', 'n', 'i', 'c' ),
	FT_ENC_TAG( FT_ENCODING_SJIS,    's', 'j', 'i', 's' ),
	FT_ENC_TAG( FT_ENCODING_GB2312,  'g', 'b', ' ', ' ' ),
	FT_ENC_TAG( FT_ENCODING_BIG5,    'b', 'i', 'g', '5' ),
	FT_ENC_TAG( FT_ENCODING_WANSUNG, 'w', 'a', 'n', 's' ),
	FT_ENC_TAG( FT_ENCODING_JOHAB,   'j', 'o', 'h', 'a' ),
	/* for backwards compatibility */
	FT_ENCODING_MS_SJIS    = FT_ENCODING_SJIS,
	FT_ENCODING_MS_GB2312  = FT_ENCODING_GB2312,
	FT_ENCODING_MS_BIG5    = FT_ENCODING_BIG5,
	FT_ENCODING_MS_WANSUNG = FT_ENCODING_WANSUNG,
	FT_ENCODING_MS_JOHAB   = FT_ENCODING_JOHAB,
	FT_ENC_TAG( FT_ENCODING_ADOBE_STANDARD, 'A', 'D', 'O', 'B' ),
	FT_ENC_TAG( FT_ENCODING_ADOBE_EXPERT,   'A', 'D', 'B', 'E' ),
	FT_ENC_TAG( FT_ENCODING_ADOBE_CUSTOM,   'A', 'D', 'B', 'C' ),
	FT_ENC_TAG( FT_ENCODING_ADOBE_LATIN_1,  'l', 'a', 't', '1' ),
	FT_ENC_TAG( FT_ENCODING_OLD_LATIN_2, 'l', 'a', 't', '2' ),
	FT_ENC_TAG( FT_ENCODING_APPLE_ROMAN, 'a', 'r', 'm', 'n' )
} FT_Encoding;
typedef enum  FT_Size_Request_Type_
{
	FT_SIZE_REQUEST_TYPE_NOMINAL,
	FT_SIZE_REQUEST_TYPE_REAL_DIM,
	FT_SIZE_REQUEST_TYPE_BBOX,
	FT_SIZE_REQUEST_TYPE_CELL,
	FT_SIZE_REQUEST_TYPE_SCALES,
	FT_SIZE_REQUEST_TYPE_MAX
} FT_Size_Request_Type;

#define ft_ptrdiff_t  ptrdiff_t
typedef ft_ptrdiff_t  FT_PtrDist;
typedef struct FT_MemoryRec_*  FT_Memory;
typedef struct FT_ModuleRec_*  FT_Module;
typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FT_ListRec_*  FT_List;
typedef struct FT_ListNodeRec_*  FT_ListNode;
typedef struct FT_RendererRec_*  FT_Renderer;
typedef struct FT_GlyphSlotRec_*  FT_GlyphSlot;
typedef struct FT_FaceRec_*  FT_Face;
typedef struct FT_CharMapRec_*  FT_CharMap;
typedef struct FT_SizeRec_*  FT_Size;
typedef struct FT_DriverRec_*  FT_Driver;
typedef struct FT_StreamRec_*  FT_Stream;
typedef struct FT_Face_InternalRec_*  FT_Face_Internal;
typedef struct FT_Size_InternalRec_*  FT_Size_Internal;
typedef struct FT_Size_RequestRec_  *FT_Size_Request;
typedef struct FT_GlyphLoaderRec_*  FT_GlyphLoader;
typedef struct FT_SubGlyphRec_*  FT_SubGlyph;
typedef struct FT_Slot_InternalRec_*  FT_Slot_Internal;
typedef struct FT_RasterRec_*  FT_Raster;
typedef struct FT_Glyph_Class_  FT_Glyph_Class;
typedef struct FT_GlyphRec_*  FT_Glyph;
typedef struct FT_CMapRec_*              FT_CMap;
typedef struct FT_AutoHinterRec_  *FT_AutoHinter;
typedef const struct FT_CMap_ClassRec_*  FT_CMap_Class;
 typedef struct  FT_ServiceDescRec_
 {
	 const char*  serv_id;     /* service name         */
	 const void*  serv_data;   /* service pointer/data */
 } FT_ServiceDescRec;
typedef const FT_ServiceDescRec*  FT_ServiceDesc;





	typedef struct  FT_Parameter_
	{
		FT_ULong    tag;
		FT_Pointer  data;
	} FT_Parameter;
	typedef struct  FT_Vector_
	{
		FT_Pos  x;
		FT_Pos  y;
	} FT_Vector;
	typedef struct  FT_UnitVector_
	{
		FT_F2Dot14  x;
		FT_F2Dot14  y;
	} FT_UnitVector;
	typedef enum  FT_ValidationLevel_
	{
		FT_VALIDATE_DEFAULT = 0,
		FT_VALIDATE_TIGHT,
		FT_VALIDATE_PARANOID
	} FT_ValidationLevel;
	/* validator structure */
	typedef struct  FT_ValidatorRec_
	{
		const FT_Byte*      base;        /* address of table in memory       */
	const FT_Byte*      limit;       /* `base' + sizeof(table) in memory */
	FT_ValidationLevel  level;       /* validation level                 */
	FT_Error            error;       /* error returned. 0 means success  */
		ft_jmp_buf          jump_buffer; /* used for exception handling      */
	} FT_ValidatorRec;






	typedef void*
		(*FT_Alloc_Func)( FT_Memory  memory,
											long       size );
	typedef void
		(*FT_Free_Func)( FT_Memory  memory,
										 void*      block );
	 typedef void*
		(*FT_Realloc_Func)( FT_Memory  memory,
												long       cur_size,
												long       new_size,
												void*      block );
	 typedef FT_Error
		 (*FT_Face_InitFunc)( FT_Stream      stream,
		 FT_Face        face,
		 FT_Int         typeface_index,
		 FT_Int         num_params,
		 FT_Parameter*  parameters );
	 typedef void
		 (*FT_Face_DoneFunc)( FT_Face  face );
	 typedef FT_Error
		 (*FT_Size_InitFunc)( FT_Size  size );
	 typedef void
		 (*FT_Size_DoneFunc)( FT_Size  size );
	 typedef FT_Error
		 (*FT_Slot_InitFunc)( FT_GlyphSlot  slot );
	 typedef void
		 (*FT_Slot_DoneFunc)( FT_GlyphSlot  slot );
	 typedef FT_Error
		 (*FT_Size_RequestFunc)( FT_Size          size,
		 FT_Size_Request  req );
	 typedef FT_Error
		 (*FT_Size_SelectFunc)( FT_Size   size,
		 FT_ULong  size_index );
	 typedef FT_Error
		 (*FT_Slot_LoadFunc)( FT_GlyphSlot  slot,
		 FT_Size       size,
		 FT_UInt       glyph_index,
		 FT_Int32      load_flags );
	 typedef FT_Error
		 (*FT_Face_GetKerningFunc)( FT_Face     face,
		 FT_UInt     left_glyph,
		 FT_UInt     right_glyph,
		 FT_Vector*  kerning );
	 typedef FT_Error
		 (*FT_Face_AttachFunc)( FT_Face    face,
		 FT_Stream  stream );
	 typedef FT_Error
		 (*FT_Face_GetAdvancesFunc)( FT_Face    face,
		 FT_UInt    first,
		 FT_UInt    count,
		 FT_Int32   flags,
		 FT_Fixed*  advances );




	 struct  FT_MemoryRec_
	 {
		 void*            user;
		 FT_Alloc_Func    alloc;
		 FT_Free_Func     free;
		 FT_Realloc_Func  realloc;
	 };
	typedef struct  FT_Matrix_
	{
		FT_Fixed  xx, xy;
		FT_Fixed  yx, yy;
	} FT_Matrix;
	typedef struct  FT_BBox_
	{
		FT_Pos  xMin, yMin;
		FT_Pos  xMax, yMax;
	} FT_BBox;
	typedef struct  FT_Bitmap_Size_
	{
		FT_Short  height;
		FT_Short  width;
		FT_Pos    size;
		FT_Pos    x_ppem;
		FT_Pos    y_ppem;
	} FT_Bitmap_Size;


	typedef void  (*FT_Generic_Finalizer)(void*  object);


	typedef struct  FT_Generic_
	{
		void*                 data;
		FT_Generic_Finalizer  finalizer;
	} FT_Generic;
	typedef struct  FT_ListNodeRec_
	{
		FT_ListNode  prev;
		FT_ListNode  next;
		void*        data;
	} FT_ListNodeRec;
	typedef struct  FT_ListRec_
	{
		FT_ListNode  head;
		FT_ListNode  tail;
	} FT_ListRec;
	typedef struct  FT_Size_Metrics_
	{
		FT_UShort  x_ppem;      /* horizontal pixels per EM               */
		FT_UShort  y_ppem;      /* vertical pixels per EM                 */
		FT_Fixed   x_scale;     /* scaling values used to convert font    */
		FT_Fixed   y_scale;     /* units to 26.6 fractional pixels        */
		FT_Pos     ascender;    /* ascender in 26.6 frac. pixels          */
		FT_Pos     descender;   /* descender in 26.6 frac. pixels         */
		FT_Pos     height;      /* text height in 26.6 frac. pixels       */
		FT_Pos     max_advance; /* max horizontal advance, in 26.6 pixels */
	} FT_Size_Metrics;
	typedef struct  FT_Size_RequestRec_
	{
		FT_Size_Request_Type  type;
		FT_Long               width;
		FT_Long               height;
		FT_UInt               horiResolution;
		FT_UInt               vertResolution;
	} FT_Size_RequestRec;
	typedef FT_Error
		(*FT_Module_Constructor)( FT_Module  module );
	typedef void
		(*FT_Module_Destructor)( FT_Module  module );
	typedef FT_Module_Interface
		(*FT_Module_Requester)( FT_Module    module,
														const char*  name );
	typedef struct  FT_Module_Class_
	{
		FT_ULong               module_flags;
		FT_Long                module_size;
		const FT_String*       module_name;
		FT_Fixed               module_version;
		FT_Fixed               module_requires;
		const void*            module_interface;
		FT_Module_Constructor  module_init;
		FT_Module_Destructor   module_done;
		FT_Module_Requester    get_interface;
	} FT_Module_Class;
	typedef struct  FT_ModuleRec_
	{
		FT_Module_Class*  clazz;
		FT_Library        library;
		FT_Memory         memory;
	} FT_ModuleRec;
	typedef struct  FT_Driver_ClassRec_
	{
		FT_Module_Class           root;
		FT_Long                   face_object_size;
		FT_Long                   size_object_size;
		FT_Long                   slot_object_size;
		FT_Face_InitFunc          init_face;
		FT_Face_DoneFunc          done_face;
		FT_Size_InitFunc          init_size;
		FT_Size_DoneFunc          done_size;
		FT_Slot_InitFunc          init_slot;
		FT_Slot_DoneFunc          done_slot;
	#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
		FT_Size_ResetPointsFunc   set_char_sizes;
		FT_Size_ResetPixelsFunc   set_pixel_sizes;
	#endif /* FT_CONFIG_OPTION_OLD_INTERNALS */
		FT_Slot_LoadFunc          load_glyph;
		FT_Face_GetKerningFunc    get_kerning;
		FT_Face_AttachFunc        attach_file;
		FT_Face_GetAdvancesFunc   get_advances;
		/* since version 2.2 */
		FT_Size_RequestFunc       request_size;
		FT_Size_SelectFunc        select_size;
	} FT_Driver_ClassRec, *FT_Driver_Class;
	typedef struct  FT_Outline_
	{
		short       n_contours;      /* number of contours in glyph        */
		short       n_points;        /* number of points in the glyph      */
		FT_Vector*  points;          /* the outline's points               */
		char*       tags;            /* the points flags                   */
		short*      contours;        /* the contour end points             */
		int         flags;           /* outline masks                      */
	} FT_Outline;
	typedef struct  FT_GlyphLoadRec_
	{
		FT_Outline   outline;       /* outline                   */
		FT_Vector*   extra_points;  /* extra points table        */
		FT_Vector*   extra_points2; /* second extra points table */
		FT_UInt      num_subglyphs; /* number of subglyphs       */
		FT_SubGlyph  subglyphs;     /* subglyphs                 */
	} FT_GlyphLoadRec, *FT_GlyphLoad;
	typedef union  FT_StreamDesc_
	{
		long   value;
		void*  pointer;
	} FT_StreamDesc;
	typedef struct  FT_ServiceCacheRec_
	{
		FT_Pointer  service_POSTSCRIPT_FONT_NAME;
		FT_Pointer  service_MULTI_MASTERS;
		FT_Pointer  service_GLYPH_DICT;
		FT_Pointer  service_PFR_METRICS;
		FT_Pointer  service_WINFNT;
	} FT_ServiceCacheRec, *FT_ServiceCache;
	typedef struct  FT_Glyph_Metrics_
	{
		FT_Pos  width;
		FT_Pos  height;
		FT_Pos  horiBearingX;
		FT_Pos  horiBearingY;
		FT_Pos  horiAdvance;
		FT_Pos  vertBearingX;
		FT_Pos  vertBearingY;
		FT_Pos  vertAdvance;
	} FT_Glyph_Metrics;
	typedef struct  FT_Bitmap_
	{
		int             rows;
		int             width;
		int             pitch;
		unsigned char*  buffer;
		short           num_grays;
		char            pixel_mode;
		char            palette_mode;
		void*           palette;
	} FT_Bitmap;
	typedef struct  FT_Open_Args_
	{
		FT_UInt         flags;
		const FT_Byte*  memory_base;
		FT_Long         memory_size;
		FT_String*      pathname;
		FT_Stream       stream;
		FT_Module       driver;
		FT_Int          num_params;
		FT_Parameter*   params;
	} FT_Open_Args;
	typedef struct  FT_Span_
	{
		short           x;
		unsigned short  len;
		unsigned char   coverage;
	} FT_Span;
typedef enum  FT_Frame_Op_
{
	ft_frame_end       = 0,
	ft_frame_start     = FT_MAKE_FRAME_OP( FT_FRAME_OP_START, 0, 0 ),
	ft_frame_byte      = FT_MAKE_FRAME_OP( FT_FRAME_OP_BYTE,  0, 0 ),
	ft_frame_schar     = FT_MAKE_FRAME_OP( FT_FRAME_OP_BYTE,  0, 1 ),
	ft_frame_ushort_be = FT_MAKE_FRAME_OP( FT_FRAME_OP_SHORT, 0, 0 ),
	ft_frame_short_be  = FT_MAKE_FRAME_OP( FT_FRAME_OP_SHORT, 0, 1 ),
	ft_frame_ushort_le = FT_MAKE_FRAME_OP( FT_FRAME_OP_SHORT, 1, 0 ),
	ft_frame_short_le  = FT_MAKE_FRAME_OP( FT_FRAME_OP_SHORT, 1, 1 ),
	ft_frame_ulong_be  = FT_MAKE_FRAME_OP( FT_FRAME_OP_LONG, 0, 0 ),
	ft_frame_long_be   = FT_MAKE_FRAME_OP( FT_FRAME_OP_LONG, 0, 1 ),
	ft_frame_ulong_le  = FT_MAKE_FRAME_OP( FT_FRAME_OP_LONG, 1, 0 ),
	ft_frame_long_le   = FT_MAKE_FRAME_OP( FT_FRAME_OP_LONG, 1, 1 ),
	ft_frame_uoff3_be  = FT_MAKE_FRAME_OP( FT_FRAME_OP_OFF3, 0, 0 ),
	ft_frame_off3_be   = FT_MAKE_FRAME_OP( FT_FRAME_OP_OFF3, 0, 1 ),
	ft_frame_uoff3_le  = FT_MAKE_FRAME_OP( FT_FRAME_OP_OFF3, 1, 0 ),
	ft_frame_off3_le   = FT_MAKE_FRAME_OP( FT_FRAME_OP_OFF3, 1, 1 ),
	ft_frame_bytes     = FT_MAKE_FRAME_OP( FT_FRAME_OP_BYTES, 0, 0 ),
	ft_frame_skip      = FT_MAKE_FRAME_OP( FT_FRAME_OP_BYTES, 0, 1 )
} FT_Frame_Op;
	typedef struct  FT_Frame_Field_
	{
		FT_Byte    value;
		FT_Byte    size;
		FT_UShort  offset;
	} FT_Frame_Field;


	typedef void
		(*FT_SpanFunc)( int             y,
		int             count,
		const FT_Span*  spans,
		void*           user );
	typedef int
		(*FT_Raster_BitTest_Func)( int    y,
		int    x,
		void*  user );
	typedef void
		(*FT_Raster_BitSet_Func)( int    y,
		int    x,
		void*  user );


	typedef struct  FT_Raster_Params_
	{
		const FT_Bitmap*        target;
		const void*             source;
		int                     flags;
		FT_SpanFunc             gray_spans;
		FT_SpanFunc             black_spans;  /* doesn't work! */
		FT_Raster_BitTest_Func  bit_test;     /* doesn't work! */
		FT_Raster_BitSet_Func   bit_set;      /* doesn't work! */
		void*                   user;
		FT_BBox                 clip_box;
	} FT_Raster_Params;


	typedef int
		(*FT_Raster_NewFunc)( void*       memory,
		FT_Raster*  raster );
	typedef void
		(*FT_Raster_ResetFunc)( FT_Raster       raster,
		unsigned char*  pool_base,
		unsigned long   pool_size );
	typedef int
		(*FT_Raster_SetModeFunc)( FT_Raster      raster,
		unsigned long  mode,
		void*          args );
	typedef int
		(*FT_Raster_RenderFunc)( FT_Raster                raster,
		const FT_Raster_Params*  params );
	 typedef void
		 (*FT_Raster_DoneFunc)( FT_Raster  raster );



	typedef struct  FT_Raster_Funcs_
	{
		FT_Glyph_Format        glyph_format;
		FT_Raster_NewFunc      raster_new;
		FT_Raster_ResetFunc    raster_reset;
		FT_Raster_SetModeFunc  raster_set_mode;
		FT_Raster_RenderFunc   raster_render;
		FT_Raster_DoneFunc     raster_done;
	} FT_Raster_Funcs;



	typedef FT_Error
		(*FT_Renderer_RenderFunc)( FT_Renderer       renderer,
		FT_GlyphSlot      slot,
		FT_UInt           mode,
		const FT_Vector*  origin );
	typedef FT_Error
		(*FT_Renderer_TransformFunc)( FT_Renderer       renderer,
		FT_GlyphSlot      slot,
		const FT_Matrix*  matrix,
		const FT_Vector*  delta );
	typedef void
		(*FT_Renderer_GetCBoxFunc)( FT_Renderer   renderer,
		FT_GlyphSlot  slot,
		FT_BBox*      cbox );
	typedef FT_Error
		(*FT_Renderer_SetModeFunc)( FT_Renderer  renderer,
		FT_ULong     mode_tag,
		FT_Pointer   mode_ptr );
	typedef unsigned long
		(*FT_Stream_IoFunc)( FT_Stream       stream,
		unsigned long   offset,
		unsigned char*  buffer,
		unsigned long   count );
	typedef void
		(*FT_Stream_CloseFunc)( FT_Stream  stream );
	typedef FT_Error
		(*FT_Glyph_InitFunc)( FT_Glyph      glyph,
		FT_GlyphSlot  slot );
	/* destroys a given glyph object */
	typedef void
		(*FT_Glyph_DoneFunc)( FT_Glyph  glyph );
	typedef void
		(*FT_Glyph_TransformFunc)( FT_Glyph          glyph,
		const FT_Matrix*  matrix,
		const FT_Vector*  delta );
	typedef void
		(*FT_Glyph_GetBBoxFunc)( FT_Glyph  glyph,
		FT_BBox*  abbox );
	typedef FT_Error
		(*FT_Glyph_CopyFunc)( FT_Glyph   source,
		FT_Glyph   target );
	typedef FT_Error
		(*FT_Glyph_PrepareFunc)( FT_Glyph      glyph,
		FT_GlyphSlot  slot );
	typedef int
		(*FT_Raster_RenderFunc)( FT_Raster                raster,
		const FT_Raster_Params*  params );
	#define FT_Raster_Render_Func  FT_Raster_RenderFunc
	typedef void
		(*FT_DebugHook_Func)( void*  arg );
	typedef void
		(*FT_AutoHinter_GlobalResetFunc)( FT_AutoHinter  hinter,
		FT_Face        face );
	typedef void
		(*FT_AutoHinter_GlobalGetFunc)( FT_AutoHinter  hinter,
		FT_Face        face,
		void**         global_hints,
		long*          global_len );
	typedef void
		(*FT_AutoHinter_GlobalDoneFunc)( FT_AutoHinter  hinter,
		void*          global );
	 typedef FT_Error
		 (*FT_AutoHinter_GlyphLoadFunc)( FT_AutoHinter  hinter,
		 FT_GlyphSlot   slot,
		 FT_Size        size,
		 FT_UInt        glyph_index,
		 FT_Int32       load_flags );


	
	typedef struct  FT_AutoHinter_ServiceRec_
	{
		FT_AutoHinter_GlobalResetFunc  reset_face;
		FT_AutoHinter_GlobalGetFunc    get_global_hints;
		FT_AutoHinter_GlobalDoneFunc   done_global_hints;
		FT_AutoHinter_GlyphLoadFunc    load_glyph;
	} FT_AutoHinter_ServiceRec, *FT_AutoHinter_Service;
	typedef struct  FT_GlyphRec_
	{
		FT_Library             library;
		const FT_Glyph_Class*  clazz;
		FT_Glyph_Format        format;
		FT_Vector              advance;
	} FT_GlyphRec;
	typedef struct  FT_SubGlyphRec_
	{
		FT_Int     index;
		FT_UShort  flags;
		FT_Int     arg1;
		FT_Int     arg2;
		FT_Matrix  transform;
	} FT_SubGlyphRec;
	typedef struct  FT_GlyphLoaderRec_
	{
		FT_Memory        memory;
		FT_UInt          max_points;
		FT_UInt          max_contours;
		FT_UInt          max_subglyphs;
		FT_Bool          use_extra;
		FT_GlyphLoadRec  base;
		FT_GlyphLoadRec  current;
		void*            other;            /* for possible future extension? */
	} FT_GlyphLoaderRec;
	typedef struct  FT_FaceRec_
	{
		FT_Long           num_faces;
		FT_Long           face_index;
		FT_Long           face_flags;
		FT_Long           style_flags;
		FT_Long           num_glyphs;
		FT_String*        family_name;
		FT_String*        style_name;
		FT_Int            num_fixed_sizes;
		FT_Bitmap_Size*   available_sizes;
		FT_Int            num_charmaps;
		FT_CharMap*       charmaps;
		FT_Generic        generic;
		/*# The following member variables (down to `underline_thickness') */
		/*# are only relevant to scalable outlines; cf. @FT_Bitmap_Size    */
		/*# for bitmap fonts.                                              */
		FT_BBox           bbox;
		FT_UShort         units_per_EM;
		FT_Short          ascender;
		FT_Short          descender;
		FT_Short          height;
		FT_Short          max_advance_width;
		FT_Short          max_advance_height;
		FT_Short          underline_position;
		FT_Short          underline_thickness;
		FT_GlyphSlot      glyph;
		FT_Size           size;
		FT_CharMap        charmap;
		/*@private begin */
		FT_Driver         driver;
		FT_Memory         memory;
		FT_Stream         stream;
		FT_ListRec        sizes_list;
		FT_Generic        autohint;   /* face-specific auto-hinter data */
		void*             extensions; /* unused                         */
		FT_Face_Internal  internal;
		/*@private end */
	} FT_FaceRec;
	typedef struct  FT_CharMapRec_
	{
		FT_Face      face;
		FT_Encoding  encoding;
		FT_UShort    platform_id;
		FT_UShort    encoding_id;
	} FT_CharMapRec;
	typedef struct  FT_SizeRec_
	{
		FT_Face           face;      /* parent face object              */
		FT_Generic        generic;   /* generic pointer for client uses */
		FT_Size_Metrics   metrics;   /* size metrics                    */
		FT_Size_Internal  internal;
	} FT_SizeRec;
	typedef struct  FT_DriverRec_
	{
		FT_ModuleRec     root;
		FT_Driver_Class  clazz;
		FT_ListRec       faces_list;
		FT_GlyphLoader   glyph_loader;
	} FT_DriverRec;
	typedef struct  FT_StreamRec_
	{
		unsigned char*       base;
		unsigned long        size;
		unsigned long        pos;
		FT_StreamDesc        descriptor;
		FT_StreamDesc        pathname;
		FT_Stream_IoFunc     read;
		FT_Stream_CloseFunc  close;
		FT_Memory            memory;
		unsigned char*       cursor;
		unsigned char*       limit;
	} FT_StreamRec;
	typedef struct  FT_Face_InternalRec_
	{
	#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
		FT_UShort           reserved1;
		FT_Short            reserved2;
	#endif
		FT_Matrix           transform_matrix;
		FT_Vector           transform_delta;
		FT_Int              transform_flags;
		FT_ServiceCacheRec  services;
	#ifdef FT_CONFIG_OPTION_INCREMENTAL
		FT_Incremental_InterfaceRec*  incremental_interface;
	#endif
		FT_Bool             ignore_unpatented_hinter;
		FT_UInt             refcount;
	} FT_Face_InternalRec;
	typedef struct  FT_GlyphSlotRec_
	{
		FT_Library        library;
		FT_Face           face;
		FT_GlyphSlot      next;
		FT_UInt           reserved;       /* retained for binary compatibility */
		FT_Generic        generic;
		FT_Glyph_Metrics  metrics;
		FT_Fixed          linearHoriAdvance;
		FT_Fixed          linearVertAdvance;
		FT_Vector         advance;
		FT_Glyph_Format   format;
		FT_Bitmap         bitmap;
		FT_Int            bitmap_left;
		FT_Int            bitmap_top;
		FT_Outline        outline;
		FT_UInt           num_subglyphs;
		FT_SubGlyph       subglyphs;
		void*             control_data;
		long              control_len;
		FT_Pos            lsb_delta;
		FT_Pos            rsb_delta;
		void*             other;
		FT_Slot_Internal  internal;
	} FT_GlyphSlotRec;
	typedef struct  FT_Renderer_Class_
	{
		FT_Module_Class            root;
		FT_Glyph_Format            glyph_format;
		FT_Renderer_RenderFunc     render_glyph;
		FT_Renderer_TransformFunc  transform_glyph;
		FT_Renderer_GetCBoxFunc    get_glyph_cbox;
		FT_Renderer_SetModeFunc    set_mode;
		FT_Raster_Funcs*           raster_class;
	} FT_Renderer_Class;
	struct  FT_Glyph_Class_
	{
		FT_Long                 glyph_size;
		FT_Glyph_Format         glyph_format;
		FT_Glyph_InitFunc       glyph_init;
		FT_Glyph_DoneFunc       glyph_done;
		FT_Glyph_CopyFunc       glyph_copy;
		FT_Glyph_TransformFunc  glyph_transform;
		FT_Glyph_GetBBoxFunc    glyph_bbox;
		FT_Glyph_PrepareFunc    glyph_prepare;
	};
	typedef struct  FT_RendererRec_
	{
		FT_ModuleRec            root;
		FT_Renderer_Class*      clazz;
		FT_Glyph_Format         glyph_format;
		FT_Glyph_Class          glyph_class;
		FT_Raster               raster;
		FT_Raster_Render_Func   raster_render;
		FT_Renderer_RenderFunc  render;
	} FT_RendererRec;
	typedef struct  FT_LibraryRec_
	{
		FT_Memory          memory;           /* library's memory manager */

		FT_Int             version_major;
		FT_Int             version_minor;
		FT_Int             version_patch;

		FT_UInt            num_modules;
		FT_Module          modules[FT_MAX_MODULES];  /* module objects  */

		FT_ListRec         renderers;        /* list of renderers        */
		FT_Renderer        cur_renderer;     /* current outline renderer */
		FT_Module          auto_hinter;

		FT_Byte*           raster_pool;      /* scan-line conversion */
																					/* render pool          */
		FT_ULong           raster_pool_size; /* size of render pool in bytes */

		FT_DebugHook_Func  debug_hooks[4];

	#ifdef FT_CONFIG_OPTION_SUBPIXEL_RENDERING
		FT_LcdFilter             lcd_filter;
		FT_Int                   lcd_extra;        /* number of extra pixels */
		FT_Byte                  lcd_weights[7];   /* filter weights, if any */
		FT_Bitmap_LcdFilterFunc  lcd_filter_func;  /* filtering callback     */
	#endif

	#ifdef FT_CONFIG_OPTION_PIC
		FT_PIC_Container   pic_container;
	#endif

		FT_UInt            refcount;

	} FT_LibraryRec;
typedef struct  FT_Slot_InternalRec_
{
	FT_GlyphLoader  loader;
	FT_UInt         flags;
	FT_Bool         glyph_transformed;
	FT_Matrix       glyph_matrix;
	FT_Vector       glyph_delta;
	void*           glyph_hints;
} FT_GlyphSlot_InternalRec;
typedef FT_Error
	(*FT_CMap_InitFunc)( FT_CMap     cmap,
	FT_Pointer  init_data );
typedef void
	(*FT_CMap_DoneFunc)( FT_CMap  cmap );
typedef FT_UInt
	(*FT_CMap_CharIndexFunc)( FT_CMap    cmap,
	FT_UInt32  char_code );
typedef FT_UInt
	(*FT_CMap_CharNextFunc)( FT_CMap     cmap,
	FT_UInt32  *achar_code );
typedef FT_UInt
	(*FT_CMap_CharVarIndexFunc)( FT_CMap    cmap,
	FT_CMap    unicode_cmap,
	FT_UInt32  char_code,
	FT_UInt32  variant_selector );
typedef FT_Bool
	(*FT_CMap_CharVarIsDefaultFunc)( FT_CMap    cmap,
	FT_UInt32  char_code,
	FT_UInt32  variant_selector );
typedef FT_UInt32 *
	(*FT_CMap_VariantListFunc)( FT_CMap    cmap,
	FT_Memory  mem );
typedef FT_UInt32 *
	(*FT_CMap_CharVariantListFunc)( FT_CMap    cmap,
	FT_Memory  mem,
	FT_UInt32  char_code );
typedef FT_UInt32 *
	(*FT_CMap_VariantCharListFunc)( FT_CMap    cmap,
	FT_Memory  mem,
	FT_UInt32  variant_selector );
typedef int
	(*FT_Outline_MoveToFunc)( const FT_Vector*  to,
	void*             user );
#define FT_Outline_MoveTo_Func  FT_Outline_MoveToFunc
typedef int
	(*FT_Outline_LineToFunc)( const FT_Vector*  to,
	void*             user );
#define FT_Outline_LineTo_Func  FT_Outline_LineToFunc
typedef int
	(*FT_Outline_ConicToFunc)( const FT_Vector*  control,
	const FT_Vector*  to,
	void*             user );
#define FT_Outline_ConicTo_Func  FT_Outline_ConicToFunc
typedef int
	(*FT_Outline_CubicToFunc)( const FT_Vector*  control1,
	const FT_Vector*  control2,
	const FT_Vector*  to,
	void*             user );
#define FT_Outline_CubicTo_Func  FT_Outline_CubicToFunc
typedef struct  FT_Outline_Funcs_
{
	FT_Outline_MoveToFunc   move_to;
	FT_Outline_LineToFunc   line_to;
	FT_Outline_ConicToFunc  conic_to;
	FT_Outline_CubicToFunc  cubic_to;
	int                     shift;
	FT_Pos                  delta;
} FT_Outline_Funcs;
typedef struct  FT_CMap_ClassRec_
{
	FT_ULong               size;
	FT_CMap_InitFunc       init;
	FT_CMap_DoneFunc       done;
	FT_CMap_CharIndexFunc  char_index;
	FT_CMap_CharNextFunc   char_next;
	/* Subsequent entries are special ones for format 14 -- the variant */
	/* selector subtable which behaves like no other                    */
	FT_CMap_CharVarIndexFunc      char_var_index;
	FT_CMap_CharVarIsDefaultFunc  char_var_default;
	FT_CMap_VariantListFunc       variant_list;
	FT_CMap_CharVariantListFunc   charvariant_list;
	FT_CMap_VariantCharListFunc   variantchar_list;
} FT_CMap_ClassRec;
typedef struct  FT_CMapRec_
{
	FT_CharMapRec  charmap;
	FT_CMap_Class  clazz;
} FT_CMapRec;
typedef void
	(*FT_List_Destructor)( ft_structs_h::FT_Memory  memory,
	void*      data,
	void*      user );
#define FT_MODULE( x )          ((ft_structs_h::FT_Module)( x ))
#define FT_MODULE_CLASS( x )    FT_MODULE( x )->clazz
#define FT_MODULE_LIBRARY( x )  FT_MODULE( x )->library
#define FT_MODULE_MEMORY( x )   FT_MODULE( x )->memory
#define FT_MODULE_IS_DRIVER( x )  ( FT_MODULE_CLASS( x )->module_flags & \
									FT_MODULE_FONT_DRIVER )
#define FT_MODULE_IS_RENDERER( x )  ( FT_MODULE_CLASS( x )->module_flags & \
	FT_MODULE_RENDERER )
#define FT_MODULE_IS_HINTER( x )  ( FT_MODULE_CLASS( x )->module_flags & \
	FT_MODULE_HINTER )
#define FT_MODULE_IS_STYLER( x )  ( FT_MODULE_CLASS( x )->module_flags & \
	FT_MODULE_STYLER )
#define FT_DRIVER_IS_SCALABLE( x )  ( FT_MODULE_CLASS( x )->module_flags & \
	FT_MODULE_DRIVER_SCALABLE )
#define FT_DRIVER_USES_OUTLINES( x )  !( FT_MODULE_CLASS( x )->module_flags & \
	FT_MODULE_DRIVER_NO_OUTLINES )
#define FT_DRIVER_HAS_HINTER( x )  ( FT_MODULE_CLASS( x )->module_flags & \
	FT_MODULE_DRIVER_HAS_HINTER )
#define FT_DRIVER( x )        ((ft_structs_h::FT_Driver)(x))
	/* typecast a module as a driver, and get its driver class */
#define FT_DRIVER_CLASS( x )  FT_DRIVER( x )->clazz
#define FT_RENDERER( x )      ((ft_structs_h::FT_Renderer)( x ))
#define FT_GLYPH( x )         ((ft_structs_h::FT_Glyph)( x ))
#define FT_BITMAP_GLYPH( x )  ((ft_structs_h::FT_BitmapGlyph)( x ))
#define FT_OUTLINE_GLYPH( x ) ((ft_structs_h::FT_OutlineGlyph)( x ))
#define FT_FACE( x )          ((ft_structs_h::FT_Face)(x))
#define FT_SIZE( x )          ((ft_structs_h::FT_Size)(x))
#define FT_SLOT( x )          ((ft_structs_h::FT_GlyphSlot)(x))
#define FT_FACE_DRIVER( x )   FT_FACE( x )->driver
#define FT_FACE_LIBRARY( x )  FT_FACE_DRIVER( x )->root.library
#define FT_FACE_MEMORY( x )   FT_FACE( x )->memory
#define FT_FACE_STREAM( x )   FT_FACE( x )->stream
#define FT_SIZE_FACE( x )     FT_SIZE( x )->face
#define FT_SLOT_FACE( x )     FT_SLOT( x )->face
#define FT_FACE_SLOT( x )     FT_FACE( x )->glyph
#define FT_FACE_SIZE( x )     FT_FACE( x )->size
#define FT_CMAP( x )              ((ft_structs_h::FT_CMap)( x ))
#define FT_IS_SCALABLE( face ) \
	( face->face_flags & FT_FACE_FLAG_SCALABLE )
#define FT_HAS_FIXED_SIZES( face ) \
	( face->face_flags & FT_FACE_FLAG_FIXED_SIZES )
#define FT_REQUEST_WIDTH( req )                                            \
	( (req)->horiResolution                                          \
	? (ft_structs_h::FT_Pos)( (req)->width * (req)->horiResolution + 36 ) / 72 \
	: (req)->width )
#define FT_REQUEST_HEIGHT( req )                                            \
	( (req)->vertResolution                                           \
	? (ft_structs_h::FT_Pos)( (req)->height * (req)->vertResolution + 36 ) / 72 \
	: (req)->height )
#define FT_BOOL( x )  ( (ft_structs_h::FT_Bool)( x ) )
#define FT_VALIDATOR( x )  ((ft_structs_h::FT_Validator)( x ))
#define FT_HAS_VERTICAL( face ) \
	( face->face_flags & FT_FACE_FLAG_VERTICAL )
#define FT_FIELD_SIZE( f ) \
	(ft_structs_h::FT_Byte)sizeof ( ((FT_STRUCTURE*)0)->f )
#define FT_FIELD_SIZE_DELTA( f ) \
	(ft_structs_h::FT_Byte)sizeof ( ((FT_STRUCTURE*)0)->f[0] )
#define FT_FIELD_OFFSET( f ) \
	(ft_structs_h::FT_UShort)( offsetof( FT_STRUCTURE, f ) )
#define FT_FRAME_FIELD( frame_op, field ) \
{                               \
	ft_structs_h::frame_op,                     \
	FT_FIELD_SIZE( field ),       \
	FT_FIELD_OFFSET( field )      \
}
#define FT_MAKE_EMPTY_FIELD( frame_op )  { frame_op, 0, 0 }
#define FT_FRAME_START( size )   { ft_structs_h::ft_frame_start, 0, size }
#define FT_FRAME_END             { ft_structs_h::ft_frame_end, 0, 0 }
#define FT_FRAME_LONG( f )       FT_FRAME_FIELD( ft_frame_long_be, f )
#define FT_FRAME_ULONG( f )      FT_FRAME_FIELD( ft_frame_ulong_be, f )
#define FT_FRAME_SHORT( f )      FT_FRAME_FIELD( ft_frame_short_be, f )
#define FT_FRAME_USHORT( f )     FT_FRAME_FIELD( ft_frame_ushort_be, f )
#define FT_FRAME_OFF3( f )       FT_FRAME_FIELD( ft_frame_off3_be, f )
#define FT_FRAME_UOFF3( f )      FT_FRAME_FIELD( ft_frame_uoff3_be, f )
#define FT_FRAME_BYTE( f )       FT_FRAME_FIELD( ft_frame_byte, f )
#define FT_FRAME_CHAR( f )       FT_FRAME_FIELD( ft_frame_schar, f )
#define FT_FRAME_LONG_LE( f )    FT_FRAME_FIELD( ft_frame_long_le, f )
#define FT_FRAME_ULONG_LE( f )   FT_FRAME_FIELD( ft_frame_ulong_le, f )
#define FT_FRAME_SHORT_LE( f )   FT_FRAME_FIELD( ft_frame_short_le, f )
#define FT_FRAME_USHORT_LE( f )  FT_FRAME_FIELD( ft_frame_ushort_le, f )
#define FT_FRAME_OFF3_LE( f )    FT_FRAME_FIELD( ft_frame_off3_le, f )
#define FT_FRAME_UOFF3_LE( f )   FT_FRAME_FIELD( ft_frame_uoff3_le, f )
#define FT_FRAME_SKIP_LONG       { ft_frame_long_be, 0, 0 }
#define FT_FRAME_SKIP_SHORT      { ft_frame_short_be, 0, 0 }
#define FT_FRAME_SKIP_BYTE       { ft_frame_byte, 0, 0 }
#define FT_FRAME_BYTES( field, count ) \
{                            \
	ft_structs_h::ft_frame_bytes,            \
	count,                     \
	FT_FIELD_OFFSET( field )   \
}
#define FT_FRAME_SKIP_BYTES( count )  { ft_structs_h::ft_frame_skip, count, 0 }
#define FT_STREAM_POS()           \
	FT_Stream_Pos( stream )
#define FT_STREAM_SEEK( position )                           \
	FT_SET_ERROR( FT_Stream_Seek( stream, position ) )
#define FT_STREAM_SKIP( distance )                           \
	FT_SET_ERROR( FT_Stream_Skip( stream, distance ) )
#define FT_STREAM_READ( buffer, count )                   \
	FT_SET_ERROR( FT_Stream_Read( stream,           \
	(ft_structs_h::FT_Byte*)buffer, \
	count ) )
#define FT_STREAM_READ_AT( position, buffer, count )         \
	FT_SET_ERROR( FT_Stream_ReadAt( stream,            \
	position,         \
	(ft_structs_h::FT_Byte*)buffer, \
	count ) )
#define FT_STREAM_READ_FIELDS( fields, object )                          \
	FT_SET_ERROR( FT_Stream_ReadFields( stream, fields, object ) )
#define FT_FRAME_ENTER( size )                                       \
	FT_SET_ERROR(                                              \
	FT_DEBUG_INNER( FT_Stream_EnterFrame( stream, size ) ) )
#define FT_FRAME_EXIT()                 \
	FT_DEBUG_INNER( FT_Stream_ExitFrame( stream ) )
#define FT_STREAM_READ_FIELDS( fields, object )                          \
	FT_SET_ERROR( FT_Stream_ReadFields( stream, fields, object ) )
#define FT_FRAME_EXTRACT( size, bytes )                                       \
	FT_SET_ERROR(                                                       \
	FT_DEBUG_INNER( FT_Stream_ExtractFrame( stream, size,             \
	(ft_structs_h::FT_Byte**)&(bytes) ) ) )
#define FT_FRAME_RELEASE( bytes )                                         \
	FT_DEBUG_INNER( FT_Stream_ReleaseFrame( stream,                 \
	(ft_structs_h::FT_Byte**)&(bytes) ) )
#define FT_GET_MACRO( func, type )        ( (type)func( stream ) )
#define FT_GET_CHAR()       FT_GET_MACRO( FT_Stream_GetChar, ft_structs_h::FT_Char )
#define FT_GET_BYTE()       FT_GET_MACRO( FT_Stream_GetChar, ft_structs_h::FT_Byte )
#define FT_GET_SHORT()      FT_GET_MACRO( FT_Stream_GetUShort, ft_structs_h::FT_Short )
#define FT_GET_USHORT()     FT_GET_MACRO( FT_Stream_GetUShort, ft_structs_h::FT_UShort )
#define FT_GET_OFF3()       FT_GET_MACRO( FT_Stream_GetUOffset, ft_structs_h::FT_Long )
#define FT_GET_UOFF3()      FT_GET_MACRO( FT_Stream_GetUOffset, ft_structs_h::FT_ULong )
#define FT_GET_LONG()       FT_GET_MACRO( FT_Stream_GetULong, ft_structs_h::FT_Long )
#define FT_GET_ULONG()      FT_GET_MACRO( FT_Stream_GetULong, ft_structs_h::FT_ULong )
#define FT_GET_TAG4()       FT_GET_MACRO( FT_Stream_GetULong, ft_structs_h::FT_ULong )
#define FT_GET_SHORT_LE()   FT_GET_MACRO( FT_Stream_GetUShortLE, ft_structs_h::FT_Short )
#define FT_GET_USHORT_LE()  FT_GET_MACRO( FT_Stream_GetUShortLE, ft_structs_h::FT_UShort )
#define FT_GET_LONG_LE()    FT_GET_MACRO( FT_Stream_GetULongLE, ft_structs_h::FT_Long )
#define FT_GET_ULONG_LE()   FT_GET_MACRO( FT_Stream_GetULongLE, ft_structs_h::FT_ULong )
#define FT_CMAP( x )              ((ft_structs_h::FT_CMap)( x ))
#define FT_CMAP_PLATFORM_ID( x )  FT_CMAP( x )->charmap.platform_id
#define FT_CMAP_ENCODING_ID( x )  FT_CMAP( x )->charmap.encoding_id
#define FT_CMAP_ENCODING( x )     FT_CMAP( x )->charmap.encoding
#define FT_CMAP_FACE( x )         FT_CMAP( x )->charmap.face
#define FT_DEFINE_SERVICEDESCREC3( class_,                                  \
	serv_id_1, serv_data_1,                  \
	serv_id_2, serv_data_2,                  \
	serv_id_3, serv_data_3 )                 \
	static const ft_structs_h::FT_ServiceDescRec  class_[] =                                \
	{                                                                         \
		{ serv_id_1, serv_data_1 },                                             \
		{ serv_id_2, serv_data_2 },                                             \
		{ serv_id_3, serv_data_3 },                                             \
		{ NULL, NULL }                                                          \
	};
#define FT_GET_MACRO( func, type )        ( (type)func( stream ) )
#define FT_GET_CHAR()       FT_GET_MACRO( FT_Stream_GetChar, ft_structs_h::FT_Char )
#define FT_GET_BYTE()       FT_GET_MACRO( FT_Stream_GetChar, ft_structs_h::FT_Byte )
#define FT_GET_SHORT()      FT_GET_MACRO( FT_Stream_GetUShort, ft_structs_h::FT_Short )
#define FT_GET_USHORT()     FT_GET_MACRO( FT_Stream_GetUShort, ft_structs_h::FT_UShort )
#define FT_GET_OFF3()       FT_GET_MACRO( FT_Stream_GetUOffset, ft_structs_h::FT_Long )
#define FT_GET_UOFF3()      FT_GET_MACRO( FT_Stream_GetUOffset, ft_structs_h::FT_ULong )
#define FT_GET_LONG()       FT_GET_MACRO( FT_Stream_GetULong, ft_structs_h::FT_Long )
#define FT_GET_ULONG()      FT_GET_MACRO( FT_Stream_GetULong, ft_structs_h::FT_ULong )
#define FT_GET_TAG4()       FT_GET_MACRO( FT_Stream_GetULong, ft_structs_h::FT_ULong )
#define FT_GET_SHORT_LE()   FT_GET_MACRO( FT_Stream_GetUShortLE, ft_structs_h::FT_Short )
#define FT_GET_USHORT_LE()  FT_GET_MACRO( FT_Stream_GetUShortLE, ft_structs_h::FT_UShort )
#define FT_GET_LONG_LE()    FT_GET_MACRO( FT_Stream_GetULongLE, ft_structs_h::FT_Long )
#define FT_GET_ULONG_LE()   FT_GET_MACRO( FT_Stream_GetULongLE, ft_structs_h::FT_ULong )
#define FT_HAS_GLYPH_NAMES( face ) \
	( face->face_flags & FT_FACE_FLAG_GLYPH_NAMES )
#define FT_SERVICE_UNAVAILABLE  ((ft_structs_h::FT_Pointer)-2)  /* magic number */
#ifdef __cplusplus
#define FT_FACE_FIND_SERVICE( face, ptr, id )                               \
	FT_BEGIN_STMNT                                                            \
	ft_structs_h::FT_Module    module = FT_MODULE( FT_FACE( face )->driver );             \
	ft_structs_h::FT_Pointer   _tmp_  = NULL;                                             \
	ft_structs_h::FT_Pointer*  _pptr_ = (ft_structs_h::FT_Pointer*)&(ptr);                              \
	\
	\
	if ( module->clazz->get_interface )                                     \
	_tmp_ = module->clazz->get_interface( module, FT_SERVICE_ID_ ## id ); \
	*_pptr_ = _tmp_;                                                        \
	FT_END_STMNT
#else /* !C++ */
#define FT_FACE_FIND_SERVICE( face, ptr, id )                               \
	FT_BEGIN_STMNT                                                            \
	FT_Module   module = FT_MODULE( FT_FACE( face )->driver );              \
	FT_Pointer  _tmp_  = NULL;                                              \
	\
	if ( module->clazz->get_interface )                                     \
	_tmp_ = module->clazz->get_interface( module, FT_SERVICE_ID_ ## id ); \
	ptr = _tmp_;                                                            \
	FT_END_STMNT
#endif /* !C++ */
#ifdef __cplusplus
#define FT_FACE_LOOKUP_SERVICE( face, ptr, id )                \
	FT_BEGIN_STMNT                                               \
	ft_structs_h::FT_Pointer   svc;                                          \
	ft_structs_h::FT_Pointer*  Pptr = (ft_structs_h::FT_Pointer*)&(ptr);                   \
	\
	\
	svc = FT_FACE( face )->internal->services. service_ ## id; \
	if ( svc == FT_SERVICE_UNAVAILABLE )                       \
	svc = NULL;                                              \
		else if ( svc == NULL )                                    \
		{                                                          \
		FT_FACE_FIND_SERVICE( face, svc, id );                   \
		\
		FT_FACE( face )->internal->services. service_ ## id =    \
		(ft_structs_h::FT_Pointer)( svc != NULL ? svc                        \
		: FT_SERVICE_UNAVAILABLE );  \
		}                                                          \
		*Pptr = svc;                                               \
		FT_END_STMNT
#else /* !C++ */
#define FT_FACE_LOOKUP_SERVICE( face, ptr, id )                \
	FT_BEGIN_STMNT                                               \
	FT_Pointer  svc;                                           \
	\
	\
	svc = FT_FACE( face )->internal->services. service_ ## id; \
	if ( svc == FT_SERVICE_UNAVAILABLE )                       \
	svc = NULL;                                              \
		else if ( svc == NULL )                                    \
		{                                                          \
		FT_FACE_FIND_SERVICE( face, svc, id );                   \
		\
		FT_FACE( face )->internal->services. service_ ## id =    \
		(FT_Pointer)( svc != NULL ? svc                        \
		: FT_SERVICE_UNAVAILABLE );  \
		}                                                          \
		ptr = svc;                                                 \
		FT_END_STMNT
#endif /* !C++ */
#ifdef __cplusplus
#define FT_FACE_FIND_GLOBAL_SERVICE( face, ptr, id )               \
	FT_BEGIN_STMNT                                                   \
	ft_structs_h::FT_Module    module = FT_MODULE( FT_FACE( face )->driver );    \
	ft_structs_h::FT_Pointer   _tmp_;                                            \
	ft_structs_h::FT_Pointer*  _pptr_ = (ft_structs_h::FT_Pointer*)&(ptr);                     \
	\
	\
	_tmp_ = ft_module_get_service( module, FT_SERVICE_ID_ ## id ); \
	*_pptr_ = _tmp_;                                               \
  FT_END_STMNT
#else /* !C++ */
#define FT_FACE_FIND_GLOBAL_SERVICE( face, ptr, id )               \
	FT_BEGIN_STMNT                                                   \
	FT_Module   module = FT_MODULE( FT_FACE( face )->driver );     \
	FT_Pointer  _tmp_;                                             \
	\
	\
	_tmp_ = ft_module_get_service( module, FT_SERVICE_ID_ ## id ); \
	ptr   = _tmp_;                                                 \
	FT_END_STMNT
#endif /* !C++ */
#define FT_READ_MACRO( func, type, var )        \
	( var = (type)func( stream, &error ), \
	error != ft_error_h::FT_Err_Ok )
#define FT_READ_BYTE( var )       FT_READ_MACRO( FT_Stream_ReadChar, ft_structs_h::FT_Byte, var )
#define FT_READ_CHAR( var )       FT_READ_MACRO( FT_Stream_ReadChar, ft_structs_h::FT_Char, var )
#define FT_READ_SHORT( var )      FT_READ_MACRO( FT_Stream_ReadUShort, ft_structs_h::FT_Short, var )
#define FT_READ_USHORT( var )     FT_READ_MACRO( FT_Stream_ReadUShort, ft_structs_h::FT_UShort, var )
#define FT_READ_OFF3( var )       FT_READ_MACRO( FT_Stream_ReadUOffset, ft_structs_h::FT_Long, var )
#define FT_READ_UOFF3( var )      FT_READ_MACRO( FT_Stream_ReadUOffset, ft_structs_h::FT_ULong, var )
#define FT_READ_LONG( var )       FT_READ_MACRO( FT_Stream_ReadULong, ft_structs_h::FT_Long, var )
#define FT_READ_ULONG( var )      FT_READ_MACRO( FT_Stream_ReadULong, ft_structs_h::FT_ULong, var )
#define FT_READ_SHORT_LE( var )   FT_READ_MACRO( FT_Stream_ReadUShortLE, ft_structs_h::FT_Short, var )
#define FT_READ_USHORT_LE( var )  FT_READ_MACRO( FT_Stream_ReadUShortLE, ft_structs_h::FT_UShort, var )
#define FT_READ_LONG_LE( var )    FT_READ_MACRO( FT_Stream_ReadULongLE, ft_structs_h::FT_Long, var )
#define FT_READ_ULONG_LE( var )   FT_READ_MACRO( FT_Stream_ReadULongLE, ft_structs_h::FT_ULong, var )
#define TT_FACE_HAS_KERNING( face )  ( (face)->kern_avail_bits != 0 )
#define FT_LOAD_TARGET_MODE( x )  ( (ft_structs_h::FT_Render_Mode)( ( (x) >> 16 ) & 15 ) )
#define FT_IS_SFNT( face ) \
	( face->face_flags & FT_FACE_FLAG_SFNT )
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DEFINE_OUTLINE_FUNCS(class_, move_to_, line_to_, conic_to_,       \
	cubic_to_, shift_, delta_)                   \
	static const ft_structs_h::FT_Outline_Funcs class_ =                                     \
	{                                                                          \
	move_to_, line_to_, conic_to_, cubic_to_, shift_, delta_                 \
	};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DEFINE_OUTLINE_FUNCS(class_, move_to_, line_to_, conic_to_,       \
	cubic_to_, shift_, delta_)                   \
	static FT_Error                                                            \
	Init_Class_##class_( FT_Outline_Funcs*  clazz )                            \
	{                                                                          \
	clazz->move_to = move_to_;                                               \
	clazz->line_to = line_to_;                                               \
	clazz->conic_to = conic_to_;                                             \
	clazz->cubic_to = cubic_to_;                                             \
	clazz->shift = shift_;                                                   \
	clazz->delta = delta_;                                                   \
	return FT_Err_Ok;                                                        \
	}
#endif /* FT_CONFIG_OPTION_PIC */
#define FT_INVALID( _prefix, _error )  FT_INVALID_( _prefix, _error )
#define FT_INVALID_( _prefix, _error ) \
	ft_validator_error( valid, ft_error_h::_prefix ## _error )
/* called when a broken table is detected */
#define FT_INVALID_TOO_SHORT \
	FT_INVALID( FT_ERR_PREFIX, Invalid_Table )
#define FT_INVALID_GLYPH_ID \
	FT_INVALID( FT_ERR_PREFIX, Invalid_Glyph_Index )
#define FT_INVALID_DATA \
	FT_INVALID( FT_ERR_PREFIX, Invalid_Table )
#define FT_INVALID_OFFSET \
	FT_INVALID( FT_ERR_PREFIX, Invalid_Offset )
};
namespace ft_debug_h
{
#ifdef FT_DEBUG_LEVEL_TRACE
#define FT_TRACE( level, varformat )                      \
	do                                              \
	{                                               \
	if ( ft_trace_levels[FT_COMPONENT] >= level ) \
	FT_Message varformat;                       \
} while ( 0 )
#else /* !FT_DEBUG_LEVEL_TRACE */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
#define FT_TRACE( level, varformat )
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
#endif /* !FT_DEBUG_LEVEL_TRACE */
#define FT_TRACE0( varformat )  FT_TRACE( 0, varformat )
#define FT_TRACE1( varformat )  FT_TRACE( 1, varformat )
#define FT_TRACE2( varformat )  FT_TRACE( 2, varformat )
#define FT_TRACE3( varformat )  FT_TRACE( 3, varformat )
#define FT_TRACE4( varformat )  FT_TRACE( 4, varformat )
#define FT_TRACE5( varformat )  FT_TRACE( 5, varformat )
#define FT_TRACE6( varformat )  FT_TRACE( 6, varformat )
#define FT_TRACE7( varformat )  FT_TRACE( 7, varformat )
};
#define FT_SERVICE_ID_GLYPH_DICT  "glyph-dict"
typedef ft_structs_h::FT_Error
	(*FT_GlyphDict_GetNameFunc)( ft_structs_h::FT_Face     face,
	ft_structs_h::FT_UInt     glyph_index,
	ft_structs_h::FT_Pointer  buffer,
	ft_structs_h::FT_UInt     buffer_max );
typedef ft_structs_h::FT_UInt
	(*FT_GlyphDict_NameIndexFunc)( ft_structs_h::FT_Face     face,
	ft_structs_h::FT_String*  glyph_name );
FT_DEFINE_SERVICE( GlyphDict )
{
	FT_GlyphDict_GetNameFunc    get_name;
	FT_GlyphDict_NameIndexFunc  name_index;  /* optional */
};
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DEFINE_SERVICE_GLYPHDICTREC( class_,                        \
	get_name_,                     \
	name_index_)                   \
	static const FT_Service_GlyphDictRec  class_ =                       \
{                                                                    \
	get_name_, name_index_                                             \
};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DEFINE_SERVICE_GLYPHDICTREC( class_,                        \
	get_name_,                     \
	name_index_)                   \
	void                                                                 \
	FT_Init_Class_ ## class_( FT_Library                library,         \
	FT_Service_GlyphDictRec*  clazz )          \
{                                                                    \
	FT_UNUSED( library );                                              \
	\
	clazz->get_name   = get_name_;                                     \
	clazz->name_index = name_index_;                                   \
}
#endif /* FT_CONFIG_OPTION_PIC */



#undef  FT_USE_MODULE
#ifdef __cplusplus
#define FT_USE_MODULE( type, x )  extern "C" const type  x;
#else
#define FT_USE_MODULE( type, x )  extern const type  x;
#endif

//FT_USE_MODULE( ft_structs_h::FT_Module_Class, autofit_module_class )
FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, tt_driver_class )
//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, t1_driver_class )
//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, cff_driver_class )
//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, t1cid_driver_class )
//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, pfr_driver_class )
//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, t42_driver_class )
FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, winfnt_driver_class )
//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, pcf_driver_class )
//FT_USE_MODULE( ft_structs_h::FT_Module_Class, psaux_module_class )
//FT_USE_MODULE( ft_structs_h::FT_Module_Class, psnames_module_class )
//FT_USE_MODULE( ft_structs_h::FT_Module_Class, pshinter_module_class )
//FT_USE_MODULE( ft_structs_h::FT_Renderer_Class, ft_raster1_renderer_class )
FT_USE_MODULE( ft_structs_h::FT_Module_Class, sfnt_module_class )
FT_USE_MODULE( ft_structs_h::FT_Renderer_Class, ft_smooth_renderer_class )
//FT_USE_MODULE( ft_structs_h::FT_Renderer_Class, ft_smooth_lcd_renderer_class )
//FT_USE_MODULE( ft_structs_h::FT_Renderer_Class, ft_smooth_lcdv_renderer_class )
//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, bdf_driver_class )

#undef  FT_USE_MODULE
#define FT_USE_MODULE( type, x )  (const ft_structs_h::FT_Module_Class*)&(x),

static
const ft_structs_h::FT_Module_Class*  const ft_default_modules[] =
{
	//FT_USE_MODULE( ft_structs_h::FT_Module_Class, autofit_module_class )
	FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, tt_driver_class )
	//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, t1_driver_class )
	//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, cff_driver_class )
	//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, t1cid_driver_class )
	//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, pfr_driver_class )
	//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, t42_driver_class )
	FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, winfnt_driver_class )
	//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, pcf_driver_class )
	//FT_USE_MODULE( ft_structs_h::FT_Module_Class, psaux_module_class )
	//FT_USE_MODULE( ft_structs_h::FT_Module_Class, psnames_module_class )
	//FT_USE_MODULE( ft_structs_h::FT_Module_Class, pshinter_module_class )
	//FT_USE_MODULE( ft_structs_h::FT_Renderer_Class, ft_raster1_renderer_class )
	FT_USE_MODULE( ft_structs_h::FT_Module_Class, sfnt_module_class )
	FT_USE_MODULE( ft_structs_h::FT_Renderer_Class, ft_smooth_renderer_class )
	//FT_USE_MODULE( ft_structs_h::FT_Renderer_Class, ft_smooth_lcd_renderer_class )
	//FT_USE_MODULE( ft_structs_h::FT_Renderer_Class, ft_smooth_lcdv_renderer_class )
	//FT_USE_MODULE( ft_structs_h::FT_Driver_ClassRec, bdf_driver_class )
	0
};

#ifndef FT_CONFIG_OPTION_PIC
#define FT_OUTLINE_GLYPH_CLASS_GET &ft_outline_glyph_class
#define FT_BITMAP_GLYPH_CLASS_GET  &ft_bitmap_glyph_class
#define FT_DEFAULT_MODULES_GET     ft_default_modules
#ifdef FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK
#define FT_RACCESS_GUESS_TABLE_GET ft_raccess_guess_table
#endif
#else /* FT_CONFIG_OPTION_PIC */
#include FT_GLYPH_H
#ifdef FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK
#include FT_INTERNAL_RFORK_H
#endif
typedef struct BasePIC_
{
	FT_Module_Class** default_module_classes;
	FT_Glyph_Class ft_outline_glyph_class;
	FT_Glyph_Class ft_bitmap_glyph_class;
#ifdef FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK
	ft_raccess_guess_rec ft_raccess_guess_table[FT_RACCESS_N_RULES];
#endif
} BasePIC;
#define GET_PIC(lib)                  ((BasePIC*)((lib)->pic_container.base))
#define FT_OUTLINE_GLYPH_CLASS_GET    (&GET_PIC(library)->ft_outline_glyph_class)
#define FT_BITMAP_GLYPH_CLASS_GET     (&GET_PIC(library)->ft_bitmap_glyph_class)
#define FT_DEFAULT_MODULES_GET        (GET_PIC(library)->default_module_classes)
#ifdef FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK
#define FT_RACCESS_GUESS_TABLE_GET    (GET_PIC(library)->ft_raccess_guess_table)
#endif
/* see basepic.c for the implementation. */
void
	ft_base_pic_free( FT_Library  library );
FT_Error
	ft_base_pic_init( FT_Library  library );
#endif /* FT_CONFIG_OPTION_PIC */

FT_BASE( void )
	ft_validator_error( ft_structs_h::FT_Validator  valid,
	ft_structs_h::FT_Error      error );
FT_BASE( void )
	ft_validator_init( ft_structs_h::FT_Validator        valid,
	const ft_structs_h::FT_Byte*      base,
	const ft_structs_h::FT_Byte*      limit,
	ft_structs_h::FT_ValidationLevel  level );
FT_BASE( ft_structs_h::FT_Error )
	FT_Stream_Read( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Byte*   buffer,
	ft_structs_h::FT_ULong   count );
FT_BASE( ft_structs_h::FT_UShort )
	FT_Stream_GetUShort( ft_structs_h::FT_Stream  stream );
FT_BASE( ft_structs_h::FT_UShort )
	FT_Stream_ReadUShort( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Error*  error );
FT_BASE( ft_structs_h::FT_ULong )
	FT_Stream_ReadULong( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Error*  error );
FT_BASE( ft_structs_h::FT_Pointer )
	ft_module_get_service( ft_structs_h::FT_Module    module,
	const char*  service_id );
FT_BASE( ft_structs_h::FT_Error )
	FT_GlyphLoader_CheckSubGlyphs( ft_structs_h::FT_GlyphLoader  loader,
	ft_structs_h::FT_UInt         n_subs );
FT_BASE( ft_structs_h::FT_Char )
	FT_Stream_GetChar( ft_structs_h::FT_Stream  stream );
FT_BASE( ft_structs_h::FT_ULong )
	FT_Stream_GetULong( ft_structs_h::FT_Stream  stream );
FT_BASE( ft_structs_h::FT_Error )
	FT_Stream_ExtractFrame( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_ULong   count,
	ft_structs_h::FT_Byte**  pbytes );
FT_BASE( void )
	FT_Stream_ReleaseFrame( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Byte**  pbytes );
FT_BASE( void )
	FT_Stream_Close( ft_structs_h::FT_Stream  stream );
FT_BASE( void )
	FT_Stream_Free( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Int     external );
FT_BASE( void )
	ft_glyphslot_free_bitmap( ft_structs_h::FT_GlyphSlot  slot );
FT_BASE( void )
  FT_GlyphLoader_Done( ft_structs_h::FT_GlyphLoader  loader );
FT_BASE( void )
	FT_GlyphLoader_Reset( ft_structs_h::FT_GlyphLoader  loader );
FT_BASE( void )
	FT_GlyphLoader_Rewind( ft_structs_h::FT_GlyphLoader  loader );
FT_BASE( ft_structs_h::FT_Error )
	FT_GlyphLoader_New( ft_structs_h::FT_Memory        memory,
	ft_structs_h::FT_GlyphLoader  *aloader );
FT_BASE( ft_structs_h::FT_Error )
	FT_Match_Size( ft_structs_h::FT_Face          face,
	ft_structs_h::FT_Size_Request  req,
	ft_structs_h::FT_Bool          ignore_width,
	ft_structs_h::FT_ULong*        size_index );
FT_BASE( void )
	FT_Select_Metrics( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_ULong  strike_index );
FT_BASE( void )
	FT_Request_Metrics( ft_structs_h::FT_Face          face,
	ft_structs_h::FT_Size_Request  req );
FT_BASE( ft_structs_h::FT_Error )
	FT_Stream_New( ft_structs_h::FT_Library           library,
	const ft_structs_h::FT_Open_Args*  args,
	ft_structs_h::FT_Stream           *astream );
FT_BASE( void )
	FT_Stream_OpenMemory( ft_structs_h::FT_Stream       stream,
	const ft_structs_h::FT_Byte*  base,
	ft_structs_h::FT_ULong        size );
FT_BASE( ft_structs_h::FT_Error )
	FT_Stream_Open( ft_structs_h::FT_Stream    stream,
	const char*  filepathname );
FT_BASE( ft_structs_h::FT_Error )
	FT_New_GlyphSlot( ft_structs_h::FT_Face        face,
	ft_structs_h::FT_GlyphSlot  *aslot );
FT_BASE( ft_structs_h::FT_Pointer )
	ft_service_list_lookup( ft_structs_h::FT_ServiceDesc  service_descriptors,
	const char*     service_id );
FT_BASE( ft_structs_h::FT_Error )
	FT_Stream_Seek( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_ULong   pos );
FT_BASE( ft_structs_h::FT_Error )
	FT_Stream_EnterFrame( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_ULong   count );
FT_BASE( void )
	FT_Stream_ExitFrame( ft_structs_h::FT_Stream  stream );
FT_BASE( ft_structs_h::FT_Pointer )
	ft_mem_qalloc( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Long    size,
	ft_structs_h::FT_Error  *p_error );
FT_BASE( void )
	ft_mem_free( ft_structs_h::FT_Memory    memory,
	const void*  P );
FT_BASE( ft_structs_h::FT_Error )
	FT_Stream_ReadFields( ft_structs_h::FT_Stream              stream,
	const ft_structs_h::FT_Frame_Field*  fields,
	void*                  structure );
FT_BASE( ft_structs_h::FT_UShort )
	FT_Stream_GetUShortLE( ft_structs_h::FT_Stream  stream );
FT_BASE( ft_structs_h::FT_Long )
	FT_Stream_Pos( ft_structs_h::FT_Stream  stream );
FT_BASE( ft_structs_h::FT_Pointer )
	ft_mem_realloc( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Long    item_size,
	ft_structs_h::FT_Long    cur_count,
	ft_structs_h::FT_Long    new_count,
	void*      block,
	ft_structs_h::FT_Error  *p_error );
FT_BASE( ft_structs_h::FT_Pointer )
	ft_mem_qrealloc( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Long    item_size,
	ft_structs_h::FT_Long    cur_count,
	ft_structs_h::FT_Long    new_count,
	void*      block,
	ft_structs_h::FT_Error  *p_error );
FT_BASE( ft_structs_h::FT_Pointer )
	ft_mem_alloc( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Long    size,
	ft_structs_h::FT_Error  *p_error );
FT_BASE( ft_structs_h::FT_Error )
	FT_CMap_New( ft_structs_h::FT_CMap_Class  clazz,
	ft_structs_h::FT_Pointer     init_data,
	ft_structs_h::FT_CharMap     charmap,
	ft_structs_h::FT_CMap       *acmap );
FT_BASE( void )
	ft_synthesize_vertical_metrics( ft_structs_h::FT_Glyph_Metrics*  metrics,
	ft_structs_h::FT_Pos             advance );
FT_BASE( void )
	FT_GlyphLoader_Add( ft_structs_h::FT_GlyphLoader  loader );
FT_BASE( void )
	FT_GlyphLoader_Prepare( ft_structs_h::FT_GlyphLoader  loader );
FT_BASE( ft_structs_h::FT_Int32 )
	FT_SqrtFixed( ft_structs_h::FT_Int32  x );
FT_BASE( ft_structs_h::FT_Error )
	FT_GlyphLoader_CheckPoints( ft_structs_h::FT_GlyphLoader  loader,
	ft_structs_h::FT_UInt         n_points,
	ft_structs_h::FT_UInt         n_contours );
FT_BASE( const void* )
	FT_Get_Module_Interface( ft_structs_h::FT_Library   library,
	const char*  mod_name );
FT_BASE( ft_structs_h::FT_Error )
	FT_GlyphLoader_CreateExtra( ft_structs_h::FT_GlyphLoader  loader );
FT_BASE( ft_structs_h::FT_Error )
	FT_Stream_ReadAt( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_ULong   pos,
	ft_structs_h::FT_Byte*   buffer,
	ft_structs_h::FT_ULong   count );
FT_BASE( ft_structs_h::FT_Error )
	FT_Render_Glyph_Internal( ft_structs_h::FT_Library      library,
	ft_structs_h::FT_GlyphSlot    slot,
	ft_structs_h::FT_Render_Mode  render_mode );
FT_BASE( ft_structs_h::FT_Renderer )
	FT_Lookup_Renderer( ft_structs_h::FT_Library       library,
	ft_structs_h::FT_Glyph_Format  format,
	ft_structs_h::FT_ListNode*     node );

namespace pscmap_h
{
#define FT_SERVICE_ID_POSTSCRIPT_CMAPS  "postscript-cmaps"
};
namespace tt_h
{
	typedef struct  TT_CMapRec_
	{
		ft_structs_h::FT_CMapRec  cmap;
		ft_structs_h::FT_Byte*    data;           /* pointer to in-memory cmap table */
		ft_structs_h::FT_Int      flags;          /* for format 4 only               */
	} TT_CMapRec, *TT_CMap;
#undef  IS_HINTED
#define IS_HINTED( flags )  ( ( flags & FT_LOAD_NO_HINTING ) == 0 )
#define TT_LOADER_SET_PP( loader )                                          \
	do {                                                              \
	(loader)->pp1.x = (loader)->bbox.xMin - (loader)->left_bearing; \
	(loader)->pp1.y = 0;                                            \
	(loader)->pp2.x = (loader)->pp1.x + (loader)->advance;          \
	(loader)->pp2.y = 0;                                            \
	(loader)->pp3.x = 0;                                            \
	(loader)->pp3.y = (loader)->top_bearing + (loader)->bbox.yMax;  \
	(loader)->pp4.x = 0;                                            \
	(loader)->pp4.y = (loader)->pp3.y - (loader)->vadvance;         \
	} while ( 0 )
#define TT_MAX_CODE_RANGES  3
	typedef struct TT_DriverRec_*  TT_Driver;
	typedef struct TT_ExecContextRec_*  TT_ExecContext;
	typedef struct TT_FaceRec_*  TT_Face;
	typedef struct TT_SizeRec_*  TT_Size;	
	typedef struct TT_LoaderRec_*  TT_Loader;		
	typedef ft_structs_h::FT_GlyphSlot  TT_GlyphSlot;
#ifndef TT_CONFIG_OPTION_STATIC_INTERPRETER /* indirect implementation */
#define EXEC_OP_   TT_ExecContext  exc,
#define EXEC_OP    TT_ExecContext  exc
#define EXEC_ARG_  exc,
#define EXEC_ARG   exc
#else                                       /* static implementation */
#define EXEC_OP_   /* void */
#define EXEC_OP    /* void */
#define EXEC_ARG_  /* void */
#define EXEC_ARG   /* void */
#endif /* TT_CONFIG_OPTION_STATIC_INTERPRETER */
	
	typedef struct  TTC_HeaderRec_
	{
		ft_structs_h::FT_ULong   tag;
		ft_structs_h::FT_Fixed   version;
		ft_structs_h::FT_Long    count;
		ft_structs_h::FT_ULong*  offsets;
	} TTC_HeaderRec;
	typedef struct  TT_TableRec_
	{
		ft_structs_h::FT_ULong  Tag;        /*        table type */
		ft_structs_h::FT_ULong  CheckSum;   /*    table checksum */
		ft_structs_h::FT_ULong  Offset;     /* table file offset */
		ft_structs_h::FT_ULong  Length;     /*      table length */
	} TT_TableRec, *TT_Table;
	typedef struct  TT_Header_
	{
		ft_structs_h::FT_Fixed   Table_Version;
		ft_structs_h::FT_Fixed   Font_Revision;
		ft_structs_h::FT_Long    CheckSum_Adjust;
		ft_structs_h::FT_Long    Magic_Number;
		ft_structs_h::FT_UShort  Flags;
		ft_structs_h::FT_UShort  Units_Per_EM;
		ft_structs_h::FT_Long    Created [2];
		ft_structs_h::FT_Long    Modified[2];
		ft_structs_h::FT_Short   xMin;
		ft_structs_h::FT_Short   yMin;
		ft_structs_h::FT_Short   xMax;
		ft_structs_h::FT_Short   yMax;
		ft_structs_h::FT_UShort  Mac_Style;
		ft_structs_h::FT_UShort  Lowest_Rec_PPEM;
		ft_structs_h::FT_Short   Font_Direction;
		ft_structs_h::FT_Short   Index_To_Loc_Format;
		ft_structs_h::FT_Short   Glyph_Data_Format;
	} TT_Header;
	typedef struct  TT_HoriHeader_
	{
		ft_structs_h::FT_Fixed   Version;
		ft_structs_h::FT_Short   Ascender;
		ft_structs_h::FT_Short   Descender;
		ft_structs_h::FT_Short   Line_Gap;
		ft_structs_h::FT_UShort  advance_Width_Max;      /* advance width maximum */
		ft_structs_h::FT_Short   min_Left_Side_Bearing;  /* minimum left-sb       */
		ft_structs_h::FT_Short   min_Right_Side_Bearing; /* minimum right-sb      */
		ft_structs_h::FT_Short   xMax_Extent;            /* xmax extents          */
		ft_structs_h::FT_Short   caret_Slope_Rise;
		ft_structs_h::FT_Short   caret_Slope_Run;
		ft_structs_h::FT_Short   caret_Offset;
		ft_structs_h::FT_Short   Reserved[4];
		ft_structs_h::FT_Short   metric_Data_Format;
		ft_structs_h::FT_UShort  number_Of_HMetrics;
		/* The following fields are not defined by the TrueType specification */
		/* but they are used to connect the metrics header to the relevant    */
		/* `HMTX' table.                                                      */
		void*      long_metrics;
		void*      short_metrics;
	} TT_HoriHeader;
	typedef struct  TT_MaxProfile_
	{
		ft_structs_h::FT_Fixed   version;
		ft_structs_h::FT_UShort  numGlyphs;
		ft_structs_h::FT_UShort  maxPoints;
		ft_structs_h::FT_UShort  maxContours;
		ft_structs_h::FT_UShort  maxCompositePoints;
		ft_structs_h::FT_UShort  maxCompositeContours;
		ft_structs_h::FT_UShort  maxZones;
		ft_structs_h::FT_UShort  maxTwilightPoints;
		ft_structs_h::FT_UShort  maxStorage;
		ft_structs_h::FT_UShort  maxFunctionDefs;
		ft_structs_h::FT_UShort  maxInstructionDefs;
		ft_structs_h::FT_UShort  maxStackElements;
	ft_structs_h::FT_UShort  maxSizeOfInstructions;
	ft_structs_h::FT_UShort  maxComponentElements;
	ft_structs_h::FT_UShort  maxComponentDepth;
	} TT_MaxProfile;
	typedef struct  TT_VertHeader_
	{
		ft_structs_h::FT_Fixed   Version;
	ft_structs_h::FT_Short   Ascender;
	ft_structs_h::FT_Short   Descender;
	ft_structs_h::FT_Short   Line_Gap;
		ft_structs_h::FT_UShort  advance_Height_Max;      /* advance height maximum */
		ft_structs_h::FT_Short   min_Top_Side_Bearing;    /* minimum left-sb or top-sb       */
	ft_structs_h::FT_Short   min_Bottom_Side_Bearing; /* minimum right-sb or bottom-sb   */
	ft_structs_h::FT_Short   yMax_Extent;             /* xmax or ymax extents            */
	ft_structs_h::FT_Short   caret_Slope_Rise;
	ft_structs_h::FT_Short   caret_Slope_Run;
	ft_structs_h::FT_Short   caret_Offset;
		ft_structs_h::FT_Short   Reserved[4];
		ft_structs_h::FT_Short   metric_Data_Format;
	ft_structs_h::FT_UShort  number_Of_VMetrics;
		/* The following fields are not defined by the TrueType specification */
	/* but they're used to connect the metrics header to the relevant     */
	/* `HMTX' or `VMTX' table.                                            */
		void*      long_metrics;
		void*      short_metrics;
	} TT_VertHeader;
	typedef struct  TT_NameEntryRec_
	{
		ft_structs_h::FT_UShort  platformID;
		ft_structs_h::FT_UShort  encodingID;
	ft_structs_h::FT_UShort  languageID;
	ft_structs_h::FT_UShort  nameID;
	ft_structs_h::FT_UShort  stringLength;
	ft_structs_h::FT_ULong   stringOffset;
		/* this last field is not defined in the spec */
		/* but used by the FreeType engine            */
		ft_structs_h::FT_Byte*   string;
	} TT_NameEntryRec, *TT_NameEntry;
	typedef struct  TT_NameTableRec_
	{
		ft_structs_h::FT_UShort         format;
		ft_structs_h::FT_UInt           numNameRecords;
		ft_structs_h::FT_UInt           storageOffset;
		TT_NameEntryRec*  names;
		ft_structs_h::FT_Stream         stream;
	} TT_NameTableRec, *TT_NameTable;
	typedef struct  TT_OS2_
	{
		ft_structs_h::FT_UShort  version;                /* 0x0001 - more or 0xFFFF */
	ft_structs_h::FT_Short   xAvgCharWidth;
	ft_structs_h::FT_UShort  usWeightClass;
	ft_structs_h::FT_UShort  usWidthClass;
	ft_structs_h::FT_Short   fsType;
	ft_structs_h::FT_Short   ySubscriptXSize;
	ft_structs_h::FT_Short   ySubscriptYSize;
	ft_structs_h::FT_Short   ySubscriptXOffset;
	ft_structs_h::FT_Short   ySubscriptYOffset;
	ft_structs_h::FT_Short   ySuperscriptXSize;
	ft_structs_h::FT_Short   ySuperscriptYSize;
	ft_structs_h::FT_Short   ySuperscriptXOffset;
	ft_structs_h::FT_Short   ySuperscriptYOffset;
	ft_structs_h::FT_Short   yStrikeoutSize;
	ft_structs_h::FT_Short   yStrikeoutPosition;
	ft_structs_h::FT_Short   sFamilyClass;
		ft_structs_h::FT_Byte    panose[10];
		ft_structs_h::FT_ULong   ulUnicodeRange1;        /* Bits 0-31   */
	ft_structs_h::FT_ULong   ulUnicodeRange2;        /* Bits 32-63  */
	ft_structs_h::FT_ULong   ulUnicodeRange3;        /* Bits 64-95  */
	ft_structs_h::FT_ULong   ulUnicodeRange4;        /* Bits 96-127 */
		ft_structs_h::FT_Char    achVendID[4];
		ft_structs_h::FT_UShort  fsSelection;
	ft_structs_h::FT_UShort  usFirstCharIndex;
	ft_structs_h::FT_UShort  usLastCharIndex;
	ft_structs_h::FT_Short   sTypoAscender;
	ft_structs_h::FT_Short   sTypoDescender;
	ft_structs_h::FT_Short   sTypoLineGap;
	ft_structs_h::FT_UShort  usWinAscent;
	ft_structs_h::FT_UShort  usWinDescent;
		/* only version 1 tables: */
		ft_structs_h::FT_ULong   ulCodePageRange1;       /* Bits 0-31   */
		ft_structs_h::FT_ULong   ulCodePageRange2;       /* Bits 32-63  */
		/* only version 2 tables: */
		ft_structs_h::FT_Short   sxHeight;
	ft_structs_h::FT_Short   sCapHeight;
	ft_structs_h::FT_UShort  usDefaultChar;
	ft_structs_h::FT_UShort  usBreakChar;
	ft_structs_h::FT_UShort  usMaxContext;
	} TT_OS2;
	typedef struct  TT_ValidatorRec_
	{
		ft_structs_h::FT_ValidatorRec  validator;
		ft_structs_h::FT_UInt          num_glyphs;
	} TT_ValidatorRec, *TT_Validator;
	typedef struct  TT_Postscript_
	{
	ft_structs_h::FT_Fixed  FormatType;
	ft_structs_h::FT_Fixed  italicAngle;
	ft_structs_h::FT_Short  underlinePosition;
	ft_structs_h::FT_Short  underlineThickness;
	ft_structs_h::FT_ULong  isFixedPitch;
	ft_structs_h::FT_ULong  minMemType42;
	ft_structs_h::FT_ULong  maxMemType42;
	ft_structs_h::FT_ULong  minMemType1;
	ft_structs_h::FT_ULong  maxMemType1;
		/* Glyph names follow in the file, but we don't   */
		/* load them by default.  See the ttpost.c file.  */
	} TT_Postscript;
	typedef struct  TT_GaspRangeRec_
	{
		ft_structs_h::FT_UShort  maxPPEM;
		ft_structs_h::FT_UShort  gaspFlag;
	} TT_GaspRangeRec, *TT_GaspRange;
	typedef struct  TT_Gasp_
	{
		ft_structs_h::FT_UShort     version;
		ft_structs_h::FT_UShort     numRanges;
		TT_GaspRange  gaspRanges;
	} TT_GaspRec;
	typedef struct  TT_PCLT_
	{
	ft_structs_h::FT_Fixed   Version;
	ft_structs_h::FT_ULong   FontNumber;
	ft_structs_h::FT_UShort  Pitch;
	ft_structs_h::FT_UShort  xHeight;
	ft_structs_h::FT_UShort  Style;
	ft_structs_h::FT_UShort  TypeFamily;
	ft_structs_h::FT_UShort  CapHeight;
	ft_structs_h::FT_UShort  SymbolSet;
	ft_structs_h::FT_Char    TypeFace[16];
	ft_structs_h::FT_Char    CharacterComplement[8];
	ft_structs_h::FT_Char    FileName[6];
	ft_structs_h::FT_Char    StrokeWeight;
	ft_structs_h::FT_Char    WidthType;
	ft_structs_h::FT_Byte    SerifStyle;
	ft_structs_h::FT_Byte    Reserved;
	} TT_PCLT;
	typedef struct  TT_SBit_LineMetricsRec_
	{
		ft_structs_h::FT_Char  ascender;
		ft_structs_h::FT_Char  descender;
		ft_structs_h::FT_Byte  max_width;
	ft_structs_h::FT_Char  caret_slope_numerator;
	ft_structs_h::FT_Char  caret_slope_denominator;
	ft_structs_h::FT_Char  caret_offset;
	ft_structs_h::FT_Char  min_origin_SB;
	ft_structs_h::FT_Char  min_advance_SB;
	ft_structs_h::FT_Char  max_before_BL;
	ft_structs_h::FT_Char  min_after_BL;
	ft_structs_h::FT_Char  pads[2];
	} TT_SBit_LineMetricsRec, *TT_SBit_LineMetrics;
	typedef struct  TT_SBit_ScaleRec_
	{
		TT_SBit_LineMetricsRec  hori;
	TT_SBit_LineMetricsRec  vert;
		ft_structs_h::FT_Byte                 x_ppem;
	ft_structs_h::FT_Byte                 y_ppem;
		ft_structs_h::FT_Byte                 x_ppem_substitute;
	ft_structs_h::FT_Byte                 y_ppem_substitute;
	} TT_SBit_ScaleRec, *TT_SBit_Scale;
	typedef struct  TT_Post_20Rec_
	{
		ft_structs_h::FT_UShort   num_glyphs;
		ft_structs_h::FT_UShort   num_names;
		ft_structs_h::FT_UShort*  glyph_indices;
		ft_structs_h::FT_Char**   glyph_names;
	} TT_Post_20Rec, *TT_Post_20;
	typedef struct  TT_Post_25_
	{
		ft_structs_h::FT_UShort  num_glyphs;
		ft_structs_h::FT_Char*   offsets;
	} TT_Post_25Rec, *TT_Post_25;
	typedef struct  TT_Post_NamesRec_
	{
		ft_structs_h::FT_Bool  loaded;
		union
		{
			TT_Post_20Rec  format_20;
			TT_Post_25Rec  format_25;
		} names;
	} TT_Post_NamesRec, *TT_Post_Names;
	typedef struct  TT_DefRecord_
  {
	ft_structs_h::FT_Int   range;      /* in which code range is it located? */
	ft_structs_h::FT_Long  start;      /* where does it start?               */
	ft_structs_h::FT_Long  end;        /* where does it end?                 */
	ft_structs_h::FT_UInt  opc;        /* function #, or instruction code    */
	ft_structs_h::FT_Bool  active;     /* is it active?                      */
	} TT_DefRecord, *TT_DefArray;
	typedef struct  TT_CallRec_
	{
		ft_structs_h::FT_Int   Caller_Range;
		ft_structs_h::FT_Long  Caller_IP;
		ft_structs_h::FT_Long  Cur_Count;
	ft_structs_h::FT_Long  Cur_Restart;
	ft_structs_h::FT_Long  Cur_End;
	} TT_CallRec, *TT_CallStack;
	typedef struct  TT_CodeRange_
	{
		ft_structs_h::FT_Byte*  base;
		ft_structs_h::FT_ULong  size;
	} TT_CodeRange;
  typedef TT_CodeRange  TT_CodeRangeTable[TT_MAX_CODE_RANGES];
	typedef struct  TT_GlyphZoneRec_
	{
		ft_structs_h::FT_Memory   memory;
		ft_structs_h::FT_UShort   max_points;
	ft_structs_h::FT_UShort   max_contours;
	ft_structs_h::FT_UShort   n_points;    /* number of points in zone    */
	ft_structs_h::FT_Short    n_contours;  /* number of contours          */
		ft_structs_h::FT_Vector*  org;         /* original point coordinates  */
	ft_structs_h::FT_Vector*  cur;         /* current point coordinates   */
	ft_structs_h::FT_Vector*  orus;        /* original (unscaled) point coordinates */
		ft_structs_h::FT_Byte*    tags;        /* current touch flags         */
	ft_structs_h::FT_UShort*  contours;    /* contour end points          */
		ft_structs_h::FT_UShort   first_point; /* offset of first (#0) point  */
	} TT_GlyphZoneRec, *TT_GlyphZone;
	typedef struct  TT_SBit_MetricsRec_
	{
		ft_structs_h::FT_Byte  height;
		ft_structs_h::FT_Byte  width;
		ft_structs_h::FT_Char  horiBearingX;
	ft_structs_h::FT_Char  horiBearingY;
	ft_structs_h::FT_Byte  horiAdvance;
		ft_structs_h::FT_Char  vertBearingX;
	ft_structs_h::FT_Char  vertBearingY;
	ft_structs_h::FT_Byte  vertAdvance;
	} TT_SBit_MetricsRec, *TT_SBit_Metrics;

	
	typedef ft_structs_h::FT_Error
		(*TT_Interpreter)( void*  exec_context );
	typedef ft_structs_h::FT_F26Dot6
		(*TT_Round_Func)( EXEC_OP_ ft_structs_h::FT_F26Dot6  distance,
		ft_structs_h::FT_F26Dot6  compensation );
	typedef void
		(*TT_Move_Func)( EXEC_OP_ TT_GlyphZone  zone,
		ft_structs_h::FT_UShort     point,
		ft_structs_h::FT_F26Dot6    distance );
	typedef ft_structs_h::FT_F26Dot6
		(*TT_Project_Func)( EXEC_OP_ ft_structs_h::FT_Pos   dx,
		ft_structs_h::FT_Pos   dy );
	typedef ft_structs_h::FT_F26Dot6
		(*TT_Get_CVT_Func)( EXEC_OP_ ft_structs_h::FT_ULong  idx );
	typedef void
		(*TT_Set_CVT_Func)( EXEC_OP_ ft_structs_h::FT_ULong    idx,
		ft_structs_h::FT_F26Dot6  value );
	typedef ft_structs_h::FT_Error
		(*TT_Loader_GotoTableFunc)( TT_Face    face,
		ft_structs_h::FT_ULong   tag,
		ft_structs_h::FT_Stream  stream,
		ft_structs_h::FT_ULong*  length );
	typedef ft_structs_h::FT_Error
		(*TT_Loader_StartGlyphFunc)( TT_Loader  loader,
		ft_structs_h::FT_UInt    glyph_index,
		ft_structs_h::FT_ULong   offset,
		ft_structs_h::FT_UInt    byte_count );
	typedef void
		(*TT_Loader_EndGlyphFunc)( TT_Loader  loader );
	typedef ft_structs_h::FT_Error
		(*TT_Loader_ReadGlyphFunc)( TT_Loader  loader );
	typedef ft_structs_h::FT_Error
		(*TT_Init_Face_Func)( ft_structs_h::FT_Stream      stream,
		TT_Face        face,
		ft_structs_h::FT_Int         face_index,
		ft_structs_h::FT_Int         num_params,
		ft_structs_h::FT_Parameter*  params );
	typedef ft_structs_h::FT_Error
		(*TT_Load_Face_Func)( ft_structs_h::FT_Stream      stream,
		TT_Face        face,
		ft_structs_h::FT_Int         face_index,
		ft_structs_h::FT_Int         num_params,
		ft_structs_h::FT_Parameter*  params );
	 typedef void
		 (*TT_Done_Face_Func)( TT_Face  face );
	 typedef ft_structs_h::FT_Error
		 (*TT_Load_Any_Func)( TT_Face    face,
		 ft_structs_h::FT_ULong   tag,
		 ft_structs_h::FT_Long    offset,
		 ft_structs_h::FT_Byte   *buffer,
		 ft_structs_h::FT_ULong*  length );
	 typedef ft_structs_h::FT_Error
		 (*TT_Load_Table_Func)( TT_Face    face,
		 ft_structs_h::FT_Stream  stream );
	 typedef ft_structs_h::FT_Error
		 (*TT_Load_Metrics_Func)( TT_Face    face,
		 ft_structs_h::FT_Stream  stream,
		 ft_structs_h::FT_Bool    vertical );
	 typedef void
		 (*TT_Free_Table_Func)( TT_Face  face );
	 typedef ft_structs_h::FT_Error
		 (*TT_Load_SBit_Image_Func)( TT_Face              face,
		 ft_structs_h::FT_ULong             strike_index,
		 ft_structs_h::FT_UInt              glyph_index,
		 ft_structs_h::FT_UInt              load_flags,
		 ft_structs_h::FT_Stream            stream,
		 ft_structs_h::FT_Bitmap           *amap,
		 TT_SBit_MetricsRec  *ametrics );
	 typedef ft_structs_h::FT_Error
		 (*TT_Get_PS_Name_Func)( TT_Face      face,
		 ft_structs_h::FT_UInt      idx,
		 ft_structs_h::FT_String**  PSname );
	 typedef ft_structs_h::FT_Int
		 (*TT_Face_GetKerningFunc)( TT_Face  face,
		 ft_structs_h::FT_UInt  left_glyph,
		 ft_structs_h::FT_UInt  right_glyph );
	 typedef ft_structs_h::FT_Error
		 (*TT_Set_SBit_Strike_Func)( TT_Face          face,
		 ft_structs_h::FT_Size_Request  req,
		 ft_structs_h::FT_ULong*        astrike_index );
	 typedef ft_structs_h::FT_Error
		 (*TT_Load_Strike_Metrics_Func)( TT_Face           face,
		 ft_structs_h::FT_ULong          strike_index,
		 ft_structs_h::FT_Size_Metrics*  metrics );
	 typedef ft_structs_h::FT_Error
		 (*TT_Get_Metrics_Func)( TT_Face     face,
		 ft_structs_h::FT_Bool     vertical,
		 ft_structs_h::FT_UInt     gindex,
		 ft_structs_h::FT_Short*   abearing,
		 ft_structs_h::FT_UShort*  aadvance );
	 typedef ft_structs_h::FT_Error
		 (*FT_CMap_InitFunc)( ft_structs_h::FT_CMap     cmap,
		 ft_structs_h::FT_Pointer  init_data );
	 typedef void
		 (*FT_CMap_DoneFunc)( ft_structs_h::FT_CMap  cmap );
	 typedef ft_structs_h::FT_UInt
		 (*FT_CMap_CharIndexFunc)( ft_structs_h::FT_CMap    cmap,
		 ft_structs_h::FT_UInt32  char_code );
	 typedef ft_structs_h::FT_UInt
		 (*FT_CMap_CharNextFunc)( ft_structs_h::FT_CMap     cmap,
		 ft_structs_h::FT_UInt32  *achar_code );
	 typedef ft_structs_h::FT_UInt
		 (*FT_CMap_CharVarIndexFunc)( ft_structs_h::FT_CMap    cmap,
		 ft_structs_h::FT_CMap    unicode_cmap,
		 ft_structs_h::FT_UInt32  char_code,
		 ft_structs_h::FT_UInt32  variant_selector );
	 typedef ft_structs_h::FT_Bool
		 (*FT_CMap_CharVarIsDefaultFunc)( ft_structs_h::FT_CMap    cmap,
		 ft_structs_h::FT_UInt32  char_code,
		 ft_structs_h::FT_UInt32  variant_selector );
	 typedef ft_structs_h::FT_UInt32 *
		 (*FT_CMap_VariantListFunc)( ft_structs_h::FT_CMap    cmap,
		 ft_structs_h::FT_Memory  mem );
	 typedef ft_structs_h::FT_UInt32 *
		 (*FT_CMap_CharVariantListFunc)( ft_structs_h::FT_CMap    cmap,
		 ft_structs_h::FT_Memory  mem,
		 ft_structs_h::FT_UInt32  char_code );
	 typedef ft_structs_h::FT_UInt32 *
		 (*FT_CMap_VariantCharListFunc)( ft_structs_h::FT_CMap    cmap,
		 ft_structs_h::FT_Memory  mem,
		 ft_structs_h::FT_UInt32  variant_selector );	
	typedef struct  SFNT_Interface_
	{
		TT_Loader_GotoTableFunc      goto_table;
		TT_Init_Face_Func            init_face;
		TT_Load_Face_Func            load_face;
		TT_Done_Face_Func            done_face;
		ft_structs_h::FT_Module_Requester          get_interface;
		TT_Load_Any_Func             load_any;
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
		TT_Load_SFNT_HeaderRec_Func  load_sfnt_header;
		TT_Load_Directory_Func       load_directory;
#endif
		/* these functions are called by `load_face' but they can also  */
		/* be called from external modules, if there is a need to do so */
		TT_Load_Table_Func           load_head;
		TT_Load_Metrics_Func         load_hhea;
	TT_Load_Table_Func           load_cmap;
	TT_Load_Table_Func           load_maxp;
	TT_Load_Table_Func           load_os2;
	TT_Load_Table_Func           load_post;
		TT_Load_Table_Func           load_name;
		TT_Free_Table_Func           free_name;
		/* optional tables */
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
	TT_Load_Table_Func           load_hdmx_stub;
	TT_Free_Table_Func           free_hdmx_stub;
#endif
		/* this field was called `load_kerning' up to version 2.1.10 */
		TT_Load_Table_Func           load_kern;
		TT_Load_Table_Func           load_gasp;
	TT_Load_Table_Func           load_pclt;
		/* see `ttload.h'; this field was called `load_bitmap_header' up to */
	/* version 2.1.10                                                   */
		TT_Load_Table_Func           load_bhed;
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
		/* see `ttsbit.h' */
		TT_Set_SBit_Strike_OldFunc   set_sbit_strike_stub;
		TT_Load_Table_Func           load_sbits_stub;
	/*
	 *  The following two fields appeared in version 2.1.8, and were placed
	 *  between `load_sbits' and `load_sbit_image'.  We support them as a
	 *  special exception since they are used by Xfont library within the
	 *  X.Org xserver, and because the probability that other rogue clients
	 *  use the other version 2.1.7 fields below is _extremely_ low.
	 *
	 *  Note that this forces us to disable an interesting memory-saving
	 *  optimization though...
	 */
		TT_Find_SBit_Image_Func      find_sbit_image;
	TT_Load_SBit_Metrics_Func    load_sbit_metrics;
#endif
		TT_Load_SBit_Image_Func      load_sbit_image;
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
	TT_Free_Table_Func           free_sbits_stub;
#endif
		/* see `ttpost.h' */
		TT_Get_PS_Name_Func          get_psname;
		TT_Free_Table_Func           free_psnames;
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
	TT_CharMap_Load_Func         load_charmap_stub;
	TT_CharMap_Free_Func         free_charmap_stub;
#endif
		/* starting here, the structure differs from version 2.1.7 */
		/* this field was introduced in version 2.1.8, named `get_psname' */
		TT_Face_GetKerningFunc       get_kerning;
		/* new elements introduced after version 2.1.10 */
		/* load the font directory, i.e., the offset table and */
	/* the table directory                                 */
		TT_Load_Table_Func           load_font_dir;
		TT_Load_Metrics_Func         load_hmtx;
		TT_Load_Table_Func           load_eblc;
	TT_Free_Table_Func           free_eblc;
		TT_Set_SBit_Strike_Func      set_sbit_strike;
	TT_Load_Strike_Metrics_Func  load_strike_metrics;
		TT_Get_Metrics_Func          get_metrics;
  } SFNT_Interface;
	typedef SFNT_Interface*   SFNT_Service;
	typedef struct  TT_LoaderRec_
	{
		ft_structs_h::FT_Face          face;
	ft_structs_h::FT_Size          size;
	ft_structs_h::FT_GlyphSlot     glyph;
	ft_structs_h::FT_GlyphLoader   gloader;
		ft_structs_h::FT_ULong         load_flags;
	ft_structs_h::FT_UInt          glyph_index;
		ft_structs_h::FT_Stream        stream;
	ft_structs_h::FT_Int           byte_len;
		ft_structs_h::FT_Short         n_contours;
	ft_structs_h::FT_BBox          bbox;
	ft_structs_h::FT_Int           left_bearing;
	ft_structs_h::FT_Int           advance;
	ft_structs_h::FT_Int           linear;
	ft_structs_h::FT_Bool          linear_def;
	ft_structs_h::FT_Bool          preserve_pps;
	ft_structs_h::FT_Vector        pp1;
	ft_structs_h::FT_Vector        pp2;
		ft_structs_h::FT_ULong         glyf_offset;
		/* the zone where we load our glyphs */
	TT_GlyphZoneRec  base;
	TT_GlyphZoneRec  zone;
		TT_ExecContext   exec;
	ft_structs_h::FT_Byte*         instructions;
	ft_structs_h::FT_ULong         ins_pos;
		/* for possible extensibility in other formats */
	void*            other;
		/* since version 2.1.8 */
	ft_structs_h::FT_Int           top_bearing;
	ft_structs_h::FT_Int           vadvance;
	ft_structs_h::FT_Vector        pp3;
	ft_structs_h::FT_Vector        pp4;
		/* since version 2.2.1 */
	ft_structs_h::FT_Byte*         cursor;
	ft_structs_h::FT_Byte*         limit;
	} TT_LoaderRec;
	typedef struct  TT_DriverRec_
	{
		ft_structs_h::FT_DriverRec     root;
		TT_ExecContext   context;  /* execution context        */
		TT_GlyphZoneRec  zone;     /* glyph loader points zone */
		void*            extension_component;
	} TT_DriverRec;	
	typedef struct  TT_Size_Metrics_
	{
		/* for non-square pixels */
	ft_structs_h::FT_Long     x_ratio;
	ft_structs_h::FT_Long     y_ratio;
		ft_structs_h::FT_UShort   ppem;               /* maximum ppem size              */
	ft_structs_h::FT_Long     ratio;              /* current ratio                  */
	ft_structs_h::FT_Fixed    scale;
		ft_structs_h::FT_F26Dot6  compensations[4];   /* device-specific compensations  */
		ft_structs_h::FT_Bool     valid;
		ft_structs_h::FT_Bool     rotated;            /* `is the glyph rotated?'-flag   */
	ft_structs_h::FT_Bool     stretched;          /* `is the glyph stretched?'-flag */
	} TT_Size_Metrics;
	typedef struct  TT_GraphicsState_
	{
	ft_structs_h::FT_UShort      rp0;
	ft_structs_h::FT_UShort      rp1;
	ft_structs_h::FT_UShort      rp2;
		ft_structs_h::FT_UnitVector  dualVector;
	ft_structs_h::FT_UnitVector  projVector;
	ft_structs_h::FT_UnitVector  freeVector;
#ifdef TT_CONFIG_OPTION_UNPATENTED_HINTING
	ft_structs_h::FT_Bool        both_x_axis;
#endif
		ft_structs_h::FT_Long        loop;
	ft_structs_h::FT_F26Dot6     minimum_distance;
	ft_structs_h::FT_Int         round_state;
		ft_structs_h::FT_Bool        auto_flip;
	ft_structs_h::FT_F26Dot6     control_value_cutin;
	ft_structs_h::FT_F26Dot6     single_width_cutin;
	ft_structs_h::FT_F26Dot6     single_width_value;
	ft_structs_h::FT_Short       delta_base;
	ft_structs_h::FT_Short       delta_shift;
		ft_structs_h::FT_Byte        instruct_control;
	/* According to Greg Hitchcock from Microsoft, the `scan_control'     */
	/* variable as documented in the TrueType specification is a 32-bit   */
	/* integer; the high-word part holds the SCANTYPE value, the low-word */
	/* part the SCANCTRL value.  We separate it into two fields.          */
	ft_structs_h::FT_Bool        scan_control;
	ft_structs_h::FT_Int         scan_type;
		ft_structs_h::FT_UShort      gep0;
	ft_structs_h::FT_UShort      gep1;
	ft_structs_h::FT_UShort      gep2;
	} TT_GraphicsState;
	typedef struct  TT_SizeRec_
	{
		ft_structs_h::FT_SizeRec         root;
		/* we have our own copy of metrics so that we can modify */
		/* it without affecting auto-hinting (when used)         */
		ft_structs_h::FT_Size_Metrics    metrics;
		TT_Size_Metrics    ttmetrics;
		ft_structs_h::FT_ULong           strike_index;      /* 0xFFFFFFFF to indicate invalid */
#ifdef TT_USE_BYTECODE_INTERPRETER
		ft_structs_h::FT_UInt            num_function_defs; /* number of function definitions */
		ft_structs_h::FT_UInt            max_function_defs;
		TT_DefArray        function_defs;     /* table of function definitions  */
		FT_UInt            num_instruction_defs;  /* number of ins. definitions */
		FT_UInt            max_instruction_defs;
	TT_DefArray        instruction_defs;      /* table of ins. definitions  */
		FT_UInt            max_func;
	FT_UInt            max_ins;
		TT_CodeRangeTable  codeRangeTable;
		TT_GraphicsState   GS;
		FT_ULong           cvt_size;      /* the scaled control value table */
	FT_Long*           cvt;
		FT_UShort          storage_size; /* The storage area is now part of */
	FT_Long*           storage;      /* the instance                    */
		TT_GlyphZoneRec    twilight;     /* The instance's twilight zone    */
		/* debugging variables */
		/* When using the debugger, we must keep the */
	/* execution context tied to the instance    */
	/* object rather than asking it on demand.   */
		FT_Bool            debug;
		TT_ExecContext     context;
		FT_Bool            bytecode_ready;
		FT_Bool            cvt_ready;
#endif /* TT_USE_BYTECODE_INTERPRETER */
	} TT_SizeRec;
	  typedef struct  TT_FaceRec_
		{
			ft_structs_h::FT_FaceRec            root;
			TTC_HeaderRec         ttc_header;
			ft_structs_h::FT_ULong              format_tag;
			ft_structs_h::FT_UShort             num_tables;
			TT_Table              dir_tables;
			TT_Header             header;       /* TrueType header table          */
			TT_HoriHeader         horizontal;   /* TrueType horizontal header     */
			TT_MaxProfile         max_profile;
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
			FT_ULong              max_components;  /* stubbed to 0 */
#endif
			ft_structs_h::FT_Bool               vertical_info;
			TT_VertHeader         vertical;     /* TT Vertical header, if present */
			ft_structs_h::FT_UShort             num_names;    /* number of name records  */
			TT_NameTableRec       name_table;   /* name table              */
			TT_OS2                os2;          /* TrueType OS/2 table            */
			TT_Postscript         postscript;   /* TrueType Postscript table      */
			ft_structs_h::FT_Byte*              cmap_table;   /* extracted `cmap' table */
			ft_structs_h::FT_ULong              cmap_size;
			TT_Loader_GotoTableFunc   goto_table;
			TT_Loader_StartGlyphFunc  access_glyph_frame;
			TT_Loader_EndGlyphFunc    forget_glyph_frame;
			TT_Loader_ReadGlyphFunc   read_glyph_header;
			TT_Loader_ReadGlyphFunc   read_simple_glyph;
			TT_Loader_ReadGlyphFunc   read_composite_glyph;
			/* a typeless pointer to the SFNT_Interface table used to load */
			/* the basic TrueType tables in the face object                */
			void*                 sfnt;
			/* a typeless pointer to the FT_Service_PsCMapsRec table used to */
			/* handle glyph names <-> unicode & Mac values                   */
			void*                 psnames;
			/***********************************************************************/
			/*                                                                     */
			/* Optional TrueType/OpenType tables                                   */
			/*                                                                     */
			/***********************************************************************/
			/* horizontal device metrics */
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
			TT_HdmxRec            hdmx;
#endif
			/* grid-fitting and scaling table */
			TT_GaspRec            gasp;                 /* the `gasp' table */
			/* PCL 5 table */
			TT_PCLT               pclt;
			/* embedded bitmaps support */
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
			FT_ULong              num_sbit_strikes;
			TT_SBit_Strike        sbit_strikes;
#endif
			ft_structs_h::FT_ULong              num_sbit_scales;
			TT_SBit_Scale         sbit_scales;
			/* postscript names table */
			TT_Post_NamesRec      postscript_names;
			/***********************************************************************/
			/*                                                                     */
			/* TrueType-specific fields (ignored by the OTF-Type2 driver)          */
			/*                                                                     */
			/***********************************************************************/
			/* the glyph locations */
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
			FT_UShort             num_locations_stub;
			FT_Long*              glyph_locations_stub;
#endif
			/* the font program, if any */
			ft_structs_h::FT_ULong              font_program_size;
			ft_structs_h::FT_Byte*              font_program;
			/* the cvt program, if any */
			ft_structs_h::FT_ULong              cvt_program_size;
			ft_structs_h::FT_Byte*              cvt_program;
			/* the original, unscaled, control value table */
			ft_structs_h::FT_ULong              cvt_size;
			ft_structs_h::FT_Short*             cvt;
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
			/* the format 0 kerning table, if any */
			FT_Int                num_kern_pairs;
			FT_Int                kern_table_index;
			TT_Kern0_Pair         kern_pairs;
#endif
			/* A pointer to the bytecode interpreter to use.  This is also */
			/* used to hook the debugger for the `ttdebug' utility.        */
			TT_Interpreter        interpreter;
#ifdef TT_CONFIG_OPTION_UNPATENTED_HINTING
			/* Use unpatented hinting only. */
			FT_Bool               unpatented_hinting;
#endif
			/***********************************************************************/
			/*                                                                     */
			/* Other tables or fields. This is used by derivative formats like     */
			/* OpenType.                                                           */
			/*                                                                     */
			/***********************************************************************/
			ft_structs_h::FT_Generic            extra;
			const char*           postscript_name;
			/* since version 2.1.8, but was originally placed after */
			/* `glyph_locations_stub'                               */
			ft_structs_h::FT_ULong              glyf_len;
			/* since version 2.1.8, but was originally placed before `extra' */
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
			FT_Bool               doblend;
			GX_Blend              blend;
#endif
			/* since version 2.2 */
			ft_structs_h::FT_Byte*              horz_metrics;
			ft_structs_h::FT_ULong              horz_metrics_size;
			ft_structs_h::FT_Byte*              vert_metrics;
			ft_structs_h::FT_ULong              vert_metrics_size;
			ft_structs_h::FT_ULong              num_locations; /* in broken TTF, gid > 0xFFFF */
			ft_structs_h::FT_Byte*              glyph_locations;
			ft_structs_h::FT_Byte*              hdmx_table;
			ft_structs_h::FT_ULong              hdmx_table_size;
			ft_structs_h::FT_UInt               hdmx_record_count;
			ft_structs_h::FT_ULong              hdmx_record_size;
			ft_structs_h::FT_Byte*              hdmx_record_sizes;
			ft_structs_h::FT_Byte*              sbit_table;
			ft_structs_h::FT_ULong              sbit_table_size;
			ft_structs_h::FT_UInt               sbit_num_strikes;
			ft_structs_h::FT_Byte*              kern_table;
			ft_structs_h::FT_ULong              kern_table_size;
			ft_structs_h::FT_UInt               num_kern_tables;
			ft_structs_h::FT_UInt32             kern_avail_bits;
			ft_structs_h::FT_UInt32             kern_order_bits;
#ifdef TT_CONFIG_OPTION_BDF
			TT_BDFRec             bdf;
#endif /* TT_CONFIG_OPTION_BDF */
			/* since 2.3.0 */
			ft_structs_h::FT_ULong              horz_metrics_offset;
			ft_structs_h::FT_ULong              vert_metrics_offset;
	} TT_FaceRec;
	typedef struct  TT_ExecContextRec_
	{
		TT_Face            face;
		TT_Size            size;
		ft_structs_h::FT_Memory          memory;
		/* instructions state */
		ft_structs_h::FT_Error           error;      /* last execution error */
		ft_structs_h::FT_Long            top;        /* top of exec. stack   */
		ft_structs_h::FT_UInt            stackSize;  /* size of exec. stack  */
		ft_structs_h::FT_Long*           stack;      /* current exec. stack  */
		ft_structs_h::FT_Long            args;
		ft_structs_h::FT_UInt            new_top;    /* new top after exec.  */
		TT_GlyphZoneRec    zp0,        /* zone records */
			zp1,
			zp2,
			pts,
			twilight;
		ft_structs_h::FT_Size_Metrics    metrics;
		TT_Size_Metrics    tt_metrics; /* size metrics */
		TT_GraphicsState   GS;         /* current graphics state */
		ft_structs_h::FT_Int             curRange;  /* current code range number   */
		ft_structs_h::FT_Byte*           code;      /* current code range          */
		ft_structs_h::FT_Long            IP;        /* current instruction pointer */
		ft_structs_h::FT_Long            codeSize;  /* size of current range       */
		ft_structs_h::FT_Byte            opcode;    /* current opcode              */
		ft_structs_h::FT_Int             length;    /* length of current opcode    */
		ft_structs_h::FT_Bool            step_ins;  /* true if the interpreter must */
		/* increment IP after ins. exec */
		ft_structs_h::FT_ULong           cvtSize;
		ft_structs_h::FT_Long*           cvt;
		ft_structs_h::FT_UInt            glyphSize; /* glyph instructions buffer size */
		ft_structs_h::FT_Byte*           glyphIns;  /* glyph instructions buffer */
		ft_structs_h::FT_UInt            numFDefs;  /* number of function defs         */
	ft_structs_h::FT_UInt            maxFDefs;  /* maximum number of function defs */
	TT_DefArray        FDefs;     /* table of FDefs entries          */
		ft_structs_h::FT_UInt            numIDefs;  /* number of instruction defs */
	ft_structs_h::FT_UInt            maxIDefs;  /* maximum number of ins defs */
	TT_DefArray        IDefs;     /* table of IDefs entries     */
		ft_structs_h::FT_UInt            maxFunc;   /* maximum function index     */
	ft_structs_h::FT_UInt            maxIns;    /* maximum instruction index  */
		ft_structs_h::FT_Int             callTop,    /* top of call stack during execution */
			callSize;   /* size of call stack */
		TT_CallStack       callStack;  /* call stack */
		ft_structs_h::FT_UShort          maxPoints;    /* capacity of this context's `pts' */
		ft_structs_h::FT_Short           maxContours;  /* record, expressed in points and  */
		/* contours.                        */
		TT_CodeRangeTable  codeRangeTable;  /* table of valid code ranges */
		/* useful for the debugger   */
		ft_structs_h::FT_UShort          storeSize;  /* size of current storage */
		ft_structs_h::FT_Long*           storage;    /* storage area            */
		ft_structs_h::FT_F26Dot6         period;     /* values used for the */
		ft_structs_h::FT_F26Dot6         phase;      /* `SuperRounding'     */
		ft_structs_h::FT_F26Dot6         threshold;
#if 0
		/* this seems to be unused */
		ft_structs_h::FT_Int             cur_ppem;   /* ppem along the current proj vector */
#endif
		ft_structs_h::FT_Bool            instruction_trap; /* If `True', the interpreter will */
		/* exit after each instruction     */
		TT_GraphicsState   default_GS;       /* graphics state resulting from   */
		/* the prep program                */
		ft_structs_h::FT_Bool            is_composite;     /* true if the glyph is composite  */
		ft_structs_h::FT_Bool            pedantic_hinting; /* true if pedantic interpretation */
		/* latest interpreter additions */
		ft_structs_h::FT_Long            F_dot_P;    /* dot product of freedom and projection */
		/* vectors                               */
		TT_Round_Func      func_round; /* current rounding function             */
		TT_Project_Func    func_project,   /* current projection function */
			func_dualproj,  /* current dual proj. function */
			func_freeProj;  /* current freedom proj. func  */
		TT_Move_Func       func_move;      /* current point move function */
	TT_Move_Func       func_move_orig; /* move original position function */
		TT_Get_CVT_Func    func_read_cvt;  /* read a cvt entry              */
		TT_Set_CVT_Func    func_write_cvt; /* write a cvt entry (in pixels) */
		TT_Set_CVT_Func    func_move_cvt;  /* incr a cvt entry (in pixels)  */
		ft_structs_h::FT_Bool            grayscale;      /* are we hinting for grayscale? */
	} TT_ExecContextRec;
	typedef enum  FT_TrueTypeEngineType_
	{
		FT_TRUETYPE_ENGINE_TYPE_NONE = 0,
		FT_TRUETYPE_ENGINE_TYPE_UNPATENTED,
		FT_TRUETYPE_ENGINE_TYPE_PATENTED
	} FT_TrueTypeEngineType;
#define FT_SERVICE_ID_TRUETYPE_ENGINE  "truetype-engine"
	FT_DEFINE_SERVICE( TrueTypeEngine )
	{
		FT_TrueTypeEngineType  engine_type;
	};
#define FT_SERVICE_ID_TT_GLYF  "tt-glyf"
	typedef ft_structs_h::FT_ULong
		(*TT_Glyf_GetLocationFunc)( ft_structs_h::FT_Face    face,
		ft_structs_h::FT_UInt    gindex,
		ft_structs_h::FT_ULong  *psize );
	FT_DEFINE_SERVICE( TTGlyf )
	{
		TT_Glyf_GetLocationFunc  get_location;
	};
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DEFINE_SERVICE_TTGLYFREC( class_, get_location_ )  \
	static const FT_Service_TTGlyfRec  class_ =                 \
	{                                                           \
	get_location_                                             \
	};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DEFINE_SERVICE_TTGLYFREC( class_, get_location_ )  \
	void                                                        \
	FT_Init_Class_ ## class_( FT_Service_TTGlyfRec*  clazz )    \
	{                                                           \
	clazz->get_location = get_location_;                      \
	}
#endif /* FT_CONFIG_OPTION_PIC */	
#ifndef FT_CONFIG_OPTION_PIC
#define FT_TT_SERVICES_GET                   tt_services
#define FT_TT_SERVICE_GX_MULTI_MASTERS_GET   tt_service_gx_multi_masters
#define FT_TT_SERVICE_TRUETYPE_GLYF_GET      tt_service_truetype_glyf
#else /* FT_CONFIG_OPTION_PIC */
#include FT_MULTIPLE_MASTERS_H
#include FT_SERVICE_MULTIPLE_MASTERS_H
#include FT_SERVICE_TRUETYPE_GLYF_H
typedef struct TTModulePIC_
{
	FT_ServiceDescRec* tt_services;
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
	FT_Service_MultiMastersRec tt_service_gx_multi_masters;
#endif
	FT_Service_TTGlyfRec tt_service_truetype_glyf;
} TTModulePIC;
#define GET_PIC(lib)                         ((TTModulePIC*)((lib)->pic_container.truetype))
#define FT_TT_SERVICES_GET                   (GET_PIC(library)->tt_services)
#define FT_TT_SERVICE_GX_MULTI_MASTERS_GET   (GET_PIC(library)->tt_service_gx_multi_masters)
#define FT_TT_SERVICE_TRUETYPE_GLYF_GET      (GET_PIC(library)->tt_service_truetype_glyf)
/* see ttpic.c for the implementation */
void
	tt_driver_class_pic_free( FT_Library  library );
FT_Error
	tt_driver_class_pic_init( FT_Library  library );
#endif /* FT_CONFIG_OPTION_PIC */
#define FT_SERVICE_ID_TT_CMAP  "tt-cmaps"
typedef struct  TT_CMapInfo_
{
	ft_structs_h::FT_ULong  language;
	ft_structs_h::FT_Long   format;
} TT_CMapInfo;
typedef ft_structs_h::FT_Error
	(*TT_CMap_Info_GetFunc)( ft_structs_h::FT_CharMap    charmap,
	TT_CMapInfo  *cmap_info );
typedef ft_structs_h::FT_Error
	(*TT_CMap_ValidateFunc)( ft_structs_h::FT_Byte*      data,
	ft_structs_h::FT_Validator  valid );
FT_DEFINE_SERVICE( TTCMaps )
{
	TT_CMap_Info_GetFunc  get_cmap_info;
};
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DEFINE_SERVICE_TTCMAPSREC( class_, get_cmap_info_ )  \
	static const FT_Service_TTCMapsRec  class_ =                  \
{                                                             \
	get_cmap_info_                                              \
};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DEFINE_SERVICE_TTCMAPSREC( class_, get_cmap_info_ )  \
	void                                                          \
	FT_Init_Class_ ## class_( FT_Library              library,    \
	FT_Service_TTCMapsRec*  clazz )     \
{                                                             \
	FT_UNUSED( library );                                       \
	\
	clazz->get_cmap_info = get_cmap_info_;                      \
}
#endif /* FT_CONFIG_OPTION_PIC */
typedef const struct TT_CMap_ClassRec_*  TT_CMap_Class;
typedef struct  TT_CMap_ClassRec_
{
	ft_structs_h::FT_CMap_ClassRec      clazz;
	ft_structs_h::FT_UInt               format;
	TT_CMap_ValidateFunc  validate;
	TT_CMap_Info_GetFunc  get_cmap_info;
} TT_CMap_ClassRec;
	
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_cmap( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_pclt( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_get_metrics( TT_Face     face,
		ft_structs_h::FT_Bool     vertical,
		ft_structs_h::FT_UInt     gindex,
		ft_structs_h::FT_Short*   abearing,
		ft_structs_h::FT_UShort*  aadvance );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_hmtx( TT_Face    face,
		ft_structs_h::FT_Stream  stream,
		ft_structs_h::FT_Bool    vertical );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_font_dir( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Int )
		tt_face_get_kerning( TT_Face     face,
		ft_structs_h::FT_UInt     left_glyph,
		ft_structs_h::FT_UInt     right_glyph );
	FT_LOCAL( ft_structs_h::FT_Error  )
		tt_face_load_kern( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_gasp( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( void )
		tt_face_free_name( TT_Face  face );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_maxp( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_name( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_os2( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_post( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_driver_init( ft_structs_h::FT_Module  ttdriver );    /* TT_Driver */
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_hhea( TT_Face    face,
		ft_structs_h::FT_Stream  stream,
		ft_structs_h::FT_Bool    vertical );
	FT_LOCAL( void )
		tt_driver_done( ft_structs_h::FT_Module  ttdriver );    /* TT_Driver */
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_size_init( ft_structs_h::FT_Size  ttsize );          /* TT_Size */
	FT_LOCAL( void )
		tt_size_done( ft_structs_h::FT_Size  ttsize );          /* TT_Size */
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_slot_init( ft_structs_h::FT_GlyphSlot  slot );
	FT_LOCAL( void )
		tt_face_done_loca( TT_Face  face );
	FT_LOCAL( void )
		tt_face_free_hdmx( TT_Face  face );
	FT_LOCAL( void )
		TT_Get_HMetrics( TT_Face     face,
		ft_structs_h::FT_UInt     idx,
		ft_structs_h::FT_Short*   lsb,
		ft_structs_h::FT_UShort*  aw );
	FT_LOCAL( void )
		TT_Get_VMetrics( TT_Face     face,
		ft_structs_h::FT_UInt     idx,
		ft_structs_h::FT_Short*   tsb,
		ft_structs_h::FT_UShort*  ah );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_size_reset( TT_Size  size );
	FT_LOCAL( ft_structs_h::FT_Error )
		TT_Load_Glyph( TT_Size       size,
		TT_GlyphSlot  glyph,
		ft_structs_h::FT_UInt       glyph_index,
		ft_structs_h::FT_Int32      load_flags );
	FT_LOCAL( ft_structs_h::FT_ULong )
		tt_face_get_location( TT_Face   face,
		ft_structs_h::FT_UInt   gindex,
		ft_structs_h::FT_UInt  *asize );
	FT_LOCAL( ft_structs_h::FT_Byte* )
		tt_face_get_device_metrics( TT_Face    face,
		ft_structs_h::FT_UInt    ppem,
		ft_structs_h::FT_UInt    gindex );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_init( ft_structs_h::FT_Stream      stream,
		ft_structs_h::FT_Face        ttface,      /* TT_Face */
		ft_structs_h::FT_Int         face_index,
		ft_structs_h::FT_Int         num_params,
		ft_structs_h::FT_Parameter*  params );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_hdmx( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_loca( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_cvt( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_fpgm( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_prep( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( void )
		TT_Init_Glyph_Loading( TT_Face  face );	
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_goto_table( TT_Face    face,
		ft_structs_h::FT_ULong   tag,
		ft_structs_h::FT_Stream  stream,
		ft_structs_h::FT_ULong*  length );
	FT_LOCAL( TT_Table  )
		tt_face_lookup_table( TT_Face   face,
		ft_structs_h::FT_ULong  tag );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_any( TT_Face    face,
		ft_structs_h::FT_ULong   tag,
		ft_structs_h::FT_Long    offset,
		ft_structs_h::FT_Byte*   buffer,
		ft_structs_h::FT_ULong*  length );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_load_head( TT_Face    face,
		ft_structs_h::FT_Stream  stream );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_get_cmap_info( ft_structs_h::FT_CharMap    charmap,
		TT_CMapInfo  *cmap_info );
	FT_LOCAL( void )
		tt_face_done_kern( TT_Face  face );
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_face_build_cmaps( TT_Face  face );
#define TT_VALIDATOR( x )          ((tt_h::TT_Validator)( x ))
#define TT_VALID_GLYPH_COUNT( x )  TT_VALIDATOR( x )->num_glyphs
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DEFINE_TT_CMAP(class_, size_, init_, done_, char_index_,          \
	char_next_, char_var_index_, char_var_default_, variant_list_,           \
	charvariant_list_,variantchar_list_,                                     \
	format_, validate_, get_cmap_info_)                                      \
	FT_CALLBACK_TABLE_DEF                                                      \
	const TT_CMap_ClassRec class_ =                                            \
	{                                                                          \
	{size_, init_, done_, char_index_,                                       \
	char_next_, char_var_index_, char_var_default_, variant_list_,          \
	charvariant_list_, variantchar_list_},                                  \
	format_, validate_, get_cmap_info_                                       \
	};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DEFINE_TT_CMAP(class_, size_, init_, done_, char_index_,          \
	char_next_, char_var_index_, char_var_default_, variant_list_,           \
	charvariant_list_,variantchar_list_,                                     \
	format_, validate_, get_cmap_info_)                                      \
	void                                                                       \
	FT_Init_Class_##class_( TT_CMap_ClassRec*  clazz )                         \
	{                                                                          \
	clazz->clazz.size = size_;                                               \
	clazz->clazz.init = init_;                                               \
	clazz->clazz.done = done_;                                               \
	clazz->clazz.char_index = char_index_;                                   \
	clazz->clazz.char_next = char_next_;                                     \
	clazz->clazz.char_var_index = char_var_index_;                           \
	clazz->clazz.char_var_default = char_var_default_;                       \
	clazz->clazz.variant_list = variant_list_;                               \
	clazz->clazz.charvariant_list = charvariant_list_;                       \
	clazz->clazz.variantchar_list = variantchar_list_;                       \
	clazz->format = format_;                                                 \
	clazz->validate = validate_;                                             \
	clazz->get_cmap_info = get_cmap_info_;                                   \
	}
#endif /* FT_CONFIG_OPTION_PIC */
};

namespace sfnt_h
{
	typedef enum  FT_Sfnt_Tag_
	{
		ft_sfnt_head = 0,    /* TT_Header     */
		ft_sfnt_maxp = 1,    /* TT_MaxProfile */
		ft_sfnt_os2  = 2,    /* TT_OS2        */
	ft_sfnt_hhea = 3,    /* TT_HoriHeader */
	ft_sfnt_vhea = 4,    /* TT_VertHeader */
	ft_sfnt_post = 5,    /* TT_Postscript */
	ft_sfnt_pclt = 6,    /* TT_PCLT       */
		sfnt_max   /* internal end mark */
	} FT_Sfnt_Tag;
	typedef struct  SFNT_HeaderRec_
	{
		ft_structs_h::FT_ULong   format_tag;
		ft_structs_h::FT_UShort  num_tables;
		ft_structs_h::FT_UShort  search_range;
	ft_structs_h::FT_UShort  entry_selector;
	ft_structs_h::FT_UShort  range_shift;
		ft_structs_h::FT_ULong   offset;  /* not in file */
	} SFNT_HeaderRec, *SFNT_Header;
	typedef struct  TTC_HeaderRec_
	{
		ft_structs_h::FT_ULong   tag;
		ft_structs_h::FT_Fixed   version;
		ft_structs_h::FT_Long    count;
		ft_structs_h::FT_ULong*  offsets;
	} TTC_HeaderRec;	
#ifndef FT_CONFIG_OPTION_PIC
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
#define FT_DEFINE_DRIVERS_OLD_INTERNAL(a) \
	a,
#else
#define FT_DEFINE_DRIVERS_OLD_INTERNAL(a)
#endif
#define FT_INTERNAL(a) \
	a,
#define FT_DEFINE_SFNT_INTERFACE(class_,                                     \
	goto_table_, init_face_, load_face_, done_face_, get_interface_,         \
	load_any_, load_sfnt_header_, load_directory_, load_head_,               \
	load_hhea_, load_cmap_, load_maxp_, load_os2_, load_post_,               \
	load_name_, free_name_, load_hdmx_stub_, free_hdmx_stub_,                \
	load_kern_, load_gasp_, load_pclt_, load_bhed_,                          \
	set_sbit_strike_stub_, load_sbits_stub_, find_sbit_image_,               \
	load_sbit_metrics_, load_sbit_image_, free_sbits_stub_,                  \
	get_psname_, free_psnames_, load_charmap_stub_, free_charmap_stub_,      \
	get_kerning_, load_font_dir_, load_hmtx_, load_eblc_, free_eblc_,        \
	set_sbit_strike_, load_strike_metrics_, get_metrics_ )                   \
	static const tt_h::SFNT_Interface class_ =                                       \
	{                                                                          \
	FT_INTERNAL(goto_table_) \
	FT_INTERNAL(init_face_) \
	FT_INTERNAL(load_face_) \
	FT_INTERNAL(done_face_) \
	FT_INTERNAL(get_interface_) \
	FT_INTERNAL(load_any_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_sfnt_header_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_directory_) \
	FT_INTERNAL(load_head_) \
	FT_INTERNAL(load_hhea_) \
	FT_INTERNAL(load_cmap_) \
	FT_INTERNAL(load_maxp_) \
	FT_INTERNAL(load_os2_) \
	FT_INTERNAL(load_post_) \
	FT_INTERNAL(load_name_) \
	FT_INTERNAL(free_name_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_hdmx_stub_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(free_hdmx_stub_) \
	FT_INTERNAL(load_kern_) \
	FT_INTERNAL(load_gasp_) \
	FT_INTERNAL(load_pclt_) \
	FT_INTERNAL(load_bhed_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(set_sbit_strike_stub_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_sbits_stub_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(find_sbit_image_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_sbit_metrics_) \
	FT_INTERNAL(load_sbit_image_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(free_sbits_stub_) \
	FT_INTERNAL(get_psname_) \
	FT_INTERNAL(free_psnames_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_charmap_stub_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(free_charmap_stub_) \
	FT_INTERNAL(get_kerning_) \
	FT_INTERNAL(load_font_dir_) \
	FT_INTERNAL(load_hmtx_) \
	FT_INTERNAL(load_eblc_) \
	FT_INTERNAL(free_eblc_) \
	FT_INTERNAL(set_sbit_strike_) \
	FT_INTERNAL(load_strike_metrics_) \
	FT_INTERNAL(get_metrics_) \
};
#else /* FT_CONFIG_OPTION_PIC */
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
#define FT_DEFINE_DRIVERS_OLD_INTERNAL(a, a_) \
	clazz->a = a_;
#else
#define FT_DEFINE_DRIVERS_OLD_INTERNAL(a, a_)
#endif
#define FT_INTERNAL(a, a_) \
	clazz->a = a_;
#define FT_DEFINE_SFNT_INTERFACE(class_,                                     \
	goto_table_, init_face_, load_face_, done_face_, get_interface_,         \
	load_any_, load_sfnt_header_, load_directory_, load_head_,               \
	load_hhea_, load_cmap_, load_maxp_, load_os2_, load_post_,               \
	load_name_, free_name_, load_hdmx_stub_, free_hdmx_stub_,                \
	load_kern_, load_gasp_, load_pclt_, load_bhed_,                          \
	set_sbit_strike_stub_, load_sbits_stub_, find_sbit_image_,               \
	load_sbit_metrics_, load_sbit_image_, free_sbits_stub_,                  \
	get_psname_, free_psnames_, load_charmap_stub_, free_charmap_stub_,      \
	get_kerning_, load_font_dir_, load_hmtx_, load_eblc_, free_eblc_,        \
	set_sbit_strike_, load_strike_metrics_, get_metrics_ )                   \
	void                                                                       \
	FT_Init_Class_##class_( FT_Library library, SFNT_Interface*  clazz )       \
	{                                                                          \
	FT_UNUSED(library);                                                      \
	FT_INTERNAL(goto_table,goto_table_) \
	FT_INTERNAL(init_face,init_face_) \
	FT_INTERNAL(load_face,load_face_) \
	FT_INTERNAL(done_face,done_face_) \
	FT_INTERNAL(get_interface,get_interface_) \
	FT_INTERNAL(load_any,load_any_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_sfnt_header,load_sfnt_header_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_directory,load_directory_) \
	FT_INTERNAL(load_head,load_head_) \
	FT_INTERNAL(load_hhea,load_hhea_) \
	FT_INTERNAL(load_cmap,load_cmap_) \
	FT_INTERNAL(load_maxp,load_maxp_) \
	FT_INTERNAL(load_os2,load_os2_) \
	FT_INTERNAL(load_post,load_post_) \
	FT_INTERNAL(load_name,load_name_) \
	FT_INTERNAL(free_name,free_name_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_hdmx_stub,load_hdmx_stub_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(free_hdmx_stub,free_hdmx_stub_) \
	FT_INTERNAL(load_kern,load_kern_) \
	FT_INTERNAL(load_gasp,load_gasp_) \
	FT_INTERNAL(load_pclt,load_pclt_) \
	FT_INTERNAL(load_bhed,load_bhed_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(set_sbit_strike_stub,set_sbit_strike_stub_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_sbits_stub,load_sbits_stub_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(find_sbit_image,find_sbit_image_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_sbit_metrics,load_sbit_metrics_) \
	FT_INTERNAL(load_sbit_image,load_sbit_image_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(free_sbits_stub,free_sbits_stub_) \
	FT_INTERNAL(get_psname,get_psname_) \
	FT_INTERNAL(free_psnames,free_psnames_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(load_charmap_stub,load_charmap_stub_) \
	FT_DEFINE_DRIVERS_OLD_INTERNAL(free_charmap_stub,free_charmap_stub_) \
	FT_INTERNAL(get_kerning,get_kerning_) \
	FT_INTERNAL(load_font_dir,load_font_dir_) \
	FT_INTERNAL(load_hmtx,load_hmtx_) \
	FT_INTERNAL(load_eblc,load_eblc_) \
	FT_INTERNAL(free_eblc,free_eblc_) \
	FT_INTERNAL(set_sbit_strike,set_sbit_strike_) \
	FT_INTERNAL(load_strike_metrics,load_strike_metrics_) \
	FT_INTERNAL(get_metrics,get_metrics_) \
	}
#endif /* FT_CONFIG_OPTION_PIC */
#ifndef FT_CONFIG_OPTION_PIC
#define FT_SFNT_SERVICES_GET             sfnt_services
#define FT_SFNT_SERVICE_GLYPH_DICT_GET   sfnt_service_glyph_dict
#define FT_SFNT_SERVICE_PS_NAME_GET      sfnt_service_ps_name
#define FT_TT_SERVICE_GET_CMAP_INFO_GET  tt_h::tt_service_get_cmap_info
#define FT_SFNT_SERVICES_GET             sfnt_services
#define FT_TT_CMAP_CLASSES_GET           tt_cmap_classes
#define FT_SFNT_SERVICE_SFNT_TABLE_GET   sfnt_service_sfnt_table
#define FT_SFNT_SERVICE_BDF_GET          sfnt_service_bdf
#define FT_SFNT_INTERFACE_GET            sfnt_interface
#else /* FT_CONFIG_OPTION_PIC */
/* some include files required for members of sfntModulePIC */
#include FT_SERVICE_GLYPH_DICT_H
#include FT_SERVICE_POSTSCRIPT_NAME_H
#include FT_SERVICE_SFNT_H
#include FT_SERVICE_TT_CMAP_H
#ifdef TT_CONFIG_OPTION_BDF
#include "ttbdf.h"
#include FT_SERVICE_BDF_H
#endif
#include FT_INTERNAL_DEBUG_H
#include FT_INTERNAL_STREAM_H
#include FT_INTERNAL_SFNT_H
#include "ttcmap.h"
typedef struct sfntModulePIC_
{
	FT_ServiceDescRec* sfnt_services;
	FT_Service_GlyphDictRec sfnt_service_glyph_dict;
	FT_Service_PsFontNameRec  sfnt_service_ps_name;
	FT_Service_TTCMapsRec  tt_service_get_cmap_info;
	TT_CMap_Class* tt_cmap_classes;
	FT_Service_SFNT_TableRec sfnt_service_sfnt_table;
#ifdef TT_CONFIG_OPTION_BDF
	FT_Service_BDFRec sfnt_service_bdf;
#endif
	SFNT_Interface sfnt_interface;
} sfntModulePIC;
#define GET_PIC(lib)                      ((sfntModulePIC*)((lib)->pic_container.sfnt))
#define FT_SFNT_SERVICES_GET              (GET_PIC(library)->sfnt_services)
#define FT_SFNT_SERVICE_GLYPH_DICT_GET    (GET_PIC(library)->sfnt_service_glyph_dict)
#define FT_SFNT_SERVICE_PS_NAME_GET       (GET_PIC(library)->sfnt_service_ps_name)
#define FT_TT_SERVICE_GET_CMAP_INFO_GET   (GET_PIC(library)->tt_service_get_cmap_info)
#define FT_SFNT_SERVICES_GET              (GET_PIC(library)->sfnt_services)
#define FT_TT_CMAP_CLASSES_GET            (GET_PIC(library)->tt_cmap_classes)
#define FT_SFNT_SERVICE_SFNT_TABLE_GET    (GET_PIC(library)->sfnt_service_sfnt_table)
#define FT_SFNT_SERVICE_BDF_GET           (GET_PIC(library)->sfnt_service_bdf)
#define FT_SFNT_INTERFACE_GET             (GET_PIC(library)->sfnt_interface)
/* see sfntpic.c for the implementation */
void
	sfnt_module_class_pic_free( FT_Library  library );
FT_Error
	sfnt_module_class_pic_init( FT_Library  library );
#endif /* FT_CONFIG_OPTION_PIC */
#define FT_SERVICE_ID_SFNT_TABLE  "sfnt-table"
/*
* Used to implement FT_Load_Sfnt_Table().
*/
typedef ft_structs_h::FT_Error
	(*FT_SFNT_TableLoadFunc)( ft_structs_h::FT_Face    face,
	ft_structs_h::FT_ULong   tag,
	ft_structs_h::FT_Long    offset,
	ft_structs_h::FT_Byte*   buffer,
	ft_structs_h::FT_ULong*  length );
/*
* Used to implement FT_Get_Sfnt_Table().
*/
typedef void*
	(*FT_SFNT_TableGetFunc)( ft_structs_h::FT_Face      face,
	FT_Sfnt_Tag  tag );
/*
* Used to implement FT_Sfnt_Table_Info().
*/
typedef ft_structs_h::FT_Error
	(*FT_SFNT_TableInfoFunc)( ft_structs_h::FT_Face    face,
	ft_structs_h::FT_UInt    idx,
	ft_structs_h::FT_ULong  *tag,
	ft_structs_h::FT_ULong  *offset,
	ft_structs_h::FT_ULong  *length );
FT_DEFINE_SERVICE( SFNT_Table )
{
	FT_SFNT_TableLoadFunc  load_table;
	FT_SFNT_TableGetFunc   get_table;
	FT_SFNT_TableInfoFunc  table_info;
};
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DEFINE_SERVICE_SFNT_TABLEREC( class_, load_, get_, info_ )  \
	static const FT_Service_SFNT_TableRec  class_ =                      \
{                                                                    \
	load_, get_, info_                                                 \
};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DEFINE_SERVICE_SFNT_TABLEREC( class_, load_, get_, info_ ) \
	void                                                                \
	FT_Init_Class_ ## class_( FT_Service_SFNT_TableRec*  clazz )        \
{                                                                   \
	clazz->load_table = load_;                                        \
	clazz->get_table  = get_;                                         \
	clazz->table_info = info_;                                        \
}
#endif /* FT_CONFIG_OPTION_PIC */
#define FT_SERVICE_ID_POSTSCRIPT_FONT_NAME  "postscript-font-name"
typedef const char*
	(*FT_PsName_GetFunc)( ft_structs_h::FT_Face  face );
FT_DEFINE_SERVICE( PsFontName )
{
	FT_PsName_GetFunc  get_ps_font_name;
};
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DEFINE_SERVICE_PSFONTNAMEREC( class_, get_ps_font_name_ ) \
	static const FT_Service_PsFontNameRec  class_ =                    \
{                                                                  \
	get_ps_font_name_                                                \
};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DEFINE_SERVICE_PSFONTNAMEREC( class_, get_ps_font_name_ ) \
	void                                                               \
	FT_Init_Class_ ## class_( FT_Library                 library,      \
	FT_Service_PsFontNameRec*  clazz )       \
{                                                                  \
	FT_UNUSED( library );                                            \
	\
	clazz->get_ps_font_name = get_ps_font_name_;                     \
}
#endif /* FT_CONFIG_OPTION_PIC */
FT_LOCAL( ft_structs_h::FT_Error )
	sfnt_init_face( ft_structs_h::FT_Stream      stream,
	tt_h::TT_Face        face,
	ft_structs_h::FT_Int         face_index,
	ft_structs_h::FT_Int         num_params,
	ft_structs_h::FT_Parameter*  params );
FT_LOCAL( ft_structs_h::FT_Error )
	sfnt_load_face( ft_structs_h::FT_Stream      stream,
	tt_h::TT_Face        face,
	ft_structs_h::FT_Int         face_index,
	ft_structs_h::FT_Int         num_params,
	ft_structs_h::FT_Parameter*  params );
FT_LOCAL( void )
	sfnt_done_face( tt_h::TT_Face  face );
};
namespace ft_winfnt_h
{
#define FT_SERVICE_ID_XF86_NAME  "xf86-driver-name"
#define FT_XF86_FORMAT_TRUETYPE  "TrueType"
#define FT_XF86_FORMAT_TYPE_1    "Type 1"
#define FT_XF86_FORMAT_BDF       "BDF"
#define FT_XF86_FORMAT_PCF       "PCF"
#define FT_XF86_FORMAT_TYPE_42   "Type 42"
#define FT_XF86_FORMAT_CID       "CID Type 1"
#define FT_XF86_FORMAT_CFF       "CFF"
#define FT_XF86_FORMAT_PFR       "PFR"
#define FT_XF86_FORMAT_WINFNT    "Windows FNT"
#define FT_WinFNT_ID_CP1252    0
#define FT_WinFNT_ID_DEFAULT   1
#define FT_WinFNT_ID_SYMBOL    2
#define FT_WinFNT_ID_MAC      77
#define FT_WinFNT_ID_CP932   128
#define FT_WinFNT_ID_CP949   129
#define FT_WinFNT_ID_CP1361  130
#define FT_WinFNT_ID_CP936   134
#define FT_WinFNT_ID_CP950   136
#define FT_WinFNT_ID_CP1253  161
#define FT_WinFNT_ID_CP1254  162
#define FT_WinFNT_ID_CP1258  163
#define FT_WinFNT_ID_CP1255  177
#define FT_WinFNT_ID_CP1256  178
#define FT_WinFNT_ID_CP1257  186
#define FT_WinFNT_ID_CP1251  204
#define FT_WinFNT_ID_CP874   222
#define FT_WinFNT_ID_CP1250  238
#define FT_WinFNT_ID_OEM     255
#define FT_SERVICE_ID_WINFNT  "winfonts"
	typedef struct  FT_WinFNT_HeaderRec_
  {
	ft_structs_h::FT_UShort  version;
	ft_structs_h::FT_ULong   file_size;
	ft_structs_h::FT_Byte    copyright[60];
	ft_structs_h::FT_UShort  file_type;
	ft_structs_h::FT_UShort  nominal_point_size;
	ft_structs_h::FT_UShort  vertical_resolution;
	ft_structs_h::FT_UShort  horizontal_resolution;
	ft_structs_h::FT_UShort  ascent;
	ft_structs_h::FT_UShort  internal_leading;
	ft_structs_h::FT_UShort  external_leading;
	ft_structs_h::FT_Byte    italic;
	ft_structs_h::FT_Byte    underline;
	ft_structs_h::FT_Byte    strike_out;
	ft_structs_h::FT_UShort  weight;
	ft_structs_h::FT_Byte    charset;
	ft_structs_h::FT_UShort  pixel_width;
	ft_structs_h::FT_UShort  pixel_height;
	ft_structs_h::FT_Byte    pitch_and_family;
	ft_structs_h::FT_UShort  avg_width;
	ft_structs_h::FT_UShort  max_width;
	ft_structs_h::FT_Byte    first_char;
	ft_structs_h::FT_Byte    last_char;
	ft_structs_h::FT_Byte    default_char;
	ft_structs_h::FT_Byte    break_char;
	ft_structs_h::FT_UShort  bytes_per_row;
	ft_structs_h::FT_ULong   device_offset;
	ft_structs_h::FT_ULong   face_name_offset;
	ft_structs_h::FT_ULong   bits_pointer;
	ft_structs_h::FT_ULong   bits_offset;
	ft_structs_h::FT_Byte    reserved;
	ft_structs_h::FT_ULong   flags;
	ft_structs_h::FT_UShort  A_space;
	ft_structs_h::FT_UShort  B_space;
	ft_structs_h::FT_UShort  C_space;
	ft_structs_h::FT_UShort  color_table_offset;
	ft_structs_h::FT_ULong   reserved1[4];
	} FT_WinFNT_HeaderRec;
  typedef struct FT_WinFNT_HeaderRec_*  FT_WinFNT_Header;
	typedef ft_structs_h::FT_Error
		(*FT_WinFnt_GetHeaderFunc)( ft_structs_h::FT_Face               face,
		FT_WinFNT_HeaderRec  *aheader );
	FT_DEFINE_SERVICE( WinFnt )
	{
		FT_WinFnt_GetHeaderFunc  get_header;
  };	
	static const ft_structs_h::FT_Frame_Field  winfnt_header_fields[] =
	{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  FT_WinFNT_HeaderRec
		FT_FRAME_START( 148 ),
		FT_FRAME_USHORT_LE( version ),
		FT_FRAME_ULONG_LE ( file_size ),
		FT_FRAME_BYTES    ( copyright, 60 ),
		FT_FRAME_USHORT_LE( file_type ),
		FT_FRAME_USHORT_LE( nominal_point_size ),
		FT_FRAME_USHORT_LE( vertical_resolution ),
		FT_FRAME_USHORT_LE( horizontal_resolution ),
		FT_FRAME_USHORT_LE( ascent ),
		FT_FRAME_USHORT_LE( internal_leading ),
		FT_FRAME_USHORT_LE( external_leading ),
		FT_FRAME_BYTE     ( italic ),
		FT_FRAME_BYTE     ( underline ),
		FT_FRAME_BYTE     ( strike_out ),
		FT_FRAME_USHORT_LE( weight ),
		FT_FRAME_BYTE     ( charset ),
		FT_FRAME_USHORT_LE( pixel_width ),
		FT_FRAME_USHORT_LE( pixel_height ),
		FT_FRAME_BYTE     ( pitch_and_family ),
		FT_FRAME_USHORT_LE( avg_width ),
		FT_FRAME_USHORT_LE( max_width ),
		FT_FRAME_BYTE     ( first_char ),
		FT_FRAME_BYTE     ( last_char ),
		FT_FRAME_BYTE     ( default_char ),
		FT_FRAME_BYTE     ( break_char ),
		FT_FRAME_USHORT_LE( bytes_per_row ),
		FT_FRAME_ULONG_LE ( device_offset ),
		FT_FRAME_ULONG_LE ( face_name_offset ),
		FT_FRAME_ULONG_LE ( bits_pointer ),
		FT_FRAME_ULONG_LE ( bits_offset ),
		FT_FRAME_BYTE     ( reserved ),
		FT_FRAME_ULONG_LE ( flags ),
		FT_FRAME_USHORT_LE( A_space ),
		FT_FRAME_USHORT_LE( B_space ),
		FT_FRAME_USHORT_LE( C_space ),
		FT_FRAME_ULONG_LE ( color_table_offset ),
		FT_FRAME_BYTES    ( reserved1, 16 ),
		FT_FRAME_END
	};
	typedef struct  FNT_FontRec_
	{
		ft_structs_h::FT_ULong             offset;
		FT_WinFNT_HeaderRec  header;
		ft_structs_h::FT_Byte*             fnt_frame;
		ft_structs_h::FT_ULong             fnt_size;
		ft_structs_h::FT_String*           family_name;
	} FNT_FontRec, *FNT_Font;
	typedef struct  FNT_FaceRec_
	{
		ft_structs_h::FT_FaceRec     root;
		FNT_Font       font;
		ft_structs_h::FT_CharMap     charmap_handle;
		ft_structs_h::FT_CharMapRec  charmap;  /* a single charmap per face */
	} FNT_FaceRec, *FNT_Face;
	typedef struct  WinMZ_HeaderRec_
	{
		ft_structs_h::FT_UShort  magic;
		/* skipped content */
		ft_structs_h::FT_UShort  lfanew;
	} WinMZ_HeaderRec;
	typedef struct  FNT_CMapRec_
	{
		ft_structs_h::FT_CMapRec  cmap;
		ft_structs_h::FT_UInt32   first;
		ft_structs_h::FT_UInt32   count;
  } FNT_CMapRec, *FNT_CMap;
	typedef struct  WinNE_HeaderRec_
	{
		ft_structs_h::FT_UShort  magic;
		/* skipped content */
		ft_structs_h::FT_UShort  resource_tab_offset;
		ft_structs_h::FT_UShort  rname_tab_offset;
	} WinNE_HeaderRec;
	typedef struct  WinPE32_HeaderRec_
	{
		ft_structs_h::FT_ULong   magic;
		ft_structs_h::FT_UShort  machine;
		ft_structs_h::FT_UShort  number_of_sections;
		/* skipped content */
		ft_structs_h::FT_UShort  size_of_optional_header;
		/* skipped content */
		ft_structs_h::FT_UShort  magic32;
		/* skipped content */
		ft_structs_h::FT_ULong   rsrc_virtual_address;
		ft_structs_h::FT_ULong   rsrc_size;
		/* skipped content */
	} WinPE32_HeaderRec;	
	typedef struct  WinPE32_SectionRec_
  {
	ft_structs_h::FT_Byte   name[8];
	/* skipped content */
	ft_structs_h::FT_ULong  virtual_address;
	ft_structs_h::FT_ULong  size_of_raw_data;
	ft_structs_h::FT_ULong  pointer_to_raw_data;
	/* skipped content */

  } WinPE32_SectionRec;
	typedef struct  WinPE_RsrcDirRec_
	{
		ft_structs_h::FT_ULong   characteristics;
		ft_structs_h::FT_ULong   time_date_stamp;
		ft_structs_h::FT_UShort  major_version;
		ft_structs_h::FT_UShort  minor_version;
		ft_structs_h::FT_UShort  number_of_named_entries;
		ft_structs_h::FT_UShort  number_of_id_entries;
	} WinPE_RsrcDirRec;
	typedef struct  WinPE_RsrcDirEntryRec_
	{
		ft_structs_h::FT_ULong  name;
		ft_structs_h::FT_ULong  offset;
	} WinPE_RsrcDirEntryRec;
	typedef struct  WinPE_RsrcDataEntryRec_
	{
		ft_structs_h::FT_ULong  offset_to_data;
		ft_structs_h::FT_ULong  size;
		ft_structs_h::FT_ULong  code_page;
		ft_structs_h::FT_ULong  reserved;
	} WinPE_RsrcDataEntryRec;
};
namespace smooth_h
{
	typedef void
		(*FT_SpanFunc)( int             y,
		int             count,
		const ft_structs_h::FT_Span*  spans,
		void*           user );
#define FT_Raster_Span_Func  FT_SpanFunc
	typedef int
		(*FT_Raster_NewFunc)( void*       memory,
		ft_structs_h::FT_Raster*  raster );
#define FT_Raster_New_Func  FT_Raster_NewFunc
	typedef void
		(*FT_Raster_DoneFunc)( ft_structs_h::FT_Raster  raster );
#define FT_Raster_Done_Func  FT_Raster_DoneFunc
	typedef void
		(*FT_Raster_ResetFunc)( ft_structs_h::FT_Raster       raster,
		unsigned char*  pool_base,
		unsigned long   pool_size );
#define FT_Raster_Reset_Func  FT_Raster_ResetFunc
	typedef int
		(*FT_Raster_SetModeFunc)( ft_structs_h::FT_Raster      raster,
		unsigned long  mode,
		void*          args );
#define FT_Raster_Set_Mode_Func  FT_Raster_SetModeFunc
	typedef int
		(*FT_Raster_RenderFunc)( ft_structs_h::FT_Raster                raster,
		const ft_structs_h::FT_Raster_Params*  params );
#define FT_Raster_Render_Func  FT_Raster_RenderFunc
#ifndef FT_CONFIG_OPTION_PIC
#define FT_DEFINE_RASTER_FUNCS(class_, glyph_format_, raster_new_,           \
	raster_reset_, raster_set_mode_,              \
	raster_render_, raster_done_)                 \
	const ft_structs_h::FT_Raster_Funcs class_ =                                      \
	{                                                                          \
	glyph_format_, raster_new_, raster_reset_,                               \
	raster_set_mode_, raster_render_, raster_done_                           \
};
#else /* FT_CONFIG_OPTION_PIC */
#define FT_DEFINE_RASTER_FUNCS(class_, glyph_format_, raster_new_,           \
	raster_reset_, raster_set_mode_, raster_render_, raster_done_)           \
	void                                                                       \
	FT_Init_Class_##class_( FT_Raster_Funcs*  clazz )                          \
	{                                                                          \
	clazz->glyph_format = glyph_format_;                                     \
	clazz->raster_new = raster_new_;                                         \
	clazz->raster_reset = raster_reset_;                                     \
	clazz->raster_set_mode = raster_set_mode_;                               \
	clazz->raster_render = raster_render_;                                   \
	clazz->raster_done = raster_done_;                                       \
}
#endif /* FT_CONFIG_OPTION_PIC */
#ifndef FT_CONFIG_OPTION_PIC
#define FT_GRAYS_RASTER_GET        smooth_h::ft_grays_raster
#else /* FT_CONFIG_OPTION_PIC */
	typedef struct SmoothPIC_
	{
		int ref_count;
		FT_Raster_Funcs ft_grays_raster;
	} SmoothPIC;
#define GET_PIC(lib)               ((SmoothPIC*)((lib)->pic_container.smooth))
#define FT_GRAYS_RASTER_GET        (GET_PIC(library)->ft_grays_raster)
	/* see ftspic.c for the implementation */
	void
		ft_smooth_renderer_class_pic_free( FT_Library  library );
	void
		ft_smooth_lcd_renderer_class_pic_free( FT_Library  library );
	void
		ft_smooth_lcdv_renderer_class_pic_free( FT_Library  library );
	FT_Error
		ft_smooth_renderer_class_pic_init( FT_Library  library );
	FT_Error
		ft_smooth_lcd_renderer_class_pic_init( FT_Library  library );
	FT_Error
		ft_smooth_lcdv_renderer_class_pic_init( FT_Library  library );
#endif /* FT_CONFIG_OPTION_PIC */
};

FT_EXPORT( ft_structs_h::FT_Error )
	FT_New_Library( ft_structs_h::FT_Memory    memory,
	ft_structs_h::FT_Library  *alibrary );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Init_FreeType( ft_structs_h::FT_Library  *alibrary );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_New_Face( ft_structs_h::FT_Library   library,
	const char*  filepathname,
	ft_structs_h::FT_Long      face_index,
	ft_structs_h::FT_Face     *aface );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Set_Pixel_Sizes( ft_structs_h::FT_Face  face,
	ft_structs_h::FT_UInt  pixel_width,
	ft_structs_h::FT_UInt  pixel_height );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Done_Face( ft_structs_h::FT_Face  face );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Done_FreeType( ft_structs_h::FT_Library  library );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Done_Library( ft_structs_h::FT_Library  library );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Request_Size( ft_structs_h::FT_Face          face,
	ft_structs_h::FT_Size_Request  req );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Select_Size( ft_structs_h::FT_Face  face,
	ft_structs_h::FT_Int   strike_index );
FT_EXPORT( ft_structs_h::FT_Long )
	FT_DivFix( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Open_Face( ft_structs_h::FT_Library           library,
	const ft_structs_h::FT_Open_Args*  args,
	ft_structs_h::FT_Long              face_index,
	ft_structs_h::FT_Face             *aface );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_New_Size( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_Size*  size );
FT_EXPORT( void )
	FT_Vector_Transform( ft_structs_h::FT_Vector*        vec,
	const ft_structs_h::FT_Matrix*  matrix );
FT_EXPORT( void )
	FT_Outline_Translate( const ft_structs_h::FT_Outline*  outline,
	ft_structs_h::FT_Pos             xOffset,
	ft_structs_h::FT_Pos             yOffset );
FT_EXPORT( void )
	FT_Outline_Get_CBox( const ft_structs_h::FT_Outline*  outline,
	ft_structs_h::FT_BBox           *acbox );
FT_EXPORT( ft_structs_h::FT_Module )
	FT_Get_Module( ft_structs_h::FT_Library   library,
	const char*  module_name );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Get_Glyph_Name( ft_structs_h::FT_Face     face,
	ft_structs_h::FT_UInt     glyph_index,
	ft_structs_h::FT_Pointer  buffer,
	ft_structs_h::FT_UInt     buffer_max );
#ifdef FT_MULFIX_INLINED
#define FT_MulFix( a, b )  FT_MULFIX_INLINED( a, b )
#else
FT_EXPORT( ft_structs_h::FT_Long )
	FT_MulFix( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b );
#endif
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Load_Char( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_ULong  char_code,
	ft_structs_h::FT_Int32  load_flags );
FT_EXPORT( ft_structs_h::FT_UInt )
	FT_Get_Char_Index( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_ULong  charcode );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Load_Glyph( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_UInt   glyph_index,
	ft_structs_h::FT_Int32  load_flags );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Outline_Check( ft_structs_h::FT_Outline*  outline );
FT_EXPORT( void )
	FT_Outline_Transform( const ft_structs_h::FT_Outline*  outline,
	const ft_structs_h::FT_Matrix*   matrix );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Render_Glyph( ft_structs_h::FT_GlyphSlot    slot,
	ft_structs_h::FT_Render_Mode  render_mode );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Set_Renderer( ft_structs_h::FT_Library     library,
	ft_structs_h::FT_Renderer    renderer,
	ft_structs_h::FT_UInt        num_params,
	ft_structs_h::FT_Parameter*  parameters );
FT_EXPORT( void )
	FT_List_Up( ft_structs_h::FT_List      list,
	ft_structs_h::FT_ListNode  node );
FT_EXPORT( ft_structs_h::FT_ListNode )
	FT_List_Find( ft_structs_h::FT_List  list,
	void*    data );
FT_EXPORT( ft_structs_h::FT_Long )
	FT_MulDiv( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b,
	ft_structs_h::FT_Long  c );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Outline_Decompose( ft_structs_h::FT_Outline*              outline,
	const ft_structs_h::FT_Outline_Funcs*  func_interface,
	void*                    user );
FT_EXPORT( ft_structs_h::FT_Error )
	FT_Select_Charmap( ft_structs_h::FT_Face      face,
	ft_structs_h::FT_Encoding  encoding );
#endif
