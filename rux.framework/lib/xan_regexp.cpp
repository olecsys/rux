#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_regexp.h"
#include <xan_keyvaluepair.h>
#include <xan_error.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( RegexpChar , rux );
	_min_entries = 1;
	_max_entries = 1;
	_is_equal = 1;
	_is_letter = 0;
	_is_any = 0;
end_implement_rux_class();

begin_implement_rux_class_ns( RegexpGroup , rux );
	_min_entries = 1;
	_max_entries = 1;
end_implement_rux_class();

begin_implement_rux_class_ns( RegexpPattern , rux );
end_implement_rux_class();
namespace rux
{	
	::rux::uint8 RegexpChar::IsHardValue( void )
	{
		return _is_equal == 1 && _is_letter == 0 && _is_any == 0 && _utf8_characters.Count() == 1;
	};
	::rux::uint8 XRegexpChar::IsHardValue( void )
	{
		return (*this)()->IsHardValue();
	};
	void XRegexpChar::Correct( ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& entries )
	{
		if( entries >= (*this)()->_min_entries )
		{
			if( index < string.Length() )
				string.RemoveAt( index );
		}
		else
		{
			::rux::uint32 utf8 = 'a' , index0 = 0;
			while( entries < (*this)()->_min_entries )
			{		
				if( index < string.Length() )
					string.RemoveAt( index );
				if( (*this)()->_is_equal == 1 )
				{		
					if( (*this)()->_utf8_characters.Count() > 0 )
					{
						utf8 = (*this)()->_utf8_characters[ 0 ];
						string.InsertRange( index , XString( (char*)&utf8 , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ ) );
					}
					else
						string.Insert( index , 'a' );
				}
				else
				{
					utf8 = 'a';
					if( (*this)()->_is_letter == 1 )
						utf8 = '0';
					index0 = 0;
					for( ; ; )
					{
						for( index0 = 0 ; (*this)()->_utf8_characters.Count() ; index0++ )
						{
							if( (*this)()->_utf8_characters[ index0 ] == utf8 )
								break;
						}
						if( index0 < (*this)()->_utf8_characters.Count() )
							utf8++;
						else
							break;
					}
					string.InsertRange( index , XString( (char*)&utf8 , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ ) );
				}
				entries++;
				index++;
			}
		}
	};
	bool XRegexpChar::Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index , ::rux::uint32& entries )
	{
		return (*this)()->Equals( string , index , is_hard_value , symbol_index , entries );
	};
	bool RegexpChar::Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index , ::rux::uint32& entries )
	{
		bool equals = false;
		::rux::uint32 last_index = index , utf8 = 0 , index0 = 0;		
		::rux::uint8 one_found = 0;
		if( _is_equal == 1 )
		{
			while( entries < _max_entries 
				&& index < string.Length() )
			{
				if( _is_any == 1 )
				{
					entries++;
					one_found = 1;
				}
				else if( _is_letter == 1 && ::rux::utils::string::utf8::isalpha( string.get_UTF8Char( index ) ) )
				{
					entries++;
					one_found = 1;
				}
				else
				{
					::rux::uint8 found = 0;
					utf8 = string.get_UTF8Char( index );
					for( index0 = 0 ; index0 < _utf8_characters.Count() ; index0++ )
					{
						if( _utf8_characters[ index0 ] == utf8 )
						{
							found = 1;
							entries++;
							one_found = 1;
							break;
						}
					}
					if( found == 0 )
						break;
				}				
				index++;
				if( _min_entries <= entries )
					break;
			}
			if( _min_entries <= entries && ( one_found == 1 || index == string.Length() ) )
			{
				if( is_hard_value
					&& last_index <= symbol_index
					&& index > symbol_index )
					*is_hard_value = IsHardValue();
				equals = true;
			}
			else
				index = last_index;
		}
		else
		{
			while( entries < _max_entries 
				&& index < string.Length() )
			{
				utf8 = string.get_UTF8Char( index );
				if( _is_letter == 0 || ::rux::utils::string::utf8::isalpha( utf8 ) == false )
				{
					index0 = 0;
					for( ; index0 < _utf8_characters.Count() ; index0++ )
					{
						if( _utf8_characters[ index0 ] == utf8 )
							break;
					}
					if( index0 >= _utf8_characters.Count() )
					{
						entries++;
						one_found = 1;
					}
					else
						break;
					index++;
				}			
				else
					break;
				if( _min_entries <= entries )
					break;
			}
			if( _min_entries <= entries && ( one_found == 1 || index == string.Length() ) )
			{
				if( is_hard_value
					&& last_index <= symbol_index
					&& index > symbol_index )
					*is_hard_value = IsHardValue();
				equals = true;
			}
			else
				index = last_index;
		}
		return equals;
	};	
	void XRegexpGroup::Correct( ::rux::XString& string , ::rux::uint32& index , ::rux::uint32 most_matches_index , ::rux::uint32& entries )
	{
		if( (*this)()->_objects.Count() > most_matches_index )
		{
			if( rux_is_object( (*this)()->_objects[ most_matches_index ] , XRegexpPattern ) )
			{
				if( entries >= (*this)()->_min_entries )
				{
					if( index < string.Length() )
						string.RemoveAt( index );
				}
				else
				{
					XRegexpPattern pattern( (*this)()->_objects[ most_matches_index ] );
					::rux::uint32 last_index = 0 , matches = 0;
					while( entries < (*this)()->_min_entries )
					{
						last_index = index;
						if( pattern.Equals( string , index , matches , NULL , 0 ) == false )
						{
							index = last_index;
							pattern.Correct( string , index );
						}
						entries++;				
					}
				}
			}
		}
	};
	bool XRegexpGroup::Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& most_matches_index , ::rux::uint32& matches , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index )
	{
		return (*this)()->Equals( string , index , most_matches_index , matches , is_hard_value , symbol_index );
	};
	bool RegexpGroup::Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& most_matches_index , ::rux::uint32& matches , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index )
	{
		most_matches_index = 0;
		bool equals = false;
		::rux::uint32 entries = 0 , last_index = 0 , local_matches = 0 , max_matches = 0;
		for( ::rux::uint32 index0 = 0 ; index0 < _objects.Count() ; index0++ )
		{
			if( rux_is_object( _objects[ index0 ] , XRegexpPattern ) )
			{
				XRegexpPattern pattern( _objects[ index0 ] );
				last_index = index;
				local_matches = 0;
				if( pattern.Equals( string , index , local_matches , _objects.Count() > 1 ? NULL : is_hard_value , symbol_index ) )
				{
					entries++;
					while( entries < _max_entries && pattern.Equals( string , index , local_matches , _objects.Count() > 1 ? NULL : is_hard_value , symbol_index ) )
						entries++;
					if( _min_entries <= entries )
					{
						if( local_matches > max_matches )
						{
							most_matches_index = index0;
							max_matches = local_matches;
						}
						equals = true;
						break;
					}
					else
					{
						entries = 0;
						index = last_index;
					}
				}
				else
					index = last_index;
				if( local_matches > max_matches )
				{
					most_matches_index = index0;
					max_matches = local_matches;
				}
			}
		}	
		if( equals )
			matches = max_matches;
		return equals;
	};
	struct regexp_pattern_match
	{
		::rux::uint32 _entries;
		::rux::uint32 _last_index;
		::rux::uint32 _max_entries;
	};
	void XRegexpPattern::Correct( ::rux::XString& string , ::rux::uint32& index )
	{	
		::rux::XRegexpChar regexp_char;
		::rux::XRegexpGroup regexp_group;
		::rux::uint32 most_matches_index = 0;
		if( (*this)()->_objects.Count() > 0 )
		{
			::rux::uint32 longest_object_index = 0xffffffff;
			regexp_pattern_match* regexp_pattern_matches = alloc_array_macros( regexp_pattern_match , (*this)()->_objects.Count() );
			while( longest_object_index == 0xffffffff
				|| longest_object_index < (*this)()->_objects.Count() )
			{
				::rux::uint32 longest_index = index , longest_most_matches_index = 0 , begin_index = longest_object_index == 0xffffffff ? 0 : longest_object_index;
				bool equals = true;				
				memset( regexp_pattern_matches , 0 , sizeof( regexp_pattern_match ) * (*this)()->_objects.Count() );
				for( ::rux::uint32 index0 = longest_object_index == 0xffffffff ? 0 : longest_object_index ; index0 < (*this)()->_objects.Count() ; index0++ )
					regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;				
				for( ::rux::uint32 index0 = begin_index ; index0 < (*this)()->_objects.Count() ; index0++ )
				{
					if( rux_is_object( (*this)()->_objects[ index0 ] , XRegexpChar ) )
					{
						regexp_char = (*this)()->_objects[ index0 ];
						regexp_pattern_matches[ index0 ]._max_entries = regexp_char()->_max_entries;
						if( regexp_pattern_matches[ index0 ]._entries >= regexp_pattern_matches[ index0 ]._max_entries )
						{
							if( index0 == begin_index )
							{		
								equals = false;
								if( longest_object_index == 0xffffffff || longest_index < index )
								{
									longest_most_matches_index = most_matches_index;
									longest_index = index;
									longest_object_index = index0;
								}
								break;
							}
							else
							{
								if( longest_object_index == 0xffffffff || longest_index < index )
								{
									longest_most_matches_index = most_matches_index;
									longest_index = index;
									longest_object_index = index0;
								}
								if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
									index = regexp_pattern_matches[ index0 ]._last_index;
								regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;
								regexp_pattern_matches[ index0 ]._entries = 0;
								index0 -= 2;
							}
						}
						else
						{
							if( regexp_pattern_matches[ index0 ]._last_index == UINT_MAX )
								regexp_pattern_matches[ index0 ]._last_index = index;
							if( regexp_char.Equals( string , index , NULL , 0 , regexp_pattern_matches[ index0 ]._entries ) == false )
							{
								if( index0 == begin_index )
								{	
									equals = false;
									if( longest_object_index == 0xffffffff || longest_index < index )
									{
										longest_most_matches_index = most_matches_index;
										longest_index = index;
										longest_object_index = index0;
									}
									break;
								}
								else
								{
									if( longest_object_index == 0xffffffff || longest_index < index )
									{
										longest_most_matches_index = most_matches_index;
										longest_index = index;
										longest_object_index = index0;
									}
									if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
										index = regexp_pattern_matches[ index0 ]._last_index;
									regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;
									regexp_pattern_matches[ index0 ]._entries = 0;
									index0 -= 2;
								}
							}
						}
					}
					else if( rux_is_object( (*this)()->_objects[ index0 ] , XRegexpGroup ) )
					{
						regexp_group = (*this)()->_objects[ index0 ];
						regexp_pattern_matches[ index0 ]._max_entries = regexp_group()->_max_entries;
						if( regexp_pattern_matches[ index0 ]._entries >= regexp_pattern_matches[ index0 ]._max_entries )
						{
							if( index0 == 0 )
							{
								equals = false;			
								if( longest_object_index == 0xffffffff || longest_index < index )
								{
									longest_most_matches_index = most_matches_index;
									longest_index = index;
									longest_object_index = index0;
								}
								break;
							}
							else
							{
								if( longest_object_index == 0xffffffff || longest_index < index )
								{
									longest_most_matches_index = most_matches_index;
									longest_index = index;
									longest_object_index = index0;
								}
								if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
									index = regexp_pattern_matches[ index0 ]._last_index;
								regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;
								regexp_pattern_matches[ index0 ]._entries = 0;
								index0 -= 2;
							}
						}
						else
						{
							if( regexp_pattern_matches[ index0 ]._last_index == UINT_MAX )
								regexp_pattern_matches[ index0 ]._last_index = index;
							if( regexp_group.Equals( string , index , most_matches_index , regexp_pattern_matches[ index0 ]._entries , NULL , 0 ) == false )
							{
								if( index0 == begin_index )
								{
									equals = false;
									if( longest_object_index == 0xffffffff || longest_index < index )
									{
										longest_most_matches_index = most_matches_index;
										longest_index = index;
										longest_object_index = index0;
									}
									break;
								}
								else
								{
									if( longest_object_index == 0xffffffff || longest_index < index )
									{
										longest_most_matches_index = most_matches_index;
										longest_index = index;
										longest_object_index = index0;
									}
									if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
										index = regexp_pattern_matches[ index0 ]._last_index;
									regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;
									regexp_pattern_matches[ index0 ]._entries = 0;
									index0 -= 2;
								}
							}
						}
					}
				}				
				if( longest_object_index == 0xffffffff )
					longest_object_index = 0;
				if( equals == false )
				{
					index = longest_index;
					if( rux_is_object( (*this)()->_objects[ longest_object_index ] , XRegexpChar ) )
					{
						regexp_char = (*this)()->_objects[ longest_object_index ];
						regexp_char.Correct( string , index , regexp_pattern_matches[ longest_object_index ]._entries );
					}
					else if( rux_is_object( (*this)()->_objects[ longest_object_index ] , XRegexpGroup ) )
					{
						regexp_group = (*this)()->_objects[ longest_object_index ];
						regexp_group.Correct( string , index , longest_most_matches_index , regexp_pattern_matches[ longest_object_index ]._entries );
					}
					longest_object_index++;
				}
				else
				{
					if( string.Length() > index )
					{
						::rux::uint32 index0 = (*this)()->_objects.Count() - 1;
						if( rux_is_object( (*this)()->_objects[ index0 ] , XRegexpChar ) )
						{
							regexp_char = (*this)()->_objects[ index0 ];
							regexp_pattern_matches[ index0 ]._max_entries = regexp_char()->_max_entries;
							for( ; ; )
							{
								regexp_pattern_matches[ index0 ]._last_index = index;
								if( string.Length() == index )
									break;
								if( regexp_pattern_matches[ index0 ]._entries >= regexp_pattern_matches[ index0 ]._max_entries )
									break;
								else
								{
									if( regexp_pattern_matches[ index0 ]._last_index == UINT_MAX )
										regexp_pattern_matches[ index0 ]._last_index = index;
									if( regexp_char.Equals( string , index , NULL , 0 , regexp_pattern_matches[ index0 ]._entries ) == false )
									{
										if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
											index = regexp_pattern_matches[ index0 ]._last_index;
										break;
									}
								}
							}
						}
						else if( rux_is_object( (*this)()->_objects[ index0 ] , XRegexpGroup ) )
						{
							regexp_group = (*this)()->_objects[ index0 ];
							regexp_pattern_matches[ index0 ]._max_entries = regexp_group()->_max_entries;
							for( ; ; )
							{
								regexp_pattern_matches[ index0 ]._last_index = index;
								if( string.Length() == index )
									break;
								if( regexp_pattern_matches[ index0 ]._entries >= regexp_pattern_matches[ index0 ]._max_entries ) 
									break;
								else
								{
									if( regexp_pattern_matches[ index0 ]._last_index == UINT_MAX )
										regexp_pattern_matches[ index0 ]._last_index = index;
									if( regexp_group.Equals( string , index , most_matches_index , regexp_pattern_matches[ index0 ]._entries , NULL , 0 ) == false )
									{									
										if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
											index = regexp_pattern_matches[ index0 ]._last_index;
										break;
									}
								}
							}
						}
					}
					break;
				}
			}
			::rux::engine::free_mem( regexp_pattern_matches );
		}
	};	
	bool XRegexpPattern::Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& matches , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index )
	{	
		bool equals = true;
		::rux::XRegexpChar regexp_char;
		::rux::XRegexpGroup regexp_group;
		::rux::uint32 last_index = index , most_matches_index = 0;
		if( (*this)()->_objects.Count() > 0 )
		{
			regexp_pattern_match* regexp_pattern_matches = alloc_array_macros( regexp_pattern_match , (*this)()->_objects.Count() );
			memset( regexp_pattern_matches , 0 , sizeof( regexp_pattern_match ) * (*this)()->_objects.Count() );
			for( ::rux::uint32 index0 = 0 ; index0 < (*this)()->_objects.Count() ; index0++ )
				regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;
			for( ::rux::uint32 index0 = 0 ; index0 < (*this)()->_objects.Count() ; index0++ )
			{
				if( rux_is_object( (*this)()->_objects[ index0 ] , XRegexpChar ) )
				{
					regexp_char = (*this)()->_objects[ index0 ];
					regexp_pattern_matches[ index0 ]._max_entries = regexp_char()->_max_entries;
					if( regexp_pattern_matches[ index0 ]._entries >= regexp_pattern_matches[ index0 ]._max_entries )
					{
						if( index0 == 0 )
						{
							equals = false;					
							break;
						}
						else
						{
							if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
								index = regexp_pattern_matches[ index0 ]._last_index;
							regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;
							regexp_pattern_matches[ index0 ]._entries = 0;
							index0 -= 2;
						}
					}
					else
					{
						if( regexp_pattern_matches[ index0 ]._last_index == UINT_MAX )
							regexp_pattern_matches[ index0 ]._last_index = index;
						if( regexp_char.Equals( string , index , is_hard_value , symbol_index , regexp_pattern_matches[ index0 ]._entries ) == false )
						{
							if( index0 == 0 )
							{
								equals = false;					
								break;
							}
							else
							{
								if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
									index = regexp_pattern_matches[ index0 ]._last_index;
								regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;
								regexp_pattern_matches[ index0 ]._entries = 0;
								index0 -= 2;
							}
						}
						else
							matches++;
					}
				}
				else if( rux_is_object( (*this)()->_objects[ index0 ] , XRegexpGroup ) )
				{
					regexp_group = (*this)()->_objects[ index0 ];
					regexp_pattern_matches[ index0 ]._max_entries = regexp_group()->_max_entries;
					if( regexp_pattern_matches[ index0 ]._entries >= regexp_pattern_matches[ index0 ]._max_entries )
					{
						if( index0 == 0 )
						{
							equals = false;					
							break;
						}
						else
						{
							if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
								index = regexp_pattern_matches[ index0 ]._last_index;
							regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;
							regexp_pattern_matches[ index0 ]._entries = 0;
							index0 -= 2;
						}
					}
					else
					{
						if( regexp_pattern_matches[ index0 ]._last_index == UINT_MAX )
							regexp_pattern_matches[ index0 ]._last_index = index;
						if( regexp_group.Equals( string , index , most_matches_index , regexp_pattern_matches[ index0 ]._entries , is_hard_value , symbol_index ) == false )
						{
							if( index0 == 0 )
							{
								equals = false;
								break;
							}
							else
							{
								if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
									index = regexp_pattern_matches[ index0 ]._last_index;
								regexp_pattern_matches[ index0 ]._last_index = UINT_MAX;
								regexp_pattern_matches[ index0 ]._entries = 0;
								index0 -= 2;
							}
						}
					}
				}				
			}			
			if( equals )
			{				
				if( string.Length() > index )
				{
					::rux::uint32 index0 = (*this)()->_objects.Count() - 1;
					if( rux_is_object( (*this)()->_objects[ index0 ] , XRegexpChar ) )
					{
						regexp_char = (*this)()->_objects[ index0 ];
						regexp_pattern_matches[ index0 ]._max_entries = regexp_char()->_max_entries;
						for( ; ; )
						{
							regexp_pattern_matches[ index0 ]._last_index = index;
							if( string.Length() == index )
								break;
							if( regexp_pattern_matches[ index0 ]._entries >= regexp_pattern_matches[ index0 ]._max_entries )
								break;
							else
							{
								if( regexp_pattern_matches[ index0 ]._last_index == UINT_MAX )
									regexp_pattern_matches[ index0 ]._last_index = index;
								if( regexp_char.Equals( string , index , is_hard_value , symbol_index , regexp_pattern_matches[ index0 ]._entries ) == false )
								{
									if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
										index = regexp_pattern_matches[ index0 ]._last_index;
									break;
								}
								else
									matches++;
							}
						}
					}
					else if( rux_is_object( (*this)()->_objects[ index0 ] , XRegexpGroup ) )
					{
						regexp_group = (*this)()->_objects[ index0 ];
						regexp_pattern_matches[ index0 ]._max_entries = regexp_group()->_max_entries;
						for( ; ; )
						{
							regexp_pattern_matches[ index0 ]._last_index = index;
							if( string.Length() == index )
								break;
							if( regexp_pattern_matches[ index0 ]._entries >= regexp_pattern_matches[ index0 ]._max_entries ) 
								break;
							else
							{
								if( regexp_pattern_matches[ index0 ]._last_index == UINT_MAX )
									regexp_pattern_matches[ index0 ]._last_index = index;
								if( regexp_group.Equals( string , index , most_matches_index , regexp_pattern_matches[ index0 ]._entries , is_hard_value , symbol_index ) == false )
								{									
									if( regexp_pattern_matches[ index0 ]._last_index != UINT_MAX )
										index = regexp_pattern_matches[ index0 ]._last_index;
									break;
								}
							}
						}
					}
				}
			}
			if( equals == false )
				index = last_index;
			::rux::engine::free_mem( regexp_pattern_matches );
		}
		return equals;
	};
	RegexpPattern& XRegexp::get_RegexpPattern( const ::rux::XString& regular_expression , ::rux::XString& error )
	{
		::rux::XString utf8_regular_expression;
		utf8_regular_expression.set_ByRef( regular_expression.ConvertToUTF8() );
		::rux::uint32 utf8_index = 0 , utf8 = 0;	
		declare_variable( XObject , last_object );
		::rux::XRegexpPattern result , regexp_pattern;
		result()->_objects.Add( regexp_pattern );
		while( utf8_regular_expression.Length() > utf8_index )
		{
			utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );		
			switch( utf8 )
			{			
			case '^':
				{
					rux_write_error( "not yet implemented" , error );
					break;
				}
			case '.':
				{
					XRegexpChar regexp_char;
					regexp_char()->_is_any = 1;
					regexp_pattern()->_objects.Add( regexp_char );
					last_object = regexp_char;
					break;
				}
			case '$':
				{
					rux_write_error( "not yet implemented" , error );
					break;
				}
			case '|':
				{
					regexp_pattern = XRegexpPattern();
					result()->_objects.Add( regexp_pattern , __FILE__ , __LINE__ );
					last_object = 0;
					break;
				}
			case '(':
				{
					XRegexpGroup regexp_group( get_RegexpGroup( utf8_regular_expression , utf8_index , utf8 , last_object , error ) );
					if( error.Length() == 0 )
					{
						regexp_pattern()->_objects.Add( regexp_group );
						last_object = regexp_group;
					}
					break;
				}
			case '\\':
				{
					utf8_index++;
					if( utf8_regular_expression.Length() > utf8_index )
					{
						utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
						if( utf8 == 'd' )
						{
							XRegexpChar regexp_char;
							regexp_char()->_utf8_characters.Add( '0' );
							regexp_char()->_utf8_characters.Add( '1' );
							regexp_char()->_utf8_characters.Add( '2' );
							regexp_char()->_utf8_characters.Add( '3' );
							regexp_char()->_utf8_characters.Add( '4' );
							regexp_char()->_utf8_characters.Add( '5' );
							regexp_char()->_utf8_characters.Add( '6' );
							regexp_char()->_utf8_characters.Add( '7' );
							regexp_char()->_utf8_characters.Add( '8' );
							regexp_char()->_utf8_characters.Add( '9' );
							regexp_pattern()->_objects.Add( regexp_char );
							last_object = regexp_char;
						}
						else if( utf8 == 'D' )
						{
							XRegexpChar regexp_char;
							regexp_char()->_utf8_characters.Add( '0' );
							regexp_char()->_utf8_characters.Add( '1' );
							regexp_char()->_utf8_characters.Add( '2' );
							regexp_char()->_utf8_characters.Add( '3' );
							regexp_char()->_utf8_characters.Add( '4' );
							regexp_char()->_utf8_characters.Add( '5' );
							regexp_char()->_utf8_characters.Add( '6' );
							regexp_char()->_utf8_characters.Add( '7' );
							regexp_char()->_utf8_characters.Add( '8' );
							regexp_char()->_utf8_characters.Add( '9' );
							regexp_char()->_is_equal = 0;
							regexp_pattern()->_objects.Add( regexp_char );
							last_object = regexp_char;
						}
						else if( utf8 == 's' )
						{
							XRegexpChar regexp_char;
							regexp_char()->_utf8_characters.Add( ' ' );
							regexp_char()->_utf8_characters.Add( '\f' );
							regexp_char()->_utf8_characters.Add( '\n' );
							regexp_char()->_utf8_characters.Add( '\r' );
							regexp_char()->_utf8_characters.Add( '\t' );
							regexp_char()->_utf8_characters.Add( '\v' );							
							regexp_pattern()->_objects.Add( regexp_char );
							last_object = regexp_char;
						}
						else if( utf8 == 'S' )
						{
							XRegexpChar regexp_char;
							regexp_char()->_utf8_characters.Add( ' ' );
							regexp_char()->_utf8_characters.Add( '\f' );
							regexp_char()->_utf8_characters.Add( '\n' );
							regexp_char()->_utf8_characters.Add( '\r' );
							regexp_char()->_utf8_characters.Add( '\t' );
							regexp_char()->_utf8_characters.Add( '\v' );							
							regexp_char()->_is_equal = 0;
							regexp_pattern()->_objects.Add( regexp_char );
							last_object = regexp_char;
						}
						else if( utf8 == 'w' )
						{
							XRegexpChar regexp_char;
							regexp_char()->_utf8_characters.Add( '0' );
							regexp_char()->_utf8_characters.Add( '1' );
							regexp_char()->_utf8_characters.Add( '2' );
							regexp_char()->_utf8_characters.Add( '3' );
							regexp_char()->_utf8_characters.Add( '4' );
							regexp_char()->_utf8_characters.Add( '5' );
							regexp_char()->_utf8_characters.Add( '6' );
							regexp_char()->_utf8_characters.Add( '7' );
							regexp_char()->_utf8_characters.Add( '8' );
							regexp_char()->_utf8_characters.Add( '9' );
							regexp_char()->_utf8_characters.Add( '_' );
							regexp_char()->_is_letter = 1;
							regexp_pattern()->_objects.Add( regexp_char );
							last_object = regexp_char;
						}
						else if( utf8 == 'W' )
						{
							XRegexpChar regexp_char;
							regexp_char()->_utf8_characters.Add( '0' );
							regexp_char()->_utf8_characters.Add( '1' );
							regexp_char()->_utf8_characters.Add( '2' );
							regexp_char()->_utf8_characters.Add( '3' );
							regexp_char()->_utf8_characters.Add( '4' );
							regexp_char()->_utf8_characters.Add( '5' );
							regexp_char()->_utf8_characters.Add( '6' );
							regexp_char()->_utf8_characters.Add( '7' );
							regexp_char()->_utf8_characters.Add( '8' );
							regexp_char()->_utf8_characters.Add( '9' );
							regexp_char()->_utf8_characters.Add( '_' );
							regexp_char()->_is_letter = 1;
							regexp_char()->_is_equal = 0;
							regexp_pattern()->_objects.Add( regexp_char );
							last_object = regexp_char;
						}
						else if( utf8 == 'b' )
						{
							rux_write_error( "not yet implemented" , error );
						}
						else if( utf8 == 'B' )
						{
							rux_write_error( "not yet implemented" , error );
						}
						else
						{
							XRegexpChar regexp_char;
							regexp_char()->_utf8_characters.Add( utf8 );
							regexp_pattern()->_objects.Add( regexp_char );
							last_object = regexp_char;
						}
					}
					else
					{
						rux_write_error( "incorrect regular expression" , error );
					}
					break;
				}
			case '?':
				{
					if( rux_is_object( last_object , XRegexpGroup ) )
					{
						XRegexpGroup regexp_group( last_object );
						regexp_group()->_min_entries = 0;
						regexp_group()->_max_entries = 1;
					}
					else if( rux_is_object( last_object , XRegexpChar ) )
					{
						XRegexpChar regexp_char( last_object );
						regexp_char()->_min_entries = 0;
						regexp_char()->_max_entries = 1;
					}
					else
					{
						rux_write_error( "incorrect regular expression" , error );
					}
					break;
				}
			case '*':
				{
					if( rux_is_object( last_object , XRegexpGroup ) )
					{
						XRegexpGroup regexp_group( last_object );
						regexp_group()->_min_entries = 0;
						regexp_group()->_max_entries = UINT_MAX;
					}
					else if( rux_is_object( last_object , XRegexpChar ) )
					{
						XRegexpChar regexp_char( last_object );
						regexp_char()->_min_entries = 0;
						regexp_char()->_max_entries = UINT_MAX;
					}
					else
					{
						rux_write_error( "incorrect regular expression" , error );
					}
					break;
				}
			case '+':
				{
					if( rux_is_object( last_object , XRegexpGroup ) )
					{
						XRegexpGroup regexp_group( last_object );
						regexp_group()->_min_entries = 1;
						regexp_group()->_max_entries = UINT_MAX;
					}
					else if( rux_is_object( last_object , XRegexpChar ) )
					{
						XRegexpChar regexp_char( last_object );
						regexp_char()->_min_entries = 1;
						regexp_char()->_max_entries = UINT_MAX;
					}
					else
					{
						rux_write_error( "incorrect regular expression" , error );
					}
					break;
				}
			case '[':
				{				
					XRegexpChar regexp_char;
					regexp_pattern()->_objects.Add( regexp_char );
					if( utf8_index + 1 < utf8_regular_expression.Length() )
					{
						utf8 = utf8_regular_expression.get_UTF8Char( utf8_index + 1 );
						if( utf8 == '^' )
						{
							utf8_index++;
							regexp_char()->_is_equal = 0;
						}
					}
					utf8_index++;
					while( utf8_index < utf8_regular_expression.Length() )
					{
						utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
						if( utf8 == '\\' )
						{
							utf8_index++;
							if( utf8_regular_expression.Length() > utf8_index )
								utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
							else
							{
								rux_write_error( "incorrect regular expression" , error );
								break;
							}
						}
						else if( utf8 == '-' )
						{
							if( regexp_char()->_utf8_characters.Count() > 0 )
							{
								utf8_index++;
								if( utf8_regular_expression.Length() > utf8_index )
								{
									utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
									if( utf8 == '\\' )
									{
										utf8_index++;
										if( utf8_regular_expression.Length() > utf8_index )
										{
											utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
											if( utf8 >= regexp_char()->_utf8_characters[ regexp_char()->_utf8_characters.Count() - 1 ] )
											{
												for( ::rux::uint32 index0 = regexp_char()->_utf8_characters[ regexp_char()->_utf8_characters.Count() - 1 ] + 1 
													; index0 < utf8 ; index0++ )
													regexp_char()->_utf8_characters.Add( index0 );	
											}
											else
											{
												utf8 = '-';
												utf8_index -= 2;
											}
										}
										else
										{
											utf8 = '-';
											utf8_index -= 2;
										}
									}
									else
									{
										if( utf8 != ']' && utf8 >= regexp_char()->_utf8_characters[ regexp_char()->_utf8_characters.Count() - 1 ] )
										{
											for( ::rux::uint32 index0 = regexp_char()->_utf8_characters[ regexp_char()->_utf8_characters.Count() - 1 ] + 1 
												; index0 < utf8 ; index0++ )
												regexp_char()->_utf8_characters.Add( index0 );	
										}
										else
										{
											utf8 = '-';
											utf8_index--;
										}
									}
								}
								else
									utf8_index--;
							}
						}
						else if( utf8 == ']' )
							break;
						regexp_char()->_utf8_characters.Add( utf8 );						
						utf8_index++;
					}
					last_object = regexp_char;
					if( error.Length() == 0 
						&& utf8 != ']' )
					{
						rux_write_error( "incorrect regular expression" , error );
					}
					break;
				}
			case '{':
				{
					::rux::uint32 last_utf8_index = utf8_index;
					utf8_index++;
					if( utf8_index < utf8_regular_expression.Length() )
					{
						utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
						if( utf8 == ',' )
						{
							utf8_index++;
							if( utf8_index < utf8_regular_expression.Length() )
							{
								utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
								::rux::int32 number = -1;
								if( utf8 == '0' )
									number = 0;
								else if( utf8 == '1' )
									number = 1;
								else if( utf8 == '2' )
									number = 2;
								else if( utf8 == '3' )
									number = 3;
								else if( utf8 == '4' )
									number = 4;
								else if( utf8 == '5' )
									number = 5;
								else if( utf8 == '6' )
									number = 6;
								else if( utf8 == '7' )
									number = 7;
								else if( utf8 == '8' )
									number = 8;
								else if( utf8 == '9' )
									number = 9;
								else
								{
									utf8_index = last_utf8_index;
									XRegexpChar regexp_char;
									regexp_char()->_utf8_characters.Add( '{' );
									regexp_pattern()->_objects.Add( regexp_char );
									last_object = regexp_char;
									break;
								}								
								utf8_index++;
								if( utf8_index < utf8_regular_expression.Length() )
								{
									utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
									if( utf8 == '}' )
									{
										if( rux_is_object( last_object , XRegexpGroup ) )
										{
											XRegexpGroup regexp_group( last_object );
											regexp_group()->_min_entries = 0;
											regexp_group()->_max_entries = number;
										}
										else if( rux_is_object( last_object , XRegexpChar ) )
										{
											XRegexpChar regexp_char( last_object );
											regexp_char()->_min_entries = 0;
											regexp_char()->_max_entries = number;
										}
										else
										{
											rux_write_error( "incorrect regular expression" , error );
										}
									}
									else
									{
										utf8_index = last_utf8_index;
										XRegexpChar regexp_char;
										regexp_char()->_utf8_characters.Add( '{' );
										regexp_pattern()->_objects.Add( regexp_char );
										last_object = regexp_char;
										break;
									}
								}
								else
								{
									rux_write_error( "incorrect regular expression" , error );
								}
							}
							else
							{
								rux_write_error( "incorrect regular expression" , error );
							}
						}
						else 
						{
							::rux::int32 number = -1;
							if( utf8 == '0' )
								number = 0;
							else if( utf8 == '1' )
								number = 1;
							else if( utf8 == '2' )
								number = 2;
							else if( utf8 == '3' )
								number = 3;
							else if( utf8 == '4' )
								number = 4;
							else if( utf8 == '5' )
								number = 5;
							else if( utf8 == '6' )
								number = 6;
							else if( utf8 == '7' )
								number = 7;
							else if( utf8 == '8' )
								number = 8;
							else if( utf8 == '9' )
								number = 9;
							else
							{
								utf8_index = last_utf8_index;
								XRegexpChar regexp_char;
								regexp_char()->_utf8_characters.Add( '{' );
								regexp_pattern()->_objects.Add( regexp_char );
								last_object = regexp_char;
								break;
							}							
							utf8_index++;
							if( utf8_index < utf8_regular_expression.Length() )
							{
								utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
								if( utf8 == '}' )
								{
									if( rux_is_object( last_object , XRegexpGroup ) )
									{
										XRegexpGroup regexp_group( last_object );
										regexp_group()->_min_entries = number;
										regexp_group()->_max_entries = number;
									}
									else if( rux_is_object( last_object , XRegexpChar ) )
									{
										XRegexpChar regexp_char( last_object );
										regexp_char()->_min_entries = number;
										regexp_char()->_max_entries = number;
									}
									else
									{
										rux_write_error( "incorrect regular expression" , error );
									}
								}
								else if( utf8 == ',' )
								{
									utf8_index++;
									if( utf8_index < utf8_regular_expression.Length() )
									{
										utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
										if( utf8 == '}' )
										{
											if( rux_is_object( last_object , XRegexpGroup ) )
											{
												XRegexpGroup regexp_group( last_object );
												regexp_group()->_min_entries = number;
												regexp_group()->_max_entries = UINT_MAX;
											}
											else if( rux_is_object( last_object , XRegexpChar ) )
											{
												XRegexpChar regexp_char( last_object );
												regexp_char()->_min_entries = number;
												regexp_char()->_max_entries = UINT_MAX;
											}
											else
											{
												rux_write_error( "incorrect regular expression" , error );
											}
										}
										else
										{
											::rux::int32 number1 = -1;
											if( utf8 == '0' )
												number1 = 0;
											else if( utf8 == '1' )
												number1 = 1;
											else if( utf8 == '2' )
												number1 = 2;
											else if( utf8 == '3' )
												number1 = 3;
											else if( utf8 == '4' )
												number1 = 4;
											else if( utf8 == '5' )
												number1 = 5;
											else if( utf8 == '6' )
												number1 = 6;
											else if( utf8 == '7' )
												number1 = 7;
											else if( utf8 == '8' )
												number1 = 8;
											else if( utf8 == '9' )
												number1 = 9;
											else
											{
												utf8_index = last_utf8_index;
												XRegexpChar regexp_char;
												regexp_char()->_utf8_characters.Add( '{' );
												regexp_pattern()->_objects.Add( regexp_char );
												last_object = regexp_char;
												break;
											}											
											utf8_index++;
											if( utf8_index < utf8_regular_expression.Length() )
											{
												utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
												if( utf8 == '}' )
												{
													if( rux_is_object( last_object , XRegexpGroup ) )
													{
														XRegexpGroup regexp_group( last_object );
														regexp_group()->_min_entries = number;
														regexp_group()->_max_entries = number1;
													}
													else if( rux_is_object( last_object , XRegexpChar ) )
													{
														XRegexpChar regexp_char( last_object );
														regexp_char()->_min_entries = number;
														regexp_char()->_max_entries = number1;
													}
													else
													{
														rux_write_error( "incorrect regular expression" , error );
													}
												}
												else
												{
													utf8_index = last_utf8_index;
													XRegexpChar regexp_char;
													regexp_char()->_utf8_characters.Add( '{' );
													regexp_pattern()->_objects.Add( regexp_char );
													last_object = regexp_char;
													break;
												}
											}
											else
											{
												utf8_index = last_utf8_index;
												XRegexpChar regexp_char;
												regexp_char()->_utf8_characters.Add( '{' );
												regexp_pattern()->_objects.Add( regexp_char );
												last_object = regexp_char;
												break;
											}											
										}
									}
									else
									{
										utf8_index = last_utf8_index;
										XRegexpChar regexp_char;
										regexp_char()->_utf8_characters.Add( '{' );
										regexp_pattern()->_objects.Add( regexp_char );
										last_object = regexp_char;
										break;
									}
								}
								else
								{
									utf8_index = last_utf8_index;
									XRegexpChar regexp_char;
									regexp_char()->_utf8_characters.Add( '{' );
									regexp_pattern()->_objects.Add( regexp_char );
									last_object = regexp_char;
									break;
								}
							}
							else
							{
								utf8_index = last_utf8_index;
								XRegexpChar regexp_char;
								regexp_char()->_utf8_characters.Add( '{' );
								regexp_pattern()->_objects.Add( regexp_char );
								last_object = regexp_char;
								break;
							}
						}
					}
					else
					{
						utf8_index = last_utf8_index;
						XRegexpChar regexp_char;
						regexp_char()->_utf8_characters.Add( '{' );
						regexp_pattern()->_objects.Add( regexp_char );
						last_object = regexp_char;
						break;
					}
					break;
				}
			default:
				{
					XRegexpChar regexp_char;
					regexp_char()->_utf8_characters.Add( utf8 );
					regexp_pattern()->_objects.Add( regexp_char );
					last_object = regexp_char;
					break;
				}
			}
			if( error.Length() > 0 )
				break;
			utf8_index++;
		}
		return result++;
		//special symbols [ ] \ / ^ $ . | ? * + ( ) { }
	};
	RegexpGroup& XRegexp::get_RegexpGroup( const ::rux::XString& utf8_regular_expression , ::rux::uint32& utf8_index , ::rux::uint32& utf8 , XObject& last_object , ::rux::XString& error )
	{
		last_object = 0;
		XRegexpGroup regexp_group;
		if( utf8 == '(' )
		{
			utf8_index++;		
			XRegexpPattern regexp_pattern;
			regexp_group()->_objects.Add( regexp_pattern );
			while( utf8_regular_expression.Length() > utf8_index )
			{
				utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
				if( utf8 == ')' )
					break;
				switch( utf8 )
				{			
				case '^':
					{
						rux_write_error( "not yet implemented" , error );
						break;
					}
				case '.':
					{
						XRegexpChar regexp_char;
						regexp_char()->_is_any = 1;
						regexp_pattern()->_objects.Add( regexp_char );
						last_object = regexp_char;
						break;
					}
				case '$':
					{
						rux_write_error( "not yet implemented" , error );
						break;
					}
				case '|':
					{
						regexp_pattern = XRegexpPattern();
						regexp_group()->_objects.Add( regexp_pattern , __FILE__ , __LINE__ );
						last_object = 0;
						break;
					}
				case '(':
					{
						XRegexpGroup regexp_group( get_RegexpGroup( utf8_regular_expression , utf8_index , utf8 , last_object , error ) );
						if( error.Length() == 0 )
						{
							regexp_pattern()->_objects.Add( regexp_group );
							last_object = regexp_group;
						}
						break;
					}
				case '\\':
					{
						utf8_index++;
						if( utf8_regular_expression.Length() > utf8_index )
						{
							utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
							if( utf8 == 'd' )
							{
								XRegexpChar regexp_char;
								regexp_char()->_utf8_characters.Add( '0' );
								regexp_char()->_utf8_characters.Add( '1' );
								regexp_char()->_utf8_characters.Add( '2' );
								regexp_char()->_utf8_characters.Add( '3' );
								regexp_char()->_utf8_characters.Add( '4' );
								regexp_char()->_utf8_characters.Add( '5' );
								regexp_char()->_utf8_characters.Add( '6' );
								regexp_char()->_utf8_characters.Add( '7' );
								regexp_char()->_utf8_characters.Add( '8' );
								regexp_char()->_utf8_characters.Add( '9' );
								regexp_pattern()->_objects.Add( regexp_char );
								last_object = regexp_char;
							}
							else if( utf8 == 'D' )
							{
								XRegexpChar regexp_char;
								regexp_char()->_utf8_characters.Add( '0' );
								regexp_char()->_utf8_characters.Add( '1' );
								regexp_char()->_utf8_characters.Add( '2' );
								regexp_char()->_utf8_characters.Add( '3' );
								regexp_char()->_utf8_characters.Add( '4' );
								regexp_char()->_utf8_characters.Add( '5' );
								regexp_char()->_utf8_characters.Add( '6' );
								regexp_char()->_utf8_characters.Add( '7' );
								regexp_char()->_utf8_characters.Add( '8' );
								regexp_char()->_utf8_characters.Add( '9' );
								regexp_char()->_is_equal = 0;
								regexp_pattern()->_objects.Add( regexp_char );
								last_object = regexp_char;
							}
							else if( utf8 == 's' )
							{
								XRegexpChar regexp_char;
								regexp_char()->_utf8_characters.Add( ' ' );
								regexp_char()->_utf8_characters.Add( '\f' );
								regexp_char()->_utf8_characters.Add( '\n' );
								regexp_char()->_utf8_characters.Add( '\r' );
								regexp_char()->_utf8_characters.Add( '\t' );
								regexp_char()->_utf8_characters.Add( '\v' );							
								regexp_pattern()->_objects.Add( regexp_char );
								last_object = regexp_char;
							}
							else if( utf8 == 'S' )
							{
								XRegexpChar regexp_char;
								regexp_char()->_utf8_characters.Add( ' ' );
								regexp_char()->_utf8_characters.Add( '\f' );
								regexp_char()->_utf8_characters.Add( '\n' );
								regexp_char()->_utf8_characters.Add( '\r' );
								regexp_char()->_utf8_characters.Add( '\t' );
								regexp_char()->_utf8_characters.Add( '\v' );							
								regexp_char()->_is_equal = 0;
								regexp_pattern()->_objects.Add( regexp_char );
								last_object = regexp_char;
							}
							else if( utf8 == 'w' )
							{
								XRegexpChar regexp_char;
								regexp_char()->_utf8_characters.Add( '0' );
								regexp_char()->_utf8_characters.Add( '1' );
								regexp_char()->_utf8_characters.Add( '2' );
								regexp_char()->_utf8_characters.Add( '3' );
								regexp_char()->_utf8_characters.Add( '4' );
								regexp_char()->_utf8_characters.Add( '5' );
								regexp_char()->_utf8_characters.Add( '6' );
								regexp_char()->_utf8_characters.Add( '7' );
								regexp_char()->_utf8_characters.Add( '8' );
								regexp_char()->_utf8_characters.Add( '9' );
								regexp_char()->_utf8_characters.Add( '_' );
								regexp_char()->_is_letter = 1;
								regexp_pattern()->_objects.Add( regexp_char );
								last_object = regexp_char;
							}
							else if( utf8 == 'W' )
							{
								XRegexpChar regexp_char;
								regexp_char()->_utf8_characters.Add( '0' );
								regexp_char()->_utf8_characters.Add( '1' );
								regexp_char()->_utf8_characters.Add( '2' );
								regexp_char()->_utf8_characters.Add( '3' );
								regexp_char()->_utf8_characters.Add( '4' );
								regexp_char()->_utf8_characters.Add( '5' );
								regexp_char()->_utf8_characters.Add( '6' );
								regexp_char()->_utf8_characters.Add( '7' );
								regexp_char()->_utf8_characters.Add( '8' );
								regexp_char()->_utf8_characters.Add( '9' );
								regexp_char()->_utf8_characters.Add( '_' );
								regexp_char()->_is_letter = 1;
								regexp_char()->_is_equal = 0;
								regexp_pattern()->_objects.Add( regexp_char );
								last_object = regexp_char;
							}
							else if( utf8 == 'b' )
							{
								rux_write_error( "not yet implemented" , error );
							}
							else if( utf8 == 'B' )
							{
								rux_write_error( "not yet implemented" , error );
							}
							else
							{
								XRegexpChar regexp_char;
								regexp_char()->_utf8_characters.Add( utf8 );
								regexp_pattern()->_objects.Add( regexp_char );
								last_object = regexp_char;
							}
						}
						else
						{
							rux_write_error( "incorrect regular expression" , error );
						}
						break;
					}
				case '?':
					{
						if( rux_is_object( last_object , XRegexpGroup ) )
						{
							XRegexpGroup regexp_group( last_object );
							regexp_group()->_min_entries = 0;
							regexp_group()->_max_entries = 1;
						}
						else if( rux_is_object( last_object , XRegexpChar ) )
						{
							XRegexpChar regexp_char( last_object );
							regexp_char()->_min_entries = 0;
							regexp_char()->_max_entries = 1;
						}
						else
						{
							rux_write_error( "incorrect regular expression" , error );
						}
					}
				case '*':
					{
						if( rux_is_object( last_object , XRegexpGroup ) )
						{
							XRegexpGroup regexp_group( last_object );
							regexp_group()->_min_entries = 0;
							regexp_group()->_max_entries = UINT_MAX;
						}
						else if( rux_is_object( last_object , XRegexpChar ) )
						{
							XRegexpChar regexp_char( last_object );
							regexp_char()->_min_entries = 0;
							regexp_char()->_max_entries = UINT_MAX;
						}
						else
						{
							rux_write_error( "incorrect regular expression" , error );
						}
					}
				case '+':
					{
						if( rux_is_object( last_object , XRegexpGroup ) )
						{
							XRegexpGroup regexp_group( last_object );
							regexp_group()->_min_entries = 1;
							regexp_group()->_max_entries = UINT_MAX;
						}
						else if( rux_is_object( last_object , XRegexpChar ) )
						{
							XRegexpChar regexp_char( last_object );
							regexp_char()->_min_entries = 1;
							regexp_char()->_max_entries = UINT_MAX;
						}
						else
						{
							rux_write_error( "incorrect regular expression" , error );
						}
					}
				case '[':
					{				
						XRegexpChar regexp_char;
						regexp_pattern()->_objects.Add( regexp_char );
						if( utf8_index + 1 < utf8_regular_expression.Length() )
						{
							utf8 = utf8_regular_expression.get_UTF8Char( utf8_index + 1 );
							if( utf8 == '^' )
							{
								utf8_index++;
								regexp_char()->_is_equal = 0;
							}
						}
						utf8_index++;					
						while( utf8_index < utf8_regular_expression.Length() )
						{
							utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
							if( utf8 == '\\' )
							{
								utf8_index++;
								if( utf8_regular_expression.Length() > utf8_index )
									utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
								else
								{
									rux_write_error( "incorrect regular expression" , error );
									break;
								}
							}
							else if( utf8 == '-' )
							{
								if( regexp_char()->_utf8_characters.Count() > 0 )
								{
									utf8_index++;
									if( utf8_regular_expression.Length() > utf8_index )
									{
										utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
										if( utf8 == '\\' )
										{
											utf8_index++;
											if( utf8_regular_expression.Length() > utf8_index )
											{
												utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
												if( utf8 >= regexp_char()->_utf8_characters[ regexp_char()->_utf8_characters.Count() - 1 ] )
												{
													for( ::rux::uint32 index0 = regexp_char()->_utf8_characters[ regexp_char()->_utf8_characters.Count() - 1 ] + 1 
														; index0 < utf8 ; index0++ )
														regexp_char()->_utf8_characters.Add( index0 );	
												}
												else
												{
													utf8 = '-';
													utf8_index -= 2;
												}
											}
											else
											{
												utf8 = '-';
												utf8_index -= 2;
											}
										}
										else
										{
											if( utf8 != ']' && utf8 >= regexp_char()->_utf8_characters[ regexp_char()->_utf8_characters.Count() - 1 ] )
											{
												for( ::rux::uint32 index0 = regexp_char()->_utf8_characters[ regexp_char()->_utf8_characters.Count() - 1 ] + 1 
													; index0 < utf8 ; index0++ )
													regexp_char()->_utf8_characters.Add( index0 );	
											}
											else
											{
												utf8 = '-';
												utf8_index--;
											}
										}
									}
									else
										utf8_index--;
								}
							}
							else if( utf8 == ']' )
								break;
							regexp_char()->_utf8_characters.Add( utf8 );						
							utf8_index++;
						}
						last_object = regexp_char;
						if( error.Length() == 0 
							&& utf8 != ']' )
						{
							rux_write_error( "incorrect regular expression" , error );
						}
						break;
					}
				case '{':
					{
						::rux::uint32 last_utf8_index = utf8_index;
						utf8_index++;
						if( utf8_index < utf8_regular_expression.Length() )
						{
							utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
							if( utf8 == ',' )
							{
								utf8_index++;
								if( utf8_index < utf8_regular_expression.Length() )
								{
									utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
									::rux::int32 number = -1;
									if( utf8 == '0' )
										number = 0;
									else if( utf8 == '1' )
										number = 1;
									else if( utf8 == '2' )
										number = 2;
									else if( utf8 == '3' )
										number = 3;
									else if( utf8 == '4' )
										number = 4;
									else if( utf8 == '5' )
										number = 5;
									else if( utf8 == '6' )
										number = 6;
									else if( utf8 == '7' )
										number = 7;
									else if( utf8 == '8' )
										number = 8;
									else if( utf8 == '9' )
										number = 9;
									else
									{
										utf8_index = last_utf8_index;
										XRegexpChar regexp_char;
										regexp_char()->_utf8_characters.Add( '{' );
										regexp_pattern()->_objects.Add( regexp_char );
										last_object = regexp_char;
										break;
									}								
									utf8_index++;
									if( utf8_index < utf8_regular_expression.Length() )
									{
										utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
										if( utf8 == '}' )
										{
											if( rux_is_object( last_object , XRegexpGroup ) )
											{
												XRegexpGroup regexp_group( last_object );
												regexp_group()->_min_entries = 0;
												regexp_group()->_max_entries = number;
											}
											else if( rux_is_object( last_object , XRegexpChar ) )
											{
												XRegexpChar regexp_char( last_object );
												regexp_char()->_min_entries = 0;
												regexp_char()->_max_entries = number;
											}
											else
											{
												rux_write_error( "incorrect regular expression" , error );
											}
										}
										else
										{
											utf8_index = last_utf8_index;
											XRegexpChar regexp_char;
											regexp_char()->_utf8_characters.Add( '{' );
											regexp_pattern()->_objects.Add( regexp_char );
											last_object = regexp_char;
											break;
										}
									}
									else
									{
										rux_write_error( "incorrect regular expression" , error );
									}
								}
								else
								{
									rux_write_error( "incorrect regular expression" , error );
								}
							}
							else 
							{
								::rux::int32 number = -1;
								if( utf8 == '0' )
									number = 0;
								else if( utf8 == '1' )
									number = 1;
								else if( utf8 == '2' )
									number = 2;
								else if( utf8 == '3' )
									number = 3;
								else if( utf8 == '4' )
									number = 4;
								else if( utf8 == '5' )
									number = 5;
								else if( utf8 == '6' )
									number = 6;
								else if( utf8 == '7' )
									number = 7;
								else if( utf8 == '8' )
									number = 8;
								else if( utf8 == '9' )
									number = 9;
								else
								{
									utf8_index = last_utf8_index;
									XRegexpChar regexp_char;
									regexp_char()->_utf8_characters.Add( '{' );
									regexp_pattern()->_objects.Add( regexp_char );
									last_object = regexp_char;
									break;
								}							
								utf8_index++;
								if( utf8_index < utf8_regular_expression.Length() )
								{
									utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
									if( utf8 == '}' )
									{
										if( rux_is_object( last_object , XRegexpGroup ) )
										{
											XRegexpGroup regexp_group( last_object );
											regexp_group()->_min_entries = number;
											regexp_group()->_max_entries = number;
										}
										else if( rux_is_object( last_object , XRegexpChar ) )
										{
											XRegexpChar regexp_char( last_object );
											regexp_char()->_min_entries = number;
											regexp_char()->_max_entries = number;
										}
										else
										{
											rux_write_error( "incorrect regular expression" , error );
										}
									}
									else if( utf8 == ',' )
									{
										utf8_index++;
										if( utf8_index < utf8_regular_expression.Length() )
										{
											utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
											if( utf8 == '}' )
											{
												if( rux_is_object( last_object , XRegexpGroup ) )
												{
													XRegexpGroup regexp_group( last_object );
													regexp_group()->_min_entries = number;
													regexp_group()->_max_entries = UINT_MAX;
												}
												else if( rux_is_object( last_object , XRegexpChar ) )
												{
													XRegexpChar regexp_char( last_object );
													regexp_char()->_min_entries = number;
													regexp_char()->_max_entries = UINT_MAX;
												}
												else
												{
													rux_write_error( "incorrect regular expression" , error );
												}
											}
											else
											{
												::rux::int32 number1 = -1;
												if( utf8 == '0' )
													number1 = 0;
												else if( utf8 == '1' )
													number1 = 1;
												else if( utf8 == '2' )
													number1 = 2;
												else if( utf8 == '3' )
													number1 = 3;
												else if( utf8 == '4' )
													number1 = 4;
												else if( utf8 == '5' )
													number1 = 5;
												else if( utf8 == '6' )
													number1 = 6;
												else if( utf8 == '7' )
													number1 = 7;
												else if( utf8 == '8' )
													number1 = 8;
												else if( utf8 == '9' )
													number1 = 9;
												else
												{
													utf8_index = last_utf8_index;
													XRegexpChar regexp_char;
													regexp_char()->_utf8_characters.Add( '{' );
													regexp_pattern()->_objects.Add( regexp_char );
													last_object = regexp_char;
													break;
												}											
												utf8_index++;
												if( utf8_index < utf8_regular_expression.Length() )
												{
													utf8 = utf8_regular_expression.get_UTF8Char( utf8_index );
													if( utf8 == '}' )
													{
														if( rux_is_object( last_object , XRegexpGroup ) )
														{
															XRegexpGroup regexp_group( last_object );
															regexp_group()->_min_entries = number;
															regexp_group()->_max_entries = number1;
														}
														else if( rux_is_object( last_object , XRegexpChar ) )
														{
															XRegexpChar regexp_char( last_object );
															regexp_char()->_min_entries = number;
															regexp_char()->_max_entries = number1;
														}
														else
														{
															rux_write_error( "incorrect regular expression" , error );
														}
													}
													else
													{
														utf8_index = last_utf8_index;
														XRegexpChar regexp_char;
														regexp_char()->_utf8_characters.Add( '{' );
														regexp_pattern()->_objects.Add( regexp_char );
														last_object = regexp_char;
														break;
													}
												}
												else
												{
													utf8_index = last_utf8_index;
													XRegexpChar regexp_char;
													regexp_char()->_utf8_characters.Add( '{' );
													regexp_pattern()->_objects.Add( regexp_char );
													last_object = regexp_char;
													break;
												}											
											}
										}
										else
										{
											utf8_index = last_utf8_index;
											XRegexpChar regexp_char;
											regexp_char()->_utf8_characters.Add( '{' );
											regexp_pattern()->_objects.Add( regexp_char );
											last_object = regexp_char;
											break;
										}
									}
									else
									{
										utf8_index = last_utf8_index;
										XRegexpChar regexp_char;
										regexp_char()->_utf8_characters.Add( '{' );
										regexp_pattern()->_objects.Add( regexp_char );
										last_object = regexp_char;
										break;
									}
								}
								else
								{
									utf8_index = last_utf8_index;
									XRegexpChar regexp_char;
									regexp_char()->_utf8_characters.Add( '{' );
									regexp_pattern()->_objects.Add( regexp_char );
									last_object = regexp_char;
									break;
								}
							}
						}
						else
						{
							utf8_index = last_utf8_index;
							XRegexpChar regexp_char;
							regexp_char()->_utf8_characters.Add( '{' );
							regexp_pattern()->_objects.Add( regexp_char );
							last_object = regexp_char;
							break;
						}
						break;
					}
				default:
					{
						XRegexpChar regexp_char;
						regexp_char()->_utf8_characters.Add( utf8 );
						regexp_pattern()->_objects.Add( regexp_char );
						last_object = regexp_char;
						break;
					}
				}
				if( error.Length() > 0 )
					break;
				utf8_index++;
			}
			if( error.Length() == 0 && utf8 != ')' )
			{
				rux_write_error( "incorrect regular expression" , error );
			}
		}
		return regexp_group++;
	};
	::rux::uint8 XRegexp::IsHardValue( const ::rux::XString& string , const XRegexpPattern& regex_pattern , size_t symbol_index )
	{
		::rux::uint8 res = 0;
		::rux::uint32 index = 0 , count = 0;
		if( IndexOf( string , index , count , regex_pattern , &res , symbol_index ) == false
			|| index > 0 
			|| count != string.Length() )
			res = 0;
		return res;
	};
	::rux::String& XRegexp::Correct( const ::rux::XString& string , const XRegexpPattern& regex_pattern )
	{
		::rux::XString corrected_string( string ) , error;
		::rux::uint32 index = 0 , count = 0;
		if( IndexOf( corrected_string , index , count , regex_pattern ) == false
			|| index > 0 
			|| count != string.Length() )
		{
			if( regex_pattern()->_objects.Count() > 0 )
			{
				if( rux_is_object( regex_pattern()->_objects[ 0 ] , XRegexpPattern ) )
				{
					XRegexpPattern pattern( regex_pattern()->_objects[ 0 ] );
					index = 0;
					pattern.Correct( corrected_string , index );
					if( index < corrected_string.Length() )
						corrected_string.RemoveRange( index , corrected_string.Length() - index );
				}
			}
		}
		return corrected_string++;
	};
	::rux::Array< ::rux::XString >& XRegexp::FindAll( const ::rux::XString& string , const XRegexpPattern& regex_pattern )
	{
		::rux::XArray< ::rux::XString > finds;
		::rux::uint32 index = 0 , count = 0;
		while( IndexOf( string , index , count , regex_pattern ) )
		{
			::rux::XString find;
			find.set_ByRef( string.Substring( index , count ) );
			index += count;
			finds.Add( find );
		}
		return finds++;
	};
	bool XRegexp::IndexOf( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& count , const XRegexpPattern& regex_pattern , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index )
	{	
		bool equals = false;
		::rux::uint32 last_index = 0 , matches = 0;	
		for( ::rux::uint32 index0 = index ; index0 < string.Length() ; index0++ )
		{
			for( ::rux::uint32 index1 = 0 ; index1 < regex_pattern()->_objects.Count() ; index1++ )
			{
				if( rux_is_object( regex_pattern()->_objects[ index1 ] , XRegexpPattern ) )
				{
					XRegexpPattern pattern( regex_pattern()->_objects[ index1 ] );
					last_index = index0;
					if( pattern.Equals( string , index0 , matches , is_hard_value , symbol_index ) )
					{
						index = last_index;
						count = index0 - index;
						equals = true;
						break;
					}
					index0 = last_index ;
				}
			}
			if( equals )
				break;
		}
		return equals;
	};
	bool XRegexp::LastIndexOf( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& count , const XRegexpPattern& regex_pattern )
	{	
		bool equals = false;
		::rux::uint32 last_index = 0 , matches = 0;
		if( string.Length() > 0 )
		{
			if( index >= string.Length() )
				index = string.Length() - 1;
			for( ::rux::uint32 index0 = index ; index0 >= 0 ; index0-- )
			{
				for( ::rux::uint32 index1 = 0 ; index1 < regex_pattern()->_objects.Count() ; index1++ )
				{
					if( rux_is_object( regex_pattern()->_objects[ index1 ] , XRegexpPattern ) )
					{
						XRegexpPattern pattern( regex_pattern()->_objects[ index1 ] );
						last_index = index0;
						if( pattern.Equals( string , index0 , matches , NULL , 0 ) )
						{
							index = last_index;
							count = index0 - index;
							equals = true;
							break;
						}
						index0 = last_index ;
					}
				}
				if( index0 == 0 || equals == true )
					break;
			}
		}
		return equals;
	};
	String& XRegexp::NumberMask( void )
	{
		return XString( "(-[0-9]+)|([0-9]+)" )++;
	};
	String& XRegexp::IpPortMask( void )
	{
		return XString( "[123456789][0-9]*" )++;
	};
	::rux::String& XRegexp::TimeMask( void )
	{
		return XString( "(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]" )++;
	};
	::rux::String& XRegexp::DateMask( void )
	{
		return XString( "(0[1-9]|[12][0-9]|3[01])\\.(0[1-9]|1[012])\\.(19|20)[0-9][0-9]" )++;
	};
	::rux::String& XRegexp::Ipv4Mask( void )
	{
		return XString( "(192|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])\\.(168|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])" )++;
	};
};