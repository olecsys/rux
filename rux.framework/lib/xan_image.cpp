#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_image.h>
#include <xan_console.h>
#include <xan_path.h>
#include <xan_file.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_png.h>
#include <algorithm>
#include <xan_uint64.h>
#include <xan_boolean.h>
#include <xan_png.h>
begin_implement_rux_class_ns( Frame , rux::media );
	_media_image = NULL;
	_image_data = NULL;
	_image_data_size = 0;	
end_implement_rux_class();
namespace rux
{
	namespace media
	{		
		Frame::~Frame()
		{
			if( _media_image )
				_media_image->release();
			if( _image_data )
				rux::engine::free_mem( _image_data );
		};
		rux::uint32 XFrame::get_ImageSize( ::rux::int32 fourcc , ::rux::uint32 width , ::rux::uint32 height , ::rux::int16 bit_count )
		{
			if( fourcc == ::rux::media::XEnum_Fourcc_I420 )
				return width * height + width * height / 2;
			else if( fourcc == ::rux::media::XEnum_Fourcc_YV12 )
				return width * height + width * height / 2;
			else if( fourcc == ::rux::media::XEnum_Fourcc_RGB
				|| fourcc == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( bit_count == 24 )
					return width * height * 3;
				else if( bit_count == 32 )
					return width * height * 4;
				else
					return 0;
			}
			else if( fourcc == ::rux::media::XEnum_Fourcc_YUYV )
				return 2 * width * height;
			else
				return 0;
		};
		void XFrame::set_ImageData( char* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::media::XEnum_Fourcc fourcc , ::rux::XString& error )
		{
			(*this)()->set_ImageData( image_data , image_width , image_height , fourcc , error );
		}
		void Frame::set_ImageData( char* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::media::XEnum_Fourcc fourcc , ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _media_image )
			{
				_media_image->release();
				_media_image = NULL;
			}
			if( image_data )
			{
				if( image_width > 0 )
				{
					if( image_height > 0 )
					{
						_bitmap_info_header._size_image = 0;
						if( fourcc == ::rux::media::XEnum_Fourcc_I420 )
						{
							_bitmap_info_header._size_image = image_width * image_height + image_width * image_height / 2;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_YV12 )
						{
							_bitmap_info_header._size_image = image_width * image_height + image_width * image_height / 2;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_YUYV )
						{
							_bitmap_info_header._size_image = 2 * image_width * image_height;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_RGB
							|| fourcc == ::rux::media::XEnum_Fourcc_BI_RGB )
						{
							_bitmap_info_header._bit_count = 24;
							_bitmap_info_header._size_image = image_width * image_height * 3;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else
						{
							char sfcc[5] = {0}; *((::rux::uint32*)sfcc) = fourcc;
							rux_write_error( ::rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );
						}
						if( _image_data && _image_data_size < _bitmap_info_header._size_image )
						{
							::rux::engine::free_mem( _image_data );
							_image_data = 0;
							_image_data_size = 0;
						}
						if( _image_data == NULL && _bitmap_info_header._size_image > 0 )
						{
							_image_data = alloc_array_macros( ::rux::uint8 , _bitmap_info_header._size_image );
							_image_data_size = _bitmap_info_header._size_image;
						}
						if( _image_data )
							memcpy( _image_data , image_data , _bitmap_info_header._size_image );
					}
					else
					{
						rux_write_error( "image height is 0" , error );
					}
				}
				else
				{
					rux_write_error( "image width is 0" , error );
				}
			}
			else
			{
				rux_write_error( "image data is NULL" , error );
			}
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_function_1( Frame , CopyFrom , ::rux::media::XFrame& );
		void Frame::CopyFrom( ::rux::media::XFrame& frame )
		{
			WRITE_LOCK( _cs_image_data );
			if( _media_image )
			{
				_media_image->release();
				_media_image = 0;
			}
			frame.LockData();
			if( _image_data_size < frame()->_image_data_size )
			{
				_image_data_size = frame()->_image_data_size;
				if( _image_data )
				{
					::rux::engine::free_mem( _image_data );
					_image_data = 0;
				}
			}
			if( _image_data == 0 )
				_image_data = alloc_array_macros( ::rux::byte , _image_data_size );
			memcpy( _image_data , frame()->_image_data , _image_data_size );
			memcpy( &_bitmap_info_header , &frame()->_bitmap_info_header , sizeof( _bitmap_info_header ) );
			frame.UnLockData();
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_function_4( Frame , set_ImageData , char* , ::rux::uint32 , ::rux::uint32 , ::rux::media::XEnum_Fourcc );
		void Frame::set_ImageData( char* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::media::XEnum_Fourcc fourcc )
		{	
			WRITE_LOCK( _cs_image_data );
			if( _media_image )
			{
				_media_image->release();
				_media_image = NULL;
			}
			if( image_data )
			{
				if( image_width > 0 )
				{
					if( image_height > 0 )
					{
						_bitmap_info_header._size_image = 0;
						if( fourcc == ::rux::media::XEnum_Fourcc_I420 )
						{
							_bitmap_info_header._size_image = image_width * image_height + image_width * image_height / 2;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_YV12 )
						{
							_bitmap_info_header._size_image = image_width * image_height + image_width * image_height / 2;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_YUYV )
						{
							_bitmap_info_header._size_image = 2 * image_width * image_height;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_RGB
							|| fourcc == ::rux::media::XEnum_Fourcc_BI_RGB )
						{
							_bitmap_info_header._bit_count = 24;
							_bitmap_info_header._size_image = image_width * image_height * 3;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}				
						if( _image_data && _image_data_size < _bitmap_info_header._size_image )
						{
							rux::engine::free_mem( _image_data );
							_image_data = 0;
							_image_data_size = 0;
						}
						if( _image_data == NULL && _bitmap_info_header._size_image > 0 )
						{
							_image_data = alloc_array_macros( ::rux::uint8 , _bitmap_info_header._size_image );
							_image_data_size = _bitmap_info_header._size_image;
						}
						if( _image_data )
							memcpy( _image_data , image_data , _bitmap_info_header._size_image );
					}			
				}		
			}	
			_cs_image_data.WriteUnlock();
		};		
		implement_duplicate_internal_function_6( Frame , set_ImageData , char* , ::rux::uint32 , ::rux::uint32 , ::rux::media::XEnum_Fourcc , uint16 , ::rux::XString& );
		void Frame::set_ImageData( char* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::media::XEnum_Fourcc fourcc , uint16 bits_count , ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _media_image )
			{
				_media_image->release();
				_media_image = NULL;
			}
			if( image_data )
			{
				if( image_width > 0 )
				{
					if( image_height > 0 )
					{
						_bitmap_info_header._size_image = 0;
						if( fourcc == ::rux::media::XEnum_Fourcc_I420 )
						{
							_bitmap_info_header._size_image = image_width * image_height + image_width * image_height / 2;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_YV12 )
						{
							_bitmap_info_header._size_image = image_width * image_height + image_width * image_height / 2;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_YUYV )
						{
							_bitmap_info_header._size_image = 2 * image_width * image_height;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_RGB
							|| fourcc == ::rux::media::XEnum_Fourcc_BI_RGB )
						{
							if( bits_count == 8 )
							{
								_bitmap_info_header._bit_count = 8;
								_bitmap_info_header._size_image = image_width * image_height;
								_bitmap_info_header._compression = fourcc;
								_bitmap_info_header._width = image_width;
								_bitmap_info_header._height = image_height;		
							}
							else if( bits_count == 16 )
							{
								_bitmap_info_header._bit_count = 16;
								_bitmap_info_header._size_image = image_width * image_height * 2;
								_bitmap_info_header._compression = fourcc;
								_bitmap_info_header._width = image_width;
								_bitmap_info_header._height = image_height;		
							}
							else if( bits_count == 24 )
							{
								_bitmap_info_header._bit_count = 24;
								_bitmap_info_header._size_image = image_width * image_height * 3;
								_bitmap_info_header._compression = fourcc;
								_bitmap_info_header._width = image_width;
								_bitmap_info_header._height = image_height;		
							}
							else if( bits_count == 32 )
							{
								_bitmap_info_header._bit_count = 32;
								_bitmap_info_header._size_image = image_width * image_height * 4;
								_bitmap_info_header._compression = fourcc;
								_bitmap_info_header._width = image_width;
								_bitmap_info_header._height = image_height;		
							}
						}
						else
						{
							char sfcc[5] = {0}; *((::rux::uint32*)sfcc) = fourcc;
							rux_write_error( ::rux::XString::Format( " , image fourcc '%s' is unsupported" , sfcc ) , error );
						}
						if( _image_data && _image_data_size < _bitmap_info_header._size_image )
						{
							::rux::engine::free_mem( _image_data );
							_image_data = 0;
							_image_data_size = 0;
						}
						if( _image_data == NULL && _bitmap_info_header._size_image > 0 )
						{
							_image_data = alloc_array_macros( ::rux::uint8 , _bitmap_info_header._size_image );
							_image_data_size = _bitmap_info_header._size_image;
						}
						if( _image_data )
							memcpy( _image_data , image_data , _bitmap_info_header._size_image );
					}
					else
					{
						rux_write_error( "image height is 0" , error );
					}
				}
				else
				{
					rux_write_error( "image width is 0" , error );
				}
			}
			else
			{
				rux_write_error( "image data is NULL" , error );
			}
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_function_6( Frame , set_ImageData , char* , ::rux::uint32 , ::rux::uint32 , ::rux::media::XEnum_Fourcc , uint16 , ::rux::byte );
		void Frame::set_ImageData( char* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::media::XEnum_Fourcc fourcc , uint16 bits_count , ::rux::byte copy )
		{	
			WRITE_LOCK( _cs_image_data );
			if( _media_image )
			{
				_media_image->release();
				_media_image = NULL;
			}
			if( image_data )
			{
				if( image_width > 0 )
				{
					if( image_height > 0 )
					{
						_bitmap_info_header._size_image = 0;
						if( fourcc == ::rux::media::XEnum_Fourcc_I420 )
						{
							_bitmap_info_header._size_image = image_width * image_height + image_width * image_height / 2;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_YV12 )
						{
							_bitmap_info_header._size_image = image_width * image_height + image_width * image_height / 2;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_YUYV )
						{
							_bitmap_info_header._size_image = 2 * image_width * image_height;
							_bitmap_info_header._compression = fourcc;
							_bitmap_info_header._width = image_width;
							_bitmap_info_header._height = image_height;		
						}
						else if( fourcc == ::rux::media::XEnum_Fourcc_RGB
							|| fourcc == ::rux::media::XEnum_Fourcc_BI_RGB )
						{
							if( bits_count == 8 )
							{
								_bitmap_info_header._bit_count = 8;
								_bitmap_info_header._size_image = image_width * image_height;
								_bitmap_info_header._compression = fourcc;
								_bitmap_info_header._width = image_width;
								_bitmap_info_header._height = image_height;		
							}
							else if( bits_count == 16 )
							{
								_bitmap_info_header._bit_count = 16;
								_bitmap_info_header._size_image = image_width * image_height * 2;
								_bitmap_info_header._compression = fourcc;
								_bitmap_info_header._width = image_width;
								_bitmap_info_header._height = image_height;		
							}
							else if( bits_count == 24 )
							{
								_bitmap_info_header._bit_count = 24;
								_bitmap_info_header._size_image = image_width * image_height * 3;
								_bitmap_info_header._compression = fourcc;
								_bitmap_info_header._width = image_width;
								_bitmap_info_header._height = image_height;		
							}
							else if( bits_count == 32 )
							{
								_bitmap_info_header._bit_count = 32;
								_bitmap_info_header._size_image = image_width * image_height * 4;
								_bitmap_info_header._compression = fourcc;
								_bitmap_info_header._width = image_width;
								_bitmap_info_header._height = image_height;		
							}
						}
						if( _image_data && ( _image_data_size < _bitmap_info_header._size_image || copy == 0 ) )
						{
							::rux::engine::free_mem( _image_data );
							_image_data = 0;
							_image_data_size = 0;
						}
						if( copy )
						{
							if( _image_data == NULL && _bitmap_info_header._size_image > 0 )
							{							
								_image_data = alloc_array_macros( ::rux::uint8 , _bitmap_info_header._size_image );
								_image_data_size = _bitmap_info_header._size_image;
							}
							if( _image_data )
								memcpy( _image_data , image_data , _bitmap_info_header._size_image );
						}
						else
						{
							_image_data = (::rux::byte*)image_data;
							_image_data_size = _bitmap_info_header._size_image;
						}
					}
				}
			}
			_cs_image_data.WriteUnlock();
		};
		void Frame::check_size( ::rux::int32 size )
		{
			if( _image_data && _image_data_size < size )
			{
				rux::engine::free_mem( _image_data );
				_image_data = 0;
				_image_data_size = 0;
			}
			if( _image_data == NULL && size > 0 )
			{							
				_image_data = alloc_array_macros( ::rux::uint8 , size );
				_image_data_size = size;
			}					
		};
		void XFrame::set_ImageData( const ::rux::XString& filename , ::rux::XString& error )
		{
			(*this)()->set_ImageData( filename , error );
		};
		::rux::byte XFrame::Exists( const rux::XString& filename )
		{
			::rux::byte exists = 0;
			rux::XString cp_filename;
			cp_filename.set_ByRef( filename.ConvertToAnsi() );
			if( ::rux::io::XFile::Exists( cp_filename ) == 0 )
			{
				declare_stack_variable( char , executable_directory , 1024 );
				::rux::safe_strncpy( executable_directory , ::rux::engine::_globals->_executable_directory , 1024 );
				cp_filename.set_ByRef( ::rux::io::XPath::CalculateAbsolute( executable_directory , cp_filename ) );
				if( ::rux::io::XFile::Exists( cp_filename ) == 0 )
				{
					cp_filename.set_ByRef( filename.ConvertToAnsi() );
					::rux::uint32 count = 0;
					char directories[ 16 ][ 512 ] = { 0 , 0 };
					rux_get_search_directories( count , directories );			
					for( ::rux::uint32 index0 = 0 ; index0 < count ; index0++ )
					{
						cp_filename = directories[ index0 ];
						cp_filename += "/" + filename;
						cp_filename.set_ByRef( ::rux::io::XPath::CalculateAbsolute( executable_directory , cp_filename ) );
						if( ::rux::io::XFile::Exists( cp_filename ) == 1 )
						{
							exists = 1;
							break;
						}
					}
				}
			}
			else
				exists = 1;
			return exists;
		};
		implement_duplicate_internal_function_3( Frame , set_ImageData , char* , size_t , ::rux::XString& );
		void Frame::set_ImageData( char* image_data , size_t image_data_size , ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _media_image )
			{
				_media_image->release();
				_media_image = NULL;
			}
			if( image_data_size > sizeof( ::rux::uint64 ) && *((::rux::uint64*)image_data) == ::rux::media::png::_signature )
			{
				::rux::io::memory_stream* memory_stream = alloc_object_macros( ::rux::io::memory_stream );
				memory_stream->set_buffer( (::rux::byte*)image_data , image_data_size , 1 );
				::rux::media::png* image = alloc_object_1_param_macros( ::rux::media::png , memory_stream );
				if( image->is_animation() )
				{
					_bitmap_info_header = image->get_BitmapInfoHeader();
					if( _image_data )
					{
						::rux::engine::free_mem( _image_data );
						_image_data = NULL;
						_image_data_size = 0;
					}
					_media_image = image;
				}
				else
				{
					::rux::byte new_image = 0;
					::rux::byte* image_buffer = image->next_image( new_image );
					if( image_buffer )
					{
						_bitmap_info_header = image->get_BitmapInfoHeader();
						if( _image_data && _image_data_size < _bitmap_info_header._size_image )
						{
							::rux::engine::free_mem( _image_data );
							_image_data = NULL;
							_image_data_size = 0;
						}
						if( _image_data == NULL )
						{
							_image_data_size = _bitmap_info_header._size_image;
							_image_data = alloc_array_macros( ::rux::byte , _image_data_size );
						}
						::memcpy( _image_data , image_buffer , _bitmap_info_header._size_image );
					}
					else 
						error = "incorrect png file";
					image->release();
				}
			}
			else
			{
				error = "unknown image format";
			}
			_cs_image_data.WriteUnlock();
		};
		void Frame::set_ImageData( const ::rux::XString& filename , ::rux::XString& error )
		{	
			error.Clear();
			rux::XString cp_filename;
			cp_filename.set_ByRef( filename.ConvertToAnsi() );
			if( ::rux::io::XFile::Exists( cp_filename ) == 0 )
			{
				declare_stack_variable( char , executable_directory , 1024 );
				::rux::safe_strncpy( executable_directory , ::rux::engine::_globals->_executable_directory , 1024 );
				cp_filename.set_ByRef( ::rux::io::XPath::CalculateAbsolute( executable_directory , cp_filename ) );
				if( ::rux::io::XFile::Exists( cp_filename ) == 0 )
				{
					cp_filename.set_ByRef( filename.ConvertToAnsi() );
					::rux::uint32 count = 0;
					char directories[ 16 ][ 512 ] = { 0 , 0 };
					rux_get_search_directories( count , directories );			
					for( ::rux::uint32 index0 = 0 ; index0 < count ; index0++ )
					{
						cp_filename = directories[ index0 ];
						cp_filename += "/" + filename;
						cp_filename.set_ByRef( ::rux::io::XPath::CalculateAbsolute( executable_directory , cp_filename ) );
						if( ::rux::io::XFile::Exists( cp_filename ) == 1 )
							break;
					}
				}
			}
			::rux::XString extension;
			extension.set_ByRef( ::rux::io::XPath::GetFileNameExtension( cp_filename ) );
			extension.set_ByRef( extension.ToLower() );
			WRITE_LOCK( _cs_image_data );
			if( _media_image )
			{
				_media_image->release();
				_media_image = NULL;
			}
			if( extension == "png" )
			{
				::rux::media::png* image = alloc_object_1_param_macros( ::rux::media::png , cp_filename.str() );
				if( image->is_animation() )
				{
					_bitmap_info_header = image->get_BitmapInfoHeader();
					if( _image_data )
					{
						::rux::engine::free_mem( _image_data );
						_image_data = NULL;
						_image_data_size = 0;
					}
					_media_image = image;
				}
				else
				{
					::rux::byte new_image = 0;
					::rux::byte* image_buffer = image->next_image( new_image );
					if( image_buffer )
					{
						_bitmap_info_header = image->get_BitmapInfoHeader();
						if( _image_data && _image_data_size < _bitmap_info_header._size_image )
						{
							::rux::engine::free_mem( _image_data );
							_image_data = NULL;
							_image_data_size = 0;
						}
						if( _image_data == NULL )
						{
							_image_data_size = _bitmap_info_header._size_image;
							_image_data = alloc_array_macros( ::rux::byte , _image_data_size );
						}
						::memcpy( _image_data , image_buffer , _bitmap_info_header._size_image );
					}
					else 
						error = "file '" + cp_filename + "' is incorrect png file";
					image->release();
				}
			}
			else if( extension == "bmp" )
			{
				::rux::io::XFile file = ::rux::io::XFile::Open( cp_filename , error );
				if( file.Exists() == 1 )
				{
					_bitmap_info_header._size_image = 0;
					size_t readen_size = 0;
					::rux::media::XBitmapFileHeader bitmap_file_header;
					file.ReadBytes( (char*)&bitmap_file_header , sizeof( ::rux::media::XBitmapFileHeader ) , readen_size , error );
					if( sizeof( ::rux::media::XBitmapFileHeader ) == readen_size )
					{
						if( bitmap_file_header._type == 0x4D42 )
						{						
							file.ReadBytes( (char*)&_bitmap_info_header , sizeof( ::rux::media::XBitmapInfoHeader ) , readen_size , error );
							if( sizeof( ::rux::media::XBitmapInfoHeader ) == readen_size )
							{
								file.set_Position( bitmap_file_header._off_bits , error );
								::rux::uint32 stride = _bitmap_info_header._width * _bitmap_info_header._bit_count / 8;
								size_t size = _bitmap_info_header._height * stride;
								if( _bitmap_info_header._size_image != size )
									_bitmap_info_header._size_image = size;
								if( _image_data && _image_data_size < _bitmap_info_header._size_image )
								{
									::rux::engine::free_mem( _image_data );
									_image_data = 0;
									_image_data_size = 0;
								}
								if( _image_data == NULL && _bitmap_info_header._size_image > 0 )
								{							
									_image_data = alloc_array_macros( ::rux::uint8 , _bitmap_info_header._size_image );
									_image_data_size = _bitmap_info_header._size_image;
								}												
								if( _bitmap_info_header._bit_count == 24 )
								{							
									size_t off = ( ( stride + 3 ) & ~3 ) - stride;
									for( ::rux::uint32 index0 = size - stride ; ; index0 -= stride )
									{
										file.ReadBytes( (char*)&_image_data[ index0 ] , stride , readen_size , error );
										if( stride != readen_size )
										{
											_bitmap_info_header._size_image = 0;
											rux_write_error( "size of the file '" + filename + "' is less than necessary" , error );
											break;
										}
										if( off > 0 )
											file.set_PositionFromCurrentPosition( off , error );
										if( index0 == 0 )
											break;
									}
									if( error.Length() == 0 )
										_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
								}					
								else if( _bitmap_info_header._bit_count == 32 )
								{							
									size_t off = ( ( stride + 3 ) & ~3 ) - stride;
									for( ::rux::uint32 index0 =size - stride ; ; index0 -= stride )
									{
										file.ReadBytes( (char*)&_image_data[ index0 ] , stride , readen_size , error );
										if( stride != readen_size )
										{
											_bitmap_info_header._size_image = 0;
											rux_write_error( "size of the file '" + filename + "' is less than necessary" , error );
											break;
										}
										if( off > 0 )
											file.set_PositionFromCurrentPosition( off , error );
										if( index0 == 0 )
											break;
									}
									if( error.Length() == 0 )
										_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
								}					
							}
							else
							{						
								rux_write_error( "size of the file '" + filename + "' is less than necessary" , error );
							}
						}
						else
						{
							rux_write_error( "file '" + filename + "' is not bitmap file" , error );
						}
					}
					else
					{
						rux_write_error( "size of the file '" + filename + "' is less than necessary" , error );
					}
					file.Close();
				}
				else
				{
					rux_write_error( "file '" + filename + " does not exist" , error );
				}
			}
			else if( extension == "yv12"
				|| extension == "yuyv"
				|| extension == "i420" )
			{
				::rux::io::XFile file = ::rux::io::XFile::Open( cp_filename , error );
				if( file.Exists() == 1 )
				{			
					size_t readen_size = 0;
					::rux::uint32 width = 0 , height = 0;			
					file.ReadBytes( (char*)&width , sizeof( ::rux::uint32 ) , readen_size , error );
					if( sizeof( ::rux::uint32 ) == readen_size )
					{
						file.ReadBytes( (char*)&height , sizeof( ::rux::uint32 ) , readen_size , error );
						if( sizeof( ::rux::uint32 ) == readen_size )
						{
							char* image_data_ptr = file.ReadBytes( (size_t)( file.get_FileSize( error ) - 2 * sizeof( ::rux::uint32 ) ) , readen_size , error );
							if( image_data_ptr )
							{
								if( extension == "yv12" )
								{
									if( XFrame::get_ImageSize( ::rux::media::XEnum_Fourcc_YV12 , width , height , 0 ) == readen_size )
										set_ImageData( image_data_ptr , width , height , ::rux::media::XEnum_Fourcc_YV12 , error );
									else
									{
										rux_write_error( "size of the file '" + filename + "' is incorrect" , error );
									}
								}
								else if( extension == "yuyv" )
								{
									if( XFrame::get_ImageSize( ::rux::media::XEnum_Fourcc_YUYV , width , height , 0 ) == readen_size )
										set_ImageData( image_data_ptr , width , height , ::rux::media::XEnum_Fourcc_YUYV , error );
									else
									{
										rux_write_error( "size of the file '" + filename + "' is incorrect" , error );
									}
								}
								else if( extension == "i420" )
								{
									if( XFrame::get_ImageSize( ::rux::media::XEnum_Fourcc_I420 , width , height , 0 ) == readen_size )
										set_ImageData( image_data_ptr , width , height , ::rux::media::XEnum_Fourcc_I420 , error );
									else
									{
										rux_write_error( "size of the file '" + filename + "' is incorrect" , error );
									}
								}
								rux::engine::free_mem( image_data_ptr );
							}
							else
							{
								rux_write_error( "size of the file '" + filename + "' is less than necessary" , error );
							}
						}
						else
						{
							rux_write_error( "size of the file '" + filename + "' is less than necessary" , error );
						}
					}
					else
					{
						rux_write_error( "size of the file '" + filename + "' is less than necessary" , error );
					}
					file.Close();
				}
				else
				{
					rux_write_error( "file '" + filename + " does not exist" , error );
				}
			}
			else
			{
				rux_write_error( "files with extension '" + extension + " is not supported" , error );
			}
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_result_function_1_with_const( Frame& , Frame , ConvertToBGR24 , ::rux::XString& );
		Frame& Frame::ConvertToBGR24( ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
			{
				XFrame bgr24;
				Frame* bgr24_pointer = bgr24();
				::rux::media::rux_I420_to_BGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
				bgr24_pointer->_bitmap_info_header._bit_count = 24;
				bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return bgr24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
			{
				XFrame bgr24;
				Frame* bgr24_pointer = bgr24();
				rux::media::rux_YV12_to_BGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
				bgr24_pointer->_bitmap_info_header._bit_count = 24;
				bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return bgr24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YUYV )
			{
				XFrame bgr24;
				Frame* bgr24_pointer = bgr24();
				::rux::media::rux_YUYV_to_BGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
				bgr24_pointer->_bitmap_info_header._bit_count = 24;
				bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return bgr24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BGR3 )
			{
				_cs_image_data.WriteUnlock();
				::rux::media::XFrame res;
				res.set_ByRef( *this );
				return res++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
				|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( _bitmap_info_header._bit_count == 24 )
				{
					XFrame bgr24;
					Frame* bgr24_pointer = bgr24();
					rux::media::rux_RGB24_to_BGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
					bgr24_pointer->_bitmap_info_header._bit_count = 24;
					bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
					bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
					bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
					bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
					_cs_image_data.WriteUnlock();
					return bgr24++;
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );
					_cs_image_data.WriteUnlock();
					return XFrame()++;
				}
			}
			else
			{
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = _bitmap_info_header._compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );
				_cs_image_data.WriteUnlock();
				return XFrame()++;
			}	
		};
		implement_duplicate_internal_function_2_with_const( Frame , SaveToFile , const ::rux::XString& , ::rux::XString& );
		void Frame::SaveToFile( const ::rux::XString& filename_without_extension , ::rux::XString& error )
		{
			error.Clear();
			::rux::XString extension;
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
				extension = ".yv12";
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YUYV )
				extension = ".yuyv";
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
				extension = ".i420";
			else if( _bitmap_info_header._compression != ::rux::media::XEnum_Fourcc_Unknown )
				extension = ".bmp";
			else
			{
				rux_write_error( "image fourcc is unknown" , error );
			}
			if( error.Length() == 0 )
			{
				if( extension == ".bmp" )
					SaveAsBmp( filename_without_extension + extension , error );
				else
				{
					extension.set_ByRef( filename_without_extension + extension );
					if( ::rux::io::XFile::Exists( extension ) == 0 )
					{
						::rux::io::XFile file = ::rux::io::XFile::Create( extension , error );
						if( file.Exists() == 1 )
						{					
							file.WriteBytes( (char*)&_bitmap_info_header._width , sizeof( ::rux::uint32 ) , error );
							if( error.Length() == 0 )
							{
								file.WriteBytes( (char*)&_bitmap_info_header._height , sizeof( ::rux::uint32 ) , error );
								if( error.Length() == 0 )
									file.WriteBytes( (char*)_image_data , get_ImageSize() , error );
							}
							file.Close();
						}
						else
						{
							rux_write_error( "file '" + extension + " does not created" , error );
						}
					}
					else
					{
						rux_write_error( "file '" + extension + " already exists" , error );
					}
				}				
			}
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_function_2( Frame , ConvertToRGB24 , XFrame& , ::rux::XString& );
		void Frame::ConvertToRGB24( XFrame& destination , ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
			{		
				Frame* destination_pointer = destination();
				rux::media::rux_I420_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 24;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;		
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
			{		
				Frame* destination_pointer = destination();
				rux::media::rux_YV12_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 24;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YUYV )
			{
				Frame* destination_pointer = destination();
				::rux::media::rux_YUYV_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 24;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BGR3 )
			{
				Frame* destination_pointer = destination();
				if( _bitmap_info_header._bit_count == 24 )
					::rux::media::rux_BGR24_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				else if( _bitmap_info_header._bit_count == 32 )
					::rux::media::rux_BGR32_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 24;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
				|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( _bitmap_info_header._bit_count == 24 )
				{
					Frame* destination_pointer = destination();
					if( destination_pointer->_image_data && destination_pointer->_image_data_size < _image_data_size )
					{
						rux::engine::free_mem( destination_pointer->_image_data );
						destination_pointer->_image_data = NULL;				
					}
					if( destination_pointer->_image_data == NULL )
					{
						destination_pointer->_image_data = alloc_array_macros( ::rux::uint8 , _image_data_size );
						destination_pointer->_image_data_size = _image_data_size;
					}
					memcpy( destination_pointer->_image_data , _image_data , _image_data_size );
					destination_pointer->_bitmap_info_header._bit_count = 24;
					destination_pointer->_bitmap_info_header._size_image = 
						_bitmap_info_header._width * _bitmap_info_header._height * 3;
					destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
					destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
					destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );			
				}
			}
			else
			{
				char sfcc[5] = {0}; *((::rux::uint32*)sfcc) = _bitmap_info_header._compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );		
			}
			_cs_image_data.WriteUnlock();
		};
		void XFrame::ConvertToBGR24( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , XFrame& destination , ::rux::XString& error )
		{
			error.Clear();
			Frame* destination_pointer = destination();
			destination_pointer->_bitmap_info_header._bit_count = 24;
			destination_pointer->_bitmap_info_header._size_image = image_width * image_height * 3;
			destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
			destination_pointer->_bitmap_info_header._width = image_width;
			destination_pointer->_bitmap_info_header._height = image_height;
			if( compression == ::rux::media::XEnum_Fourcc_I420 )
				rux::media::rux_I420_to_BGR24( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
			else if( compression == ::rux::media::XEnum_Fourcc_YV12 )
				rux::media::rux_YV12_to_BGR24( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
			else if( compression == ::rux::media::XEnum_Fourcc_YUYV )
				rux::media::rux_YUYV_to_BGR24( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
			else if( compression == ::rux::media::XEnum_Fourcc_BGR3 )
			{
				if( destination_pointer->_image_data && destination_pointer->_image_data_size < image_data_size )
				{
					rux::engine::free_mem( destination_pointer->_image_data );
					destination_pointer->_image_data = NULL;				
				}
				if( destination_pointer->_image_data == NULL )
				{
					destination_pointer->_image_data = alloc_array_macros( ::rux::uint8 , image_data_size );
					destination_pointer->_image_data_size = image_data_size;
				}
				memcpy( destination_pointer->_image_data , image_data , image_data_size );
			}
			else if( compression == ::rux::media::XEnum_Fourcc_RGB
				|| compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( bit_count == 24 )
					rux::media::rux_RGB24_to_BGR24( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );
				}
			}
			else
			{
				destination_pointer->_bitmap_info_header._bit_count = 0;
				destination_pointer->_bitmap_info_header._size_image = 0;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_Unknown;
				destination_pointer->_bitmap_info_header._width = 0;
				destination_pointer->_bitmap_info_header._height = 0;		
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );
			}
		};
		implement_duplicate_internal_result_function_2_with_const( Frame& , Frame , ConvertToBGR32 , ::rux::uint8 , ::rux::XString& );
		Frame& Frame::ConvertToBGR32( ::rux::uint8 alpha , ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
			{
				XFrame bgr24;
				Frame* bgr24_pointer = bgr24();
				rux::media::rux_I420_to_BGR32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
				bgr24_pointer->_bitmap_info_header._bit_count = 32;
				bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
				bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return bgr24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
			{
				XFrame bgr24;
				Frame* bgr24_pointer = bgr24();
				rux::media::rux_YV12_to_BGR32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size , alpha );
				bgr24_pointer->_bitmap_info_header._bit_count = 32;
				bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
				bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return bgr24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YUYV )
			{
				XFrame bgr24;
				Frame* bgr24_pointer = bgr24();
				rux::media::rux_YUYV_to_BGR32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
				bgr24_pointer->_bitmap_info_header._bit_count = 32;
				bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
				bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return bgr24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BGR3 )
			{
				if( _bitmap_info_header._bit_count == 32 )
				{
					_cs_image_data.WriteUnlock();
					::rux::media::XFrame res;
					res.set_ByRef( *this );
					return res++;
				}
				else if( _bitmap_info_header._bit_count == 24 )
				{
					XFrame bgr24;
					Frame* bgr24_pointer = bgr24();
					rux::media::rux_BGR24_to_BGR32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
					bgr24_pointer->_bitmap_info_header._bit_count = 32;
					bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
					bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
					bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
					bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
					_cs_image_data.WriteUnlock();
					return bgr24++;
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );
					_cs_image_data.WriteUnlock();
					return XFrame()++;
				}
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
				|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( _bitmap_info_header._bit_count == 24 )
				{
					XFrame bgr24;
					Frame* bgr24_pointer = bgr24();
					rux::media::rux_RGB24_to_BGR32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
					bgr24_pointer->_bitmap_info_header._bit_count = 32;
					bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
					bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
					bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
					bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
					_cs_image_data.WriteUnlock();
					return bgr24++;
				}
				else if( _bitmap_info_header._bit_count == 32 )
				{
					XFrame bgr24;
					Frame* bgr24_pointer = bgr24();
					rux::media::rux_RGB32_to_BGR32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
					bgr24_pointer->_bitmap_info_header._bit_count = 32;
					bgr24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
					bgr24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
					bgr24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
					bgr24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
					_cs_image_data.WriteUnlock();
					return bgr24++;
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );
					_cs_image_data.WriteUnlock();
					return XFrame()++;
				}
			}
			else
			{
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = _bitmap_info_header._compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );
				_cs_image_data.WriteUnlock();
				return XFrame()++;
			}	
		};
		void XFrame::ConvertToRGB32( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , XFrame& destination , ::rux::uint8 alpha , ::rux::XString& error )
		{
			error.Clear();
			Frame* destination_pointer = destination();
			destination_pointer->_bitmap_info_header._bit_count = 32;
			destination_pointer->_bitmap_info_header._size_image = image_width * image_height * 3;
			destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
			destination_pointer->_bitmap_info_header._width = image_width;
			destination_pointer->_bitmap_info_header._height = image_height;
			if( compression == ::rux::media::XEnum_Fourcc_YV12 )
				rux::media::rux_YV12_to_RGB32( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size , alpha );
			else if( compression == ::rux::media::XEnum_Fourcc_I420 )
				rux::media::rux_I420_to_RGB32( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
			else if( compression == ::rux::media::XEnum_Fourcc_RGB
				|| compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( bit_count == 24 )
					rux::media::rux_RGB24_to_RGB32( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				else if( bit_count == 32 )
				{
					if( destination_pointer->_image_data && destination_pointer->_image_data_size < image_data_size )
					{
						rux::engine::free_mem( destination_pointer->_image_data );
						destination_pointer->_image_data = NULL;				
					}
					if( destination_pointer->_image_data == NULL )
					{
						destination_pointer->_image_data = alloc_array_macros( ::rux::uint8 , image_data_size );
						destination_pointer->_image_data_size = image_data_size;
					}
					memcpy( destination_pointer->_image_data , image_data , image_data_size );
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16 bits to RGB 32 bits is not supported yet'" , error );
				}
			}
			else if( compression == ::rux::media::XEnum_Fourcc_BGR3 )
				rux::media::rux_BGR24_to_RGB32( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
			else
			{
				destination_pointer->_bitmap_info_header._bit_count = 0;
				destination_pointer->_bitmap_info_header._size_image = 0;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_Unknown;
				destination_pointer->_bitmap_info_header._width = 0;
				destination_pointer->_bitmap_info_header._height = 0;		
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 32 is not supported yet" , sfcc ) , error );
			}	
		};
		void XFrame::ConvertToRGB24( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , ::rux::uint8*& destination , ::rux::int32& destination_size , ::rux::XString& error )
		{
			error.Clear();
			if( compression == ::rux::media::XEnum_Fourcc_I420 )
				rux::media::rux_I420_to_RGB24( image_data , image_width , image_height , destination , destination_size );		
			else if( compression == ::rux::media::XEnum_Fourcc_YV12 )
				rux::media::rux_YV12_to_RGB24( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_YUYV )
				rux::media::rux_YUYV_to_RGB24( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_BGR3 )
				rux::media::rux_BGR24_to_RGB24( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_RGB
				|| compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( bit_count == 24 )
					memcpy( destination , image_data , image_data_size );			
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );			
				}
			}
			else
			{		
				char sfcc[5] = {0}; *((::rux::uint32*)sfcc) = compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );		
			}
		};
		void XFrame::ConvertToBGR24( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , ::rux::uint8*& destination , ::rux::int32& destination_size , ::rux::XString& error )
		{
			error.Clear();
			if( compression == ::rux::media::XEnum_Fourcc_I420 )
				rux::media::rux_I420_to_BGR24( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_YV12 )
				rux::media::rux_YV12_to_BGR24( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_YUYV )
				rux::media::rux_YUYV_to_BGR24( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_BGR3 )
				memcpy( destination , image_data , image_data_size );
			else if( compression == ::rux::media::XEnum_Fourcc_RGB
				|| compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( bit_count == 24 )
					rux::media::rux_RGB24_to_BGR24( image_data , image_width , image_height , destination , destination_size );
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );
				}
			}
			else
			{
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );
			}
		};
		void XFrame::ConvertToRGB32( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , ::rux::uint8*& destination , ::rux::int32& destination_size , ::rux::uint8 alpha , ::rux::XString& error )
		{
			error.Clear();	
			if( compression == ::rux::media::XEnum_Fourcc_YV12 )
				rux::media::rux_YV12_to_RGB32( image_data , image_width , image_height , destination , destination_size , alpha );
			else if( compression == ::rux::media::XEnum_Fourcc_I420 )
				rux::media::rux_I420_to_RGB32( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_RGB
				|| compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( bit_count == 24 )
					rux::media::rux_RGB24_to_RGB32( image_data , image_width , image_height , destination , destination_size );
				else if( bit_count == 32 )
					memcpy( destination , image_data , image_data_size );
				else
				{
					rux_write_error( "conversion from RGB 8,16 bits to RGB 32 bits is not supported yet'" , error );
				}
			}
			else if( compression == ::rux::media::XEnum_Fourcc_BGR3 )
				rux::media::rux_BGR24_to_RGB32( image_data , image_width , image_height , destination , destination_size );
			else
			{
				char sfcc[5] = {0}; *((::rux::uint32*)sfcc) = compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 32 is not supported yet" , sfcc ) , error );
			}	
		};
		void XFrame::ConvertToRGB24( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , XFrame& destination , ::rux::XString& error )
		{
			error.Clear();
			Frame* destination_pointer = destination();
			destination_pointer->_bitmap_info_header._bit_count = 24;
			destination_pointer->_bitmap_info_header._size_image = image_width * image_height * 3;
			destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
			destination_pointer->_bitmap_info_header._width = image_width;
			destination_pointer->_bitmap_info_header._height = image_height;		
			if( compression == ::rux::media::XEnum_Fourcc_I420 )
				rux::media::rux_I420_to_RGB24( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );		
			else if( compression == ::rux::media::XEnum_Fourcc_YV12 )
				rux::media::rux_YV12_to_RGB24( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
			else if( compression == ::rux::media::XEnum_Fourcc_YUYV )
				rux::media::rux_YUYV_to_RGB24( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
			else if( compression == ::rux::media::XEnum_Fourcc_BGR3 )
				rux::media::rux_BGR24_to_RGB24( image_data , image_width , image_height , destination_pointer->_image_data , destination_pointer->_image_data_size );
			else if( compression == ::rux::media::XEnum_Fourcc_RGB
				|| compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( bit_count == 24 )
				{
					if( destination_pointer->_image_data && destination_pointer->_image_data_size < image_data_size )
					{
						rux::engine::free_mem( destination_pointer->_image_data );
						destination_pointer->_image_data = NULL;				
					}
					if( destination_pointer->_image_data == NULL )
					{
						destination_pointer->_image_data = alloc_array_macros( ::rux::uint8 , image_data_size );
						destination_pointer->_image_data_size = image_data_size;
					}
					memcpy( destination_pointer->_image_data , image_data , image_data_size );			
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );			
				}
			}
			else
			{
				destination_pointer->_bitmap_info_header._bit_count = 0;
				destination_pointer->_bitmap_info_header._size_image = 0;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_Unknown;
				destination_pointer->_bitmap_info_header._width = 0;
				destination_pointer->_bitmap_info_header._height = 0;		
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );		
			}
		};
		implement_duplicate_internal_function_2( Frame , ConvertToBGR24 , XFrame& , ::rux::XString& );
		void Frame::ConvertToBGR24( XFrame& destination , ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
			{
				Frame* destination_pointer = destination();
				rux::media::rux_I420_to_BGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 24;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
			{
				Frame* destination_pointer = destination();
				rux::media::rux_YV12_to_BGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 24;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YUYV )
			{
				Frame* destination_pointer = destination();
				rux::media::rux_YUYV_to_BGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 24;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BGR3 )
			{
				Frame* destination_pointer = destination();
				if( destination_pointer->_image_data && destination_pointer->_image_data_size < _image_data_size )
				{
					rux::engine::free_mem( destination_pointer->_image_data );
					destination_pointer->_image_data = NULL;				
				}
				if( destination_pointer->_image_data == NULL )
				{
					destination_pointer->_image_data = alloc_array_macros( ::rux::uint8 , _image_data_size );
					destination_pointer->_image_data_size = _image_data_size;
				}
				memcpy( destination_pointer->_image_data , _image_data , _image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 24;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
				|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( _bitmap_info_header._bit_count == 24 )
				{
					Frame* destination_pointer = destination();
					rux::media::rux_RGB24_to_BGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
					destination_pointer->_bitmap_info_header._bit_count = 24;
					destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
					destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BGR3;
					destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
					destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );
				}
			}
			else
			{
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = _bitmap_info_header._compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );
			}
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_function_3( Frame , ConvertToRGB32 , XFrame& , ::rux::uint8 , ::rux::XString& );
		void Frame::ConvertToRGB32( XFrame& destination , ::rux::uint8 alpha , ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
			{		
				Frame* destination_pointer = destination();
				rux::media::rux_YV12_to_RGB32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size , alpha );
				destination_pointer->_bitmap_info_header._bit_count = 32;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
			{
				Frame* destination_pointer = destination();
				rux::media::rux_I420_to_RGB32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 32;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
				|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( _bitmap_info_header._bit_count == 24 )
				{
					Frame* destination_pointer = destination();
					rux::media::rux_RGB24_to_RGB32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
					destination_pointer->_bitmap_info_header._bit_count = 32;
					destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
					destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
					destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
					destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				}
				else if( _bitmap_info_header._bit_count == 32 )
				{
					Frame* destination_pointer = destination();
					if( destination_pointer->_image_data && destination_pointer->_image_data_size < _image_data_size )
					{
						rux::engine::free_mem( destination_pointer->_image_data );
						destination_pointer->_image_data = NULL;				
					}
					if( destination_pointer->_image_data == NULL )
					{
						destination_pointer->_image_data = alloc_array_macros( ::rux::uint8 , _image_data_size );
						destination_pointer->_image_data_size = _image_data_size;
					}
					memcpy( destination_pointer->_image_data , _image_data , _image_data_size );
					destination_pointer->_bitmap_info_header._bit_count = 32;
					destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
					destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
					destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
					destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16 bits to RGB 32 bits is not supported yet'" , error );
				}
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BGR3 )
			{
				Frame* destination_pointer = destination();
				rux::media::rux_BGR24_to_RGB32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , destination_pointer->_image_data , destination_pointer->_image_data_size );
				destination_pointer->_bitmap_info_header._bit_count = 24;
				destination_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				destination_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				destination_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				destination_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
			}
			else
			{
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = _bitmap_info_header._compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 32 is not supported yet" , sfcc ) , error );
			}
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_result_function_1( Frame& , Frame , ConvertToRGB24 , ::rux::XString& );
		Frame& Frame::ConvertToRGB24( ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
			{
				XFrame rgb24;
				Frame* rgb24_pointer = rgb24();
				rux::media::rux_I420_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb24_pointer->_image_data , rgb24_pointer->_image_data_size );
				rgb24_pointer->_bitmap_info_header._bit_count = 24;
				rgb24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				rgb24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				rgb24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				rgb24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return rgb24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
			{
				XFrame rgb24;
				Frame* rgb24_pointer = rgb24();
				rux::media::rux_YV12_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb24_pointer->_image_data , rgb24_pointer->_image_data_size );
				rgb24_pointer->_bitmap_info_header._bit_count = 24;
				rgb24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				rgb24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				rgb24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				rgb24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return rgb24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YUYV )
			{
				XFrame rgb24;
				Frame* rgb24_pointer = rgb24();
				rux::media::rux_YUYV_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb24_pointer->_image_data , rgb24_pointer->_image_data_size );
				rgb24_pointer->_bitmap_info_header._bit_count = 24;
				rgb24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				rgb24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				rgb24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				rgb24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return rgb24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BGR3 )
			{
				XFrame rgb24;
				Frame* rgb24_pointer = rgb24();
				if( _bitmap_info_header._bit_count == 24 )
					rux::media::rux_BGR24_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb24_pointer->_image_data , rgb24_pointer->_image_data_size );
				else if( _bitmap_info_header._bit_count == 32 )
					rux::media::rux_BGR32_to_RGB24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb24_pointer->_image_data , rgb24_pointer->_image_data_size );
				rgb24_pointer->_bitmap_info_header._bit_count = 24;
				rgb24_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				rgb24_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				rgb24_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				rgb24_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return rgb24++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
				|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( _bitmap_info_header._bit_count == 24 )
				{
					_cs_image_data.WriteUnlock();
					::rux::media::XFrame res;
					res.set_ByRef( *this );
					return res++;
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );
					_cs_image_data.WriteUnlock();
					return XFrame()++;
				}
			}
			else
			{
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = _bitmap_info_header._compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );
				_cs_image_data.WriteUnlock();
				return XFrame()++;
			}
		};
		Frame& Frame::ConvertToRGB32( ::rux::uint8 alpha , ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
			{
				XFrame rgb32;
				Frame* rgb32_pointer = rgb32();
				rux::media::rux_YV12_to_RGB32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb32_pointer->_image_data , rgb32_pointer->_image_data_size , alpha );
				rgb32_pointer->_bitmap_info_header._bit_count = 32;
				rgb32_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				rgb32_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				rgb32_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				rgb32_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return rgb32++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
			{
				XFrame rgb32;
				Frame* rgb32_pointer = rgb32();
				rux::media::rux_I420_to_RGB32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb32_pointer->_image_data , rgb32_pointer->_image_data_size );
				rgb32_pointer->_bitmap_info_header._bit_count = 32;
				rgb32_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
				rgb32_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				rgb32_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				rgb32_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return rgb32++;
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
				|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( _bitmap_info_header._bit_count == 24 )
				{
					XFrame rgb32;
					Frame* rgb32_pointer = rgb32();
					rux::media::rux_RGB24_to_RGB32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb32_pointer->_image_data , rgb32_pointer->_image_data_size );
					rgb32_pointer->_bitmap_info_header._bit_count = 32;
					rgb32_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 4;
					rgb32_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
					rgb32_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
					rgb32_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
					_cs_image_data.WriteUnlock();
					return rgb32++;
				}
				else if( _bitmap_info_header._bit_count == 32 )
				{
					_cs_image_data.WriteUnlock();
					return XFrame( *this )++;
				}
				else
				{
					rux_write_error( "conversion from RGB 8,16 bits to RGB 32 bits is not supported yet'" , error );
					_cs_image_data.WriteUnlock();
					return XFrame()++;
				}
			}
			else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BGR3 )
			{
				XFrame rgb32;
				Frame* rgb32_pointer = rgb32();
				if( _bitmap_info_header._bit_count == 24 )
					rux::media::rux_BGR24_to_RGB32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb32_pointer->_image_data , rgb32_pointer->_image_data_size );
				else if( _bitmap_info_header._bit_count == 32 )
					rux::media::rux_BGR32_to_RGB32( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , rgb32_pointer->_image_data , rgb32_pointer->_image_data_size );
				rgb32_pointer->_bitmap_info_header._bit_count = 32;
				rgb32_pointer->_bitmap_info_header._size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
				rgb32_pointer->_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
				rgb32_pointer->_bitmap_info_header._width = _bitmap_info_header._width;
				rgb32_pointer->_bitmap_info_header._height = _bitmap_info_header._height;
				_cs_image_data.WriteUnlock();
				return rgb32++;
			}
			else
			{
				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = _bitmap_info_header._compression;
				rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );
				_cs_image_data.WriteUnlock();
				return XFrame()++;
			}
		}
		Frame& XFrame::ConvertToRGB32( ::rux::uint8 alpha , ::rux::XString& error )
		{
			return (*this)()->ConvertToRGB32( alpha , error );
		};
		implement_duplicate_internal_result_function_1_with_const( ::rux::byte* , Frame , get_ImageData , ::rux::byte& );
		::rux::uint8* Frame::get_ImageData( ::rux::byte& new_image ) const
		{
			new_image = 0;
			if( _media_image )
				return _media_image->next_image( new_image );
			else
				return _image_data;
		};
		implement_duplicate_internal_result_function_with_const( rux::uint32 , Frame , get_ImageWidth );
		rux::uint32 Frame::get_ImageWidth( void ) const
		{
			return _bitmap_info_header._width;
		};
		implement_duplicate_internal_result_function_with_const( rux::uint32 , Frame , get_ImageHeight );
		rux::uint32 Frame::get_ImageHeight( void ) const
		{
			return _bitmap_info_header._height;
		};
		implement_duplicate_internal_result_function_with_const( rux::uint32 , Frame , get_ImageSize );
		rux::uint32 Frame::get_ImageSize( void )
		{
			return _bitmap_info_header._size_image;
		};
		implement_duplicate_internal_function_4( Frame , Resize , XFrame& , ::rux::uint32 , ::rux::uint32 , ::rux::XString& );
		void Frame::Resize( XFrame& destination , ::rux::uint32 new_image_width , ::rux::uint32 new_image_height , ::rux::XString& error )
		{
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			Frame* destination_pointer = destination();
			if( new_image_width < _bitmap_info_header._width || new_image_height < _bitmap_info_header._height )
			{
				if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
				{		
					destination_pointer->_bitmap_info_header._size_image = new_image_width * new_image_height + new_image_width * new_image_height / 2;
					if( destination_pointer->_image_data && destination_pointer->_image_data_size < destination_pointer->_bitmap_info_header._size_image )
					{
						rux::engine::free_mem( destination_pointer->_image_data );
						destination_pointer->_image_data = NULL;
						destination_pointer->_image_data_size = 0;
					}
					if( destination_pointer->_image_data == NULL && destination_pointer->_bitmap_info_header._size_image > 0 )
					{
						destination_pointer->_image_data = alloc_array_macros( ::rux::uint8 , destination_pointer->_bitmap_info_header._size_image );
						destination_pointer->_image_data_size = destination_pointer->_bitmap_info_header._size_image;
					}			
					destination_pointer->_bitmap_info_header._compression = _bitmap_info_header._compression;
					destination_pointer->_bitmap_info_header._width = new_image_width;
					destination_pointer->_bitmap_info_header._height = new_image_height;			
					::rux::uint8* in_ptr = (uint8*)_image_data;
					::rux::uint8* out_ptr = (uint8*)destination_pointer->_image_data;  
					rux::uint32 width = _bitmap_info_header._width;
					rux::uint32 height = _bitmap_info_header._height;
					double x = (double)width / (double)new_image_width;
					double y = (double)height / (double)new_image_height;
					rux::uint32 index0 = 0 , index1 = 0;
					::rux::uint8* out_ptr1 = out_ptr;
					::rux::uint8* in_ptr1 = in_ptr;
					for( index0 = 0; index0 < new_image_height ; index0++ )
					{
						for( index1 = 0; index1 < new_image_width ; index1++ )
						{
							*out_ptr1++ = *( in_ptr1 + int( index1 * x ) + width * int( index0 * y ) );
						}
					}
					in_ptr += width * height;
					out_ptr += new_image_width * new_image_height;    
					width = _bitmap_info_header._width / 2;
					height = _bitmap_info_header._height / 2;
					rux::uint32 new_width = new_image_width / 2;
					rux::uint32 new_height = new_image_height / 2;		
					x = (double)width / (double)new_width;
					y = (double)height / (double)new_height;		
					out_ptr1 = out_ptr;
					in_ptr1 = in_ptr;
					for( index0 = 0; index0 < new_height ; index0++ )
					{
						for( index1 = 0; index1 < new_width ; index1++ )
						{
							*out_ptr1++ = *( in_ptr1 + int( index1 * x ) + width * int( index0 * y ) );
						}
					}
					in_ptr += _bitmap_info_header._width * _bitmap_info_header._height / 4;
					out_ptr += new_image_width * new_image_height / 4;    
					out_ptr1 = out_ptr;
					in_ptr1 = in_ptr;
					for( index0 = 0; index0 < new_height ; index0++ )
					{
						for( index1 = 0; index1 < new_width ; index1++ )
						{
							*out_ptr1++ = *( in_ptr1 + int( index1 * x ) + width * int( index0 * y ) );
						}
					}		
				}
				else
				{
					rux_write_error( ::rux::XString::Format( "resizing from fourcc '%d' is not supported yet" , _bitmap_info_header._compression ) , error );		
				}
			}
			else 
			{
				rux_write_error( ::rux::XString::Format( "reducting is supported only" ) , error );		
			}
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_result_function_3( Frame& , Frame , Resize , ::rux::uint32 , ::rux::uint32 , ::rux::XString& );
		Frame& Frame::Resize( ::rux::uint32 new_image_width , ::rux::uint32 new_image_height , ::rux::XString& error )
		{
			XFrame resized_frame;
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			Frame* resized_frame_pointer = resized_frame();			
			if( new_image_width < _bitmap_info_header._width || new_image_height < _bitmap_info_header._height )
			{
				if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
				{		
					resized_frame_pointer->_bitmap_info_header._size_image = new_image_width * new_image_height + new_image_width * new_image_height / 2;
					resized_frame_pointer->_bitmap_info_header._compression = _bitmap_info_header._compression;
					resized_frame_pointer->_bitmap_info_header._width = new_image_width;
					resized_frame_pointer->_bitmap_info_header._height = new_image_height;			
					resized_frame_pointer->_image_data_size = resized_frame_pointer->_bitmap_info_header._size_image;
					resized_frame_pointer->_image_data = alloc_array_macros( ::rux::uint8 , resized_frame_pointer->_image_data_size );
					::rux::uint8* in_ptr = (uint8*)_image_data;
					::rux::uint8* out_ptr = (uint8*)resized_frame_pointer->_image_data;  
					rux::uint32 width = _bitmap_info_header._width;
					rux::uint32 height = _bitmap_info_header._height;
					double x = (double)width / (double)new_image_width;
					double y = (double)height / (double)new_image_height;
					rux::uint32 index0 = 0 , index1 = 0;
					::rux::uint8* out_ptr1 = out_ptr;
					::rux::uint8* in_ptr1 = in_ptr;
					for( index0 = 0; index0 < new_image_height ; index0++ )
					{
						for( index1 = 0; index1 < new_image_width ; index1++ )
						{
							*out_ptr1++ = *( in_ptr1 + int( index1 * x ) + width * int( index0 * y ) );
						}
					}
					in_ptr += width * height;
					out_ptr += new_image_width * new_image_height;    
					width = _bitmap_info_header._width / 2;
					height = _bitmap_info_header._height / 2;
					rux::uint32 new_width = new_image_width / 2;
					rux::uint32 new_height = new_image_height / 2;		
					x = (double)width / (double)new_width;
					y = (double)height / (double)new_height;		
					out_ptr1 = out_ptr;
					in_ptr1 = in_ptr;
					for( index0 = 0; index0 < new_height ; index0++ )
					{
						for( index1 = 0; index1 < new_width ; index1++ )
						{
							*out_ptr1++ = *( in_ptr1 + int( index1 * x ) + width * int( index0 * y ) );
						}
					}
					in_ptr += _bitmap_info_header._width * _bitmap_info_header._height / 4;
					out_ptr += new_image_width * new_image_height / 4;    
					out_ptr1 = out_ptr;
					in_ptr1 = in_ptr;
					for( index0 = 0; index0 < new_height ; index0++ )
					{
						for( index1 = 0; index1 < new_width ; index1++ )
						{
							*out_ptr1++ = *( in_ptr1 + int( index1 * x ) + width * int( index0 * y ) );
						}
					}		
				}
				else
				{
					rux_write_error( ::rux::XString::Format( "resizing from fourcc '%d' is not supported yet" , _bitmap_info_header._compression ) , error );		
				}
			}
			else 
			{
				rux_write_error( ::rux::XString::Format( "reducting is supported only" ) , error );		
			}
			_cs_image_data.WriteUnlock();
			return resized_frame++;
		};
		implement_duplicate_internal_function_2_with_const( Frame , SaveAsBmp , const ::rux::XString& , ::rux::XString& );
		void Frame::SaveAsBmp( const ::rux::XString& filename , ::rux::XString& error )
		{
			WRITE_LOCK( _cs_image_data );
			if( filename.Length() > 0 )
			{
				if( ::rux::io::XFile::Exists( filename ) == 0 )
				{
					if( _bitmap_info_header._size_image > 0 )
					{
						if( _bitmap_info_header._compression != ::rux::media::XEnum_Fourcc_BGR3 )
						{	
							if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
								|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
							{
								if( _bitmap_info_header._bit_count == 24 )
								{
									XFrame frame = ConvertToBGR24( error );
									if( error.Length() == 0 )
										frame.SaveAsBmp( filename , error );
								}
								else if( _bitmap_info_header._bit_count == 32 )
								{
									XFrame frame = ConvertToBGR32( 255 , error );
									if( error.Length() == 0 )
										frame.SaveAsBmp( filename , error );
								}
							}
							else
							{
								XFrame frame = ConvertToBGR24( error );
								if( error.Length() == 0 )
									frame.SaveAsBmp( filename , error );
							}
						}
						else
						{
							rux::io::XFile bmp_file = ::rux::io::XFile::Create( filename , error );
							if( bmp_file.Exists() == 1 )
							{
								rux::media::XBitmapFileHeader bitmap_file_header;
								bitmap_file_header._type = 0x4D42;
								bitmap_file_header._size = sizeof( bitmap_file_header ) + sizeof( _bitmap_info_header ) + _bitmap_info_header._size_image;
								bitmap_file_header._reserved1 = 0;
								bitmap_file_header._reserved2 = 0;
								bitmap_file_header._off_bits = sizeof( bitmap_file_header ) + sizeof( _bitmap_info_header );
								bmp_file.WriteBytes( (char*)&bitmap_file_header , sizeof( bitmap_file_header ) , error );
								if( error.Length() == 0 )
								{
									rux::media::XBitmapInfoHeader bitmap_info_header;
									bitmap_info_header._bit_count = _bitmap_info_header._bit_count;
									bitmap_info_header._clr_important = _bitmap_info_header._clr_important;
									bitmap_info_header._clr_used = _bitmap_info_header._clr_used;
									bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BI_RGB;
									bitmap_info_header._height = _bitmap_info_header._height;
									bitmap_info_header._planes = _bitmap_info_header._planes;
									bitmap_info_header._size = _bitmap_info_header._size;
									bitmap_info_header._size_image = _bitmap_info_header._size_image;
									bitmap_info_header._width = _bitmap_info_header._width;
									bitmap_info_header._x_pels_per_meter = _bitmap_info_header._x_pels_per_meter;
									bitmap_info_header._y_pels_per_meter = _bitmap_info_header._y_pels_per_meter;							
									bmp_file.WriteBytes( (char*)&bitmap_info_header , sizeof( bitmap_info_header ) , error );
									if( error.Length() == 0 )
									{
										rux::uint32 stride = _bitmap_info_header._width * bitmap_info_header._bit_count / 8;
										size_t off = ( ( stride + 3 ) & ~3 ) - stride;
										for( ::rux::uint32 index0 = bitmap_info_header._size_image - stride ; index0 >= 0 ; index0 -= stride )
										{
											bmp_file.WriteBytes( (char*)&_image_data[ index0 ] , stride , error );	
											if( off > 0 )
											{
												declare_stack_variable( char , zeroes , 256 );
												for( size_t index1 = 0 ; index1 < off ; index1 += 256 )
												{
													if( off - index1 < 256 )
														bmp_file.WriteBytes( zeroes , off - index1 , error );	
													else
														bmp_file.WriteBytes( zeroes , 256 , error );	
												}
											}
											if( index0 == 0 )
												break;
										}
									}
								}
								bmp_file.Close();
							}
							else
							{
								rux_write_error( "file '" + filename + "' could not be created" , error );
							}
						}
					}
					else
					{
						rux_write_error( "frame data size is equal 0" , error );
					}
				}
				else
				{
					rux_write_error( "file '" + filename + "' exists" , error );
				}
			}
			else
			{
				rux_write_error( "filename is empty" , error );
			}
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_function_4( Frame , ConvertToBmpData , ::rux::uint8*& , uint64& , uint64& , ::rux::XString& );
		void Frame::ConvertToBmpData( ::rux::uint8*& destination_data_ptr , uint64& destination_data_ptr_size , uint64& size , ::rux::XString& error )
		{	
			error.Clear();
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._size_image > 0 )
			{
				rux::uint32 width = 0 , height = 0 , size_image = 0;
				uint16 bit_count = 0;		
				if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
				{			
					bit_count = 24;
					size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;			
					width = _bitmap_info_header._width;
					height = _bitmap_info_header._height;				
				}
				/*else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
				{			
					bit_count = 24;
					size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;			
					width = _bitmap_info_header._width;
					height = _bitmap_info_header._height;				
				}
				else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YUYV )
				{			
					bit_count = 24;
					size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
					width = _bitmap_info_header._width;
					height = _bitmap_info_header._height;				
				}*/
				else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BGR3 )
				{
					bit_count = 24;
					size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
					width = _bitmap_info_header._width;
					height = _bitmap_info_header._height;				
				}
				/*else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
					|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
				{
					if( _bitmap_info_header._bit_count == 24 )
					{				
						bit_count = 24;
						size_image = _bitmap_info_header._width * _bitmap_info_header._height * 3;
						width = _bitmap_info_header._width;
						height = _bitmap_info_header._height;					
					}
					else
					{
						rux_write_error( "conversion from RGB 8,16,32 bits to RGB 24 bits is not supported yet'" , error );
					}
				}*/
				else
				{
					char sfcc[5] = {0}; *((rux::uint32*)sfcc) = _bitmap_info_header._compression;
					rux_write_error( ::rux::XString::Format( " , conversion from fourcc '%s' to RGB 24 is not supported yet" , sfcc ) , error );
				}
				if( error.Length() == 0 )
				{
					size = sizeof( ::rux::media::XBitmapFileHeader ) + sizeof( ::rux::media::XBitmapInfoHeader ) + size_image;
					if( destination_data_ptr && destination_data_ptr_size < size )
					{
						rux::engine::free_mem( destination_data_ptr );
						destination_data_ptr = NULL;
						destination_data_ptr_size = 0;
					}
					if( destination_data_ptr == NULL )
					{
						destination_data_ptr = alloc_array_macros( ::rux::uint8 , size );
						destination_data_ptr_size = size;
					}
					((rux::media::XBitmapFileHeader*)destination_data_ptr)->_type = 0x4D42;
					((rux::media::XBitmapFileHeader*)destination_data_ptr)->_size = sizeof( ::rux::media::XBitmapFileHeader ) + sizeof( _bitmap_info_header ) + _bitmap_info_header._size_image;
					((rux::media::XBitmapFileHeader*)destination_data_ptr)->_reserved1 = 0;
					((rux::media::XBitmapFileHeader*)destination_data_ptr)->_reserved2 = 0;
					((rux::media::XBitmapFileHeader*)destination_data_ptr)->_off_bits = sizeof( ::rux::media::XBitmapFileHeader ) + sizeof( _bitmap_info_header );			
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_bit_count = bit_count;
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_clr_important = 0;
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_clr_used = 0;
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_compression = ::rux::media::XEnum_Fourcc_BI_RGB;
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_height = height;
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_planes = 1;
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_size = sizeof( ::rux::media::XBitmapInfoHeader );
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_size_image = size_image;
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_width = width;
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_x_pels_per_meter = 0;
					((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_y_pels_per_meter = 0;			
					if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YV12 )
					{				
						::rux::uint8* bgr_data_ptr = (uint8*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) + sizeof( ::rux::media::XBitmapInfoHeader ) ];
						::rux::int32 bgr_data_size = size_image;
						rux::media::rux_YV12_to_RotatedBGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr_data_ptr , bgr_data_size );
					}
					/*else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_I420 )
					{
						rux::media::rux_I420_to_RotatedBGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
					}
					else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_YUYV )
					{				
						rux::media::rux_YUYV_to_RotatedBGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );
					}*/
					else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BGR3 )
					{
						char* rgb_ptr = (char*)_image_data;			
						rux::uint32 stride = ((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_width * 3;
						for( ::rux::uint32 index0 = ((rux::media::XBitmapInfoHeader*)&destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) ] )->_size_image - stride ; index0 >= 0 ; index0 -= stride )
						{
							memcpy( &destination_data_ptr[ sizeof( ::rux::media::XBitmapFileHeader ) + sizeof( ::rux::media::XBitmapInfoHeader ) + index0 ] , rgb_ptr , stride );
							rgb_ptr += stride;
							if( index0 == 0 )
								break;
						}
					}
					/*else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB
						|| _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_BI_RGB )
					{
						if( _bitmap_info_header._bit_count == 24 )
						{					
							rux::media::rux_RGB24_to_RotatedBGR24( _image_data , _bitmap_info_header._width , _bitmap_info_header._height , bgr24_pointer->_image_data , bgr24_pointer->_image_data_size );					
						}			
					}	*/	
				}
			}
			else
			{
				rux_write_error( "frame data size is equal 0" , error );
			}	
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_result_function_2( ::rux::uint8* , Frame , ConvertToBmpData , uint64& , ::rux::XString& );
		::rux::uint8* Frame::ConvertToBmpData( uint64& size , ::rux::XString& error )
		{
			error.Clear();
			::rux::uint8* bmp_data_ptr = NULL;
			WRITE_LOCK( _cs_image_data );
			if( _bitmap_info_header._size_image > 0 )
			{
				if( _bitmap_info_header._compression != ::rux::media::XEnum_Fourcc_BGR3 )
				{		
					XFrame frame = ConvertToBGR24( error );
					if( error.Length() == 0 )
					{
						bmp_data_ptr = frame.ConvertToBmpData( size , error );
					}
				}
				else
				{
					rux::media::XBitmapFileHeader bitmap_file_header;
					bitmap_file_header._type = 0x4D42;
					bitmap_file_header._size = sizeof( bitmap_file_header ) + sizeof( _bitmap_info_header ) + _bitmap_info_header._size_image;
					bitmap_file_header._reserved1 = 0;
					bitmap_file_header._reserved2 = 0;
					bitmap_file_header._off_bits = sizeof( bitmap_file_header ) + sizeof( _bitmap_info_header );			
					rux::media::XBitmapInfoHeader bitmap_info_header;
					bitmap_info_header._bit_count = _bitmap_info_header._bit_count;
					bitmap_info_header._clr_important = _bitmap_info_header._clr_important;
					bitmap_info_header._clr_used = _bitmap_info_header._clr_used;
					bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_BI_RGB;
					bitmap_info_header._height = _bitmap_info_header._height;
					bitmap_info_header._planes = _bitmap_info_header._planes;
					bitmap_info_header._size = _bitmap_info_header._size;
					bitmap_info_header._size_image = _bitmap_info_header._size_image;
					bitmap_info_header._width = _bitmap_info_header._width;
					bitmap_info_header._x_pels_per_meter = _bitmap_info_header._x_pels_per_meter;
					bitmap_info_header._y_pels_per_meter = _bitmap_info_header._y_pels_per_meter;		
					size = sizeof( bitmap_file_header ) + sizeof( bitmap_info_header ) + bitmap_info_header._size_image;
					bmp_data_ptr = alloc_array_macros( ::rux::uint8 , size );
					memcpy( bmp_data_ptr , &bitmap_file_header , sizeof( bitmap_file_header ) );
					memcpy( &bmp_data_ptr[ sizeof( bitmap_file_header ) ] , &bitmap_info_header , sizeof( bitmap_info_header ) );			
					char* rgb_ptr = (char*)_image_data;			
					rux::uint32 stride = bitmap_info_header._width * 3;
					for( ::rux::uint32 index0 = bitmap_info_header._size_image - stride ; index0 >= 0 ; index0 -= stride )
					{
						memcpy( &bmp_data_ptr[ sizeof( bitmap_file_header ) + sizeof( bitmap_info_header ) + index0 ] , rgb_ptr , stride );
						rgb_ptr += stride;
						if( index0 == 0 )
							break;
					}			
				}
			}
			else
			{
				rux_write_error( "frame data size is equal 0" , error );
			}
			_cs_image_data.WriteUnlock();
			return bmp_data_ptr;		
		};
		implement_duplicate_internal_result_function_with_const( ::rux::int32 , Frame , get_Fourcc );
		::rux::int32 Frame::get_Fourcc( void ) const
		{
			return _bitmap_info_header._compression;
		};
		implement_duplicate_internal_result_function( uint16 , Frame , get_BitCount );
		uint16 Frame::get_BitCount( void )
		{
			return _bitmap_info_header._bit_count;
		};
		implement_duplicate_internal_function( Frame , LockData );
		void Frame::LockData( void )
		{
			WRITE_LOCK( _cs_image_data );
		};
		implement_duplicate_internal_function( Frame , UnLockData );
		void Frame::UnLockData( void )
		{
			_cs_image_data.WriteUnlock();
		};
		implement_duplicate_internal_function( Frame , set_EmptyData );
		void Frame::set_EmptyData( void )
		{
			WRITE_LOCK( _cs_image_data );
			if( _media_image )
			{
				_media_image->release();
				_media_image = NULL;
			}
			_bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_Unknown;
			_cs_image_data.WriteUnlock();
		};
		void RGBtoHSV( ::rux::uint8 r_byte , ::rux::uint8 g_byte , ::rux::uint8 b_byte , float& h, float& s, float& v )
		{	
			float r = (float)r_byte / 255.f , g = (float)g_byte / 255.f , b = (float)b_byte / 255.f;
			float min = std::min< float >( r , g );
			min = std::min< float >( b , min );
			float max = std::max< float >( r , g );
			max = std::max< float >( b , max );
			v = max;				// v
			float delta = max - min;
			if( max != 0 )
				s = delta / max;		// s
			else
			{
				// r = g = b = 0		// s = 0, v is undefined
				s = 0;
				h = -1;
				return;
			}
			if( delta == 0 )
				h = 0;
			else if( r == max )
				h = ( g - b ) / delta;		// between yellow & magenta
			else if( g == max )
				h = 2 + ( b - r ) / delta;	// between cyan & yellow
			else
				h = 4 + ( r - g ) / delta;	// between magenta & cyan
			h *= 60;				// degrees
			if( h < 0 )
				h += 360;
		}
		void HSVtoRGB( ::rux::uint8& r_byte , ::rux::uint8& g_byte , ::rux::uint8& b_byte , float h , float s , float v )
		{
			float r = 0.f , g = 0.f , b = 0.f;
			if( s == 0 ) 
			{
				// achromatic (grey)
				r = g = b = v;
				r_byte = g_byte = b_byte = (uint8)floor( r * 255.f + 0.5f );
				return;
			}
			h /= 60;			// sector 0 to 5
			float i = floor( h );
			float f = h - i;			// factorial part of h
			float p = v * ( 1 - s );
			float q = v * ( 1 - s * f );
			float t = v * ( 1 - s * ( 1 - f ) );
			switch( (int32)i ) 
			{
			case 0:
				{
					r = v;
					g = t;
					b = p;
					break;
				}
			case 1:
				{
					r = q;
					g = v;
					b = p;
					break;
				}
			case 2:
				{
					r = p;
					g = v;
					b = t;
					break;
				}
			case 3:
				{
					r = p;
					g = q;
					b = v;
					break;
				}
			case 4:
				{
					r = t;
					g = p;
					b = v;
					break;
				}
			default:
				{
					r = v;
					g = p;
					b = q;
					break;
				}
			}
			r_byte = (uint8)floor( r * 255.f + 0.5f );
			g_byte = (uint8)floor( g * 255.f + 0.5f );
			b_byte = (uint8)floor( b * 255.f + 0.5f );
		};
		void XFrame::ReplaceNotTransparentColors( ::rux::gui::Color* base_replace_color , ::rux::XString& error )
		{	
			set_ByRef( ConvertToRGB32( 255 , error ) );
			if( error.Length() == 0 )
			{			
				::rux::uint8* replace_rgba = base_replace_color ? base_replace_color->get_ShaderColorMap() : NULL;
				float replace_hue = 0.f , replace_saturation = 0.f , replace_value = 0.f;
				if( replace_rgba )
				{
					Frame* __this = (*this)();
					RGBtoHSV( replace_rgba[ 0 ] , replace_rgba[ 1 ] , replace_rgba[ 2 ] ,
						replace_hue , replace_saturation , replace_value );
					WRITE_LOCK( __this->_cs_image_data );		
					::rux::uint8* image_data = __this->_image_data;
					size_t size_image = __this->_bitmap_info_header._size_image;		
					for( size_t index0 = 0 ; index0 < size_image ; index0 += 4 )
					{
						if( image_data[ index0 + 3 ] > 0 )
						{
							float hue = 0.f , saturation = 0.f , value = 0.f;
							RGBtoHSV( image_data[ index0 ] , image_data[ index0 + 1 ] , image_data[ index0 + 2 ] ,
								hue , saturation , value );
							HSVtoRGB( image_data[ index0 ] , image_data[ index0 + 1 ] , image_data[ index0 + 2 ] ,
								replace_hue , saturation == 0 ? replace_saturation : saturation , value );
						}
					}
					__this->_cs_image_data.WriteUnlock();
				}
			}	
		};
		void XFrame::set_Opacity( float opacity , ::rux::XString& error )
		{
			Frame* __this = (*this)();
			error.Clear();
			set_ByRef( ConvertToRGB32( 255 , error ) );
			if( error.Length() == 0 )
			{		
				WRITE_LOCK( __this->_cs_image_data );		
				::rux::uint8* image_data = __this->_image_data;
				size_t size_image = __this->_bitmap_info_header._size_image;		
				for( size_t index0 = 3 ; index0 < size_image ; index0 += 4 )
					image_data[ index0 ] *= opacity;
				__this->_cs_image_data.WriteUnlock();
			}
		};
	};
};