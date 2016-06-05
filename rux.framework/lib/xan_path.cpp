#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_path.h"
namespace rux
{
	namespace io
	{
		void XPath::GetDirectoryName( char* file_name_ptr , rux::alloc_t alloc , char** directory_name_ptr_ptr )
		{	
			if( alloc )
			{
				if( directory_name_ptr_ptr )
				{
					rux::int32 local_index = -1;	
					rux::uint32 local_index3 = 0;
					rux::uint32 local_count1 = file_name_ptr ? XString::LengthA( file_name_ptr ) : 0;
					if( local_count1 > 0 )
					{
						local_index3 = local_count1 - 1;
						for( local_index3 ; local_index3 >= 0 ; local_index3-- )
						{
							if( file_name_ptr[ local_index3 ] == '\\' )
							{
								local_index = local_index3;
								break;
							}
							if( local_index3 == 0 )
								break;
						}
					}
					if( local_index > -1 )
					{
						if( local_index > 0 )
						{
							*directory_name_ptr_ptr = (char*)alloc( local_index + 1 );
							memcpy( *directory_name_ptr_ptr , file_name_ptr , local_index );
							(*directory_name_ptr_ptr)[ local_index ] = '\0';
							return;
						}
						else
						{
							*directory_name_ptr_ptr = (char*)alloc( 1 );					
							(*directory_name_ptr_ptr)[ local_index ] = '\0';
							return;
						}
					}
					else
					{
						if( local_count1 > 0 )
						{
							local_index3 = local_count1 - 1;
							for( local_index3 ; local_index3 >= 0 ; local_index3-- )
							{
								if( file_name_ptr[ local_index3 ] == '/' )
								{
									local_index = local_index3;
									break;
								}
								if( local_index3 == 0 )
									break;
							}
						}
						if( local_index > -1 )
						{
							if( local_index > 0 )
							{
								*directory_name_ptr_ptr = (char*)alloc( local_index + 1 );
								memcpy( *directory_name_ptr_ptr , file_name_ptr , local_index );
								(*directory_name_ptr_ptr)[ local_index ] = '\0';
								return;
							}
							else
							{
								*directory_name_ptr_ptr = (char*)alloc( 1 );					
								(*directory_name_ptr_ptr)[ local_index ] = '\0';
								return;
							}
						}
					}
					if( local_count1 > 0 )
					{
						*directory_name_ptr_ptr = (char*)alloc( local_count1 + 1 );
						memcpy( *directory_name_ptr_ptr , file_name_ptr , local_count1 );
						(*directory_name_ptr_ptr)[ local_count1 ] = '\0';
					}
					else
					{
						*directory_name_ptr_ptr = (char*)alloc( 1 );					
						(*directory_name_ptr_ptr)[ local_index ] = '\0';
					}
				}
			}
		};
		rux::String& XPath::GetDirectoryName( const rux::XString& file_name )
		{	
			rux::XString local_file_name( file_name.ReplaceAll( '\\' , '/' ) );
			rux::int32 index0 = local_file_name.LastIndexOf( '/' );
			if( index0 > -1 )
				local_file_name = local_file_name.Substring( 0 , index0 );
			return local_file_name++;
		};
		rux::String& XPath::GetDirectoryName( const char* path_ptr )
		{
			rux::XString local_file_name( path_ptr );
			local_file_name = local_file_name.ReplaceAll( '\\' , '/' );
			rux::int32 index0 = local_file_name.LastIndexOf( '/' );
			if( index0 > -1 )
				local_file_name = local_file_name.Substring( 0 , index0 );
			return local_file_name++;
		};
		rux::String& XPath::GetFileName( const rux::XString& file_name )
		{	
			rux::XString local_file_name( file_name.ReplaceAll( '\\' , '/' ) );
			rux::int32 index0 = local_file_name.LastIndexOf( '/' );
			if( index0 > -1 && static_cast<rux::uint32>( index0 + 1 ) < local_file_name.Length() )
				local_file_name = local_file_name.Substring( index0 + 1 );	
			return local_file_name++;
		};
		rux::String& XPath::GetFileName( const char* file_name_ptr )
		{
			rux::XString local_file_name( file_name_ptr );
			local_file_name = local_file_name.ReplaceAll( '\\' , '/' );
			rux::int32 index0 = local_file_name.LastIndexOf( '/' );
			if( index0 > -1 && static_cast<rux::uint32>( index0 + 1 ) < local_file_name.Length() )
				local_file_name = local_file_name.Substring( index0 + 1 );	
			return local_file_name++;
		};
		rux::String& XPath::GetFileNameWithoutExtension( const char* file_name_ptr )
		{
			rux::XString local_file_name( file_name_ptr );
			local_file_name = local_file_name.ReplaceAll( '\\' , '/' );
			rux::int32 index0 = local_file_name.LastIndexOf( '/' );
			if( index0 > -1 && static_cast<rux::uint32>( index0 + 1 ) < local_file_name.Length() )	
				local_file_name = local_file_name.Substring( index0 + 1 );
			index0 = local_file_name.LastIndexOf( '.' );
			if( index0 > -1 )
				local_file_name = local_file_name.Substring( 0 , index0 );
			return local_file_name++;
		};
		void XPath::GetFileNameWithoutExtension( const rux::XString& file_name , rux::alloc_t alloc , wchar_t** file_name_without_extension )
		{
			if( file_name_without_extension )
			{
				rux::XString local_file_name;
				local_file_name.set_ByRef( file_name.ConvertToUTF16() );
				rux::int32 local_index = -1;	
				rux::uint32 local_index3 = 0;
				rux::uint32 local_count1 = local_file_name()->_count - 2;
				if( local_count1 > 0 )
				{
					local_index3 = local_count1 - 2;
					for( local_index3 ; local_index3 >= 0 ; local_index3 -= 2 )
					{
						if( *(wchar_t*)&local_file_name.str()[ local_index3 ] == '\\' )
						{
							local_index = local_index3;
							break;
						}
						if( local_index3 == 0 )
							break;
					}
				}
				if( local_index > -1 )
				{
					local_index += 2;
					if( local_index < static_cast<rux::int32>( local_count1 ) )
					{
						rux::int32 local_index2 = -1;
						local_index3 = local_count1 - 2;
						for( local_index3 ; local_index3 >= static_cast<rux::uint32>( local_index ) ; local_index3 -= 2 )
						{
							if( *(wchar_t*)&local_file_name.str()[ local_index3 ] == '.' )
							{
								local_index2 = local_index3;
								break;
							}
							if( local_index3 == 0 )
								break;
						}
						if( local_index2 > -1 )
						{
							if( local_index + 2 != local_index2 )
							{
								*file_name_without_extension = (wchar_t*)alloc( local_index2 - local_index + sizeof( wchar_t ) );
								memcpy( *file_name_without_extension , &local_file_name.str()[ local_index ] , local_index2 - local_index );
								(*file_name_without_extension)[ ( local_index2 - local_index ) / sizeof( wchar_t ) ] = '\0';
								return;
							}
							else
							{
								*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
								(*file_name_without_extension)[ 0 ] = '\0';
								return;
							}
						}
						else
						{
							*file_name_without_extension = (wchar_t*)alloc( local_count1 - static_cast<rux::uint32>( local_index ) + sizeof( wchar_t ) );
							memcpy( *file_name_without_extension , &local_file_name.str()[ local_index ] , local_count1 - static_cast<rux::uint32>( local_index ) );
							(*file_name_without_extension)[ ( local_count1 - static_cast<rux::uint32>( local_index ) ) / sizeof( wchar_t ) ] = '\0';
							return;
						}
					}
					else
					{
						*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
						(*file_name_without_extension)[ 0 ] = '\0';
						return;
					}
				}
				else
				{
					if( local_count1 > 0 )
					{
						local_index3 = local_count1 - 2;
						for( local_index3 ; local_index3 >= 0 ; local_index3 -= 2 )
						{
							if( *(wchar_t*)&local_file_name.str()[ local_index3 ] == '/' )
							{
								local_index = local_index3;
								break;
							}
							if( local_index3 == 0 )
								break;
						}
					}
					if( local_index > -1 )
					{
						local_index += 2;
						if( local_index < static_cast<rux::int32>( local_count1 ) )
						{
							rux::int32 local_index2 = -1;
							local_index3 = local_count1 - 2;
							for( local_index3 ; local_index3 >= static_cast<rux::uint32>( local_index ) ; local_index3 -= 2 )
							{
								if( *(wchar_t*)&local_file_name.str()[ local_index3 ] == '.' )
								{
									local_index2 = local_index3;
									break;
								}
								if( local_index3 == 0 )
									break;
							}
							if( local_index2 > -1 )
							{
								if( local_index + 2 != local_index2 )
								{
									*file_name_without_extension = (wchar_t*)alloc( local_index2 - local_index + sizeof( wchar_t ) );
									memcpy( *file_name_without_extension , &local_file_name.str()[ local_index ] , local_index2 - local_index );
									(*file_name_without_extension)[ ( local_index2 - local_index ) / sizeof( wchar_t ) ] = '\0';
									return;
								}
								else
								{
									*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
									(*file_name_without_extension)[ 0 ] = '\0';
									return;
								}
							}
							else
							{
								*file_name_without_extension = (wchar_t*)alloc( local_count1 - static_cast<rux::uint32>( local_index ) + sizeof( wchar_t ) );
								memcpy( *file_name_without_extension , &local_file_name.str()[ local_index ] , local_count1 - static_cast<rux::uint32>( local_index ) );
								(*file_name_without_extension)[ ( local_count1 - static_cast<rux::uint32>( local_index ) ) / sizeof( wchar_t ) ] = '\0';
								return;
							}
						}
						else
						{
							*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
							(*file_name_without_extension)[ 0 ] = '\0';
							return;
						}
					}
				}
				if( local_count1 > 0 )
				{
					*file_name_without_extension = (wchar_t*)alloc( local_count1 + sizeof( wchar_t ) );
					memcpy( *file_name_without_extension , local_file_name.str() , local_count1 );
					(*file_name_without_extension)[ local_count1 / sizeof( wchar_t ) ] = '\0';			
				}
				else
				{
					*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
					(*file_name_without_extension)[ 0 ] = '\0';
				}
			}
		}
		const char* XPath::get_Alias( void )
		{
			return "path";
		};
		void XPath::GetFileNameWithoutExtension( const wchar_t* file_name_ptr , rux::alloc_t alloc , wchar_t** file_name_without_extension )
		{
			if( file_name_without_extension )
			{
				rux::int32 local_index = -1;	
				rux::uint32 local_index3 = 0;
				rux::uint32 local_count1 = file_name_ptr ? wcslen( file_name_ptr ) : 0;
				if( local_count1 > 0 )
				{
					local_index3 = local_count1 - 1;
					for( local_index3 ; local_index3 >= 0 ; local_index3-- )
					{
						if( file_name_ptr[ local_index3 ] == '\\' )
						{
							local_index = local_index3;
							break;
						}
						if( local_index3 == 0 )
							break;
					}
				}
				if( local_index > -1 )
				{
					local_index++;
					if( local_index < static_cast<rux::int32>( local_count1 ) )
					{
						rux::int32 local_index2 = -1;
						local_index3 = local_count1 - 1;
						for( local_index3 ; local_index3 >= static_cast<rux::uint32>( local_index ) ; local_index3-- )
						{
							if( file_name_ptr[ local_index3 ] == '.' )
							{
								local_index2 = local_index3;
								break;
							}
							if( local_index3 == 0 )
								break;
						}
						if( local_index2 > -1 )
						{
							if( local_index + 1 != local_index2 )
							{
								*file_name_without_extension = (wchar_t*)alloc( ( local_index2 - local_index + 1 ) * sizeof( wchar_t ) );
								memcpy( *file_name_without_extension , &file_name_ptr[ local_index ] , ( local_index2 - local_index ) * sizeof( wchar_t ) );
								(*file_name_without_extension)[ ( local_index2 - local_index ) ] = '\0';
								return;
							}
							else
							{
								*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
								(*file_name_without_extension)[ 0 ] = '\0';
								return;
							}
						}
						else
						{
							*file_name_without_extension = (wchar_t*)alloc( ( local_count1 - static_cast<rux::uint32>( local_index ) + 1 ) * sizeof( wchar_t ) );
							memcpy( *file_name_without_extension , &file_name_ptr[ local_index ] , ( local_count1 - static_cast<rux::uint32>( local_index ) ) * sizeof( wchar_t ) );
							(*file_name_without_extension)[ ( local_count1 - static_cast<rux::uint32>( local_index ) ) ] = '\0';
							return;
						}
					}
					else
					{
						*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
						(*file_name_without_extension)[ 0 ] = '\0';
						return;
					}
				}
				else
				{
					if( local_count1 > 0 )
					{
						local_index3 = local_count1 - 1;
						for( local_index3 ; local_index3 >= 0 ; local_index3-- )
						{
							if( file_name_ptr[ local_index3 ] == '/' )
							{
								local_index = local_index3;
								break;
							}
							if( local_index3 == 0 )
								break;
						}
					}
					if( local_index > -1 )
					{
						local_index++;
						if( local_index < static_cast<rux::int32>( local_count1 ) )
						{
							rux::int32 local_index2 = -1;
							local_index3 = local_count1 - 1;
							for( local_index3 ; local_index3 >= static_cast<rux::uint32>( local_index ) ; local_index3-- )
							{
								if( file_name_ptr[ local_index3 ] == '.' )
								{
									local_index2 = local_index3;
									break;
								}
								if( local_index3 == 0 )
									break;
							}
							if( local_index2 > -1 )
							{
								if( local_index + 1 != local_index2 )
								{
									*file_name_without_extension = (wchar_t*)alloc( ( local_index2 - local_index + 1 ) * sizeof( wchar_t ) );
									memcpy( *file_name_without_extension , &file_name_ptr[ local_index ] , ( local_index2 - local_index ) * sizeof( wchar_t ) );
									(*file_name_without_extension)[ ( local_index2 - local_index ) ] = '\0';
									return;
								}
								else
								{
									*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
									(*file_name_without_extension)[ 0 ] = '\0';
									return;
								}
							}
							else
							{
								*file_name_without_extension = (wchar_t*)alloc( ( local_count1 - static_cast<rux::uint32>( local_index ) + 1 ) * sizeof( wchar_t ) );
								memcpy( *file_name_without_extension , &file_name_ptr[ local_index ] , ( local_count1 - static_cast<rux::uint32>( local_index ) ) * sizeof( wchar_t ) );
								(*file_name_without_extension)[ ( local_count1 - static_cast<rux::uint32>( local_index ) ) ] = '\0';
								return;
							}
						}
						else
						{
							*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
							(*file_name_without_extension)[ 0 ] = '\0';
							return;
						}
					}
				}
				if( local_count1 > 0 )
				{
					*file_name_without_extension = (wchar_t*)alloc( ( local_count1 + 1 ) * sizeof( wchar_t ) );
					memcpy( *file_name_without_extension , file_name_ptr , local_count1 * sizeof( wchar_t ) );
					(*file_name_without_extension)[ local_count1 ] = '\0';			
				}
				else
				{
					*file_name_without_extension = (wchar_t*)alloc( sizeof( wchar_t ) );						
					(*file_name_without_extension)[ 0 ] = '\0';
				}
			}
		};
		rux::String& XPath::GetFileNameWithoutExtension( const rux::XString& file_name )
		{	
			rux::XString local_file_name( file_name.ReplaceAll( '\\' , '/' ) );
			rux::int32 index0 = local_file_name.LastIndexOf( '/' );
			if( index0 > -1 && static_cast<rux::uint32>( index0 + 1 ) < local_file_name.Length() )	
				local_file_name = local_file_name.Substring( index0 + 1 );
			index0 = local_file_name.LastIndexOf( '.' );
			if( index0 > -1 )
				local_file_name = local_file_name.Substring( 0 , index0 );
			return local_file_name++;
		};
		rux::String& XPath::GetFileNameExtension( const rux::XString& file_name )
		{	
			rux::int32 index0 = file_name.LastIndexOf( '.' );
			if( index0 > -1 && static_cast<rux::uint32>( index0 + 1 ) < file_name.Length() )
				return file_name.Substring( index0 + 1 );
			else
				return XString()++;
		};
		rux::uint8 XPath::IsAbsolutePath( const rux::XString& path )
		{
			rux::XString temp_path;
			temp_path.set_ByRef( path.ReplaceAll( '\\' , '/' ) );
			temp_path.set_ByRef( temp_path.ConvertToAnsi() );
			return IsAbsolutePath( temp_path.str() , temp_path.Length() );
		};
		rux::uint8 XPath::IsAbsolutePath( const char* path , size_t path_length )
		{			
			if( path_length == SIZE_MAX )
				path_length = strlen( path );
			return ( path_length > 0 && path[ 0 ] == '/' )
				|| ( path_length > 1 && path[ 1 ] == ':' )
				|| ( path_length > 2 && path[ 2 ] == ':' )
				|| ( path_length > 3 && path[ 3 ] == ':' ) ? 1 : 0;
		};
		rux::String& XPath::CalculateAbsolute( const rux::XString& working_path , const rux::XString& path )
		{
			rux::XString temp_working_path , temp_path , result;
			temp_working_path.set_ByRef( working_path.ReplaceAll( '\\' , '/' ) );	
			temp_path.set_ByRef( path.ReplaceAll( '\\' , '/' ) );	
			if( IsAbsolutePath( temp_path ) == 0 )
			{
				if( temp_working_path.get_UTF8Char( temp_working_path.Length() - 1 ) == '/' )
					temp_working_path.RemoveAt( temp_working_path.Length() - 1 );
				result.set_ByRef( temp_working_path + "/" + temp_path );
				result.set_ByRef( ExcludeRelative( result ) );
			}
			else
				result.set_ByRef( ExcludeRelative( temp_path ) );
			return result++;
		};
		rux::String& XPath::CalculateRelative( const rux::XString& relative_path , const rux::XString& path )
		{
			rux::XString temp_relative_path , temp_path , result;
			temp_relative_path.set_ByRef( relative_path.ReplaceAll( '\\' , '/' ) );	
			temp_path.set_ByRef( path.ReplaceAll( '\\' , '/' ) );	
			if( ( ( temp_relative_path.Length() > 0 && temp_relative_path.get_UTF8Char( 0 ) == '/' )
				|| ( temp_relative_path.Length() > 1 && temp_relative_path.get_UTF8Char( 1 ) == ':' ) )
				&& ( ( temp_path.Length() > 0 && temp_path.get_UTF8Char( 0 ) == '/' )
				|| ( temp_path.Length() > 1 && temp_path.get_UTF8Char( 1 ) == ':' ) ) 
				&& temp_path.get_UTF8Char( 0 ) == temp_relative_path.get_UTF8Char( 0 ) )
			{
				temp_relative_path.set_ByRef( ExcludeRelative( temp_relative_path ) );
				temp_path.set_ByRef( ExcludeRelative( temp_path ) );
				rux::XArray< XString > temp_relative_path_splits , temp_path_splits;
				temp_relative_path_splits.set_ByRef( temp_relative_path.Split( '/' ) );
				temp_path_splits.set_ByRef( temp_path.Split( '/' ) );
				rux::uint32 equal_count = 0;
				for( ; ; equal_count++ )
				{
					if( temp_path_splits.Count() <= equal_count
						|| temp_relative_path_splits.Count() <= equal_count 
						|| temp_path_splits[ equal_count ] != temp_relative_path_splits[ equal_count ] )
						break;
				}
				result += "./";
				for( rux::uint32 index0 = equal_count ; index0 < temp_relative_path_splits.Count() ; index0++ )
					result += "../";		
				for( ; equal_count < temp_path_splits.Count() ; equal_count++)
					result.set_ByRef( result + temp_path_splits[ equal_count ] + "/" );
				if( result.Length() > 0 && result.get_UTF8Char( result.Length() - 1 ) == '/' )
					result.RemoveAt( result.Length() - 1 );
			}
			return result++;
		};
		rux::String& XPath::ExcludeRelative( const rux::XString& path )
		{
			declare_variable( XString , result );
			result.set_ByRef( path.ReplaceAll( '\\' , '/' ) );
			if( ( result.Length() > 0 && result.get_UTF8Char( 0 ) == '/' )
				|| ( result.Length() > 1 && result.get_UTF8Char( 1 ) == ':' ) )
			{
				rux::int32 index0 = -1 , index1 = -1;
				while( ( index0 = result.IndexOf( "../" ) ) != -1 )
				{
					if( index0 == 0 )
						break;
					if( index0 > 2 )
					{
						index1 = result.LastIndexOf( "/" , index0 - 2 );
						if( index1 > -1 )
						{
							if( result.get_UTF8Char( index1 + 1 ) != '.' )
								result.set_ByRef( result.Substring( 0 , index1 ) + "/" + result.Substring( index0 + 3 ) );
							else
							{
								result.RemoveAt( index1 );
								result.RemoveAt( index1 );
							}
						}
						else
							break;
					}
					else
						break;			
				}
				while( ( index0 = result.IndexOf( "/.." ) ) != -1 )
				{
					if( index0 == 0 )
						break;
					if( index0 > 1 )
					{
						index1 = result.LastIndexOf( "/" , index0 - 1 );
						if( index1 > -1 )
						{
							if( result.get_UTF8Char( index1 + 1 ) != '.' )
								result.set_ByRef( result.Substring( 0 , index1 ) + "/" + result.Substring( index0 + 3 ) );
							else
							{
								result.RemoveAt( index1 );
								result.RemoveAt( index1 );
							}
						}
						else
							break;
					}
					else
						break;			
				}
				result.set_ByRef( result.ReplaceAll( "./" , "" ) );
				result.set_ByRef( result.ReplaceAll( ".\\" , "" ) );	
				result.set_ByRef( result.ReplaceAll( "/." , "" ) );
				result.set_ByRef( result.ReplaceAll( "\\." , "" ) );
			}
			return result++;
		};
	};
};