#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uiclipboard.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
implement_rux_base_methods_ns( Clipboard , rux::gui );
implement_rux_set_operators_ns_( Clipboard , rux::gui );
namespace rux
{
	namespace gui
	{
		implement_ctor_and_register_type( Clipboard );
		Clipboard::Clipboard( const char* , const char* , ::rux::int32 )
			: XGCRef( get_static_Type )
		{
		};
		XClipboard::XClipboard()
		{	
		};
		void XClipboard::x_initialize( void )
		{
		};
		XClipboard::XClipboard( const Clipboard& uiclipboard )
		{
			uiclipboard.AddRef();
			(*this) = uiclipboard;
			uiclipboard.Release();
		};
		XClipboard::XClipboard( const XObject& object )
		{
			*static_cast<XGCHandle<Clipboard>*>(this) = object;
		};
		XClipboard::XClipboard( const XClipboard& uiclipboard )
		{
			(*(static_cast<XGCHandle<Clipboard>*>( this ))) = uiclipboard;
		};
		XClipboard& XClipboard::operator =( const Clipboard& uiclipboard )
		{
			uiclipboard.AddRef();
			(*(static_cast<XGCHandle<Clipboard>*>( this ))) = uiclipboard;
			uiclipboard.Release();
			return *this;
		};
		XClipboard& XClipboard::operator =( const XClipboard& uiclipboard )
		{
			(*(static_cast<XGCHandle<Clipboard>*>( this ))) = uiclipboard;
			return *this;
		};
		rux::String& XClipboard::GetStringData( ::rux::XString& error )
		{
			rux::XString result;
			error.Clear();
		#ifdef __WINDOWS__
			if( IsClipboardFormatAvailable( CF_UNICODETEXT ) )
			{
				if( OpenClipboard( NULL ) )
				{
					HGLOBAL hglb = GetClipboardData( CF_UNICODETEXT );
					if( hglb )
					{
						char* string_ptr = (char*)GlobalLock( hglb );
						if( string_ptr )
							result.set_ByRef( ::rux::XString( string_ptr , XEnumCodePage_UTF16 , 1 , NULL , __FILE__ , __LINE__ ) );
						GlobalUnlock( hglb );
					}
					else
					{
						rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
					}
					CloseClipboard();
				}
				else
				{
					rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
				}
			}
		#else
			rux_write_error( "method does implemented yet" , error );
		#endif
			return result++;
		};
		void XClipboard::SetStringData( const ::rux::XString& data , ::rux::XString& error )
		{	
			error.Clear();
		#ifdef __WINDOWS__	
			if( OpenClipboard( NULL ) )
			{
				if( EmptyClipboard() )
				{
					if( data.Length() > 0 )
					{
						rux::XString utf16_data;
						utf16_data.set_ByRef( data.ConvertToUTF16() );
						HGLOBAL hglb = GlobalAlloc( GMEM_MOVEABLE , utf16_data.Size() );
						if( hglb ) 
						{
							void* memory = GlobalLock( hglb ); 
							memcpy( memory , utf16_data.str() , utf16_data.Size() );
							GlobalUnlock( hglb ); 
							SetClipboardData( CF_UNICODETEXT , memory );
						}
						else
						{
							rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
						}
					}
				}
				else
				{
					rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
				}
				CloseClipboard();
			}
			else
			{
				rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
			}	
		#else
			rux_write_error( "method does implemented yet" , error );
		#endif
		};
	};
};