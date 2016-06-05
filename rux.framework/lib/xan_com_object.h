#ifndef XAN_COM_OBJECT_H
#define XAN_COM_OBJECT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_interlocked.h>
#include <xan_memorymanager_defines.h>
#include <xan_string.h>
#ifdef __WINDOWS__
#include <comdef.h>
namespace rux
{
	namespace com
	{
		dll_internal ::rux::String& RefiidToString( REFIID riid );
		dll_internal ::rux::uint8 RegisterSelfComServer( REFIID riid , const ::rux::XString& LibId , const ::rux::XString& ClassId );
		dll_internal ::rux::uint8 UnregisterComServer( REFIID riid , const ::rux::XString& LibId , const ::rux::XString& ClassId );
				
		template< class com_interface0 >
		class Base : public com_interface0
		{		
			rux_volatile _ref;
		public:
			STDMETHODIMP_( ULONG ) AddRef() 
			{
				XInterlocked::Increment( &_objects_count );
				return XInterlocked::Increment( &_ref ); 
			};
			STDMETHODIMP_(ULONG) Release() 
			{
				rux_interlocked_return val = XInterlocked::Decrement( &_ref );
				if( _ref == 0 )
					::rux::engine::free_object( this );
				XInterlocked::Decrement( &_objects_count );
				return val;
			};
			Base()
			{
			};
			virtual ~Base()
			{
			};
			STDMETHODIMP_( HRESULT ) QueryInterface( REFIID riid , LPVOID* ppv )
			{
				*ppv = NULL;
				if( IsEqualIID( riid , IID_IUnknown ) 
					|| IsEqualIID( riid , __uuidof( com_interface0 ) ) )
				{
					*ppv = (com_interface0*)this;
					AddRef();
					return S_OK;
				}
				return E_NOINTERFACE;
			};			
		};
		template< class com_interface0 , class com_interface1 >
		class Base2 : public com_interface0 , public com_interface1
		{			
			rux_volatile _ref;
		public:
			STDMETHODIMP_( ULONG ) AddRef() 
			{
				XInterlocked::Increment( &_objects_count );
				return XInterlocked::Increment( &_ref ); 
			};
			STDMETHODIMP_(ULONG) Release() 
			{
				rux_interlocked_return val = XInterlocked::Decrement( &_ref );
				if( _ref == 0 )
					::rux::engine::free_object( this );
				XInterlocked::Decrement( &_objects_count );
				return val;
			};
			Base2()
			{			
			};
			virtual ~Base2()
			{
			};
			STDMETHODIMP_( HRESULT ) QueryInterface( REFIID riid , LPVOID* ppv )
			{
				*ppv = NULL;
				if( IsEqualIID( riid , IID_IUnknown ) 
					|| IsEqualIID( riid , __uuidof( com_interface0 ) ) 
					|| IsEqualIID( riid , __uuidof( com_interface1 ) ) )
				{
					*ppv = (com_interface0*)this;
					AddRef();
					return S_OK;
				}
				return E_NOINTERFACE;
			};
		};
		template< class com_interface0 , class com_interface1 , class com_interface2 >
		class Base3 : public com_interface0 , public com_interface1 , public com_interface2
		{		
			rux_volatile _ref;
		public:
			STDMETHODIMP_( ULONG ) AddRef() 
			{
				XInterlocked::Increment( &_objects_count );
				return XInterlocked::Increment( &_ref ); 
			};
			STDMETHODIMP_(ULONG) Release() 
			{
				rux_interlocked_return val = XInterlocked::Decrement( &_ref );
				if( _ref == 0 )
					::rux::engine::free_object( this );
				XInterlocked::Decrement( &_objects_count );
				return val;
			};
			Base3()
			{			
			};
			virtual ~Base3()
			{
			};
			STDMETHODIMP_( HRESULT ) QueryInterface( REFIID riid , LPVOID* ppv )
			{
				*ppv = NULL;
				if( IsEqualIID( riid , IID_IUnknown ) 
					|| IsEqualIID( riid , __uuidof( com_interface0 ) )
					|| IsEqualIID( riid , __uuidof( com_interface1 ) ) 
					|| IsEqualIID( riid , __uuidof( com_interface2 ) ) )
				{
					*ppv = (com_interface0*)this;
					AddRef();
					return S_OK;
				}
				return E_NOINTERFACE;
			};
		};
				
		template< class com_class >
		class Factory :  public ::rux::com::Base< IClassFactory >
		{
		public:
			Factory()
			{
			};
			virtual ~Factory()
			{
			};
			STDMETHODIMP CreateInstance( LPUNKNOWN pUnkOuter , REFIID riid , LPVOID* ppvObj )
			{
				*ppvObj = NULL;
				if( pUnkOuter )
    				return CLASS_E_NOAGGREGATION;
				com_class* obj = ::rux::engine::alloc_object< com_class >();
				if( obj == 0 )
    				return E_OUTOFMEMORY;
				HRESULT hr = obj->QueryInterface( riid , ppvObj );
				if( hr != S_OK )
					::rux::engine::free_object< com_class >( obj );
				return hr;
			};
			STDMETHODIMP LockServer( BOOL )
			{	 
				return S_OK; 
			};// not implemented
		};
#define begin_implement_rux_com_object_class( rux_class ) \
		rux_class::rux_class()\
		{
#define end_implement_rux_com_object_class() \
		}
#define begin_declare_rux_com_object_class( rux_class , com_interface ) class rux_class : public ::rux::com::Base< com_interface >\
		{\
		public:\
			rux_class()
#define begin_declare_rux_com_object_class_2( rux_class , com_interface0 , com_interface1 ) class rux_class : public ::rux::com::Base2< com_interface0 , com_interface1 >\
		{\
		public:\
			rux_class()
#define begin_declare_rux_com_object_class_3( rux_class , com_interface0 , com_interface1 , com_interface2 ) class rux_class : public ::rux::com::Base3< com_interface0 , com_interface1 , com_interface2 >\
		{\
		public:\
			rux_class()
#define end_declare_rux_com_object_class( rux_class ) \
		};\
		class rux_class##Factory : public ::rux::com::Factory< rux_class >\
		{\
		public:\
		}
#define declare_rux_com_object_clsid( byte0 , byte1 , byte2 , byte3 , byte4 , byte5 , byte6 , byte7 , byte8 , byte9 , byte10 , byte11 , byte12 , byte13 , byte14 , byte15 , prog_id_name ) \
		static const GUID CLSID_##prog_id_name = \
		{ 0x##byte0##byte1##byte2##byte3 , 0x##byte4##byte5 , 0x##byte6##byte7 , { 0x##byte8 , 0x##byte9 , 0x##byte10 , 0x##byte11 , 0x##byte12 , 0x##byte13 , 0x##byte14 , 0x##byte15 } }
#define declare_rux_com_object_guid_string( byte0 , byte1 , byte2 , byte3 , byte4 , byte5 , byte6 , byte7 , byte8 , byte9 , byte10 , byte11 , byte12 , byte13 , byte14 , byte15 ) \
		""#byte0#byte1#byte2#byte3"-"#byte4#byte5"-"#byte6#byte7"-"#byte8#byte9"-"#byte10#byte11#byte12#byte13#byte14#byte15""
#define begin_declare_rux_com_interface( guid_string , com_interface ) \
		interface __declspec( uuid( guid_string ) ) I##com_interface : public IUnknown\
		{
#define end_declare_rux_com_interface() \
		}
	};
};
#endif
#endif
