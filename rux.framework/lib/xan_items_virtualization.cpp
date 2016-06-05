#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_items_virtualization.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( RowCell , rux::gui );

end_implement_rux_class();

begin_implement_rux_class_ns( Row , rux::gui );

end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		implement_duplicate_internal_function_1( RowCell , set_ColumnName , const ::rux::XString& );
		void RowCell::set_ColumnName( const ::rux::XString&  column_name )
		{
			_column_name = column_name;
		};
		implement_duplicate_internal_function_1( RowCell , set_ColumnValue , XObject& );
		void RowCell::set_ColumnValue( XObject&  column_value )
		{
			_column_value = column_value;
		};
		implement_duplicate_internal_result_function_with_const( Object& , Row , get_ItemObject );
		Object& Row::get_ItemObject( void ) const
		{
			return _item_object++;
		};
		implement_duplicate_internal_function_1_with_const( Row , set_ItemObject , XObject& );
		void Row::set_ItemObject( XObject& item_object ) const
		{
			_item_object.set_ByRef( item_object );
		};
		implement_duplicate_internal_function_1_with_const( Row , set_Tag , XObject& );
		void Row::set_Tag( XObject& tag ) const
		{
			_tag.set_ByRef( tag );
		};
		implement_duplicate_internal_result_function_with_const( Object& , Row , get_Tag );
		Object& Row::get_Tag( void ) const
		{
			return _tag++;
		};
		implement_duplicate_internal_result_function_2_with_const( RowCell& , Row , FindCell , const ::rux::XString& , ::rux::uint8& );
		RowCell& Row::FindCell( const ::rux::XString& column_name , ::rux::uint8& found ) const
		{
			XRowCell cell;
			found = 0;
			for( size_t index0 = 0 ; index0 < _cells.Count() ; index0++ )
			{
				if( _cells[ index0 ]._column_name == column_name )
				{
					cell.set_ByRef( _cells[ index0 ] );
					found = 1;
					break;
				}
			}
			return cell++;
		};
		implement_duplicate_internal_function_1( Row , AddCell , ::rux::gui::XRowCell& );
		void Row::AddCell( ::rux::gui::XRowCell& cell )
		{
			_cells.Add( cell );
		};
		ItemsVirtualization::ItemsVirtualization( ::rux::get_static_Type_t get_static_Type_ )
			: XGCRef( get_static_Type_ )
		{
		};
	};
};