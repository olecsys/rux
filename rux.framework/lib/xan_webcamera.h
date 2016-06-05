#ifndef XAN_WEBCAMERA_H
#define XAN_WEBCAMERA_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_image.h>
#include <xan_thread.h>
#ifdef __LINUX__
#ifndef __ANDROID__
#include <linux/videodev2.h>
#endif
#endif
namespace rux
{
	namespace media
	{
		class WebCamera;
	};
};
typedef void (*rux_on_frame_callback)( rux::media::WebCamera* sender , const rux::media::XFrame& frame );
enum XEnum_WebCameraCaptureType
{
	XEnum_WebCameraCaptureType_READ,
	XEnum_WebCameraCaptureType_MMAP,
	XEnum_WebCameraCaptureType_USERPTR
};
namespace rux
{	
	namespace media
	{			
		begin_declare_rux_class( WebCamera );
			~WebCamera();
			//rux::Array< rux::media::XFrameFormat >& get_FrameFormats( rux::XString& error );
			//void StartCapture( const rux::media::XFrameFormat& frame_format , rux::XString& error );
			//void StartCapture( rux::media::FrameFormat& frame_format , rux::XString& error );
			void StopCapture();
			void set_OnFrame( rux_on_frame_callback on_frame_callback );
#ifdef __LINUX__
			static int32 rux_ioctl( int32 fd , int32 request , void* arg );
			static void free_mem_unmap( void* image_data_ptr , rux::uint32 image_data_ptr_size );
#endif
			static void private_CapturingThread( void* param );	
		begin_declare_rux_class_members( WebCamera );
			rux::XString _name;
		#ifdef __LINUX__
			int32 _descriptor;
			rux::uint32 _capabilities;
		#endif	
			XEnum_WebCameraCaptureType _capture_type;
			rux::XArray< rux::media::XFrame > _frames;
			XCrtSect _cs_capture_thread;
			rux::threading::XThread _capture_thread;
			XCrtSect _cs_on_frame_callback;
			rux_on_frame_callback _on_frame_callback;
		end_declare_rux_class_members( WebCamera );			
			//rux::Array< rux::media::XFrameFormat >& get_FrameFormats( rux::XString& error );
			static rux::Array< XWebCamera >& get_MediaDevices( rux::XString& error );		
			//void StartCapture( const rux::media::XFrameFormat& frame_format , rux::XString& error );
			//void StartCapture( rux::media::FrameFormat& frame_format , rux::XString& error );
			void StopCapture();
			void set_OnFrame( rux_on_frame_callback on_frame_callback );
		end_declare_rux_class();
	};
};
#endif
