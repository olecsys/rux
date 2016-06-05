#ifndef XAN_ITEMS_VIRTUALIZATION_H
#define XAN_ITEMS_VIRTUALIZATION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class ListBox;
		};
		begin_declare_rux_class( RowCell );
			friend class ::rux::gui::controls::ListBox;
			friend class Row;
			void set_ColumnName( const ::rux::XString& column_name );
			void set_ColumnValue( XObject& column_value );
		begin_declare_rux_class_members( RowCell );
			::rux::XString _column_name;
			XObject _column_value;
		end_declare_rux_class_members( RowCell );
			void set_ColumnName( const ::rux::XString& column_name );
			void set_ColumnValue( XObject& column_value );
		end_declare_rux_class();

		begin_declare_rux_class( Row );
			friend class ::rux::gui::controls::ListBox;
			void AddCell( ::rux::gui::XRowCell& cell );
			RowCell& FindCell( const ::rux::XString& column_name , ::rux::uint8& found ) const;
			void set_Tag( XObject& tag ) const;
			Object& get_Tag( void ) const;
			Object& get_ItemObject( void ) const;
			void set_ItemObject( XObject& tag ) const;
		begin_declare_rux_class_members( Row );
			::rux::XArray< ::rux::gui::XRowCell > _cells;
			XObject _tag;
			XObject _item_object;
		end_declare_rux_class_members( Row );
			void AddCell( ::rux::gui::XRowCell& cell );
			RowCell& FindCell( const ::rux::XString& column_name , ::rux::uint8& found ) const;
			void set_Tag( XObject& tag ) const;
			Object& get_Tag( void ) const;
			Object& get_ItemObject( void ) const;
			void set_ItemObject( XObject& tag ) const;
		end_declare_rux_class();

		class ItemsVirtualization : public XGCRef
		{
		public:
			ItemsVirtualization( ::rux::get_static_Type_t get_static_Type_ );
			virtual size_t Count( void ) = 0;
			virtual ::rux::Array< ::rux::gui::XRow >& Get( size_t offset , size_t limit ) = 0;
		};
	};
};
#endif