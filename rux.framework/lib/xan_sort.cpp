#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_sort.h>
#include <xan_uint32.h>
#include <xan_uint64.h>
#include <xan_int32.h>
#include <xan_int64.h>
#include <xan_float.h>
#include <xan_double.h>
namespace rux
{
	namespace data
	{
		void sort::bubble_sort( rux::XArray< XObject >& elements , const rux::XString& field_name , const char* field_name_delimiter )
		{
			rux::XString cp_field_name;
			cp_field_name.set_ByRef( field_name.ConvertToAnsi() );
			rux::data::sort::bubble_sort( elements , cp_field_name.str() , field_name_delimiter );
		};
		void sort::bubble_sort( rux::XArray< XObject >& elements , const char* field_name , const char* field_name_delimiter )
		{
			uint8 swapped = 0 , found = 0;
			size_t count = elements.Count();
			if( count > 0 )
			{
				for( size_t index0 = 0 ; index0 < count - 1 ; index0++ )
				{
					swapped = 0;
					for( size_t index1 = 0 ; index1 < count - index0 - 1 ; index1++ )
					{
						if( rux_is_object( elements[ index1 ] , XDataObject ) 
							&& rux_is_object( elements[ index1 + 1 ] , XDataObject ) )
						{
							declare_variable_param( rux::data::XDataObject , obj0 , elements[ index1 ] );
							declare_variable_param( rux::data::XDataObject , obj1 , elements[ index1 + 1 ] );
							XObject val0( obj0.GetValue< XObject >( field_name , found , SIZE_MAX , field_name_delimiter ) );
							if( found == 1 )
							{
								XObject val1( obj1.GetValue< XObject >( field_name , found , SIZE_MAX , field_name_delimiter ) );
								if( found == 1 )
								{
									if( rux_is_object( val0 , rux::XString )
										&& rux_is_object( val1 , rux::XString ) )
									{
										rux::XString str0 , str1;
										str0.set_ByRef( val0 );
										str1.set_ByRef( val1 );
										if( str0 > str1 )
										{
											elements.Swap( index1 , index1 + 1 );
											swapped = 1;
										}
									}
									else if( rux_is_object( val0 , rux::XInt64 )
										&& rux_is_object( val1 , rux::XInt64 ) )
									{
										rux::XInt64 str0 , str1;
										str0.set_ByRef( val0 );
										str1.set_ByRef( val1 );
										if( str0() > str1() )
										{
											elements.Swap( index1 , index1 + 1 );
											swapped = 1;
										}
									}
									else if( rux_is_object( val0 , rux::XUInt64 )
										&& rux_is_object( val1 , rux::XUInt64 ) )
									{
										rux::XUInt64 str0 , str1;
										str0.set_ByRef( val0 );
										str1.set_ByRef( val1 );
										if( str0() > str1() )
										{
											elements.Swap( index1 , index1 + 1 );
											swapped = 1;
										}
									}
									else if( rux_is_object( val0 , rux::XInt32 )
										&& rux_is_object( val1 , rux::XInt32 ) )
									{
										rux::XInt32 str0 , str1;
										str0.set_ByRef( val0 );
										str1.set_ByRef( val1 );
										if( str0() > str1() )
										{
											elements.Swap( index1 , index1 + 1 );
											swapped = 1;
										}
									}
									else if( rux_is_object( val0 , rux::XUInt32 )
										&& rux_is_object( val1 , rux::XUInt32 ) )
									{
										rux::XUInt32 str0 , str1;
										str0.set_ByRef( val0 );
										str1.set_ByRef( val1 );
										if( str0() > str1() )
										{
											elements.Swap( index1 , index1 + 1 );
											swapped = 1;
										}
									}
									else if( rux_is_object( val0 , rux::XDouble )
										&& rux_is_object( val1 , rux::XDouble ) )
									{
										rux::XDouble str0 , str1;
										str0.set_ByRef( val0 );
										str1.set_ByRef( val1 );
										if( str0() > str1() )
										{
											elements.Swap( index1 , index1 + 1 );
											swapped = 1;
										}
									}
									else if( rux_is_object( val0 , rux::XFloat )
										&& rux_is_object( val1 , rux::XFloat ) )
									{
										rux::XFloat str0 , str1;
										str0.set_ByRef( val0 );
										str1.set_ByRef( val1 );
										if( str0() > str1() )
										{
											elements.Swap( index1 , index1 + 1 );
											swapped = 1;
										}
									}
								}
							}
						}
					}
					if( swapped == 0 )
						break;
				}
			}
		};
	};
};