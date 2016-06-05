#ifndef XAN_RANDOMNUMBERGENERATOR_H
#define XAN_RANDOMNUMBERGENERATOR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_array.h>
namespace rux
{	
	begin_declare_rux_class( RandomNumberGenerator );
		::rux::uint8 _feedback[ 128 ];
	begin_declare_rux_class_members( RandomNumberGenerator );
	end_declare_rux_class_members( RandomNumberGenerator );
		void Generate( ::rux::uint8 number[ 128 ] );	
	end_declare_rux_class();
};
#endif
