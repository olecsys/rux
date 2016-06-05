#ifndef XAN_PIPE_H
#define XAN_PIPE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_thread.h>
#include <xan_async_command_pool.h>
#include <xan_data.h>
typedef void (*rux_on_pipe_callback)( XData& received_data , XData& send_data , const char* client , const char* server );
namespace rux
{	
	begin_declare_rux_class( PipeClient );
		::rux::uint32 _buffer_size;
	begin_declare_rux_class_members( PipeClient );
	end_declare_rux_class_members( PipeClient );
		void Send( const ::rux::XString& pipe_name , void* data , size_t data_size , XData& received_data , ::rux::XString& error );
	end_declare_rux_class();

	begin_declare_rux_class( PipeServer );
		XCrtSect _cs_listen_thread;
		::rux::threading::XThread _listen_thread;
		XCrtSect _cs_buffer_size;
		::rux::uint32 _buffer_size;
		::rux::threading::XAsyncCommandPool _async_command_pool;
	#ifdef __UNIX__
		::rux::int32 _request_fifo;	
		::rux::int32 _unique;
	#endif
		rux_on_pipe_callback _on_pipe_callback;
		::rux::XString _pipe_name;	
		XArray< XString > _allow_user_or_group;
		XArray< XString > _deny_user_or_group;
		~PipeServer();	
		void StartListen( const ::rux::XString& pipe_name , ::rux::XString& error );
		void StopListen( void );
	begin_declare_rux_class_members( PipeServer );
	end_declare_rux_class_members( PipeServer );
		void StartListen( const ::rux::XString& pipe_name , ::rux::XString& error );
		void StopListen( void );
		void AddAllAllowAccessUserOrGroup( const ::rux::XString& user_or_group );
		void AddAllDenyAccessUserOrGroup( const ::rux::XString& user_or_group );	
		void set_OnReceived( rux_on_pipe_callback on_pipe_callback );
		static void listen_thread( void* param , size_t ___rux__thread_index1986 );	
		static void client_thread( void* param , size_t ___rux__thread_index1986 );	
	end_declare_rux_class();
	struct pipe_server_struct
	{
		PipeServer* _pipe_server;
#ifdef __WINDOWS__
		HANDLE _pipe;
#else
		::rux::int32 _unique;
		::rux::int32 _client_request;
#endif
	};
};
#endif
