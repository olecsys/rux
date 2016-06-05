#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_webcamera.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_file_descriptor_waiter.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( WebCamera , rux::media );
#ifdef __LINUX__
	_descriptor = -1;
	_capabilities = 0;
#endif
	_capture_type = XEnum_WebCameraCaptureType_MMAP;
	_on_frame_callback= NULL;
end_implement_rux_class();
namespace rux
{
	namespace media
	{	
		WebCamera::~WebCamera()
		{
		#ifdef __LINUX__
			if( _descriptor != -1 )
				close( _descriptor );
		#endif
		};
		//implement_duplicate_internal_result_function_1( rux::Array< rux::media::XFrameFormat >& , WebCamera , get_FrameFormats , rux::XString& );
		//implement_duplicate_internal_function_2( WebCamera , StartCapture , const rux::media::XFrameFormat& , rux::XString& );
		//implement_duplicate_internal_function_2( WebCamera , StartCapture , rux::media::FrameFormat& , rux::XString& );
		implement_duplicate_internal_function( WebCamera , StopCapture );
		implement_duplicate_internal_function_1( WebCamera , set_OnFrame , rux_on_frame_callback );
		rux::Array< XWebCamera >& XWebCamera::get_MediaDevices( rux::XString& error )
		{
			error.Clear();
			rux::XArray< XWebCamera > media_devices;
		#ifdef __LINUX__
		#ifndef __ANDROID__
			DIR* dir_ptr = opendir( "/dev/" );
			XWebCamera media_device;
			rux::XString name;
			int32 device_descriptor = -1;
			struct v4l2_capability capability;
			if( dir_ptr )
			{
				::rux::int32 name_max = pathconf( "/dev/" , _PC_NAME_MAX );
				if( name_max == -1 )
					name_max = 255;
				size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
				dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
				dirent* dirent_result = 0;		
				rux::uint32 symbol = 0;
				while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
				{
					if( rux::XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
						&& rux::XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
					{				
						if( dirent_object_ptr->d_type == DT_CHR )
						{
							name.set_ByRef( rux::XString() );
							name = "/dev/";
							name += dirent_object_ptr->d_name;
							device_descriptor = open( name.str() , O_RDWR | O_NONBLOCK, 0 );
							if( device_descriptor != -1 )
							{						
								memset( &capability , 0 , sizeof( capability ) );
								if( rux::media::WebCamera::rux_ioctl( device_descriptor , VIDIOC_QUERYCAP, &capability ) != -1 )
								{
									if( ( capability.capabilities & V4L2_CAP_VIDEO_CAPTURE ) != 0 )
									{					
										media_device = XWebCamera();
										media_device()->_name.set_ByRef( name );
										media_device()->_capabilities = capability.capabilities;
										media_devices.Add( media_device );
									}
								}
								close( device_descriptor );
							}
						}
					}
				}
				closedir( dir_ptr );
				::rux::engine::free_mem( dirent_object_ptr );	
			}
			else
			{
				rux_write_error( "directory '/dev/' does not exist or access is denied\n" , error );
			}
		#endif
		#endif
			return media_devices++;
		};
		/*rux::Array< XFrameFormat >& WebCamera::get_FrameFormats( rux::XString& error )
		{
			error.Clear();
			rux::XArray< XFrameFormat > frame_formats;
			XFrameFormat frame_format;
		#ifdef __LINUX__
		#ifndef __ANDROID__
			if( _descriptor == -1 )
				_descriptor = open( _name.str() , O_RDWR | O_NONBLOCK, 0 );
			if( _descriptor != -1 )
			{						
				v4l2_fmtdesc fmtdesc;
				v4l2_frmsizeenum framesizeenum;
				int32 result = -1;
				rux::uint32 index1 = 0 , index2 = 0;
				do
				{
					memset( &fmtdesc , 0 , sizeof( fmtdesc ) );
					fmtdesc.index = index1;
					fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
					result = rux_ioctl( _descriptor , VIDIOC_ENUM_FMT , &fmtdesc );
					if( result != -1 )
					{
						index2 = 0;
						do
						{
							memset( &framesizeenum , 0 , sizeof( framesizeenum ) );
							framesizeenum.index = index2;
							framesizeenum.pixel_format = fmtdesc.pixelformat;
							result = rux_ioctl( _descriptor , VIDIOC_ENUM_FRAMESIZES , &framesizeenum );
							if( result != -1 )
							{
								if( framesizeenum.type == V4L2_FRMSIZE_TYPE_DISCRETE )
								{
									frame_format = XFrameFormat();
									if( fmtdesc.pixelformat == V4L2_PIX_FMT_YVU420 )
									{
										frame_format()->_bitmap_info_header._width = framesizeenum.discrete.width;
										frame_format()->_bitmap_info_header._height = framesizeenum.discrete.height;
										frame_format()->_bitmap_info_header._compression = rux::media::XEnum_Fourcc_YV12;
										frame_format()->_bitmap_info_header._size_image = frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height + frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height / 2;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_YUV420 )
									{
										frame_format()->_bitmap_info_header._width = framesizeenum.discrete.width;
										frame_format()->_bitmap_info_header._height = framesizeenum.discrete.height;
										frame_format()->_bitmap_info_header._compression = rux::media::XEnum_Fourcc_I420;
										frame_format()->_bitmap_info_header._size_image = frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height + frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height / 2;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB24 )
									{
										frame_format()->_bitmap_info_header._width = framesizeenum.discrete.width;
										frame_format()->_bitmap_info_header._height = framesizeenum.discrete.height;
										frame_format()->_bitmap_info_header._bit_count = 24;
										frame_format()->_bitmap_info_header._compression = rux::media::XEnum_Fourcc_RGB;
										frame_format()->_bitmap_info_header._size_image = frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height * 3;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB32 )
									{
										frame_format()->_bitmap_info_header._width = framesizeenum.discrete.width;
										frame_format()->_bitmap_info_header._height = framesizeenum.discrete.height;
										frame_format()->_bitmap_info_header._bit_count = 32;
										frame_format()->_bitmap_info_header._compression = rux::media::XEnum_Fourcc_RGB;
										frame_format()->_bitmap_info_header._size_image = frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height * 4;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB332 )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB444 )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%u' is unsupported\n" , fmtdesc.pixelformat ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB555 )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%u' is unsupported\n" , fmtdesc.pixelformat ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB565 )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%u' is unsupported\n" , fmtdesc.pixelformat ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB555X )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%u' is unsupported\n" , fmtdesc.pixelformat ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB565X )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%u' is unsupported\n" , fmtdesc.pixelformat ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_BGR24 )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%u' is unsupported\n" , fmtdesc.pixelformat ) , error );
										break;
									}					
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_BGR32 )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%u' is unsupported\n" , fmtdesc.pixelformat ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_GREY )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%u' is unsupported\n" , fmtdesc.pixelformat ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_Y16 )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%u' is unsupported\n" , fmtdesc.pixelformat ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_YUYV )
									{
										frame_format()->_bitmap_info_header._width = framesizeenum.discrete.width;
										frame_format()->_bitmap_info_header._height = framesizeenum.discrete.height;
										frame_format()->_bitmap_info_header._compression = rux::media::XEnum_Fourcc_YUYV;
										frame_format()->_bitmap_info_header._size_image = 2 * frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height;								
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_UYVY )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_Y41P )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_YVU410 )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_YUV422P )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_YUV411P )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );
										break;
									}
									else if( fmtdesc.pixelformat == V4L2_PIX_FMT_NV12 )
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );
										break;
									}
									else
									{
										char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
										rux_write_error( rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );
										break;
									}
									frame_formats.Add( frame_format );
								}
								else
								{
									rux_write_error( rux::XString::Format( "v4l2_frmsizeenum type '%u' is unsupported\n" , framesizeenum.type ) , error );
									break;
								}
							}
							else
								break;
							index2++;
						}
						while( result != -1 );
						if( index2 == 0 )
						{
							frame_format = XFrameFormat();
							if( fmtdesc.pixelformat == V4L2_PIX_FMT_YVU420 )
							{
								frame_format()->_bitmap_info_header._width = 640;
								frame_format()->_bitmap_info_header._height = 480;						
								frame_format()->_bitmap_info_header._compression = rux::media::XEnum_Fourcc_YV12;
								frame_format()->_bitmap_info_header._size_image = frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height + frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height / 2;
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_YUV420 )
							{
								frame_format()->_bitmap_info_header._width = 640;
								frame_format()->_bitmap_info_header._height = 480;
								frame_format()->_bitmap_info_header._compression = rux::media::XEnum_Fourcc_I420;
								frame_format()->_bitmap_info_header._size_image = frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height + frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height / 2;
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB24 )
							{
								frame_format()->_bitmap_info_header._width = 640;
								frame_format()->_bitmap_info_header._height = 480;
								frame_format()->_bitmap_info_header._bit_count = 24;
								frame_format()->_bitmap_info_header._compression = rux::media::XEnum_Fourcc_RGB;
								frame_format()->_bitmap_info_header._size_image = frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height * 3;
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB32 )
							{
								frame_format()->_bitmap_info_header._width = 640;
								frame_format()->_bitmap_info_header._height = 480;
								frame_format()->_bitmap_info_header._bit_count = 32;
								frame_format()->_bitmap_info_header._compression = rux::media::XEnum_Fourcc_RGB;
								frame_format()->_bitmap_info_header._size_image = frame_format()->_bitmap_info_header._width * frame_format()->_bitmap_info_header._height * 4;
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB332 )
							{
						
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB444 )
							{
						
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB555 )
							{
						
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB565 )
							{
						
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB555X )
							{
						
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_RGB565X )
							{
						
							}
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_BGR24 )
							{
						
							}					
							else if( fmtdesc.pixelformat == V4L2_PIX_FMT_BGR32 )
							{
						
							}					
							else
							{
								char sfcc[5] = {0}; *((rux::uint32*)sfcc) = fmtdesc.pixelformat;
								rux_write_error( rux::XString::Format( "image fourcc '%s' is unsupported" , sfcc ) , error );						
								break;
							}
							frame_formats.Add( frame_format );
						}
					}
					else
					{
						rux_write_error( XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );
						break;
					}
					if( error.Length() > 0 )
						break;
					index1++;
				}
				while( result != -1 );		
			}
			else
			{
				rux_write_error( XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );
			}
		#endif
		#endif
			return frame_formats++;
		};*/
		#ifdef __LINUX__
		#ifndef __ANDROID__
		int32 WebCamera::rux_ioctl( int32 fd , int32 request , void* arg )
		{
			int result = -1;
			do
			{
				result = ioctl( fd , request , arg );
			}
			while( -1 == result && EINTR == errno );
			return result;
		};
		#endif
		#endif
		//void WebCamera::StartCapture( rux::media::FrameFormat& frame_format , rux::XString& error )
		//{
		//	error.Clear();
		//#ifdef __LINUX__
		//#ifndef __ANDROID__
		//	if( _descriptor == -1 )
		//		_descriptor = open( _name.str() , O_RDWR | O_NONBLOCK, 0 );
		//	if( _descriptor != -1 )
		//	{						
		//		switch( _capture_type ) 
		//		{
		//		case XEnum_WebCameraCaptureType_READ:
		//			{
		//				if( ( _capabilities & V4L2_CAP_READWRITE ) == 0 )
		//				{							
		//					rux_write_error( "Media device '" + _name + "' does not support readwrite" , error );
		//				}
		//				break;
		//			}
		//		case XEnum_WebCameraCaptureType_MMAP:
		//		case XEnum_WebCameraCaptureType_USERPTR:
		//			{
		//				if( ( _capabilities & V4L2_CAP_STREAMING ) == 0 )
		//				{						
		//					rux_write_error( "Web camera '" + _name + "' does not support streaming" , error );
		//				}
		//				break;					
		//			}
		//		}
		//		if( error.Length() == 0 )
		//		{
		//			struct v4l2_cropcap cropcap;
		//			struct v4l2_crop crop;
		//			memset( &cropcap , 0 , sizeof( cropcap ) );
		//			memset( &crop , 0 , sizeof( crop ) );
		//			cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		//			if( rux_ioctl( _descriptor , VIDIOC_CROPCAP , &cropcap ) == 0 )
		//			{
		//				crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		//				crop.c = cropcap.defrect;
		//				if( -1 == rux_ioctl( _descriptor , VIDIOC_S_CROP, &crop ) )
		//				{
		//					switch( errno )
		//					{
		//						case EINVAL:
		//						{
		//							break;							
		//						}
		//						default:
		//						{
		//							break;
		//						}
		//					}
		//				}
		//			}
		//			else
		//			{				
		//			}			
		//			struct v4l2_format format;
		//			rux::uint32 min = 0;					
		//			memset( &format , 0 , sizeof( format ) );

		//			format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		//			format.fmt.pix.width = frame_format._bitmap_info_header._width;
		//			format.fmt.pix.height = frame_format._bitmap_info_header._height;
		//			if( frame_format._bitmap_info_header._compression == rux::media::XEnum_Fourcc_YV12 )
		//				min = V4L2_PIX_FMT_YVU420;
		//			else if( frame_format._bitmap_info_header._compression == rux::media::XEnum_Fourcc_I420 )
		//				min = V4L2_PIX_FMT_YUV420;
		//			else if( frame_format._bitmap_info_header._compression == rux::media::XEnum_Fourcc_YUYV )
		//				min = V4L2_PIX_FMT_YUYV;
		//			else if( frame_format._bitmap_info_header._compression == rux::media::XEnum_Fourcc_RGB )
		//			{
		//				if( frame_format._bitmap_info_header._bit_count == 24 )
		//					min = V4L2_PIX_FMT_RGB24;
		//				else if( frame_format._bitmap_info_header._bit_count == 32 )
		//					min = V4L2_PIX_FMT_RGB32;
		//				else
		//				{
		//					rux_write_error( rux::XString::Format( "Web camera '%s' does not support image fourcc RGB with bit count = %u" , _name.str() , frame_format._bitmap_info_header._bit_count ) , error );							
		//				}
		//			}
		//			else
		//			{
		//				char sfcc[5] = {0}; *((rux::uint32*)sfcc) = frame_format._bitmap_info_header._compression;
		//				rux_write_error( rux::XString::Format( "Web camera '%s' does not support image fourcc '%s'" , _name.str() , sfcc ) , error );							
		//			}
		//			if( error.Length() == 0 )
		//			{
		//				format.fmt.pix.pixelformat = min;
		//				format.fmt.pix.field = V4L2_FIELD_INTERLACED;
		//				if( rux_ioctl( _descriptor , VIDIOC_S_FMT , &format ) == -1 )
		//				{
		//						rux_write_error( rux::XString::Format( "Web camera '%s' does not support image fourcc '%u'" , _name.str() , frame_format._bitmap_info_header._compression ) , error );
		//				}
		//				if( error.Length() == 0 )
		//				{
		//					if( format.fmt.pix.pixelformat == V4L2_PIX_FMT_YVU420
		//						|| format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUV420 )
		//					{						
		//						min = 3 * format.fmt.pix.width * 2;
		//						if( format.fmt.pix.bytesperline < min )
		//							format.fmt.pix.bytesperline = min;
		//						min = format.fmt.pix.bytesperline * format.fmt.pix.height;
		//						if( format.fmt.pix.sizeimage < min )
		//							format.fmt.pix.sizeimage = min;
		//					}
		//					else
		//					{
		//						min = format.fmt.pix.width * 2;
		//						if( format.fmt.pix.bytesperline < min )
		//							format.fmt.pix.bytesperline = min;
		//						min = format.fmt.pix.bytesperline * format.fmt.pix.height;
		//						if( format.fmt.pix.sizeimage < min )
		//							format.fmt.pix.sizeimage = min;			
		//					}
		//					switch( _capture_type )
		//					{
		//						case XEnum_WebCameraCaptureType_READ:
		//						{
		//							//init_read (format.fmt.pix.sizeimage , format.fmt.pix.width , format.fmt.pix.height , error );
		//							//start_capturing();
		//							//mainloop( device_descriptor );
		//							break;
		//						}
		//						case XEnum_WebCameraCaptureType_MMAP:
		//						{
		//							struct v4l2_requestbuffers requestbuffers;
		//							memset( &requestbuffers , 0 , sizeof( requestbuffers ) );
		//							requestbuffers.count = 5;
		//							requestbuffers.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		//							requestbuffers.memory = V4L2_MEMORY_MMAP;
		//							if( -1 == rux_ioctl( _descriptor , VIDIOC_REQBUFS , &requestbuffers ) )
		//							{								
		//								rux_write_error( "VIDIOC_REQBUFS , " + XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );
		//							}
		//							if( error.Length() == 0 )
		//							{
		//								if( requestbuffers.count < 2 )
		//								{
		//									rux_write_error( "too few buffers count" , error );
		//								}
		//								if( error.Length() == 0 )
		//								{
		//									struct v4l2_buffer buffer;
		//									XFrame frame;
		//									uint8* image_data_ptr = NULL;
		//									rux::uint32 index0 = 0;
		//									_frames.Clear();
		//									for( index0 = 0 ; index0 < requestbuffers.count ; ++index0 )
		//									{
		//										memset( &buffer , 0 , sizeof( buffer ) );
		//										buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		//										buffer.memory = V4L2_MEMORY_MMAP;
		//										buffer.index = index0;
		//										if( -1 == rux_ioctl( _descriptor , VIDIOC_QUERYBUF , &buffer ) )
		//										{
		//											rux_write_error( "VIDIOC_QUERYBUF , " + XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );
		//											break;
		//										}
		//										frame = XFrame();
		//										image_data_ptr = (uint8*)mmap( NULL , buffer.length , PROT_READ | PROT_WRITE , MAP_SHARED , _descriptor , buffer.m.offset );
		//										if( MAP_FAILED == image_data_ptr )
		//										{
		//											rux_write_error( "mmap return MAP_FAILED" , error );											
		//										}
		//										if( error.Length() == 0 )
		//										{
		//											//frame.set_ImageData( (char*)image_data_ptr , frame_format._bitmap_info_header._width , frame_format._bitmap_info_header._height , (rux::media::XEnum_Fourcc)frame_format._bitmap_info_header._compression , free_mem_unmap , error );
		//											_frames.Add( frame );
		//										}
		//									}
		//									if( error.Length() == 0 )
		//									{
		//										enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		//										if( -1 == rux_ioctl( _descriptor , VIDIOC_STREAMON , &buf_type ) )
		//										{
		//											rux_write_error( "VIDIOC_STREAMON , " + XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );											
		//										}
		//									}
		//								}
		//							}
		//							break;
		//						}
		//						case XEnum_WebCameraCaptureType_USERPTR:
		//						{
		//							//init_userp (fmt.fmt.pix.sizeimage);
		//							//start_capturing();
		//							//mainloop( device_descriptor );
		//							break;
		//						}
		//					}				
		//					if( error.Length() == 0 )
		//					{
		//						CRITICAL_SECTION_LOCK( _cs_capture_thread );
		//						if( _capture_thread.get_IsStarted() == 0 )
		//						{
		//							_capture_thread.set_ThreadMode( XThreadMode_Repeat );
		//							_capture_thread.set_Timeout( 1 );		
		//							_capture_thread.set_ThreadName( "media capturing" );
		//							_capture_thread.Start();								
		//						}
		//						_cs_capture_thread.UnLock();
		//						_capture_thread.Push( private_CapturingThread , this );
		//					}
		//				}
		//			}
		//		}
		//	}
		//	else
		//	{
		//		rux_write_error( XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );
		//	}
		//#endif
		//#endif
		//};
		/*void WebCamera::StartCapture( const XFrameFormat& frame_format , rux::XString& error )
		{	
			StartCapture( *frame_format() , error );
		};*/
		void WebCamera::StopCapture()
		{
			CRITICAL_SECTION_LOCK( _cs_capture_thread );
			if( _capture_thread.get_IsStarted() == 1 )
			{
				_capture_thread.Stop();
		#ifdef __LINUX__		
		#ifndef __ANDROID__
				if( _descriptor != -1 )
				{
					switch( _capture_type )		
					{
						case XEnum_WebCameraCaptureType_READ:
						{
							break;
						}
						case XEnum_WebCameraCaptureType_MMAP:
						case XEnum_WebCameraCaptureType_USERPTR:
						{
							enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
							if( -1 == rux_ioctl( _descriptor , VIDIOC_STREAMOFF , &buf_type ) )
							{
								rux::XString error;
								rux_write_error( "VIDIOC_STREAMOFF , " + XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );											
							}
							break;
						}		
					}
				}
		#endif
		#endif	
			}
			_cs_capture_thread.UnLock();
		};
		#ifdef __LINUX__
		#ifndef __ANDROID__
		void WebCamera::free_mem_unmap( void* image_data_ptr , rux::uint32 image_data_ptr_size )
		{
			if( image_data_ptr )
				munmap( image_data_ptr , image_data_ptr_size );
		}
		#endif
		#endif
		void WebCamera::private_CapturingThread( void* param )
		{
			WebCamera* media_device_ptr = (WebCamera*)param;
			if( media_device_ptr->_capture_thread.get_IsWaitForExit() == 0 )
			{
				rux::XString error;
		#ifdef __LINUX__
		#ifndef __ANDROID__
				::rux::io::file_descriptor_waiter fd_waiter;
				fd_waiter.add( media_device_ptr->_descriptor );
				for( ; ; )
				{
					int32 result = 0;
					if( fd_waiter.wait( 2000 , 1 , 0 ) )
					{				
						struct v4l2_buffer buffer;
						result = 1;
						switch( media_device_ptr->_capture_type )
						{
							case XEnum_WebCameraCaptureType_READ:
							{
					//			if( -1 == read( device_descriptor , buffers[0].start , buffers[0].length ) )
								{
									switch( errno )
									{
										case EAGAIN:
										{
											result = 0;
											break;
										}
										case EIO:
										default:
										{
					//						errno_exit ("read");						
										}
									}
								}
					//			process_image (buffers[0].start);
								break;
							}
							case XEnum_WebCameraCaptureType_MMAP:
							{
								memset( &buffer , 0 , sizeof( buffer ) );
								buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
								buffer.memory = V4L2_MEMORY_MMAP;
								if( -1 == rux_ioctl( media_device_ptr->_descriptor , VIDIOC_DQBUF , &buffer ) )
								{
									switch( errno )
									{
										case EAGAIN:
										{
											result = 0;
											break;
										}
										case EIO:
										default:
										{
					//						errno_exit ("VIDIOC_DQBUF");						
										}
									}
								}
								if( media_device_ptr->_frames.Count() > buffer.index )
								{
									CRITICAL_SECTION_LOCK( media_device_ptr->_cs_on_frame_callback );
									if( media_device_ptr->_on_frame_callback )
										media_device_ptr->_on_frame_callback( media_device_ptr , media_device_ptr->_frames[ buffer.index ] );
									media_device_ptr->_cs_on_frame_callback.UnLock();
								}
					//			assert (buf.index < n_buffers);
					//			process_image (buffers[buf.index].start);
								if( -1 == rux_ioctl( media_device_ptr->_descriptor , VIDIOC_QBUF , &buffer ) )
								{
					//				errno_exit ("VIDIOC_QBUF");
								}
								break;
							}
							case XEnum_WebCameraCaptureType_USERPTR:
							{
								memset( &buffer , 0 , sizeof( buffer ) );
								buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
								buffer.memory = V4L2_MEMORY_USERPTR;
								if( -1 == rux_ioctl( media_device_ptr->_descriptor , VIDIOC_DQBUF , &buffer ) )
								{
									switch( errno )
									{
										case EAGAIN:
										{
											result = 0;
											break;
										}
										case EIO:
										default:
										{
					//						errno_exit ("VIDIOC_DQBUF");
										}
									}
								}
					//			for( i = 0 ; i < n_buffers ; ++i )
					//			{
					//				if (buf.m.userptr == (unsigned long) buffers[i].start
					//					&& buf.length == buffers[i].length)
					//					break;
					//			}
					//			assert (i < n_buffers);
					//			process_image ((void *) buffer.m.userptr);
								if( -1 == rux_ioctl( media_device_ptr->_descriptor , VIDIOC_QBUF , &buffer ) )
								{
					//				errno_exit ("VIDIOC_QBUF");
								}
								break;
							}
						}
						if( result != 0 )
						{
							break;
						}
					}
					else
						break;
				}
		#endif
		#endif
			}
		};
		void WebCamera::set_OnFrame( rux_on_frame_callback on_frame_callback )
		{
			CRITICAL_SECTION_LOCK( _cs_on_frame_callback );
			_on_frame_callback = on_frame_callback;
			_cs_on_frame_callback.UnLock();
		};
	};
};