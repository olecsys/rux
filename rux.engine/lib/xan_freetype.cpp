#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_freetype.h>
FT_BASE_DEF( void )
	ft_validator_error( ft_structs_h::FT_Validator  valid,
	ft_structs_h::FT_Error      error )
{
	volatile ft_jmp_buf* jump_buffer = &valid->jump_buffer;
	valid->error = error;
	ft_longjmp( *(ft_jmp_buf*) jump_buffer, 1 );
};
static ft_structs_h::FT_Error
	find_unicode_charmap( ft_structs_h::FT_Face  face )
{
	ft_structs_h::FT_CharMap*  first;
	ft_structs_h::FT_CharMap*  cur;
	/* caller should have already checked that `face' is valid */
	FT_ASSERT( face );
	first = face->charmaps;
	if ( !first )
		return ft_error_h::FT_Err_Invalid_CharMap_Handle;
	/*
	*  The original TrueType specification(s) only specified charmap
	*  formats that are capable of mapping 8 or 16 bit character codes to
	*  glyph indices.
	*
	*  However, recent updates to the Apple and OpenType specifications
	*  introduced new formats that are capable of mapping 32-bit character
	*  codes as well.  And these are already used on some fonts, mainly to
	*  map non-BMP Asian ideographs as defined in Unicode.
	*
	*  For compatibility purposes, these fonts generally come with
	*  *several* Unicode charmaps:
	*
	*   - One of them in the "old" 16-bit format, that cannot access
	*     all glyphs in the font.
	*
	*   - Another one in the "new" 32-bit format, that can access all
	*     the glyphs.
	*
	*  This function has been written to always favor a 32-bit charmap
	*  when found.  Otherwise, a 16-bit one is returned when found.
	*/
	/* Since the `interesting' table, with IDs (3,10), is normally the */
	/* last one, we loop backwards.  This loses with type1 fonts with  */
	/* non-BMP characters (<.0001%), this wins with .ttf with non-BMP  */
	/* chars (.01% ?), and this is the same about 99.99% of the time!  */
	cur = first + face->num_charmaps;  /* points after the last one */
	for ( ; --cur >= first; )
	{
		if ( cur[0]->encoding == ft_structs_h::FT_ENCODING_UNICODE )
		{
			/* XXX If some new encodings to represent UCS-4 are added, */
			/*     they should be added here.                          */
			if ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
				cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
				( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
				cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) )
			{
#ifdef FT_MAX_CHARMAP_CACHEABLE
				if ( cur - first > FT_MAX_CHARMAP_CACHEABLE )
				{
					FT_ERROR(( "find_unicode_charmap: UCS-4 cmap is found "
						"at too late position (%d)\n", cur - first ));
					continue;
				}
#endif
				face->charmap = cur[0];
				return ft_error_h::FT_Err_Ok;
			}
		}
	}
	/* We do not have any UCS-4 charmap.                */
	/* Do the loop again and search for UCS-2 charmaps. */
	cur = first + face->num_charmaps;
	for ( ; --cur >= first; )
	{
		if ( cur[0]->encoding == ft_structs_h::FT_ENCODING_UNICODE )
		{
#ifdef FT_MAX_CHARMAP_CACHEABLE
			if ( cur - first > FT_MAX_CHARMAP_CACHEABLE )
			{
				FT_ERROR(( "find_unicode_charmap: UCS-2 cmap is found "
					"at too late position (%d)\n", cur - first ));
				continue;
			}
#endif
			face->charmap = cur[0];
			return ft_error_h::FT_Err_Ok;
		}
	}
	return ft_error_h::FT_Err_Invalid_CharMap_Handle;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Select_Charmap( ft_structs_h::FT_Face      face,
	ft_structs_h::FT_Encoding  encoding )
{
	ft_structs_h::FT_CharMap*  cur;
	ft_structs_h::FT_CharMap*  limit;
	if ( !face )
		return ft_error_h::FT_Err_Invalid_Face_Handle;
	if ( encoding == ft_structs_h::FT_ENCODING_NONE )
		return ft_error_h::FT_Err_Invalid_Argument;
	/* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
	/* charmap available, i.e., one with UCS-4 characters, if possible.   */
	/*                                                                    */
	/* This is done by find_unicode_charmap() above, to share code.       */
	if ( encoding == ft_structs_h::FT_ENCODING_UNICODE )
		return find_unicode_charmap( face );
	cur = face->charmaps;
	if ( !cur )
		return ft_error_h::FT_Err_Invalid_CharMap_Handle;
	limit = cur + face->num_charmaps;
	for ( ; cur < limit; cur++ )
	{
		if ( cur[0]->encoding == encoding )
		{
#ifdef FT_MAX_CHARMAP_CACHEABLE
			if ( cur - face->charmaps > FT_MAX_CHARMAP_CACHEABLE )
			{
				FT_ERROR(( "FT_Select_Charmap: requested charmap is found (%d), "
					"but in too late position to cache\n",
					cur - face->charmaps ));
				continue;
			}
#endif
			face->charmap = cur[0];
			return 0;
		}
	}
	return ft_error_h::FT_Err_Invalid_Argument;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Outline_Decompose( ft_structs_h::FT_Outline*              outline,
	const ft_structs_h::FT_Outline_Funcs*  func_interface,
	void*                    user )
{
#undef SCALED
#define SCALED( x )  ( ( (x) << shift ) - delta )
	ft_structs_h::FT_Vector   v_last;
	ft_structs_h::FT_Vector   v_control;
	ft_structs_h::FT_Vector   v_start;
	ft_structs_h::FT_Vector*  point;
	ft_structs_h::FT_Vector*  limit;
	char*       tags;
	ft_structs_h::FT_Error    error;
	ft_structs_h::FT_Int   n;         /* index of contour in outline     */
	ft_structs_h::FT_UInt  first;     /* index of first point in contour */
	ft_structs_h::FT_Int   tag;       /* current point's state           */
	ft_structs_h::FT_Int   shift;
	ft_structs_h::FT_Pos   delta;
	if ( !outline || !func_interface )
		return ft_error_h::FT_Err_Invalid_Argument;
	shift = func_interface->shift;
	delta = func_interface->delta;
	first = 0;
	for ( n = 0; n < outline->n_contours; n++ )
	{
		ft_structs_h::FT_Int  last;  /* index of last point in contour */
		FT_TRACE5(( "FT_Outline_Decompose: Outline %d\n", n ));
		last = outline->contours[n];
		if ( last < 0 )
			goto Invalid_Outline;
		limit = outline->points + last;
		v_start   = outline->points[first];
		v_start.x = SCALED( v_start.x );
		v_start.y = SCALED( v_start.y );
		v_last   = outline->points[last];
		v_last.x = SCALED( v_last.x );
		v_last.y = SCALED( v_last.y );
		v_control = v_start;
		point = outline->points + first;
		tags  = outline->tags   + first;
		tag   = FT_CURVE_TAG( tags[0] );
		/* A contour cannot start with a cubic control point! */
		if ( tag == FT_CURVE_TAG_CUBIC )
			goto Invalid_Outline;
		/* check first point to determine origin */
		if ( tag == FT_CURVE_TAG_CONIC )
		{
			/* first point is conic control.  Yes, this happens. */
			if ( FT_CURVE_TAG( outline->tags[last] ) == FT_CURVE_TAG_ON )
			{
				/* start at last point if it is on the curve */
				v_start = v_last;
				limit--;
			}
			else
			{
				v_start.x = ( v_start.x + v_last.x ) / 2;
				v_start.y = ( v_start.y + v_last.y ) / 2;
				v_last = v_start;
			}
			point--;
			tags--;
		}
		FT_TRACE5(( "  move to (%.2f, %.2f)\n",
			v_start.x / 64.0, v_start.y / 64.0 ));
		error = func_interface->move_to( &v_start, user );
		if ( error )
			goto Exit;
		while ( point < limit )
		{
			point++;
			tags++;
			tag = FT_CURVE_TAG( tags[0] );
			switch ( tag )
			{
			case FT_CURVE_TAG_ON:  /* emit a single line_to */
				{
					ft_structs_h::FT_Vector  vec;
					vec.x = SCALED( point->x );
					vec.y = SCALED( point->y );
					FT_TRACE5(( "  line to (%.2f, %.2f)\n",
						vec.x / 64.0, vec.y / 64.0 ));
					error = func_interface->line_to( &vec, user );
					if ( error )
						goto Exit;
					continue;
				}
			case FT_CURVE_TAG_CONIC:  /* consume conic arcs */
				v_control.x = SCALED( point->x );
				v_control.y = SCALED( point->y );
Do_Conic:
				if ( point < limit )
				{
					ft_structs_h::FT_Vector  vec;
					ft_structs_h::FT_Vector  v_middle;
					point++;
					tags++;
					tag = FT_CURVE_TAG( tags[0] );
					vec.x = SCALED( point->x );
					vec.y = SCALED( point->y );
					if ( tag == FT_CURVE_TAG_ON )
					{
						FT_TRACE5(( "  conic to (%.2f, %.2f)"
							" with control (%.2f, %.2f)\n",
							vec.x / 64.0, vec.y / 64.0,
							v_control.x / 64.0, v_control.y / 64.0 ));
						error = func_interface->conic_to( &v_control, &vec, user );
						if ( error )
							goto Exit;
						continue;
					}
					if ( tag != FT_CURVE_TAG_CONIC )
						goto Invalid_Outline;
					v_middle.x = ( v_control.x + vec.x ) / 2;
					v_middle.y = ( v_control.y + vec.y ) / 2;
					FT_TRACE5(( "  conic to (%.2f, %.2f)"
						" with control (%.2f, %.2f)\n",
						v_middle.x / 64.0, v_middle.y / 64.0,
						v_control.x / 64.0, v_control.y / 64.0 ));
					error = func_interface->conic_to( &v_control, &v_middle, user );
					if ( error )
						goto Exit;
					v_control = vec;
					goto Do_Conic;
				}
				FT_TRACE5(( "  conic to (%.2f, %.2f)"
					" with control (%.2f, %.2f)\n",
					v_start.x / 64.0, v_start.y / 64.0,
					v_control.x / 64.0, v_control.y / 64.0 ));
				error = func_interface->conic_to( &v_control, &v_start, user );
				goto Close;
			default:  /* FT_CURVE_TAG_CUBIC */
				{
					ft_structs_h::FT_Vector  vec1, vec2;
					if ( point + 1 > limit                             ||
						FT_CURVE_TAG( tags[1] ) != FT_CURVE_TAG_CUBIC )
						goto Invalid_Outline;
					point += 2;
					tags  += 2;
					vec1.x = SCALED( point[-2].x );
					vec1.y = SCALED( point[-2].y );
					vec2.x = SCALED( point[-1].x );
					vec2.y = SCALED( point[-1].y );
					if ( point <= limit )
					{
						ft_structs_h::FT_Vector  vec;
						vec.x = SCALED( point->x );
						vec.y = SCALED( point->y );
						FT_TRACE5(( "  cubic to (%.2f, %.2f)"
							" with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
							vec.x / 64.0, vec.y / 64.0,
							vec1.x / 64.0, vec1.y / 64.0,
							vec2.x / 64.0, vec2.y / 64.0 ));
						error = func_interface->cubic_to( &vec1, &vec2, &vec, user );
						if ( error )
							goto Exit;
						continue;
					}
					FT_TRACE5(( "  cubic to (%.2f, %.2f)"
						" with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
						v_start.x / 64.0, v_start.y / 64.0,
						vec1.x / 64.0, vec1.y / 64.0,
						vec2.x / 64.0, vec2.y / 64.0 ));
					error = func_interface->cubic_to( &vec1, &vec2, &v_start, user );
					goto Close;
				}
			}
		}
		/* close the contour with a line segment */
		FT_TRACE5(( "  line to (%.2f, %.2f)\n",
			v_start.x / 64.0, v_start.y / 64.0 ));
		error = func_interface->line_to( &v_start, user );
Close:
		if ( error )
			goto Exit;
		first = last + 1;
	}
	FT_TRACE5(( "FT_Outline_Decompose: Done\n", n ));
	return ft_error_h::FT_Err_Ok;
Exit:
	FT_TRACE5(( "FT_Outline_Decompose: Error %d\n", error ));
	return error;
Invalid_Outline:
	return ft_error_h::FT_Err_Invalid_Outline;
};
FT_EXPORT_DEF( void )
	FT_List_Up( ft_structs_h::FT_List      list,
	ft_structs_h::FT_ListNode  node )
{
	ft_structs_h::FT_ListNode  before, after;
	before = node->prev;
	after  = node->next;
	/* check whether we are already on top of the list */
	if ( !before )
		return;
	before->next = after;
	if ( after )
		after->prev = before;
	else
		list->tail = before;
	node->prev       = 0;
	node->next       = list->head;
	list->head->prev = node;
	list->head       = node;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Set_Renderer( ft_structs_h::FT_Library     library,
	ft_structs_h::FT_Renderer    renderer,
	ft_structs_h::FT_UInt        num_params,
	ft_structs_h::FT_Parameter*  parameters )
{
	ft_structs_h::FT_ListNode  node;
	ft_structs_h::FT_Error     error = ft_error_h::FT_Err_Ok;
	if ( !library )
		return ft_error_h::FT_Err_Invalid_Library_Handle;
	if ( !renderer )
		return ft_error_h::FT_Err_Invalid_Argument;
	node = FT_List_Find( &library->renderers, renderer );
	if ( !node )
	{
		error = ft_error_h::FT_Err_Invalid_Argument;
		goto Exit;
	}
	FT_List_Up( &library->renderers, node );
	if ( renderer->glyph_format == ft_structs_h::FT_GLYPH_FORMAT_OUTLINE )
		library->cur_renderer = renderer;
	if ( num_params > 0 )
	{
		ft_structs_h::FT_Renderer_SetModeFunc  set_mode = renderer->clazz->set_mode;
		for ( ; num_params > 0; num_params-- )
		{
			error = set_mode( renderer, parameters->tag, parameters->data );
			if ( error )
				break;
			parameters++;
		}
	}
Exit:
	return error;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Render_Glyph_Internal( ft_structs_h::FT_Library      library,
	ft_structs_h::FT_GlyphSlot    slot,
	ft_structs_h::FT_Render_Mode  render_mode )
{
	ft_structs_h::FT_Error     error = ft_error_h::FT_Err_Ok;
	ft_structs_h::FT_Renderer  renderer;
	/* if it is already a bitmap, no need to do anything */
	switch ( slot->format )
	{
	case ft_structs_h::FT_GLYPH_FORMAT_BITMAP:   /* already a bitmap, don't do anything */
		break;
	default:
		{
			ft_structs_h::FT_ListNode  node   = 0;
			ft_structs_h::FT_Bool      update = 0;
			/* small shortcut for the very common case */
			if ( slot->format == ft_structs_h::FT_GLYPH_FORMAT_OUTLINE )
			{
				renderer = library->cur_renderer;
				node     = library->renderers.head;
			}
			else
				renderer = FT_Lookup_Renderer( library, slot->format, &node );
			error = ft_error_h::FT_Err_Unimplemented_Feature;
			while ( renderer )
			{
				error = renderer->render( renderer, slot, render_mode, NULL );
				if ( !error                                               ||
					FT_ERROR_BASE( error ) != ft_error_h::FT_Err_Cannot_Render_Glyph )
					break;
				renderer = FT_Lookup_Renderer( library, slot->format, &node );
				update   = 1;
			}
			if ( !error && update && renderer )
				FT_Set_Renderer( library, renderer, 0, 0 );
		}
	}
	return error;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Render_Glyph( ft_structs_h::FT_GlyphSlot    slot,
	ft_structs_h::FT_Render_Mode  render_mode )
{
	ft_structs_h::FT_Library  library;
	if ( !slot || !slot->face )
		return ft_error_h::FT_Err_Invalid_Argument;
	library = FT_FACE_LIBRARY( slot->face );
	return FT_Render_Glyph_Internal( library, slot, render_mode );
};
FT_EXPORT_DEF( void )
	FT_Outline_Transform( const ft_structs_h::FT_Outline*  outline,
	const ft_structs_h::FT_Matrix*   matrix )
{
	ft_structs_h::FT_Vector*  vec;
	ft_structs_h::FT_Vector*  limit;
	if ( !outline || !matrix )
		return;
	vec   = outline->points;
	limit = vec + outline->n_points;
	for ( ; vec < limit; vec++ )
		FT_Vector_Transform( vec, matrix );
};
static ft_structs_h::FT_Renderer
	ft_lookup_glyph_renderer( ft_structs_h::FT_GlyphSlot  slot );
#ifdef GRID_FIT_METRICS
static void
	ft_glyphslot_grid_fit_metrics( ft_structs_h::FT_GlyphSlot  slot,
	ft_structs_h::FT_Bool       vertical )
{
	ft_structs_h::FT_Glyph_Metrics*  metrics = &slot->metrics;
	ft_structs_h::FT_Pos             right, bottom;
	if ( vertical )
	{
		metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
		metrics->horiBearingY = FT_PIX_CEIL ( metrics->horiBearingY );
		right  = FT_PIX_CEIL( metrics->vertBearingX + metrics->width );
		bottom = FT_PIX_CEIL( metrics->vertBearingY + metrics->height );
		metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
		metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );
		metrics->width  = right - metrics->vertBearingX;
		metrics->height = bottom - metrics->vertBearingY;
	}
	else
	{
		metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
		metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );
		right  = FT_PIX_CEIL ( metrics->horiBearingX + metrics->width );
		bottom = FT_PIX_FLOOR( metrics->horiBearingY - metrics->height );
		metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
		metrics->horiBearingY = FT_PIX_CEIL ( metrics->horiBearingY );
		metrics->width  = right - metrics->horiBearingX;
		metrics->height = metrics->horiBearingY - bottom;
	}
	metrics->horiAdvance = FT_PIX_ROUND( metrics->horiAdvance );
	metrics->vertAdvance = FT_PIX_ROUND( metrics->vertAdvance );
};
#endif /* GRID_FIT_METRICS */
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Outline_Check( ft_structs_h::FT_Outline*  outline )
{
	if ( outline )
	{
		ft_structs_h::FT_Int  n_points   = outline->n_points;
		ft_structs_h::FT_Int  n_contours = outline->n_contours;
		ft_structs_h::FT_Int  end0, end;
		ft_structs_h::FT_Int  n;
		/* empty glyph? */
		if ( n_points == 0 && n_contours == 0 )
			return 0;
		/* check point and contour counts */
		if ( n_points <= 0 || n_contours <= 0 )
			goto Bad;
		end0 = end = -1;
		for ( n = 0; n < n_contours; n++ )
		{
			end = outline->contours[n];
			/* note that we don't accept empty contours */
			if ( end <= end0 || end >= n_points )
				goto Bad;
			end0 = end;
		}
		if ( end != n_points - 1 )
			goto Bad;
		/* XXX: check the tags array */
		return 0;
	}
Bad:
	return ft_error_h::FT_Err_Invalid_Argument;
};
static void
	ft_glyphslot_clear( ft_structs_h::FT_GlyphSlot  slot )
{
	/* free bitmap if needed */
	ft_glyphslot_free_bitmap( slot );
	/* clear all public fields in the glyph slot */
	FT_ZERO( &slot->metrics );
	FT_ZERO( &slot->outline );
	slot->bitmap.width      = 0;
	slot->bitmap.rows       = 0;
	slot->bitmap.pitch      = 0;
	slot->bitmap.pixel_mode = 0;
	/* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */
	slot->bitmap_left   = 0;
	slot->bitmap_top    = 0;
	slot->num_subglyphs = 0;
	slot->subglyphs     = 0;
	slot->control_data  = 0;
	slot->control_len   = 0;
	slot->other         = 0;
	slot->format        = ft_structs_h::FT_GLYPH_FORMAT_NONE;
	slot->linearHoriAdvance = 0;
	slot->linearVertAdvance = 0;
	slot->lsb_delta         = 0;
	slot->rsb_delta         = 0;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Load_Glyph( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_UInt   glyph_index,
	ft_structs_h::FT_Int32  load_flags )
{
	ft_structs_h::FT_Error      error;
	ft_structs_h::FT_Driver     driver;
	ft_structs_h::FT_GlyphSlot  slot;
	ft_structs_h::FT_Library    library;
	ft_structs_h::FT_Bool       autohint = FALSE;
	ft_structs_h::FT_Module     hinter;
	tt_h::TT_Face       ttface = (tt_h::TT_Face)face;
	if ( !face || !face->size || !face->glyph )
		return ft_error_h::FT_Err_Invalid_Face_Handle;
	slot = face->glyph;
	ft_glyphslot_clear( slot );
	driver  = face->driver;
	library = driver->root.library;
	hinter  = library->auto_hinter;
	if ( load_flags & FT_LOAD_NO_RECURSE )
		load_flags |= FT_LOAD_NO_SCALE         |
		FT_LOAD_IGNORE_TRANSFORM;
	if ( load_flags & FT_LOAD_NO_SCALE )
	{
		load_flags |= FT_LOAD_NO_HINTING |
			FT_LOAD_NO_BITMAP;
		load_flags &= ~FT_LOAD_RENDER;
	}
	if ( hinter                                           &&
		!( load_flags & FT_LOAD_NO_HINTING )             &&
		!( load_flags & FT_LOAD_NO_AUTOHINT )            &&
		FT_DRIVER_IS_SCALABLE( driver )                  &&
		FT_DRIVER_USES_OUTLINES( driver )                &&
		!FT_IS_TRICKY( face )                            &&
		( ( load_flags & FT_LOAD_IGNORE_TRANSFORM )    ||
		( face->internal->transform_matrix.yx == 0 &&
		face->internal->transform_matrix.xx != 0 ) ||
		( face->internal->transform_matrix.xx == 0 &&
		face->internal->transform_matrix.yx != 0 ) ) )
	{
		if ( ( load_flags & FT_LOAD_FORCE_AUTOHINT ) ||
			!FT_DRIVER_HAS_HINTER( driver )         )
			autohint = TRUE;
		else
		{
			ft_structs_h::FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );
			if ( mode == ft_structs_h::FT_RENDER_MODE_LIGHT                       ||
				face->internal->ignore_unpatented_hinter           ||
				( FT_IS_SFNT( face )                             &&
				ttface->num_locations                          &&
				ttface->max_profile.maxSizeOfInstructions == 0 ) )
				autohint = TRUE;
		}
	}
	if ( autohint )
	{
		ft_structs_h::FT_AutoHinter_Service  hinting;
		if ( FT_HAS_FIXED_SIZES( face )             &&
			( load_flags & FT_LOAD_NO_BITMAP ) == 0 )
		{
			error = driver->clazz->load_glyph( slot, face->size,
				glyph_index,
				load_flags | FT_LOAD_SBITS_ONLY );
			if ( !error && slot->format == ft_structs_h::FT_GLYPH_FORMAT_BITMAP )
				goto Load_Ok;
		}
		{
			ft_structs_h::FT_Face_Internal  internal        = face->internal;
			ft_structs_h::FT_Int            transform_flags = internal->transform_flags;
			internal->transform_flags = 0;
			hinting = (ft_structs_h::FT_AutoHinter_Service)hinter->clazz->module_interface;
			error   = hinting->load_glyph( (ft_structs_h::FT_AutoHinter)hinter,
				slot, face->size,
				glyph_index, load_flags );
			internal->transform_flags = transform_flags;
		}
	}
	else
	{
		error = driver->clazz->load_glyph( slot,
			face->size,
			glyph_index,
			load_flags );
		if ( error )
			goto Exit;
		if ( slot->format == ft_structs_h::FT_GLYPH_FORMAT_OUTLINE )
		{
			error = FT_Outline_Check( &slot->outline );
			if ( error )
				goto Exit;
#ifdef GRID_FIT_METRICS
			if ( !( load_flags & FT_LOAD_NO_HINTING ) )
				ft_glyphslot_grid_fit_metrics( slot,
				FT_BOOL( load_flags & FT_LOAD_VERTICAL_LAYOUT ) );
#endif
		}
	}
Load_Ok:
	if ( load_flags & FT_LOAD_VERTICAL_LAYOUT )
	{
		slot->advance.x = 0;
		slot->advance.y = slot->metrics.vertAdvance;
	}
	else
	{
		slot->advance.x = slot->metrics.horiAdvance;
		slot->advance.y = 0;
	}
	if ( ( load_flags & FT_LOAD_LINEAR_DESIGN ) == 0 &&
		( FT_IS_SCALABLE( face ) )                  )
	{
		ft_structs_h::FT_Size_Metrics*  metrics = &face->size->metrics;
		slot->linearHoriAdvance = FT_MulDiv( slot->linearHoriAdvance,
			metrics->x_scale, 64 );
		slot->linearVertAdvance = FT_MulDiv( slot->linearVertAdvance,
			metrics->y_scale, 64 );
	}
	if ( ( load_flags & FT_LOAD_IGNORE_TRANSFORM ) == 0 )
	{
		ft_structs_h::FT_Face_Internal  internal = face->internal;
		if ( internal->transform_flags )
		{
			ft_structs_h::FT_Renderer  renderer = ft_lookup_glyph_renderer( slot );
			if ( renderer )
				error = renderer->clazz->transform_glyph(
				renderer, slot,
				&internal->transform_matrix,
				&internal->transform_delta );
			else if ( slot->format == ft_structs_h::FT_GLYPH_FORMAT_OUTLINE )
			{
				if ( internal->transform_flags & 1 )
					FT_Outline_Transform( &slot->outline,
					&internal->transform_matrix );
				if ( internal->transform_flags & 2 )
					FT_Outline_Translate( &slot->outline,
					internal->transform_delta.x,
					internal->transform_delta.y );
			}
			FT_Vector_Transform( &slot->advance, &internal->transform_matrix );
		}
	}
	FT_TRACE5(( "  x advance: %d\n" , slot->advance.x ));
	FT_TRACE5(( "  y advance: %d\n" , slot->advance.y ));
	FT_TRACE5(( "  linear x advance: %d\n" , slot->linearHoriAdvance ));
	FT_TRACE5(( "  linear y advance: %d\n" , slot->linearVertAdvance ));
	if ( !error                                    &&
		slot->format != ft_structs_h::FT_GLYPH_FORMAT_BITMAP    &&
		slot->format != ft_structs_h::FT_GLYPH_FORMAT_COMPOSITE &&
		load_flags & FT_LOAD_RENDER )
	{
		ft_structs_h::FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );
		if ( mode == ft_structs_h::FT_RENDER_MODE_NORMAL      &&
			(load_flags & FT_LOAD_MONOCHROME ) )
			mode = ft_structs_h::FT_RENDER_MODE_MONO;
		error = FT_Render_Glyph( slot, mode );
	}
Exit:
	return error;
};
static ft_structs_h::FT_Renderer
	ft_lookup_glyph_renderer( ft_structs_h::FT_GlyphSlot  slot )
{
		ft_structs_h::FT_Face      face    = slot->face;
		ft_structs_h::FT_Library   library = FT_FACE_LIBRARY( face );
		ft_structs_h::FT_Renderer  result  = library->cur_renderer;
		if ( !result || result->glyph_format != slot->format )
			result = FT_Lookup_Renderer( library, slot->format, 0 );
		return result;
};
FT_EXPORT_DEF( ft_structs_h::FT_UInt )
	FT_Get_Char_Index( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_ULong  charcode )
{
	ft_structs_h::FT_UInt  result = 0;
	if ( face && face->charmap )
	{
		ft_structs_h::FT_CMap  cmap = FT_CMAP( face->charmap );
		if ( charcode > 0xFFFFFFFFUL )
		{
			FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
			FT_TRACE1(( " 0x%x is truncated\n", charcode ));
		}
		result = cmap->clazz->char_index( cmap, (ft_structs_h::FT_UInt32)charcode );
	}
	return result;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Load_Char( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_ULong  char_code,
	ft_structs_h::FT_Int32  load_flags )
{
	ft_structs_h::FT_UInt  glyph_index;
	if ( !face )
		return ft_error_h::FT_Err_Invalid_Face_Handle;
	glyph_index = (ft_structs_h::FT_UInt)char_code;
	if ( face->charmap )
		glyph_index = FT_Get_Char_Index( face, char_code );
	return FT_Load_Glyph( face, glyph_index, load_flags );
  }
FT_BASE_DEF( void )
	ft_validator_init( ft_structs_h::FT_Validator        valid,
	const ft_structs_h::FT_Byte*      base,
	const ft_structs_h::FT_Byte*      limit,
	ft_structs_h::FT_ValidationLevel  level )
{
	valid->base  = base;
	valid->limit = limit;
	valid->level = level;
	valid->error = ft_error_h::FT_Err_Ok;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Stream_Read( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Byte*   buffer,
	ft_structs_h::FT_ULong   count )
{
	return FT_Stream_ReadAt( stream, stream->pos, buffer, count );
};
FT_BASE_DEF( ft_structs_h::FT_UShort )
	FT_Stream_GetUShort( ft_structs_h::FT_Stream  stream )
{
	ft_structs_h::FT_Byte*  p;
	ft_structs_h::FT_Short  result;
	FT_ASSERT( stream && stream->cursor );
	result         = 0;
	p              = stream->cursor;
	if ( p + 1 < stream->limit )
		result       = FT_NEXT_USHORT( p );
	stream->cursor = p;
	return result;
};
FT_BASE_DEF( ft_structs_h::FT_UShort )
	FT_Stream_ReadUShort( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Error*  error )
{
	ft_structs_h::FT_Byte   reads[2];
	ft_structs_h::FT_Byte*  p = 0;
	ft_structs_h::FT_Short  result = 0;
	FT_ASSERT( stream );
	*error = ft_error_h::FT_Err_Ok;
	if ( stream->pos + 1 < stream->size )
	{
		if ( stream->read )
		{
			if ( stream->read( stream, stream->pos, reads, 2L ) != 2L )
				goto Fail;
			p = reads;
		}
		else
		{
			p = stream->base + stream->pos;
		}
		if ( p )
			result = FT_NEXT_USHORT( p );
	}
	else
		goto Fail;
	stream->pos += 2;
	return result;
Fail:
	*error = ft_error_h::FT_Err_Invalid_Stream_Operation;
	FT_ERROR(( "FT_Stream_ReadUShort:"
		" invalid i/o; pos = 0x%lx, size = 0x%lx\n",
		stream->pos, stream->size ));
	return 0;
};
FT_BASE_DEF( ft_structs_h::FT_ULong )
	FT_Stream_ReadULong( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Error*  error )
{
	ft_structs_h::FT_Byte   reads[4];
	ft_structs_h::FT_Byte*  p = 0;
	ft_structs_h::FT_Long   result = 0;
	FT_ASSERT( stream );
	*error = ft_error_h::FT_Err_Ok;
	if ( stream->pos + 3 < stream->size )
	{
		if ( stream->read )
		{
			if ( stream->read( stream, stream->pos, reads, 4L ) != 4L )
				goto Fail;
			p = reads;
		}
		else
		{
			p = stream->base + stream->pos;
		}
		if ( p )
			result = FT_NEXT_ULONG( p );
	}
	else
		goto Fail;
	stream->pos += 4;
	return result;
Fail:
	*error = ft_error_h::FT_Err_Invalid_Stream_Operation;
	FT_ERROR(( "FT_Stream_ReadULong:"
		" invalid i/o; pos = 0x%lx, size = 0x%lx\n",
		stream->pos, stream->size ));
	return 0;
};
FT_BASE_DEF( ft_structs_h::FT_Pointer )
	ft_module_get_service( ft_structs_h::FT_Module    module,
	const char*  service_id )
{
	ft_structs_h::FT_Pointer  result = NULL;
	if ( module )
	{
		FT_ASSERT( module->clazz && module->clazz->get_interface );
		/* first, look for the service in the module
		*/
		if ( module->clazz->get_interface )
			result = module->clazz->get_interface( module, service_id );
		if ( result == NULL )
		{
			/* we didn't find it, look in all other modules then
			*/
			ft_structs_h::FT_Library  library = module->library;
			ft_structs_h::FT_Module*  cur     = library->modules;
			ft_structs_h::FT_Module*  limit   = cur + library->num_modules;
			for ( ; cur < limit; cur++ )
			{
				if ( cur[0] != module )
				{
					FT_ASSERT( cur[0]->clazz );
					if ( cur[0]->clazz->get_interface )
					{
						result = cur[0]->clazz->get_interface( cur[0], service_id );
						if ( result != NULL )
							break;
					}
				}
			}
		}
	}
	return result;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Stream_ReadAt( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_ULong   pos,
	ft_structs_h::FT_Byte*   buffer,
	ft_structs_h::FT_ULong   count )
{
	ft_structs_h::FT_Error  error = ft_error_h::FT_Err_Ok;
	ft_structs_h::FT_ULong  read_bytes;
	if ( pos >= stream->size )
	{
		FT_ERROR(( "FT_Stream_ReadAt:"
			" invalid i/o; pos = 0x%lx, size = 0x%lx\n",
			pos, stream->size ));
		return ft_error_h::FT_Err_Invalid_Stream_Operation;
	}
	if ( stream->read )
		read_bytes = stream->read( stream, pos, buffer, count );
	else
	{
		read_bytes = stream->size - pos;
		if ( read_bytes > count )
			read_bytes = count;
		FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
	}
	stream->pos = pos + read_bytes;
	if ( read_bytes < count )
	{
		FT_ERROR(( "FT_Stream_ReadAt:"
			" invalid read; expected %lu bytes, got %lu\n",
			count, read_bytes ));
		error = ft_error_h::FT_Err_Invalid_Stream_Operation;
	}
	return error;
};
FT_EXPORT_DEF( void )
	FT_Vector_Transform( ft_structs_h::FT_Vector*        vector,
	const ft_structs_h::FT_Matrix*  matrix )
{
	ft_structs_h::FT_Pos  xz, yz;
	if ( !vector || !matrix )
		return;
	xz = FT_MulFix( vector->x, matrix->xx ) +
		FT_MulFix( vector->y, matrix->xy );
	yz = FT_MulFix( vector->x, matrix->yx ) +
		FT_MulFix( vector->y, matrix->yy );
	vector->x = xz;
	vector->y = yz;
};
static void
	FT_GlyphLoader_Adjust_Subglyphs( ft_structs_h::FT_GlyphLoader  loader )
{
	ft_structs_h::FT_GlyphLoad  base    = &loader->base;
	ft_structs_h::FT_GlyphLoad  current = &loader->current;
	current->subglyphs = base->subglyphs + base->num_subglyphs;
};
static void
	FT_GlyphLoader_Adjust_Points( ft_structs_h::FT_GlyphLoader  loader )
{
	ft_structs_h::FT_Outline*  base    = &loader->base.outline;
	ft_structs_h::FT_Outline*  current = &loader->current.outline;
	current->points   = base->points   + base->n_points;
	current->tags     = base->tags     + base->n_points;
	current->contours = base->contours + base->n_contours;
	/* handle extra points table - if any */
	if ( loader->use_extra )
	{
		loader->current.extra_points  = loader->base.extra_points +
			base->n_points;
		loader->current.extra_points2 = loader->base.extra_points2 +
			base->n_points;
	}
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_GlyphLoader_CheckSubGlyphs( ft_structs_h::FT_GlyphLoader  loader,
	ft_structs_h::FT_UInt         n_subs )
{
	ft_structs_h::FT_Memory     memory = loader->memory;
	ft_structs_h::FT_Error      error  = ft_error_h::FT_Err_Ok;
	ft_structs_h::FT_UInt       new_max, old_max;
	ft_structs_h::FT_GlyphLoad  base    = &loader->base;
	ft_structs_h::FT_GlyphLoad  current = &loader->current;
	new_max = base->num_subglyphs + current->num_subglyphs + n_subs;
	old_max = loader->max_subglyphs;
	if ( new_max > old_max )
	{
		new_max = FT_PAD_CEIL( new_max, 2 );
		if ( FT_RENEW_ARRAY( base->subglyphs, old_max, new_max ) )
			goto Exit;
		loader->max_subglyphs = new_max;
		FT_GlyphLoader_Adjust_Subglyphs( loader );
	}
Exit:
	return error;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_GlyphLoader_CreateExtra( ft_structs_h::FT_GlyphLoader  loader )
{
	ft_structs_h::FT_Error   error;
	ft_structs_h::FT_Memory  memory = loader->memory;
	if ( !FT_NEW_ARRAY( loader->base.extra_points, 2 * loader->max_points ) )
	{
		loader->use_extra          = 1;
		loader->base.extra_points2 = loader->base.extra_points +
			loader->max_points;
		FT_GlyphLoader_Adjust_Points( loader );
	}
	return error;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Get_Glyph_Name( ft_structs_h::FT_Face     face,
	ft_structs_h::FT_UInt     glyph_index,
	ft_structs_h::FT_Pointer  buffer,
	ft_structs_h::FT_UInt     buffer_max )
{
	ft_structs_h::FT_Error  error = ft_error_h::FT_Err_Invalid_Argument;
	/* clean up buffer */
	if ( buffer && buffer_max > 0 )
		((ft_structs_h::FT_Byte*)buffer)[0] = 0;
	if ( face                                     &&
		(ft_structs_h::FT_Long)glyph_index <= face->num_glyphs &&
		FT_HAS_GLYPH_NAMES( face )               )
	{
		FT_Service_GlyphDict  service;
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FACE_LOOKUP_SERVICE( face,
			service,
			GLYPH_DICT );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		if ( service && service->get_name )
			error = service->get_name( face, glyph_index, buffer, buffer_max );
	}
	return error;
};
FT_BASE_DEF( ft_structs_h::FT_Char )
	FT_Stream_GetChar( ft_structs_h::FT_Stream  stream )
{
	ft_structs_h::FT_Char  result;
	FT_ASSERT( stream && stream->cursor );
	result = 0;
	if ( stream->cursor < stream->limit )
		result = *stream->cursor++;
	return result;
};
FT_BASE_DEF( ft_structs_h::FT_ULong )
	FT_Stream_GetULong( ft_structs_h::FT_Stream  stream )
{
	ft_structs_h::FT_Byte*  p;
	ft_structs_h::FT_Long   result;
	FT_ASSERT( stream && stream->cursor );
	result         = 0;
	p              = stream->cursor;
	if ( p + 3 < stream->limit )
		result       = FT_NEXT_ULONG( p );
	stream->cursor = p;
	return result;
};
FT_BASE_DEF( const void* )
	FT_Get_Module_Interface( ft_structs_h::FT_Library   library,
	const char*  mod_name )
{
	ft_structs_h::FT_Module  module;
	/* test for valid `library' delayed to FT_Get_Module() */
	module = FT_Get_Module( library, mod_name );
	return module ? module->clazz->module_interface : 0;
};
FT_EXPORT_DEF( ft_structs_h::FT_Module )
	FT_Get_Module( ft_structs_h::FT_Library   library,
	const char*  module_name )
{
	ft_structs_h::FT_Module   result = 0;
	ft_structs_h::FT_Module*  cur;
	ft_structs_h::FT_Module*  limit;
	if ( !library || !module_name )
		return result;
	cur   = library->modules;
	limit = cur + library->num_modules;
	for ( ; cur < limit; cur++ )
		if ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
		{
			result = cur[0];
			break;
		}
		return result;
};
FT_EXPORT_DEF( void )
	FT_Outline_Get_CBox( const ft_structs_h::FT_Outline*  outline,
	ft_structs_h::FT_BBox           *acbox )
{
	ft_structs_h::FT_Pos  xMin, yMin, xMax, yMax;
	if ( outline && acbox )
	{
		if ( outline->n_points == 0 )
		{
			xMin = 0;
			yMin = 0;
			xMax = 0;
			yMax = 0;
		}
		else
		{
			ft_structs_h::FT_Vector*  vec   = outline->points;
			ft_structs_h::FT_Vector*  limit = vec + outline->n_points;
			xMin = xMax = vec->x;
			yMin = yMax = vec->y;
			vec++;
			for ( ; vec < limit; vec++ )
			{
				ft_structs_h::FT_Pos  x, y;
				x = vec->x;
				if ( x < xMin ) xMin = x;
				if ( x > xMax ) xMax = x;
				y = vec->y;
				if ( y < yMin ) yMin = y;
				if ( y > yMax ) yMax = y;
			}
		}
		acbox->xMin = xMin;
		acbox->xMax = xMax;
		acbox->yMin = yMin;
		acbox->yMax = yMax;
	}
};
FT_EXPORT_DEF( void )
	FT_Outline_Translate( const ft_structs_h::FT_Outline*  outline,
	ft_structs_h::FT_Pos             xOffset,
	ft_structs_h::FT_Pos             yOffset )
{
	ft_structs_h::FT_UShort   n;
	ft_structs_h::FT_Vector*  vec;
	if ( !outline )
		return;
	vec = outline->points;
	for ( n = 0; n < outline->n_points; n++ )
	{
		vec->x += xOffset;
		vec->y += yOffset;
		vec++;
	}
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_GlyphLoader_CheckPoints( ft_structs_h::FT_GlyphLoader  loader,
	ft_structs_h::FT_UInt         n_points,
	ft_structs_h::FT_UInt         n_contours )
{
	ft_structs_h::FT_Memory    memory  = loader->memory;
	ft_structs_h::FT_Error     error   = ft_error_h::FT_Err_Ok;
	ft_structs_h::FT_Outline*  base    = &loader->base.outline;
	ft_structs_h::FT_Outline*  current = &loader->current.outline;
	ft_structs_h::FT_Bool      adjust  = 0;
	ft_structs_h::FT_UInt      new_max, old_max;
	/* check points & tags */
	new_max = base->n_points + current->n_points + n_points;
	old_max = loader->max_points;
	if ( new_max > old_max )
	{
		new_max = FT_PAD_CEIL( new_max, 8 );
		if ( new_max > FT_OUTLINE_POINTS_MAX )
			return ft_error_h::FT_Err_Array_Too_Large;
		if ( FT_RENEW_ARRAY( base->points, old_max, new_max ) ||
			FT_RENEW_ARRAY( base->tags,   old_max, new_max ) )
			goto Exit;
		if ( loader->use_extra )
		{
			if ( FT_RENEW_ARRAY( loader->base.extra_points,
				old_max * 2, new_max * 2 ) )
				goto Exit;
			FT_ARRAY_MOVE( loader->base.extra_points + new_max,
				loader->base.extra_points + old_max,
				old_max );
			loader->base.extra_points2 = loader->base.extra_points + new_max;
		}
		adjust = 1;
		loader->max_points = new_max;
	}
	/* check contours */
	old_max = loader->max_contours;
	new_max = base->n_contours + current->n_contours +
		n_contours;
	if ( new_max > old_max )
	{
		new_max = FT_PAD_CEIL( new_max, 4 );
		if ( new_max > FT_OUTLINE_CONTOURS_MAX )
			return ft_error_h::FT_Err_Array_Too_Large;
		if ( FT_RENEW_ARRAY( base->contours, old_max, new_max ) )
			goto Exit;
		adjust = 1;
		loader->max_contours = new_max;
	}
	if ( adjust )
		FT_GlyphLoader_Adjust_Points( loader );
Exit:
	return error;
};
FT_BASE_DEF( ft_structs_h::FT_Int32 )
	FT_SqrtFixed( ft_structs_h::FT_Int32  x )
{
	ft_structs_h::FT_UInt32  root, rem_hi, rem_lo, test_div;
	ft_structs_h::FT_Int     count;
	root = 0;
	if ( x > 0 )
	{
		rem_hi = 0;
		rem_lo = x;
		count  = 24;
		do
		{
			rem_hi   = ( rem_hi << 2 ) | ( rem_lo >> 30 );
			rem_lo <<= 2;
			root   <<= 1;
			test_div = ( root << 1 ) + 1;
			if ( rem_hi >= test_div )
			{
				rem_hi -= test_div;
				root   += 1;
			}
		} while ( --count );
	}
	return (ft_structs_h::FT_Int32)root;
};
FT_BASE_DEF( void )
	FT_GlyphLoader_Prepare( ft_structs_h::FT_GlyphLoader  loader )
{
	ft_structs_h::FT_GlyphLoad  current = &loader->current;
	current->outline.n_points   = 0;
	current->outline.n_contours = 0;
	current->num_subglyphs      = 0;
	FT_GlyphLoader_Adjust_Points   ( loader );
	FT_GlyphLoader_Adjust_Subglyphs( loader );
};
FT_BASE_DEF( void )
	FT_GlyphLoader_Add( ft_structs_h::FT_GlyphLoader  loader )
{
	ft_structs_h::FT_GlyphLoad  base;
	ft_structs_h::FT_GlyphLoad  current;
	ft_structs_h::FT_UInt       n_curr_contours;
	ft_structs_h::FT_UInt       n_base_points;
	ft_structs_h::FT_UInt       n;
	if ( !loader )
		return;
	base    = &loader->base;
	current = &loader->current;
	n_curr_contours = current->outline.n_contours;
	n_base_points   = base->outline.n_points;
	base->outline.n_points =
		(short)( base->outline.n_points + current->outline.n_points );
	base->outline.n_contours =
		(short)( base->outline.n_contours + current->outline.n_contours );
	base->num_subglyphs += current->num_subglyphs;
	/* adjust contours count in newest outline */
	for ( n = 0; n < n_curr_contours; n++ )
		current->outline.contours[n] =
		(short)( current->outline.contours[n] + n_base_points );
	/* prepare for another new glyph image */
	FT_GlyphLoader_Prepare( loader );
};
FT_BASE_DEF( ft_structs_h::FT_Pointer )
	ft_service_list_lookup( ft_structs_h::FT_ServiceDesc  service_descriptors,
	const char*     service_id )
{
	ft_structs_h::FT_Pointer      result = NULL;
	ft_structs_h::FT_ServiceDesc  desc   = service_descriptors;
	if ( desc && service_id )
	{
		for ( ; desc->serv_id != NULL; desc++ )
		{
			if ( ft_strcmp( desc->serv_id, service_id ) == 0 )
			{
				result = (ft_structs_h::FT_Pointer)desc->serv_data;
				break;
			}
		}
	}
	return result;
};
FT_BASE_DEF( void )
	ft_synthesize_vertical_metrics( ft_structs_h::FT_Glyph_Metrics*  metrics,
	ft_structs_h::FT_Pos             advance )
{
	ft_structs_h::FT_Pos  height = metrics->height;
	/* compensate for glyph with bbox above/below the baseline */
	if ( metrics->horiBearingY < 0 )
	{
		if ( height < metrics->horiBearingY )
			height = metrics->horiBearingY;
	}
	else if ( metrics->horiBearingY > 0 )
		height -= metrics->horiBearingY;
		/* the factor 1.2 is a heuristical value */
	if ( !advance )
		advance = height * 12 / 10;
	metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
	metrics->vertBearingY = ( advance - height ) / 2;
	metrics->vertAdvance  = advance;
};
static void
	ft_cmap_done_internal( ft_structs_h::FT_CMap  cmap );
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_CMap_New( ft_structs_h::FT_CMap_Class  clazz,
	ft_structs_h::FT_Pointer     init_data,
	ft_structs_h::FT_CharMap     charmap,
	ft_structs_h::FT_CMap       *acmap )
{
	ft_structs_h::FT_Error   error = ft_error_h::FT_Err_Ok;
	ft_structs_h::FT_Face    face;
	ft_structs_h::FT_Memory  memory;
	ft_structs_h::FT_CMap    cmap = NULL;
	if ( clazz == NULL || charmap == NULL || charmap->face == NULL )
		return ft_error_h::FT_Err_Invalid_Argument;
	face   = charmap->face;
	memory = FT_FACE_MEMORY( face );
	if ( !FT_ALLOC( cmap, clazz->size ) )
	{
		cmap->charmap = *charmap;
		cmap->clazz   = clazz;
		if ( clazz->init )
		{
			error = clazz->init( cmap, init_data );
			if ( error )
				goto Fail;
		}
		/* add it to our list of charmaps */
		if ( FT_RENEW_ARRAY( face->charmaps,
			face->num_charmaps,
			face->num_charmaps + 1 ) )
			goto Fail;
		face->charmaps[face->num_charmaps++] = (ft_structs_h::FT_CharMap)cmap;
	}
Exit:
	if ( acmap )
		*acmap = cmap;
	return error;
Fail:
	ft_cmap_done_internal( cmap );
	cmap = NULL;
	goto Exit;
};
FT_BASE_DEF( ft_structs_h::FT_Pointer )
	ft_mem_realloc( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Long    item_size,
	ft_structs_h::FT_Long    cur_count,
	ft_structs_h::FT_Long    new_count,
	void*      block,
	ft_structs_h::FT_Error  *p_error )
{
	ft_structs_h::FT_Error  error = ft_error_h::FT_Err_Ok;
	block = ft_mem_qrealloc( memory, item_size,
		cur_count, new_count, block, &error );
	if ( !error && new_count > cur_count )
		FT_MEM_ZERO( (char*)block + cur_count * item_size,
		( new_count - cur_count ) * item_size );
	*p_error = error;
	return block;
};
FT_BASE_DEF( ft_structs_h::FT_Pointer )
ft_mem_qrealloc( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Long    item_size,
	ft_structs_h::FT_Long    cur_count,
	ft_structs_h::FT_Long    new_count,
	void*      block,
	ft_structs_h::FT_Error  *p_error )
{
	ft_structs_h::FT_Error  error = error = ft_error_h::FT_Err_Ok;
	/* Note that we now accept `item_size == 0' as a valid parameter, in
	* order to cover very weird cases where an ALLOC_MULT macro would be
	* called.
	*/
	if ( cur_count < 0 || new_count < 0 || item_size < 0 )
	{
		/* may help catch/prevent nasty security issues */
		error = error = ft_error_h::FT_Err_Invalid_Argument;
	}
	else if ( new_count == 0 || item_size == 0 )
	{
		ft_mem_free( memory, block );
		block = NULL;
	}
	else if ( new_count > FT_INT_MAX/item_size )
	{
		error = error = ft_error_h::FT_Err_Array_Too_Large;
	}
	else if ( cur_count == 0 )
	{
		FT_ASSERT( block == NULL );
		block = ft_mem_alloc( memory, new_count*item_size, &error );
	}
	else
	{
		ft_structs_h::FT_Pointer  block2;
		ft_structs_h::FT_Long     cur_size = cur_count*item_size;
		ft_structs_h::FT_Long     new_size = new_count*item_size;
		block2 = memory->realloc( memory, cur_size, new_size, block );
		if ( block2 == NULL )
			error = ft_error_h::FT_Err_Out_Of_Memory;
		else
			block = block2;
	}
	*p_error = error;
	return block;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Stream_ExtractFrame( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_ULong   count,
	ft_structs_h::FT_Byte**  pbytes )
{
	ft_structs_h::FT_Error  error;
	error = FT_Stream_EnterFrame( stream, count );
	if ( !error )
	{
		*pbytes = (ft_structs_h::FT_Byte*)stream->cursor;
		/* equivalent to FT_Stream_ExitFrame(), with no memory block release */
		stream->cursor = 0;
		stream->limit  = 0;
	}
	return error;
};
FT_BASE_DEF( void )
	FT_Stream_ReleaseFrame( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Byte**  pbytes )
{
	if ( stream && stream->read )
	{
		ft_structs_h::FT_Memory  memory = stream->memory;
#ifdef FT_DEBUG_MEMORY
		ft_mem_free( memory, *pbytes );
		*pbytes = NULL;
#else
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( *pbytes );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
#endif
	}
	*pbytes = 0;
};
FT_BASE_DEF( ft_structs_h::FT_Long )
	FT_Stream_Pos( ft_structs_h::FT_Stream  stream )
{
	return stream->pos;
};
FT_BASE_DEF( ft_structs_h::FT_UShort )
	FT_Stream_GetUShortLE( ft_structs_h::FT_Stream  stream )
{
	ft_structs_h::FT_Byte*  p;
	ft_structs_h::FT_Short  result;
	FT_ASSERT( stream && stream->cursor );
	result         = 0;
	p              = stream->cursor;
	if ( p + 1 < stream->limit )
		result       = FT_NEXT_USHORT_LE( p );
	stream->cursor = p;
	return result;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Stream_ReadFields( ft_structs_h::FT_Stream              stream,
	const ft_structs_h::FT_Frame_Field*  fields,
	void*                  structure )
{
	ft_structs_h::FT_Error  error;
	ft_structs_h::FT_Bool   frame_accessed = 0;
	ft_structs_h::FT_Byte*  cursor;
	if ( !fields || !stream )
		return ft_error_h::FT_Err_Invalid_Argument;
	cursor = stream->cursor;
	error = ft_error_h::FT_Err_Ok;
	do
	{
		ft_structs_h::FT_ULong  value;
		ft_structs_h::FT_Int    sign_shift;
		ft_structs_h::FT_Byte*  p;
		switch ( fields->value )
		{
		case ft_structs_h::ft_frame_start:  /* access a new frame */
			error = FT_Stream_EnterFrame( stream, fields->offset );
			if ( error )
				goto Exit;
			frame_accessed = 1;
			cursor         = stream->cursor;
			fields++;
			continue;  /* loop! */
		case ft_structs_h::ft_frame_bytes:  /* read a byte sequence */
		case ft_structs_h::ft_frame_skip:   /* skip some bytes      */
			{
				ft_structs_h::FT_UInt  len = fields->size;
				if ( cursor + len > stream->limit )
				{
					error = ft_error_h::FT_Err_Invalid_Stream_Operation;
					goto Exit;
				}
				if ( fields->value == ft_structs_h::ft_frame_bytes )
				{
					p = (ft_structs_h::FT_Byte*)structure + fields->offset;
					FT_MEM_COPY( p, cursor, len );
				}
				cursor += len;
				fields++;
				continue;
			}
		case ft_structs_h::ft_frame_byte:
		case ft_structs_h::ft_frame_schar:  /* read a single byte */
			value = FT_NEXT_BYTE(cursor);
			sign_shift = 24;
			break;
		case ft_structs_h::ft_frame_short_be:
		case ft_structs_h::ft_frame_ushort_be:  /* read a 2-byte big-endian short */
			value = FT_NEXT_USHORT(cursor);
			sign_shift = 16;
			break;
		case ft_structs_h::ft_frame_short_le:
		case ft_structs_h::ft_frame_ushort_le:  /* read a 2-byte little-endian short */
			value = FT_NEXT_USHORT_LE(cursor);
			sign_shift = 16;
			break;
		case ft_structs_h::ft_frame_long_be:
		case ft_structs_h::ft_frame_ulong_be:  /* read a 4-byte big-endian long */
			value = FT_NEXT_ULONG(cursor);
			sign_shift = 0;
			break;
		case ft_structs_h::ft_frame_long_le:
		case ft_structs_h::ft_frame_ulong_le:  /* read a 4-byte little-endian long */
			value = FT_NEXT_ULONG_LE(cursor);
			sign_shift = 0;
			break;
		case ft_structs_h::ft_frame_off3_be:
		case ft_structs_h::ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
			value = FT_NEXT_UOFF3(cursor);
			sign_shift = 8;
			break;
		case ft_structs_h::ft_frame_off3_le:
		case ft_structs_h::ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
			value = FT_NEXT_UOFF3_LE(cursor);
			sign_shift = 8;
			break;
		default:
			/* otherwise, exit the loop */
			stream->cursor = cursor;
			goto Exit;
		}
		/* now, compute the signed value is necessary */
		if ( fields->value & FT_FRAME_OP_SIGNED )
			value = (ft_structs_h::FT_ULong)( (ft_structs_h::FT_Int32)( value << sign_shift ) >> sign_shift );
		/* finally, store the value in the object */
		p = (ft_structs_h::FT_Byte*)structure + fields->offset;
		switch ( fields->size )
		{
		case (8 / FT_CHAR_BIT):
			*(ft_structs_h::FT_Byte*)p = (ft_structs_h::FT_Byte)value;
			break;
		case (16 / FT_CHAR_BIT):
			*(ft_structs_h::FT_UShort*)p = (ft_structs_h::FT_UShort)value;
			break;
		case (32 / FT_CHAR_BIT):
			*(ft_structs_h::FT_UInt32*)p = (ft_structs_h::FT_UInt32)value;
			break;
		default:  /* for 64-bit systems */
			*(ft_structs_h::FT_ULong*)p = (ft_structs_h::FT_ULong)value;
		}
		/* go to next field */
		fields++;
	}
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	while ( 1 );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
Exit:
	/* close the frame if it was opened by this read */
	if ( frame_accessed )
		FT_Stream_ExitFrame( stream );
	return error;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Stream_EnterFrame( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_ULong   count )
{
	ft_structs_h::FT_Error  error = ft_error_h::FT_Err_Ok;
	ft_structs_h::FT_ULong  read_bytes;
	/* check for nested frame access */
	FT_ASSERT( stream && stream->cursor == 0 );
	if ( stream->read )
	{
		/* allocate the frame in memory */
		ft_structs_h::FT_Memory  memory = stream->memory;
		/* simple sanity check */
		if ( count > stream->size )
		{
			FT_ERROR(( "FT_Stream_EnterFrame:"
				" frame size (%lu) larger than stream size (%lu)\n",
				count, stream->size ));
			error = ft_error_h::FT_Err_Invalid_Stream_Operation;
			goto Exit;
		}
#ifdef FT_DEBUG_MEMORY
		/* assume _ft_debug_file and _ft_debug_lineno are already set */
		stream->base = (unsigned char*)ft_mem_qalloc( memory, count, &error );
		if ( error )
			goto Exit;
#else
		if ( FT_QALLOC( stream->base, count ) )
			goto Exit;
#endif
		/* read it */
		read_bytes = stream->read( stream, stream->pos,
			stream->base, count );
		if ( read_bytes < count )
		{
			FT_ERROR(( "FT_Stream_EnterFrame:"
				" invalid read; expected %lu bytes, got %lu\n",
				count, read_bytes ));
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
			FT_FREE( stream->base );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
			error = ft_error_h::FT_Err_Invalid_Stream_Operation;
		}
		stream->cursor = stream->base;
		stream->limit  = stream->cursor + count;
		stream->pos   += read_bytes;
	}
	else
	{
		/* check current and new position */
		if ( stream->pos >= stream->size        ||
			stream->size - stream->pos < count )
		{
			FT_ERROR(( "FT_Stream_EnterFrame:"
				" invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
				stream->pos, count, stream->size ));
			error = ft_error_h::FT_Err_Invalid_Stream_Operation;
			goto Exit;
		}
		/* set cursor */
		stream->cursor = stream->base + stream->pos;
		stream->limit  = stream->cursor + count;
		stream->pos   += count;
	}
Exit:
	return error;
};
FT_BASE_DEF( void )
	FT_Stream_ExitFrame( ft_structs_h::FT_Stream  stream )
{
	/* IMPORTANT: The assertion stream->cursor != 0 was removed, given    */
	/*            that it is possible to access a frame of length 0 in    */
	/*            some weird fonts (usually, when accessing an array of   */
	/*            0 records, like in some strange kern tables).           */
	/*                                                                    */
	/*  In this case, the loader code handles the 0-length table          */
	/*  gracefully; however, stream.cursor is really set to 0 by the      */
	/*  FT_Stream_EnterFrame() call, and this is not an error.            */
	/*                                                                    */
	FT_ASSERT( stream );
	if ( stream->read )
	{
		ft_structs_h::FT_Memory  memory = stream->memory;
#ifdef FT_DEBUG_MEMORY
		ft_mem_free( memory, stream->base );
		stream->base = NULL;
#else
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( stream->base );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
#endif
	}
	stream->cursor = 0;
	stream->limit  = 0;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Stream_Seek( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_ULong   pos )
{
	ft_structs_h::FT_Error  error = ft_error_h::FT_Err_Ok;
	if ( stream->read )
	{
		if ( stream->read( stream, pos, 0, 0 ) )
		{
			FT_ERROR(( "FT_Stream_Seek:"
				" invalid i/o; pos = 0x%lx, size = 0x%lx\n",
				pos, stream->size ));
			error = ft_error_h::FT_Err_Invalid_Stream_Operation;
		}
	}
	/* note that seeking to the first position after the file is valid */
	else if ( pos > stream->size )
	{
		FT_ERROR(( "FT_Stream_Seek:"
			" invalid i/o; pos = 0x%lx, size = 0x%lx\n",
			pos, stream->size ));
		error = ft_error_h::FT_Err_Invalid_Stream_Operation;
	}
	if ( !error )
		stream->pos = pos;
	return error;
};
#ifdef FT_LONG64
/* documentation is in freetype.h */
FT_EXPORT_DEF( ft_structs_h::FT_Long )
	FT_MulDiv( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b,
	ft_structs_h::FT_Long  c )
{
	ft_structs_h::FT_Int   s;
	ft_structs_h::FT_Long  d;
	s = 1;
	if ( a < 0 ) { a = -a; s = -1; }
	if ( b < 0 ) { b = -b; s = -s; }
	if ( c < 0 ) { c = -c; s = -s; }
	d = (ft_structs_h::FT_Long)( c > 0 ? ( (ft_structs_h::FT_Int64)a * b + ( c >> 1 ) ) / c
		: 0x7FFFFFFFL );
	return ( s > 0 ) ? d : -d;
}
#ifdef TT_USE_BYTECODE_INTERPRETER
/* documentation is in ftcalc.h */
FT_BASE_DEF( FT_Long )
	FT_MulDiv_No_Round( FT_Long  a,
	FT_Long  b,
	FT_Long  c )
{
	FT_Int   s;
	FT_Long  d;
	s = 1;
	if ( a < 0 ) { a = -a; s = -1; }
	if ( b < 0 ) { b = -b; s = -s; }
	if ( c < 0 ) { c = -c; s = -s; }
	d = (FT_Long)( c > 0 ? (FT_Int64)a * b / c
		: 0x7FFFFFFFL );
	return ( s > 0 ) ? d : -d;
}
#endif /* TT_USE_BYTECODE_INTERPRETER */
/* documentation is in freetype.h */
FT_EXPORT_DEF( ft_structs_h::FT_Long )
	FT_MulFix( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b )
{
#ifdef FT_MULFIX_ASSEMBLER
	return FT_MULFIX_ASSEMBLER( a, b );
#else
	ft_structs_h::FT_Int   s = 1;
	ft_structs_h::FT_Long  c;
	if ( a < 0 )
	{
		a = -a;
		s = -1;
	}
	if ( b < 0 )
	{
		b = -b;
		s = -s;
	}
	c = (ft_structs_h::FT_Long)( ( (ft_structs_h::FT_Int64)a * b + 0x8000L ) >> 16 );
	return ( s > 0 ) ? c : -c;
#endif /* FT_MULFIX_ASSEMBLER */
}
/* documentation is in freetype.h */
FT_EXPORT_DEF( ft_structs_h::FT_Long )
	FT_DivFix( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b )
{
	ft_structs_h::FT_Int32   s;
	ft_structs_h::FT_UInt32  q;
	s = 1;
	if ( a < 0 ) { a = -a; s = -1; }
	if ( b < 0 ) { b = -b; s = -s; }
	if ( b == 0 )
		/* check for division by 0 */
		q = 0x7FFFFFFFL;
	else
		/* compute result directly */
		q = (ft_structs_h::FT_UInt32)( ( ( (ft_structs_h::FT_Int64)a << 16 ) + ( b >> 1 ) ) / b );
	return ( s < 0 ? -(ft_structs_h::FT_Long)q : (ft_structs_h::FT_Long)q );
}
#else /* !FT_LONG64 */
static void
	ft_multo64( ft_structs_h::FT_UInt32  x,
	ft_structs_h::FT_UInt32  y,
	ft_structs_h::FT_Int64  *z )
{
	ft_structs_h::FT_UInt32  lo1, hi1, lo2, hi2, lo, hi, i1, i2;
	lo1 = x & 0x0000FFFFU;  hi1 = x >> 16;
	lo2 = y & 0x0000FFFFU;  hi2 = y >> 16;
	lo = lo1 * lo2;
	i1 = lo1 * hi2;
	i2 = lo2 * hi1;
	hi = hi1 * hi2;
	/* Check carry overflow of i1 + i2 */
	i1 += i2;
	hi += (ft_structs_h::FT_UInt32)( i1 < i2 ) << 16;
	hi += i1 >> 16;
	i1  = i1 << 16;
	/* Check carry overflow of i1 + lo */
	lo += i1;
	hi += ( lo < i1 );
	z->lo = lo;
	z->hi = hi;
}
static ft_structs_h::FT_UInt32
	ft_div64by32( ft_structs_h::FT_UInt32  hi,
	ft_structs_h::FT_UInt32  lo,
	ft_structs_h::FT_UInt32  y )
{
	ft_structs_h::FT_UInt32  r, q;
	ft_structs_h::FT_Int     i;
	q = 0;
	r = hi;
	if ( r >= y )
		return (ft_structs_h::FT_UInt32)0x7FFFFFFFL;
	i = 32;
	do
	{
		r <<= 1;
		q <<= 1;
		r  |= lo >> 31;
		if ( r >= y )
		{
			r -= y;
			q |= 1;
		}
		lo <<= 1;
	} while ( --i );
	return q;
}
static void
	FT_Add64( ft_structs_h::FT_Int64*  x,
	ft_structs_h::FT_Int64*  y,
	ft_structs_h::FT_Int64  *z )
{
	register ft_structs_h::FT_UInt32  lo, hi;
	lo = x->lo + y->lo;
	hi = x->hi + y->hi + ( lo < x->lo );
	z->lo = lo;
	z->hi = hi;
}
/* documentation is in freetype.h */
/* The FT_MulDiv function has been optimized thanks to ideas from      */
/* Graham Asher.  The trick is to optimize computation when everything */
/* fits within 32-bits (a rather common case).                         */
/*                                                                     */
/*  we compute 'a*b+c/2', then divide it by 'c'. (positive values)     */
/*                                                                     */
/*  46340 is FLOOR(SQRT(2^31-1)).                                      */
/*                                                                     */
/*  if ( a <= 46340 && b <= 46340 ) then ( a*b <= 0x7FFEA810 )         */
/*                                                                     */
/*  0x7FFFFFFF - 0x7FFEA810 = 0x157F0                                  */
/*                                                                     */
/*  if ( c < 0x157F0*2 ) then ( a*b+c/2 <= 0x7FFFFFFF )                */
/*                                                                     */
/*  and 2*0x157F0 = 176096                                             */
/*                                                                     */
FT_EXPORT_DEF( ft_structs_h::FT_Long )
	FT_MulDiv( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b,
	ft_structs_h::FT_Long  c )
{
	long  s;
	/* XXX: this function does not allow 64-bit arguments */
	if ( a == 0 || b == c )
		return a;
	s  = a; a = FT_ABS( a );
	s ^= b; b = FT_ABS( b );
	s ^= c; c = FT_ABS( c );
	if ( a <= 46340L && b <= 46340L && c <= 176095L && c > 0 )
		a = ( a * b + ( c >> 1 ) ) / c;
	else if ( (ft_structs_h::FT_Int32)c > 0 )
	{
		ft_structs_h::FT_Int64  temp, temp2;
		ft_multo64( (ft_structs_h::FT_Int32)a, (ft_structs_h::FT_Int32)b, &temp );
		temp2.hi = 0;
		temp2.lo = (ft_structs_h::FT_UInt32)(c >> 1);
		FT_Add64( &temp, &temp2, &temp );
		a = ft_div64by32( temp.hi, temp.lo, (ft_structs_h::FT_Int32)c );
	}
	else
		a = 0x7FFFFFFFL;
	return ( s < 0 ? -a : a );
}
#ifdef TT_USE_BYTECODE_INTERPRETER
FT_BASE_DEF( ft_structs_h::FT_Long )
	FT_MulDiv_No_Round( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b,
	ft_structs_h::FT_Long  c )
{
	long  s;
	if ( a == 0 || b == c )
		return a;
	s  = a; a = FT_ABS( a );
	s ^= b; b = FT_ABS( b );
	s ^= c; c = FT_ABS( c );
	if ( a <= 46340L && b <= 46340L && c > 0 )
		a = a * b / c;
	else if ( (ft_structs_h::FT_Int32)c > 0 )
	{
		ft_structs_h::FT_Int64  temp;
		ft_multo64( (ft_structs_h::FT_Int32)a, (ft_structs_h::FT_Int32)b, &temp );
		a = ft_div64by32( temp.hi, temp.lo, (ft_structs_h::FT_Int32)c );
	}
	else
		a = 0x7FFFFFFFL;
	return ( s < 0 ? -a : a );
}
#endif /* TT_USE_BYTECODE_INTERPRETER */
/* documentation is in freetype.h */
FT_EXPORT_DEF( ft_structs_h::FT_Long )
	FT_MulFix( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b )
{
#ifdef FT_MULFIX_ASSEMBLER
	return FT_MULFIX_ASSEMBLER( a, b );
#elif 0
	/*
	*  This code is nonportable.  See comment below.
	*
	*  However, on a platform where right-shift of a signed quantity fills
	*  the leftmost bits by copying the sign bit, it might be faster.
	*/
	ft_structs_h::FT_Long   sa, sb;
	ft_structs_h::FT_ULong  ua, ub;
	if ( a == 0 || b == 0x10000L )
		return a;
	/*
	*  This is a clever way of converting a signed number `a' into its
	*  absolute value (stored back into `a') and its sign.  The sign is
	*  stored in `sa'; 0 means `a' was positive or zero, and -1 means `a'
	*  was negative.  (Similarly for `b' and `sb').
	*
	*  Unfortunately, it doesn't work (at least not portably).
	*
	*  It makes the assumption that right-shift on a negative signed value
	*  fills the leftmost bits by copying the sign bit.  This is wrong.
	*  According to K&R 2nd ed, section `A7.8 Shift Operators' on page 206,
	*  the result of right-shift of a negative signed value is
	*  implementation-defined.  At least one implementation fills the
	*  leftmost bits with 0s (i.e., it is exactly the same as an unsigned
	*  right shift).  This means that when `a' is negative, `sa' ends up
	*  with the value 1 rather than -1.  After that, everything else goes
	*  wrong.
	*/
	sa = ( a >> ( sizeof ( a ) * 8 - 1 ) );
	a  = ( a ^ sa ) - sa;
	sb = ( b >> ( sizeof ( b ) * 8 - 1 ) );
	b  = ( b ^ sb ) - sb;
	ua = (ft_structs_h::FT_ULong)a;
	ub = (ft_structs_h::FT_ULong)b;
	if ( ua <= 2048 && ub <= 1048576L )
		ua = ( ua * ub + 0x8000U ) >> 16;
	else
	{
		ft_structs_h::FT_ULong  al = ua & 0xFFFFU;
		ua = ( ua >> 16 ) * ub +  al * ( ub >> 16 ) +
			( ( al * ( ub & 0xFFFFU ) + 0x8000U ) >> 16 );
	}
	sa ^= sb,
		ua  = (ft_structs_h::FT_ULong)(( ua ^ sa ) - sa);
	return (ft_structs_h::FT_Long)ua;
#else /* 0 */
	ft_structs_h::FT_Long   s;
	ft_structs_h::FT_ULong  ua, ub;
	if ( a == 0 || b == 0x10000L )
		return a;
	s  = a; a = FT_ABS( a );
	s ^= b; b = FT_ABS( b );
	ua = (ft_structs_h::FT_ULong)a;
	ub = (ft_structs_h::FT_ULong)b;
	if ( ua <= 2048 && ub <= 1048576L )
		ua = ( ua * ub + 0x8000UL ) >> 16;
	else
	{
		ft_structs_h::FT_ULong  al = ua & 0xFFFFUL;
		ua = ( ua >> 16 ) * ub +  al * ( ub >> 16 ) +
			( ( al * ( ub & 0xFFFFUL ) + 0x8000UL ) >> 16 );
	}
	return ( s < 0 ? -(ft_structs_h::FT_Long)ua : (ft_structs_h::FT_Long)ua );
#endif /* 0 */
}
/* documentation is in freetype.h */
FT_EXPORT_DEF( ft_structs_h::FT_Long )
	FT_DivFix( ft_structs_h::FT_Long  a,
	ft_structs_h::FT_Long  b )
{
	ft_structs_h::FT_Int32   s;
	ft_structs_h::FT_UInt32  q;
	/* XXX: this function does not allow 64-bit arguments */
	s  = (ft_structs_h::FT_Int32)a; a = FT_ABS( a );
	s ^= (ft_structs_h::FT_Int32)b; b = FT_ABS( b );
	if ( (ft_structs_h::FT_UInt32)b == 0 )
	{
		/* check for division by 0 */
		q = (ft_structs_h::FT_UInt32)0x7FFFFFFFL;
	}
	else if ( ( a >> 16 ) == 0 )
	{
		/* compute result directly */
		q = (ft_structs_h::FT_UInt32)( ( a << 16 ) + ( b >> 1 ) ) / (ft_structs_h::FT_UInt32)b;
	}
	else
	{
		/* we need more bits; we have to do it by hand */
		ft_structs_h::FT_Int64  temp, temp2;
		temp.hi  = (ft_structs_h::FT_Int32) ( a >> 16 );
		temp.lo  = (ft_structs_h::FT_UInt32)( a << 16 );
		temp2.hi = 0;
		temp2.lo = (ft_structs_h::FT_UInt32)( b >> 1 );
		FT_Add64( &temp, &temp2, &temp );
		q = ft_div64by32( temp.hi, temp.lo, (ft_structs_h::FT_Int32)b );
	}
	return ( s < 0 ? -(ft_structs_h::FT_Int32)q : (ft_structs_h::FT_Int32)q );
}
#if 0
/* documentation is in ftcalc.h */
FT_EXPORT_DEF( void )
	FT_MulTo64( ft_structs_h::FT_Int32   x,
	ft_structs_h::FT_Int32   y,
	ft_structs_h::FT_Int64  *z )
{
	ft_structs_h::FT_Int32  s;
	s  = x; x = FT_ABS( x );
	s ^= y; y = FT_ABS( y );
	ft_multo64( x, y, z );
	if ( s < 0 )
	{
		z->lo = (ft_structs_h::FT_UInt32)-(ft_structs_h::FT_Int32)z->lo;
		z->hi = ~z->hi + !( z->lo );
	}
}
/* apparently, the second version of this code is not compiled correctly */
/* on Mac machines with the MPW C compiler..  tsk, tsk, tsk...           */
#if 1
FT_EXPORT_DEF( ft_structs_h::FT_Int32 )
	FT_Div64by32( ft_structs_h::FT_Int64*  x,
	ft_structs_h::FT_Int32   y )
{
	ft_structs_h::FT_Int32   s;
	ft_structs_h::FT_UInt32  q, r, i, lo;
	s  = x->hi;
	if ( s < 0 )
	{
		x->lo = (ft_structs_h::FT_UInt32)-(ft_structs_h::FT_Int32)x->lo;
		x->hi = ~x->hi + !x->lo;
	}
	s ^= y;  y = FT_ABS( y );
	/* Shortcut */
	if ( x->hi == 0 )
	{
		if ( y > 0 )
			q = x->lo / y;
		else
			q = 0x7FFFFFFFL;
		return ( s < 0 ? -(ft_structs_h::FT_Int32)q : (ft_structs_h::FT_Int32)q );
	}
	r  = x->hi;
	lo = x->lo;
	if ( r >= (ft_structs_h::FT_UInt32)y ) /* we know y is to be treated as unsigned here */
		return ( s < 0 ? 0x80000001UL : 0x7FFFFFFFUL );
	/* Return Max/Min Int32 if division overflow. */
	/* This includes division by zero!            */
	q = 0;
	for ( i = 0; i < 32; i++ )
	{
		r <<= 1;
		q <<= 1;
		r  |= lo >> 31;
		if ( r >= (ft_structs_h::FT_UInt32)y )
		{
			r -= y;
			q |= 1;
		}
		lo <<= 1;
	}
	return ( s < 0 ? -(ft_structs_h::FT_Int32)q : (ft_structs_h::FT_Int32)q );
}
#else /* 0 */
FT_EXPORT_DEF( ft_structs_h::FT_Int32 )
	FT_Div64by32( ft_structs_h::FT_Int64*  x,
	ft_structs_h::FT_Int32   y )
{
	ft_structs_h::FT_Int32   s;
	ft_structs_h::FT_UInt32  q;
	s  = x->hi;
	if ( s < 0 )
	{
		x->lo = (FT_UInt32)-(FT_Int32)x->lo;
		x->hi = ~x->hi + !x->lo;
	}
	s ^= y;  y = FT_ABS( y );
	/* Shortcut */
	if ( x->hi == 0 )
	{
		if ( y > 0 )
			q = ( x->lo + ( y >> 1 ) ) / y;
		else
			q = 0x7FFFFFFFL;
		return ( s < 0 ? -(FT_Int32)q : (FT_Int32)q );
	}
	q = ft_div64by32( x->hi, x->lo, y );
	return ( s < 0 ? -(FT_Int32)q : (FT_Int32)q );
}
#endif /* 0 */
#endif /* 0 */
#endif /* FT_LONG64 */
FT_BASE_DEF( ft_structs_h::FT_Pointer )
	ft_mem_qalloc( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Long    size,
	ft_structs_h::FT_Error  *p_error )
{
	ft_structs_h::FT_Error    error = ft_error_h::FT_Err_Ok;
	ft_structs_h::FT_Pointer  block = NULL;
	if ( size > 0 )
	{
		block = memory->alloc( memory, size );
		if ( block == NULL )
			error = ft_error_h::FT_Err_Out_Of_Memory;
	}
	else if ( size < 0 )
	{
		/* may help catch/prevent security issues */
		error = ft_error_h::FT_Err_Invalid_Argument;
	}
	*p_error = error;
	return block;
};
FT_BASE_DEF( ft_structs_h::FT_Pointer )
	ft_mem_alloc( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Long    size,
	ft_structs_h::FT_Error  *p_error )
{
	ft_structs_h::FT_Error    error;
	ft_structs_h::FT_Pointer  block = ft_mem_qalloc( memory, size, &error );
	if ( !error && size > 0 )
		FT_MEM_ZERO( block, size );
	*p_error = error;
	return block;
};
FT_BASE_DEF( void )
	ft_mem_free( ft_structs_h::FT_Memory   memory,
	const void *P )
{
	if ( P )
		memory->free( memory, (void*)P );
};
FT_CALLBACK_DEF( void* )
  ft_alloc( ft_structs_h::FT_Memory  memory,
            long       size )
  {
    FT_UNUSED( memory );
#if defined( __WINDOWS__ ) && defined( __x86__ )
    return ::malloc( size );
#else
		return malloc( size );
#endif
  }
FT_CALLBACK_DEF( void* )
  ft_realloc( ft_structs_h::FT_Memory  memory,
              long       cur_size,
              long       new_size,
              void*      block )
  {
    FT_UNUSED( memory );
    FT_UNUSED( cur_size );
#if defined( __WINDOWS__ ) && defined( __x86__ )
    return ::realloc( block, new_size );
#else
		return realloc( block, new_size );
#endif
  }
FT_CALLBACK_DEF( void )
  ft_free( ft_structs_h::FT_Memory  memory,
           void*      block )
  {
    FT_UNUSED( memory );
#if defined( __WINDOWS__ ) && defined( __x86__ )
    ::free( block );
#else
		free( block );
#endif
  }

FT_BASE_DEF( ft_structs_h::FT_Memory )
  FT_New_Memory( void )
  {
    ft_structs_h::FT_Memory  memory;

#if defined( __WINDOWS__ ) && defined( __x86__ )
    memory = (ft_structs_h::FT_Memory)ft_smalloc( sizeof ( *memory ) );
#else
		memory = (ft_structs_h::FT_Memory)ft_smalloc( sizeof ( *memory ) );
#endif
    if ( memory )
    {
      memory->user    = 0;
      memory->alloc   = ft_alloc;
      memory->realloc = ft_realloc;
      memory->free    = ft_free;
#ifdef FT_DEBUG_MEMORY
      ft_mem_debug_init( memory );
#endif
    }

    return memory;
  }
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_New_Library( ft_structs_h::FT_Memory    memory,
	ft_structs_h::FT_Library  *alibrary )
{
	ft_structs_h::FT_Library  library = NULL;
	ft_structs_h::FT_Error    error;
	if ( !memory )
		return ft_error_h::FT_Err_Invalid_Argument;
#ifdef FT_DEBUG_LEVEL_ERROR
	/* init debugging support */
	ft_debug_init();
#endif
	/* first of all, allocate the library object */
	if ( FT_NEW( library ) )
		return error;
	library->memory = memory;
#ifdef FT_CONFIG_OPTION_PIC
	/* initialize position independent code containers */
	error = ft_pic_container_init( library );
	if ( error )
		goto Fail;
#endif
	/* allocate the render pool */
	library->raster_pool_size = FT_RENDER_POOL_SIZE;
#if FT_RENDER_POOL_SIZE > 0
	if ( FT_ALLOC( library->raster_pool, FT_RENDER_POOL_SIZE ) )
		goto Fail;
#endif
	library->version_major = FREETYPE_MAJOR;
	library->version_minor = FREETYPE_MINOR;
	library->version_patch = FREETYPE_PATCH;
	library->refcount = 1;
	/* That's ok now */
	*alibrary = library;
	return ft_error_h::FT_Err_Ok;
Fail:
#ifdef FT_CONFIG_OPTION_PIC
	ft_pic_container_destroy( library );
#endif
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( library );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	return error;
};
FT_EXPORT_DEF( void )
	FT_List_Add( ft_structs_h::FT_List      list,
	ft_structs_h::FT_ListNode  node )
{
	ft_structs_h::FT_ListNode  before = list->tail;
	node->next = 0;
	node->prev = before;
	if ( before )
		before->next = node;
	else
		list->head = node;
	list->tail = node;
};
FT_BASE_DEF( ft_structs_h::FT_Renderer )
	FT_Lookup_Renderer( ft_structs_h::FT_Library       library,
	ft_structs_h::FT_Glyph_Format  format,
	ft_structs_h::FT_ListNode*     node )
{
	ft_structs_h::FT_ListNode  cur;
	ft_structs_h::FT_Renderer  result = 0;
	if ( !library )
		goto Exit;
	cur = library->renderers.head;
	if ( node )
	{
		if ( *node )
			cur = (*node)->next;
		*node = 0;
	}
	while ( cur )
	{
		ft_structs_h::FT_Renderer  renderer = FT_RENDERER( cur->data );
		if ( renderer->glyph_format == format )
		{
			if ( node )
				*node = cur;
			result = renderer;
			break;
		}
		cur = cur->next;
	}
Exit:
	return result;
};
static void
	ft_set_current_renderer( ft_structs_h::FT_Library  library )
{
	ft_structs_h::FT_Renderer  renderer;
	renderer = FT_Lookup_Renderer( library, ft_structs_h::FT_GLYPH_FORMAT_OUTLINE, 0 );
	library->cur_renderer = renderer;
};
static ft_structs_h::FT_Error
	ft_add_renderer( ft_structs_h::FT_Module  module )
{
	ft_structs_h::FT_Library   library = module->library;
	ft_structs_h::FT_Memory    memory  = library->memory;
	ft_structs_h::FT_Error     error;
	ft_structs_h::FT_ListNode  node    = NULL;
	if ( FT_NEW( node ) )
		goto Exit;
	{
		ft_structs_h::FT_Renderer         render = FT_RENDERER( module );
		ft_structs_h::FT_Renderer_Class*  clazz  = (ft_structs_h::FT_Renderer_Class*)module->clazz;
		render->clazz        = clazz;
		render->glyph_format = clazz->glyph_format;
		/* allocate raster object if needed */
		if ( clazz->glyph_format == ft_structs_h::FT_GLYPH_FORMAT_OUTLINE &&
			clazz->raster_class->raster_new                )
		{
			error = clazz->raster_class->raster_new( memory, &render->raster );
			if ( error )
				goto Fail;
			render->raster_render = clazz->raster_class->raster_render;
			render->render        = clazz->render_glyph;
		}
		/* add to list */
		node->data = module;
		FT_List_Add( &library->renderers, node );
		ft_set_current_renderer( library );
	}
Fail:
	if ( error )
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( node );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
Exit:
	return error;
};
FT_EXPORT_DEF( ft_structs_h::FT_ListNode )
	FT_List_Find( ft_structs_h::FT_List  list,
	void*    data )
{
	ft_structs_h::FT_ListNode  cur;
	cur = list->head;
	while ( cur )
	{
		if ( cur->data == data )
			return cur;
		cur = cur->next;
	}
	return (ft_structs_h::FT_ListNode)0;
};
FT_EXPORT_DEF( void )
	FT_List_Finalize( ft_structs_h::FT_List             list,
	ft_structs_h::FT_List_Destructor  destroy,
	ft_structs_h::FT_Memory           memory,
	void*               user )
{
	ft_structs_h::FT_ListNode  cur;
	cur = list->head;
	while ( cur )
	{
		ft_structs_h::FT_ListNode  next = cur->next;
		void*        data = cur->data;
		if ( destroy )
			destroy( memory, data, user );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( cur );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		cur = next;
	}
	list->head = 0;
	list->tail = 0;
};
FT_EXPORT_DEF( void )
	FT_List_Remove( ft_structs_h::FT_List      list,
	ft_structs_h::FT_ListNode  node )
{
	ft_structs_h::FT_ListNode  before, after;
	before = node->prev;
	after  = node->next;
	if ( before )
		before->next = after;
	else
		list->head = after;
	if ( after )
		after->prev = before;
	else
		list->tail = before;
};
static void
	ft_remove_renderer( ft_structs_h::FT_Module  module )
{
	ft_structs_h::FT_Library   library = module->library;
	ft_structs_h::FT_Memory    memory  = library->memory;
	ft_structs_h::FT_ListNode  node;
	node = FT_List_Find( &library->renderers, module );
	if ( node )
	{
		ft_structs_h::FT_Renderer  render = FT_RENDERER( module );
		/* release raster object, if any */
		if ( render->clazz->glyph_format == ft_structs_h::FT_GLYPH_FORMAT_OUTLINE &&
			render->raster                                         )
			render->clazz->raster_class->raster_done( render->raster );
		/* remove from list */
		FT_List_Remove( &library->renderers, node );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( node );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		ft_set_current_renderer( library );
	}
};
FT_BASE_DEF( void )
	ft_glyphslot_free_bitmap( ft_structs_h::FT_GlyphSlot  slot )
{
	if ( slot->internal && ( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
	{
		ft_structs_h::FT_Memory  memory = FT_FACE_MEMORY( slot->face );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( slot->bitmap.buffer );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
	}
	else
	{
		/* assume that the bitmap buffer was stolen or not */
		/* allocated from the heap                         */
		slot->bitmap.buffer = NULL;
	}
};
FT_BASE_DEF( void )
	FT_GlyphLoader_Rewind( ft_structs_h::FT_GlyphLoader  loader )
{
	ft_structs_h::FT_GlyphLoad  base    = &loader->base;
	ft_structs_h::FT_GlyphLoad  current = &loader->current;
	base->outline.n_points   = 0;
	base->outline.n_contours = 0;
	base->num_subglyphs      = 0;
	*current = *base;
};
FT_BASE_DEF( void )
	FT_GlyphLoader_Reset( ft_structs_h::FT_GlyphLoader  loader )
{
	ft_structs_h::FT_Memory memory = loader->memory;
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( loader->base.outline.points );
	FT_FREE( loader->base.outline.tags );
	FT_FREE( loader->base.outline.contours );
	FT_FREE( loader->base.extra_points );
	FT_FREE( loader->base.subglyphs );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	loader->base.extra_points2 = NULL;
	loader->max_points    = 0;
	loader->max_contours  = 0;
	loader->max_subglyphs = 0;
	FT_GlyphLoader_Rewind( loader );
};
FT_BASE_DEF( void )
	FT_GlyphLoader_Done( ft_structs_h::FT_GlyphLoader  loader )
{
	if ( loader )
	{
		ft_structs_h::FT_Memory memory = loader->memory;
		FT_GlyphLoader_Reset( loader );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( loader );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	}
};
static void
	ft_glyphslot_done( ft_structs_h::FT_GlyphSlot  slot )
{
	ft_structs_h::FT_Driver        driver = slot->face->driver;
	ft_structs_h::FT_Driver_Class  clazz  = driver->clazz;
	ft_structs_h::FT_Memory        memory = driver->root.memory;
	if ( clazz->done_slot )
		clazz->done_slot( slot );
	/* free bitmap buffer if needed */
	ft_glyphslot_free_bitmap( slot );
	/* slot->internal might be NULL in out-of-memory situations */
	if ( slot->internal )
	{
		/* free glyph loader */
		if ( FT_DRIVER_USES_OUTLINES( driver ) )
		{
			FT_GlyphLoader_Done( slot->internal->loader );
			slot->internal->loader = 0;
		}
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( slot->internal );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	}
};
FT_BASE_DEF( void )
	FT_Stream_Close( ft_structs_h::FT_Stream  stream )
{
	if ( stream && stream->close )
		stream->close( stream );
};
FT_BASE_DEF( void )
	FT_Stream_Free( ft_structs_h::FT_Stream  stream,
	ft_structs_h::FT_Int     external )
{
	if ( stream )
	{
		ft_structs_h::FT_Memory  memory = stream->memory;
		FT_Stream_Close( stream );
		if ( !external )
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
			FT_FREE( stream );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	}
};
FT_BASE_DEF( void )
	FT_Done_GlyphSlot( ft_structs_h::FT_GlyphSlot  slot )
{
	if ( slot )
	{
		ft_structs_h::FT_Driver     driver = slot->face->driver;
		ft_structs_h::FT_Memory     memory = driver->root.memory;
		ft_structs_h::FT_GlyphSlot  prev;
		ft_structs_h::FT_GlyphSlot  cur;
		/* Remove slot from its parent face's list */
		prev = NULL;
		cur  = slot->face->glyph;
		while ( cur )
		{
			if ( cur == slot )
			{
				if ( !prev )
					slot->face->glyph = cur->next;
				else
					prev->next = cur->next;
				/* finalize client-specific data */
				if ( slot->generic.finalizer )
					slot->generic.finalizer( slot );
				ft_glyphslot_done( slot );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
				FT_FREE( slot );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
				break;
			}
			prev = cur;
			cur  = cur->next;
		}
	}
};
static void
	destroy_size( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Size    size,
	ft_structs_h::FT_Driver  driver )
{
	/* finalize client-specific data */
	if ( size->generic.finalizer )
		size->generic.finalizer( size );
	/* finalize format-specific stuff */
	if ( driver->clazz->done_size )
		driver->clazz->done_size( size );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( size->internal );
	FT_FREE( size );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
};
static void
	ft_cmap_done_internal( ft_structs_h::FT_CMap  cmap )
{
	ft_structs_h::FT_CMap_Class  clazz  = cmap->clazz;
	ft_structs_h::FT_Face        face   = cmap->charmap.face;
	ft_structs_h::FT_Memory      memory = FT_FACE_MEMORY(face);
	if ( clazz->done )
		clazz->done( cmap );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( cmap );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
};
static void
	destroy_charmaps( ft_structs_h::FT_Face    face,
	ft_structs_h::FT_Memory  memory )
{
	ft_structs_h::FT_Int  n;
	if ( !face )
		return;
	for ( n = 0; n < face->num_charmaps; n++ )
	{
		ft_structs_h::FT_CMap  cmap = FT_CMAP( face->charmaps[n] );
		ft_cmap_done_internal( cmap );
		face->charmaps[n] = NULL;
	}
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( face->charmaps );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	face->num_charmaps = 0;
};
static void
	destroy_face( ft_structs_h::FT_Memory  memory,
	ft_structs_h::FT_Face    face,
	ft_structs_h::FT_Driver  driver )
{
	ft_structs_h::FT_Driver_Class  clazz = driver->clazz;
	/* discard auto-hinting data */
	if ( face->autohint.finalizer )
		face->autohint.finalizer( face->autohint.data );
	/* Discard glyph slots for this face.                           */
	/* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
	while ( face->glyph )
		FT_Done_GlyphSlot( face->glyph );
	/* discard all sizes for this face */
	FT_List_Finalize( &face->sizes_list,
		(ft_structs_h::FT_List_Destructor)destroy_size,
		memory,
		driver );
	face->size = 0;
	/* now discard client data */
	if ( face->generic.finalizer )
		face->generic.finalizer( face );
	/* discard charmaps */
	destroy_charmaps( face, memory );
	/* finalize format-specific stuff */
	if ( clazz->done_face )
		clazz->done_face( face );
	/* close the stream for this face if needed */
	FT_Stream_Free(
		face->stream,
		( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0 );
	face->stream = 0;
	/* get rid of it */
	if ( face->internal )
	{
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( face->internal );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	}
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( face );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
};
static void
	Destroy_Driver( ft_structs_h::FT_Driver  driver )
{
	FT_List_Finalize( &driver->faces_list,
		(ft_structs_h::FT_List_Destructor)destroy_face,
		driver->root.memory,
		driver );
	/* check whether we need to drop the driver's glyph loader */
	if ( FT_DRIVER_USES_OUTLINES( driver ) )
		FT_GlyphLoader_Done( driver->glyph_loader );
};
static void
	Destroy_Module( ft_structs_h::FT_Module  module )
{
	ft_structs_h::FT_Memory         memory  = module->memory;
	ft_structs_h::FT_Module_Class*  clazz   = module->clazz;
	ft_structs_h::FT_Library        library = module->library;
	if ( library && library->auto_hinter == module )
		library->auto_hinter = 0;
	/* if the module is a renderer */
	if ( FT_MODULE_IS_RENDERER( module ) )
		ft_remove_renderer( module );
	/* if the module is a font driver, add some steps */
	if ( FT_MODULE_IS_DRIVER( module ) )
		Destroy_Driver( FT_DRIVER( module ) );
	/* finalize the module object */
	if ( clazz->module_done )
		clazz->module_done( module );
	/* discard it */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( module );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
};
FT_BASE_DEF( void )
	FT_Done_Memory( ft_structs_h::FT_Memory  memory )
{
#ifdef FT_DEBUG_MEMORY
	ft_mem_debug_done( memory );
#endif
	ft_sfree( memory );
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Remove_Module( ft_structs_h::FT_Library  library,
	ft_structs_h::FT_Module   module )
{
	/* try to find the module from the table, then remove it from there */
	if ( !library )
		return ft_error_h::FT_Err_Invalid_Library_Handle;
	if ( module )
	{
		ft_structs_h::FT_Module*  cur   = library->modules;
		ft_structs_h::FT_Module*  limit = cur + library->num_modules;
		for ( ; cur < limit; cur++ )
		{
			if ( cur[0] == module )
			{
				/* remove it from the table */
				library->num_modules--;
				limit--;
				while ( cur < limit )
				{
					cur[0] = cur[1];
					cur++;
				}
				limit[0] = 0;
				/* destroy the module */
				Destroy_Module( module );
				return ft_error_h::FT_Err_Ok;
			}
		}
	}
	return ft_error_h::FT_Err_Invalid_Driver_Handle;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_GlyphLoader_New( ft_structs_h::FT_Memory        memory,
	ft_structs_h::FT_GlyphLoader  *aloader )
{
	ft_structs_h::FT_GlyphLoader  loader = NULL;
	ft_structs_h::FT_Error        error;
	if ( !FT_NEW( loader ) )
	{
		loader->memory = memory;
		*aloader       = loader;
	}
	return error;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Add_Module( ft_structs_h::FT_Library              library,
	const ft_structs_h::FT_Module_Class*  clazz )
{
	ft_structs_h::FT_Error   error;
	ft_structs_h::FT_Memory  memory;
	ft_structs_h::FT_Module  module = NULL;
	ft_structs_h::FT_UInt    nn;
#define FREETYPE_VER_FIXED  ( ( (ft_structs_h::FT_Long)FREETYPE_MAJOR << 16 ) | \
	FREETYPE_MINOR                  )
	if ( !library )
		return ft_error_h::FT_Err_Invalid_Library_Handle;
	if ( !clazz )
		return ft_error_h::FT_Err_Invalid_Argument;
	/* check freetype version */
	if ( clazz->module_requires > FREETYPE_VER_FIXED )
		return ft_error_h::FT_Err_Invalid_Version;
	/* look for a module with the same name in the library's table */
	for ( nn = 0; nn < library->num_modules; nn++ )
	{
		module = library->modules[nn];
		if ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
		{
			/* this installed module has the same name, compare their versions */
			if ( clazz->module_version <= module->clazz->module_version )
				return ft_error_h::FT_Err_Lower_Module_Version;
			/* remove the module from our list, then exit the loop to replace */
			/* it by our new version..                                        */
			FT_Remove_Module( library, module );
			break;
		}
	}
	memory = library->memory;
	error  = ft_error_h::FT_Err_Ok;
	if ( library->num_modules >= FT_MAX_MODULES )
	{
		error = ft_error_h::FT_Err_Too_Many_Drivers;
		goto Exit;
	}
	/* allocate module object */
	if ( FT_ALLOC( module, clazz->module_size ) )
		goto Exit;
	/* base initialization */
	module->library = library;
	module->memory  = memory;
	module->clazz   = (ft_structs_h::FT_Module_Class*)clazz;
	/* check whether the module is a renderer - this must be performed */
	/* before the normal module initialization                         */
	if ( FT_MODULE_IS_RENDERER( module ) )
	{
		/* add to the renderers list */
		error = ft_add_renderer( module );
		if ( error )
			goto Fail;
	}
	/* is the module a auto-hinter? */
	if ( FT_MODULE_IS_HINTER( module ) )
		library->auto_hinter = module;
	/* if the module is a font driver */
	if ( FT_MODULE_IS_DRIVER( module ) )
	{
		/* allocate glyph loader if needed */
		ft_structs_h::FT_Driver  driver = FT_DRIVER( module );
		driver->clazz = (ft_structs_h::FT_Driver_Class)module->clazz;
		if ( FT_DRIVER_USES_OUTLINES( driver ) )
		{
			error = FT_GlyphLoader_New( memory, &driver->glyph_loader );
			if ( error )
				goto Fail;
		}
	}
	if ( clazz->module_init )
	{
		error = clazz->module_init( module );
		if ( error )
			goto Fail;
	}
	/* add module to the library's table */
	library->modules[library->num_modules++] = module;
Exit:
	return error;
Fail:
	if ( FT_MODULE_IS_DRIVER( module ) )
	{
		ft_structs_h::FT_Driver  driver = FT_DRIVER( module );
		if ( FT_DRIVER_USES_OUTLINES( driver ) )
			FT_GlyphLoader_Done( driver->glyph_loader );
	}
	if ( FT_MODULE_IS_RENDERER( module ) )
	{
		ft_structs_h::FT_Renderer  renderer = FT_RENDERER( module );
		if ( renderer->clazz->glyph_format == ft_structs_h::FT_GLYPH_FORMAT_OUTLINE &&
			renderer->raster                                         )
			renderer->clazz->raster_class->raster_done( renderer->raster );
	}
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( module );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	goto Exit;
};
FT_EXPORT_DEF( void )
	FT_Add_Default_Modules( ft_structs_h::FT_Library  library )
{
	ft_structs_h::FT_Error                       error;
	const ft_structs_h::FT_Module_Class* const*  cur;
	/* FT_DEFAULT_MODULES_GET dereferences `library' in PIC mode */
#ifdef FT_CONFIG_OPTION_PIC
	if ( !library )
		return;
#endif
	/* GCC 4.6 warns the type difference:
	*   FT_Module_Class** != const FT_Module_Class* const*
	*/
	cur = (const ft_structs_h::FT_Module_Class* const*)FT_DEFAULT_MODULES_GET;
	/* test for valid `library' delayed to FT_Add_Module() */
	while ( *cur )
	{
		error = FT_Add_Module( library, *cur );
		/* notify errors, but don't stop */
		if ( error )
			FT_TRACE0(( "FT_Add_Default_Module:"
			" Cannot install `%s', error = 0x%x\n",
			(*cur)->module_name, error ));
		cur++;
	}
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Init_FreeType( ft_structs_h::FT_Library  *alibrary )
{
	ft_structs_h::FT_Error   error;
	ft_structs_h::FT_Memory  memory;
	/* First of all, allocate a new system object -- this function is part */
	/* of the system-specific component, i.e. `ftsystem.c'.                */
	memory = FT_New_Memory();
	if ( !memory )
	{
		FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
		return ft_error_h::FT_Err_Unimplemented_Feature;
	}
	/* build a library out of it, then fill it with the set of */
	/* default drivers.                                        */
	error = FT_New_Library( memory, alibrary );
	if ( error )
		FT_Done_Memory( memory );
	else
		FT_Add_Default_Modules( *alibrary );
	return error;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Done_FreeType( ft_structs_h::FT_Library  library )
{
	if ( library )
	{
		ft_structs_h::FT_Memory  memory = library->memory;
		/* Discard the library object */
		FT_Done_Library( library );
		/* discard memory manager */
		FT_Done_Memory( memory );
	}    
	return ft_error_h::FT_Err_Ok;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Done_Library( ft_structs_h::FT_Library  library )
{
	ft_structs_h::FT_Memory  memory;
	if ( !library )
		return ft_error_h::FT_Err_Invalid_Library_Handle;
	library->refcount--;
	if ( library->refcount > 0 )
		goto Exit;
	memory = library->memory;
	/*
	* Close all faces in the library.  If we don't do this, we can have
	* some subtle memory leaks.
	*
	* Example:
	*
	*  - the cff font driver uses the pshinter module in cff_size_done
	*  - if the pshinter module is destroyed before the cff font driver,
	*    opened FT_Face objects managed by the driver are not properly
	*    destroyed, resulting in a memory leak
	*
	* Some faces are dependent on other faces, like Type42 faces that
	* depend on TrueType faces synthesized internally.
	*
	* The order of drivers should be specified in driver_name[].
	*/
	{
		ft_structs_h::FT_UInt      m, n;
		const char*  driver_name[] = { "type42", NULL };
		for ( m = 0;
			m < sizeof ( driver_name ) / sizeof ( driver_name[0] );
			m++ )
		{
			for ( n = 0; n < library->num_modules; n++ )
			{
				ft_structs_h::FT_Module    module      = library->modules[n];
				const char*  module_name = module->clazz->module_name;
				ft_structs_h::FT_List      faces;
				if ( driver_name[m]                                &&
					ft_strcmp( module_name, driver_name[m] ) != 0 )
					continue;
				if ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
					continue;
				FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));
				faces = &FT_DRIVER( module )->faces_list;
				while ( faces->head )
				{
					FT_Done_Face( FT_FACE( faces->head->data ) );
					if ( faces->head )
						FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
				}
			}
		}
	}
	/* Close all other modules in the library */
#if 1
	/* XXX Modules are removed in the reversed order so that  */
	/* type42 module is removed before truetype module.  This */
	/* avoids double free in some occasions.  It is a hack.   */
	while ( library->num_modules > 0 )
		FT_Remove_Module( library,
		library->modules[library->num_modules - 1] );
#else
	{
		FT_UInt  n;
		for ( n = 0; n < library->num_modules; n++ )
		{
			FT_Module  module = library->modules[n];
			if ( module )
			{
				Destroy_Module( module );
				library->modules[n] = 0;
			}
		}
	}
#endif
	/* Destroy raster objects */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( library->raster_pool );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	library->raster_pool_size = 0;
#ifdef FT_CONFIG_OPTION_PIC
	/* Destroy pic container contents */
	ft_pic_container_destroy( library );
#endif
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
	FT_FREE( library );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
Exit:
	return ft_error_h::FT_Err_Ok;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Done_Face( ft_structs_h::FT_Face  face )
{
	ft_structs_h::FT_Error     error;
	ft_structs_h::FT_Driver    driver;
	ft_structs_h::FT_Memory    memory;
	ft_structs_h::FT_ListNode  node;
	error = ft_error_h::FT_Err_Invalid_Face_Handle;
	if ( face && face->driver )
	{
		face->internal->refcount--;
		if ( face->internal->refcount > 0 )
			error = ft_error_h::FT_Err_Ok;
		else
		{
			driver = face->driver;
			memory = driver->root.memory;
			/* find face in driver's list */
			node = FT_List_Find( &driver->faces_list, face );
			if ( node )
			{
				/* remove face object from the driver's list */
				FT_List_Remove( &driver->faces_list, node );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
				FT_FREE( node );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
				/* now destroy the object proper */
				destroy_face( memory, face, driver );
				error = ft_error_h::FT_Err_Ok;
			}
		}
	}
	return error;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Set_Pixel_Sizes( ft_structs_h::FT_Face  face,
	ft_structs_h::FT_UInt  pixel_width,
	ft_structs_h::FT_UInt  pixel_height )
{
	ft_structs_h::FT_Size_RequestRec  req;
	if ( pixel_width == 0 )
		pixel_width = pixel_height;
	else if ( pixel_height == 0 )
		pixel_height = pixel_width;
	if ( pixel_width  < 1 )
		pixel_width  = 1;
	if ( pixel_height < 1 )
		pixel_height = 1;
	/* use `>=' to avoid potential compiler warning on 16bit platforms */
	if ( pixel_width  >= 0xFFFFU )
		pixel_width  = 0xFFFFU;
	if ( pixel_height >= 0xFFFFU )
		pixel_height = 0xFFFFU;
	req.type           = ft_structs_h::FT_SIZE_REQUEST_TYPE_NOMINAL;
	req.width          = pixel_width << 6;
	req.height         = pixel_height << 6;
	req.horiResolution = 0;
	req.vertResolution = 0;
	return FT_Request_Size( face, &req );
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Request_Size( ft_structs_h::FT_Face          face,
	ft_structs_h::FT_Size_Request  req )
{
	ft_structs_h::FT_Driver_Class  clazz;
	ft_structs_h::FT_ULong         strike_index;
	if ( !face )
		return ft_error_h::FT_Err_Invalid_Face_Handle;
	if ( !req || req->width < 0 || req->height < 0 ||
		req->type >= ft_structs_h::FT_SIZE_REQUEST_TYPE_MAX )
		return ft_error_h::FT_Err_Invalid_Argument;
	clazz = face->driver->clazz;
	if ( clazz->request_size )
	{
		ft_structs_h::FT_Error  error;
		error = clazz->request_size( face->size, req );
#ifdef FT_DEBUG_LEVEL_TRACE
		{
			FT_Size_Metrics*  metrics = &face->size->metrics;
			FT_TRACE5(( "FT_Request_Size (font driver's `request_size'):\n" ));
			FT_TRACE5(( "  x scale: %d (%f)\n",
				metrics->x_scale, metrics->x_scale / 65536.0 ));
			FT_TRACE5(( "  y scale: %d (%f)\n",
				metrics->y_scale, metrics->y_scale / 65536.0 ));
			FT_TRACE5(( "  ascender: %f\n",    metrics->ascender / 64.0 ));
			FT_TRACE5(( "  descender: %f\n",   metrics->descender / 64.0 ));
			FT_TRACE5(( "  height: %f\n",      metrics->height / 64.0 ));
			FT_TRACE5(( "  max advance: %f\n", metrics->max_advance / 64.0 ));
			FT_TRACE5(( "  x ppem: %d\n",      metrics->x_ppem ));
			FT_TRACE5(( "  y ppem: %d\n",      metrics->y_ppem ));
		}
#endif
		return error;
	}
	/*
	* The reason that a driver doesn't have `request_size' defined is
	* either that the scaling here suffices or that the supported formats
	* are bitmap-only and size matching is not implemented.
	*
	* In the latter case, a simple size matching is done.
	*/
	if ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
	{
		ft_structs_h::FT_Error  error;
		error = FT_Match_Size( face, req, 0, &strike_index );
		if ( error )
			return error;
		FT_TRACE3(( "FT_Request_Size: bitmap strike %lu matched\n",
			strike_index ));
		return FT_Select_Size( face, (ft_structs_h::FT_Int)strike_index );
	}
	FT_Request_Metrics( face, req );
	return ft_error_h::FT_Err_Ok;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Match_Size( ft_structs_h::FT_Face          face,
	ft_structs_h::FT_Size_Request  req,
	ft_structs_h::FT_Bool          ignore_width,
	ft_structs_h::FT_ULong*        size_index )
{
	ft_structs_h::FT_Int   i;
	ft_structs_h::FT_Long  w, h;
	if ( !FT_HAS_FIXED_SIZES( face ) )
		return ft_error_h::FT_Err_Invalid_Face_Handle;
	/* FT_Bitmap_Size doesn't provide enough info... */
	if ( req->type != ft_structs_h::FT_SIZE_REQUEST_TYPE_NOMINAL )
		return ft_error_h::FT_Err_Unimplemented_Feature;
	w = FT_REQUEST_WIDTH ( req );
	h = FT_REQUEST_HEIGHT( req );
	if ( req->width && !req->height )
		h = w;
	else if ( !req->width && req->height )
		w = h;
	w = FT_PIX_ROUND( w );
	h = FT_PIX_ROUND( h );
	for ( i = 0; i < face->num_fixed_sizes; i++ )
	{
		ft_structs_h::FT_Bitmap_Size*  bsize = face->available_sizes + i;
		if ( h != FT_PIX_ROUND( bsize->y_ppem ) )
			continue;
		if ( w == FT_PIX_ROUND( bsize->x_ppem ) || ignore_width )
		{
			if ( size_index )
				*size_index = (ft_structs_h::FT_ULong)i;
			return ft_error_h::FT_Err_Ok;
		}
	}
	return ft_error_h::FT_Err_Invalid_Pixel_Size;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Select_Size( ft_structs_h::FT_Face  face,
	ft_structs_h::FT_Int   strike_index )
{
	ft_structs_h::FT_Driver_Class  clazz;
	if ( !face || !FT_HAS_FIXED_SIZES( face ) )
		return ft_error_h::FT_Err_Invalid_Face_Handle;
	if ( strike_index < 0 || strike_index >= face->num_fixed_sizes )
		return ft_error_h::FT_Err_Invalid_Argument;
	clazz = face->driver->clazz;
	if ( clazz->select_size )
	{
		ft_structs_h::FT_Error  error;
		error = clazz->select_size( face->size, (ft_structs_h::FT_ULong)strike_index );
#ifdef FT_DEBUG_LEVEL_TRACE
		{
			ft_structs_h::FT_Size_Metrics*  metrics = &face->size->metrics;
			FT_TRACE5(( "FT_Select_Size (font driver's `select_size'):\n" ));
			FT_TRACE5(( "  x scale: %d (%f)\n",
				metrics->x_scale, metrics->x_scale / 65536.0 ));
			FT_TRACE5(( "  y scale: %d (%f)\n",
				metrics->y_scale, metrics->y_scale / 65536.0 ));
			FT_TRACE5(( "  ascender: %f\n",    metrics->ascender / 64.0 ));
			FT_TRACE5(( "  descender: %f\n",   metrics->descender / 64.0 ));
			FT_TRACE5(( "  height: %f\n",      metrics->height / 64.0 ));
			FT_TRACE5(( "  max advance: %f\n", metrics->max_advance / 64.0 ));
			FT_TRACE5(( "  x ppem: %d\n",      metrics->x_ppem ));
			FT_TRACE5(( "  y ppem: %d\n",      metrics->y_ppem ));
		}
#endif
		return error;
	}
	FT_Select_Metrics( face, (ft_structs_h::FT_ULong)strike_index );
	return ft_error_h::FT_Err_Ok;
};
static void
	ft_recompute_scaled_metrics( ft_structs_h::FT_Face           face,
	ft_structs_h::FT_Size_Metrics*  metrics )
{
	/* Compute root ascender, descender, test height, and max_advance */
#ifdef GRID_FIT_METRICS
	metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
		metrics->y_scale ) );
	metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
		metrics->y_scale ) );
	metrics->height      = FT_PIX_ROUND( FT_MulFix( face->height,
		metrics->y_scale ) );
	metrics->max_advance = FT_PIX_ROUND( FT_MulFix( face->max_advance_width,
		metrics->x_scale ) );
#else /* !GRID_FIT_METRICS */
	metrics->ascender    = FT_MulFix( face->ascender,
		metrics->y_scale );
	metrics->descender   = FT_MulFix( face->descender,
		metrics->y_scale );
	metrics->height      = FT_MulFix( face->height,
		metrics->y_scale );
	metrics->max_advance = FT_MulFix( face->max_advance_width,
		metrics->x_scale );
#endif /* !GRID_FIT_METRICS */
};
FT_BASE_DEF( void )
	FT_Select_Metrics( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_ULong  strike_index )
{
	ft_structs_h::FT_Size_Metrics*  metrics;
	ft_structs_h::FT_Bitmap_Size*   bsize;
	metrics = &face->size->metrics;
	bsize   = face->available_sizes + strike_index;
	metrics->x_ppem = (ft_structs_h::FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
	metrics->y_ppem = (ft_structs_h::FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );
	if ( FT_IS_SCALABLE( face ) )
	{
		metrics->x_scale = FT_DivFix( bsize->x_ppem,
			face->units_per_EM );
		metrics->y_scale = FT_DivFix( bsize->y_ppem,
			face->units_per_EM );
		ft_recompute_scaled_metrics( face, metrics );
	}
	else
	{
		metrics->x_scale     = 1L << 16;
		metrics->y_scale     = 1L << 16;
		metrics->ascender    = bsize->y_ppem;
		metrics->descender   = 0;
		metrics->height      = bsize->height << 6;
		metrics->max_advance = bsize->x_ppem;
	}
	FT_TRACE5(( "FT_Select_Metrics:\n" ));
	FT_TRACE5(( "  x scale: %d (%f)\n",
		metrics->x_scale, metrics->x_scale / 65536.0 ));
	FT_TRACE5(( "  y scale: %d (%f)\n",
		metrics->y_scale, metrics->y_scale / 65536.0 ));
	FT_TRACE5(( "  ascender: %f\n",    metrics->ascender / 64.0 ));
	FT_TRACE5(( "  descender: %f\n",   metrics->descender / 64.0 ));
	FT_TRACE5(( "  height: %f\n",      metrics->height / 64.0 ));
	FT_TRACE5(( "  max advance: %f\n", metrics->max_advance / 64.0 ));
	FT_TRACE5(( "  x ppem: %d\n",      metrics->x_ppem ));
	FT_TRACE5(( "  y ppem: %d\n",      metrics->y_ppem ));
};
FT_BASE_DEF( void )
	FT_Request_Metrics( ft_structs_h::FT_Face          face,
	ft_structs_h::FT_Size_Request  req )
{
	ft_structs_h::FT_Size_Metrics*  metrics;
	metrics = &face->size->metrics;
	if ( FT_IS_SCALABLE( face ) )
	{
		ft_structs_h::FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;
		switch ( req->type )
		{
		case ft_structs_h::FT_SIZE_REQUEST_TYPE_NOMINAL:
			w = h = face->units_per_EM;
			break;
		case ft_structs_h::FT_SIZE_REQUEST_TYPE_REAL_DIM:
			w = h = face->ascender - face->descender;
			break;
		case ft_structs_h::FT_SIZE_REQUEST_TYPE_BBOX:
			w = face->bbox.xMax - face->bbox.xMin;
			h = face->bbox.yMax - face->bbox.yMin;
			break;
		case ft_structs_h::FT_SIZE_REQUEST_TYPE_CELL:
			w = face->max_advance_width;
			h = face->ascender - face->descender;
			break;
		case ft_structs_h::FT_SIZE_REQUEST_TYPE_SCALES:
			metrics->x_scale = (ft_structs_h::FT_Fixed)req->width;
			metrics->y_scale = (ft_structs_h::FT_Fixed)req->height;
			if ( !metrics->x_scale )
				metrics->x_scale = metrics->y_scale;
			else if ( !metrics->y_scale )
				metrics->y_scale = metrics->x_scale;
			goto Calculate_Ppem;
		case ft_structs_h::FT_SIZE_REQUEST_TYPE_MAX:
			break;
		}
		/* to be on the safe side */
		if ( w < 0 )
			w = -w;
		if ( h < 0 )
			h = -h;
		scaled_w = FT_REQUEST_WIDTH ( req );
		scaled_h = FT_REQUEST_HEIGHT( req );
		/* determine scales */
		if ( req->width )
		{
			metrics->x_scale = FT_DivFix( scaled_w, w );
			if ( req->height )
			{
				metrics->y_scale = FT_DivFix( scaled_h, h );
				if ( req->type == ft_structs_h::FT_SIZE_REQUEST_TYPE_CELL )
				{
					if ( metrics->y_scale > metrics->x_scale )
						metrics->y_scale = metrics->x_scale;
					else
						metrics->x_scale = metrics->y_scale;
				}
			}
			else
			{
				metrics->y_scale = metrics->x_scale;
				scaled_h = FT_MulDiv( scaled_w, h, w );
			}
		}
		else
		{
			metrics->x_scale = metrics->y_scale = FT_DivFix( scaled_h, h );
			scaled_w = FT_MulDiv( scaled_h, w, h );
		}
Calculate_Ppem:
		/* calculate the ppems */
		if ( req->type != ft_structs_h::FT_SIZE_REQUEST_TYPE_NOMINAL )
		{
			scaled_w = FT_MulFix( face->units_per_EM, metrics->x_scale );
			scaled_h = FT_MulFix( face->units_per_EM, metrics->y_scale );
		}
		metrics->x_ppem = (ft_structs_h::FT_UShort)( ( scaled_w + 32 ) >> 6 );
		metrics->y_ppem = (ft_structs_h::FT_UShort)( ( scaled_h + 32 ) >> 6 );
		ft_recompute_scaled_metrics( face, metrics );
	}
	else
	{
		FT_ZERO( metrics );
		metrics->x_scale = 1L << 16;
		metrics->y_scale = 1L << 16;
	}
	FT_TRACE5(( "FT_Request_Metrics:\n" ));
	FT_TRACE5(( "  x scale: %d (%f)\n",
		metrics->x_scale, metrics->x_scale / 65536.0 ));
	FT_TRACE5(( "  y scale: %d (%f)\n",
		metrics->y_scale, metrics->y_scale / 65536.0 ));
	FT_TRACE5(( "  ascender: %f\n",    metrics->ascender / 64.0 ));
	FT_TRACE5(( "  descender: %f\n",   metrics->descender / 64.0 ));
	FT_TRACE5(( "  height: %f\n",      metrics->height / 64.0 ));
	FT_TRACE5(( "  max advance: %f\n", metrics->max_advance / 64.0 ));
	FT_TRACE5(( "  x ppem: %d\n",      metrics->x_ppem ));
	FT_TRACE5(( "  y ppem: %d\n",      metrics->y_ppem ));
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_New_Face( ft_structs_h::FT_Library   library,
	const char*  pathname,
	ft_structs_h::FT_Long      face_index,
	ft_structs_h::FT_Face     *aface )
{
	ft_structs_h::FT_Open_Args  args;
	/* test for valid `library' and `aface' delayed to FT_Open_Face() */
	if ( !pathname )
		return ft_error_h::FT_Err_Invalid_Argument;
	args.flags    = FT_OPEN_PATHNAME;
	args.pathname = (char*)pathname;
	args.stream   = NULL;
	return FT_Open_Face( library, &args, face_index, aface );
};
static ft_structs_h::FT_Error
	open_face( ft_structs_h::FT_Driver      driver,
	ft_structs_h::FT_Stream      stream,
	ft_structs_h::FT_Long        face_index,
	ft_structs_h::FT_Int         num_params,
	ft_structs_h::FT_Parameter*  params,
	ft_structs_h::FT_Face       *aface )
{
	ft_structs_h::FT_Memory         memory;
	ft_structs_h::FT_Driver_Class   clazz;
	ft_structs_h::FT_Face           face = 0;
	ft_structs_h::FT_Error          error, error2;
	ft_structs_h::FT_Face_Internal  internal = NULL;
	clazz  = driver->clazz;
	memory = driver->root.memory;
	/* allocate the face object and perform basic initialization */
	if ( FT_ALLOC( face, clazz->face_object_size ) )
		goto Fail;
	if ( FT_NEW( internal ) )
		goto Fail;
	face->internal = internal;
	face->driver   = driver;
	face->memory   = memory;
	face->stream   = stream;
#ifdef FT_CONFIG_OPTION_INCREMENTAL
	{
		int  i;
		face->internal->incremental_interface = 0;
		for ( i = 0; i < num_params && !face->internal->incremental_interface;
			i++ )
			if ( params[i].tag == FT_PARAM_TAG_INCREMENTAL )
				face->internal->incremental_interface =
				(FT_Incremental_Interface)params[i].data;
	}
#endif
	if ( clazz->init_face )
		error = clazz->init_face( stream,
		face,
		(ft_structs_h::FT_Int)face_index,
		num_params,
		params );
	if ( error )
		goto Fail;
	/* select Unicode charmap by default */
	error2 = find_unicode_charmap( face );
	/* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
	/* is returned.                                                      */
	/* no error should happen, but we want to play safe */
	if ( error2 && error2 != ft_error_h::FT_Err_Invalid_CharMap_Handle )
	{
		error = error2;
		goto Fail;
	}
	*aface = face;
Fail:
	if ( error )
	{
		destroy_charmaps( face, memory );
		if ( clazz->done_face )
			clazz->done_face( face );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( internal );
		FT_FREE( face );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		*aface = 0;
	}
	return error;
}
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_Open_Face( ft_structs_h::FT_Library           library,
	const ft_structs_h::FT_Open_Args*  args,
	ft_structs_h::FT_Long              face_index,
	ft_structs_h::FT_Face             *aface )
{
	ft_structs_h::FT_Error     error;
	ft_structs_h::FT_Driver    driver;
	ft_structs_h::FT_Memory    memory;
	ft_structs_h::FT_Stream    stream = NULL;
	ft_structs_h::FT_Face      face   = NULL;
	ft_structs_h::FT_ListNode  node   = NULL;
	ft_structs_h::FT_Bool      external_stream;
	ft_structs_h::FT_Module*   cur;
	ft_structs_h::FT_Module*   limit;
	/* test for valid `library' delayed to */
	/* FT_Stream_New()                     */
	if ( ( !aface && face_index >= 0 ) || !args )
		return ft_error_h::FT_Err_Invalid_Argument;
	external_stream = FT_BOOL( ( args->flags & FT_OPEN_STREAM ) &&
		args->stream                     );
	/* create input stream */
	error = FT_Stream_New( library, args, &stream );
	if ( error )
		goto Fail3;
	memory = library->memory;
	/* If the font driver is specified in the `args' structure, use */
	/* it.  Otherwise, we scan the list of registered drivers.      */
	if ( ( args->flags & FT_OPEN_DRIVER ) && args->driver )
	{
		driver = FT_DRIVER( args->driver );
		/* not all modules are drivers, so check... */
		if ( FT_MODULE_IS_DRIVER( driver ) )
		{
			ft_structs_h::FT_Int         num_params = 0;
			ft_structs_h::FT_Parameter*  params     = 0;
			if ( args->flags & FT_OPEN_PARAMS )
			{
				num_params = args->num_params;
				params     = args->params;
			}
			error = open_face( driver, stream, face_index,
				num_params, params, &face );
			if ( !error )
				goto FreetypeSuccess;
		}
		else
			error = ft_error_h::FT_Err_Invalid_Handle;
		FT_Stream_Free( stream, external_stream );
		goto Fail;
	}
	else
	{
		/* check each font driver for an appropriate format */
		cur   = library->modules;
		limit = cur + library->num_modules;
		for ( ; cur < limit; cur++ )
		{
			/* not all modules are font drivers, so check... */
			if ( FT_MODULE_IS_DRIVER( cur[0] ) )
			{
				ft_structs_h::FT_Int         num_params = 0;
				ft_structs_h::FT_Parameter*  params     = 0;
				driver = FT_DRIVER( cur[0] );
				if ( args->flags & FT_OPEN_PARAMS )
				{
					num_params = args->num_params;
					params     = args->params;
				}
				error = open_face( driver, stream, face_index,
					num_params, params, &face );
				if ( !error )
					goto FreetypeSuccess;
#ifdef FT_CONFIG_OPTION_MAC_FONTS
				if ( ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
					FT_ERROR_BASE( error ) == FT_Err_Table_Missing           )
				{
					/* TrueType but essential tables are missing */
					if ( FT_Stream_Seek( stream, 0 ) )
						break;
					error = open_face_PS_from_sfnt_stream( library,
						stream,
						face_index,
						num_params,
						params,
						aface );
					if ( !error )
					{
						FT_Stream_Free( stream, external_stream );
						return error;
					}
				}
#endif
				if ( FT_ERROR_BASE( error ) != ft_error_h::FT_Err_Unknown_File_Format )
					goto Fail3;
			}
		}
Fail3:
		/* If we are on the mac, and we get an FT_Err_Invalid_Stream_Operation */
		/* it may be because we have an empty data fork, so we need to check   */
		/* the resource fork.                                                  */
		if ( FT_ERROR_BASE( error ) != ft_error_h::FT_Err_Cannot_Open_Stream       &&
			FT_ERROR_BASE( error ) != ft_error_h::FT_Err_Unknown_File_Format      &&
			FT_ERROR_BASE( error ) != ft_error_h::FT_Err_Invalid_Stream_Operation )
			goto Fail2;
#if !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
		error = load_mac_face( library, stream, face_index, aface, args );
		if ( !error )
		{
			/* We don't want to go to Success here.  We've already done that. */
			/* On the other hand, if we succeeded we still need to close this */
			/* stream (we opened a different stream which extracted the       */
      /* interesting information out of this stream here.  That stream  */
      /* will still be open and the face will point to it).             */
      FT_Stream_Free( stream, external_stream );
      return error;
    }
		if ( FT_ERROR_BASE( error ) != FT_Err_Unknown_File_Format )
			goto Fail2;
#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */
		/* no driver is able to handle this format */
		error = ft_error_h::FT_Err_Unknown_File_Format;
Fail2:
		FT_Stream_Free( stream, external_stream );
		goto Fail;
	}
FreetypeSuccess:
	FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));
	/* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
	if ( external_stream )
		face->face_flags |= FT_FACE_FLAG_EXTERNAL_STREAM;
	/* add the face object to its driver's list */
	if ( FT_NEW( node ) )
		goto Fail;
	node->data = face;
	/* don't assume driver is the same as face->driver, so use */
	/* face->driver instead.                                   */
	FT_List_Add( &face->driver->faces_list, node );
	/* now allocate a glyph slot object for the face */
	FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));
	if ( face_index >= 0 )
	{
		error = FT_New_GlyphSlot( face, NULL );
		if ( error )
			goto Fail;
		/* finally, allocate a size object for the face */
		{
			ft_structs_h::FT_Size  size;
			FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));
			error = FT_New_Size( face, &size );
			if ( error )
				goto Fail;
			face->size = size;
		}
	}
	/* some checks */
	if ( FT_IS_SCALABLE( face ) )
	{
		if ( face->height < 0 )
			face->height = (ft_structs_h::FT_Short)-face->height;
		if ( !FT_HAS_VERTICAL( face ) )
			face->max_advance_height = (ft_structs_h::FT_Short)face->height;
	}
	if ( FT_HAS_FIXED_SIZES( face ) )
	{
		ft_structs_h::FT_Int  i;
		for ( i = 0; i < face->num_fixed_sizes; i++ )
		{
			ft_structs_h::FT_Bitmap_Size*  bsize = face->available_sizes + i;
			if ( bsize->height < 0 )
				bsize->height = (ft_structs_h::FT_Short)-bsize->height;
			if ( bsize->x_ppem < 0 )
				bsize->x_ppem = (ft_structs_h::FT_Short)-bsize->x_ppem;
			if ( bsize->y_ppem < 0 )
				bsize->y_ppem = -bsize->y_ppem;
		}
	}
	/* initialize internal face data */
	{
		ft_structs_h::FT_Face_Internal  internal = face->internal;
		internal->transform_matrix.xx = 0x10000L;
		internal->transform_matrix.xy = 0;
		internal->transform_matrix.yx = 0;
		internal->transform_matrix.yy = 0x10000L;
		internal->transform_delta.x = 0;
		internal->transform_delta.y = 0;
		internal->refcount = 1;
	}
	if ( aface )
		*aface = face;
	else
		FT_Done_Face( face );
	goto Exit;
Fail:
	FT_Done_Face( face );
Exit:
	FT_TRACE4(( "FT_Open_Face: Return %d\n", error ));
	return error;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Stream_New( ft_structs_h::FT_Library           library,
	const ft_structs_h::FT_Open_Args*  args,
	ft_structs_h::FT_Stream           *astream )
{
	ft_structs_h::FT_Error   error;
	ft_structs_h::FT_Memory  memory;
	ft_structs_h::FT_Stream  stream = NULL;
	*astream = 0;
	if ( !library )
		return ft_error_h::FT_Err_Invalid_Library_Handle;
	if ( !args )
		return ft_error_h::FT_Err_Invalid_Argument;
	memory = library->memory;
	if ( FT_NEW( stream ) )
		goto Exit;
	stream->memory = memory;
	if ( args->flags & FT_OPEN_MEMORY )
	{
		/* create a memory-based stream */
		FT_Stream_OpenMemory( stream,
			(const ft_structs_h::FT_Byte*)args->memory_base,
			args->memory_size );
	}
#ifndef FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT
	else if ( args->flags & FT_OPEN_PATHNAME )
	{
		/* create a normal system stream */
		error = FT_Stream_Open( stream, args->pathname );
		stream->pathname.pointer = args->pathname;
	}
	else if ( ( args->flags & FT_OPEN_STREAM ) && args->stream )
	{
		/* use an existing, user-provided stream */
		/* in this case, we do not need to allocate a new stream object */
		/* since the caller is responsible for closing it himself       */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( stream );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		stream = args->stream;
	}
#endif
	else
		error = ft_error_h::FT_Err_Invalid_Argument;
	if ( error )
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( stream );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	else
		stream->memory = memory;  /* just to be certain */
	*astream = stream;
Exit:
	return error;
};
FT_BASE_DEF( void )
	FT_Stream_OpenMemory( ft_structs_h::FT_Stream       stream,
	const ft_structs_h::FT_Byte*  base,
	ft_structs_h::FT_ULong        size )
{
	stream->base   = (ft_structs_h::FT_Byte*) base;
	stream->size   = size;
	stream->pos    = 0;
	stream->cursor = 0;
	stream->read   = 0;
	stream->close  = 0;
};
FT_CALLBACK_DEF( void )
	ft_ansi_stream_close( ft_structs_h::FT_Stream  stream )
{
	ft_fclose( STREAM_FILE( stream ) );
	stream->descriptor.pointer = NULL;
	stream->size               = 0;
	stream->base               = 0;
};
FT_CALLBACK_DEF( unsigned long )
	ft_ansi_stream_io( ft_structs_h::FT_Stream       stream,
	unsigned long   offset,
	unsigned char*  buffer,
	unsigned long   count )
{
	FT_FILE*  file;
	if ( !count && offset > stream->size )
		return 1;
	file = STREAM_FILE( stream );
	if ( stream->pos != offset )
		ft_fseek( file, offset, SEEK_SET );
	return (unsigned long)ft_fread( buffer, 1, count, file );
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_Stream_Open( ft_structs_h::FT_Stream    stream,
	const char*  filepathname )
{
	FT_FILE*  file;
	if ( !stream )
		return ft_error_h::FT_Err_Invalid_Stream_Handle;
	stream->descriptor.pointer = NULL;
	stream->pathname.pointer   = (char*)filepathname;
	stream->base               = 0;
	stream->pos                = 0;
	stream->read               = NULL;
	stream->close              = NULL;
	file = ft_fopen( filepathname, "rb" );
	if ( !file )
	{
		FT_ERROR(( "FT_Stream_Open:"
			" could not open `%s'\n", filepathname ));
		return ft_error_h::FT_Err_Cannot_Open_Resource;
	}
	ft_fseek( file, 0, SEEK_END );
	stream->size = ft_ftell( file );
	if ( !stream->size )
	{
		FT_ERROR(( "FT_Stream_Open:" ));
		FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
		ft_fclose( file );
		return ft_error_h::FT_Err_Cannot_Open_Stream;
	}
	ft_fseek( file, 0, SEEK_SET );
	stream->descriptor.pointer = file;
	stream->read  = ft_ansi_stream_io;
	stream->close = ft_ansi_stream_close;
	FT_TRACE1(( "FT_Stream_Open:" ));
	FT_TRACE1(( " opened `%s' (%d bytes) successfully\n",
		filepathname, stream->size ));
	return ft_error_h::FT_Err_Ok;
};
static ft_structs_h::FT_Error
	ft_glyphslot_init( ft_structs_h::FT_GlyphSlot  slot )
{
	ft_structs_h::FT_Driver         driver   = slot->face->driver;
	ft_structs_h::FT_Driver_Class   clazz    = driver->clazz;
	ft_structs_h::FT_Memory         memory   = driver->root.memory;
	ft_structs_h::FT_Error          error    = ft_error_h::FT_Err_Ok;
	ft_structs_h::FT_Slot_Internal  internal = NULL;
	slot->library = driver->root.library;
	if ( FT_NEW( internal ) )
		goto Exit;
	slot->internal = internal;
	if ( FT_DRIVER_USES_OUTLINES( driver ) )
		error = FT_GlyphLoader_New( memory, &internal->loader );
	if ( !error && clazz->init_slot )
		error = clazz->init_slot( slot );
Exit:
	return error;
};
FT_BASE_DEF( ft_structs_h::FT_Error )
	FT_New_GlyphSlot( ft_structs_h::FT_Face        face,
	ft_structs_h::FT_GlyphSlot  *aslot )
{
	ft_structs_h::FT_Error         error;
	ft_structs_h::FT_Driver        driver;
	ft_structs_h::FT_Driver_Class  clazz;
	ft_structs_h::FT_Memory        memory;
	ft_structs_h::FT_GlyphSlot     slot = NULL;
	if ( !face || !face->driver )
		return ft_error_h::FT_Err_Invalid_Argument;
	driver = face->driver;
	clazz  = driver->clazz;
	memory = driver->root.memory;
	FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
	if ( !FT_ALLOC( slot, clazz->slot_object_size ) )
	{
		slot->face = face;
		error = ft_glyphslot_init( slot );
		if ( error )
		{
			ft_glyphslot_done( slot );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
			FT_FREE( slot );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
			goto Exit;
		}
		slot->next  = face->glyph;
		face->glyph = slot;
		if ( aslot )
			*aslot = slot;
	}
	else if ( aslot )
		*aslot = 0;
Exit:
	FT_TRACE4(( "FT_New_GlyphSlot: Return %d\n", error ));
	return error;
};
FT_EXPORT_DEF( ft_structs_h::FT_Error )
	FT_New_Size( ft_structs_h::FT_Face   face,
	ft_structs_h::FT_Size  *asize )
{
	ft_structs_h::FT_Error         error;
	ft_structs_h::FT_Memory        memory;
	ft_structs_h::FT_Driver        driver;
	ft_structs_h::FT_Driver_Class  clazz;
	ft_structs_h::FT_Size          size = 0;
	ft_structs_h::FT_ListNode      node = 0;
	if ( !face )
		return ft_error_h::FT_Err_Invalid_Face_Handle;
	if ( !asize )
		return ft_error_h::FT_Err_Invalid_Size_Handle;
	if ( !face->driver )
		return ft_error_h::FT_Err_Invalid_Driver_Handle;
	*asize = 0;
	driver = face->driver;
	clazz  = driver->clazz;
	memory = face->memory;
	/* Allocate new size object and perform basic initialisation */
	if ( FT_ALLOC( size, clazz->size_object_size ) || FT_NEW( node ) )
		goto Exit;
	size->face = face;
	/* for now, do not use any internal fields in size objects */
	size->internal = 0;
	if ( clazz->init_size )
		error = clazz->init_size( size );
	/* in case of success, add to the face's list */
	if ( !error )
	{
		*asize     = size;
		node->data = size;
		FT_List_Add( &face->sizes_list, node );
	}
Exit:
	if ( error )
	{
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( node );
		FT_FREE( size );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	}
	return error;
};
namespace ft_winfnt_h
{
	static const ft_structs_h::FT_Frame_Field  winmz_header_fields[] =
	{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  WinMZ_HeaderRec
		FT_FRAME_START( 64 ),
		FT_FRAME_USHORT_LE ( magic ),
		FT_FRAME_SKIP_BYTES( 29 * 2 ),
		FT_FRAME_ULONG_LE  ( lfanew ),
		FT_FRAME_END
	};
	static const ft_structs_h::FT_Frame_Field  winne_header_fields[] =
	{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  WinNE_HeaderRec
		FT_FRAME_START( 40 ),
		FT_FRAME_USHORT_LE ( magic ),
		FT_FRAME_SKIP_BYTES( 34 ),
		FT_FRAME_USHORT_LE ( resource_tab_offset ),
		FT_FRAME_USHORT_LE ( rname_tab_offset ),
		FT_FRAME_END
	};
	static const ft_structs_h::FT_Frame_Field  winpe32_header_fields[] =
	{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  WinPE32_HeaderRec
		FT_FRAME_START( 248 ),
		FT_FRAME_ULONG_LE  ( magic ),   /* PE00 */
		FT_FRAME_USHORT_LE ( machine ), /* 0x014c - i386 */
		FT_FRAME_USHORT_LE ( number_of_sections ),
		FT_FRAME_SKIP_BYTES( 12 ),
		FT_FRAME_USHORT_LE ( size_of_optional_header ),
		FT_FRAME_SKIP_BYTES( 2 ),
		FT_FRAME_USHORT_LE ( magic32 ), /* 0x10b */
		FT_FRAME_SKIP_BYTES( 110 ),
		FT_FRAME_ULONG_LE  ( rsrc_virtual_address ),
		FT_FRAME_ULONG_LE  ( rsrc_size ),
		FT_FRAME_SKIP_BYTES( 104 ),
		FT_FRAME_END
	};
	static const ft_structs_h::FT_Frame_Field  winpe32_section_fields[] =
	{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  WinPE32_SectionRec
		FT_FRAME_START( 40 ),
		FT_FRAME_BYTES     ( name, 8 ),
		FT_FRAME_SKIP_BYTES( 4 ),
		FT_FRAME_ULONG_LE  ( virtual_address ),
		FT_FRAME_ULONG_LE  ( size_of_raw_data ),
		FT_FRAME_ULONG_LE  ( pointer_to_raw_data ),
		FT_FRAME_SKIP_BYTES( 16 ),
		FT_FRAME_END
	};
	static const ft_structs_h::FT_Frame_Field  winpe_rsrc_dir_fields[] =
	{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  WinPE_RsrcDirRec
		FT_FRAME_START( 16 ),
		FT_FRAME_ULONG_LE ( characteristics ),
		FT_FRAME_ULONG_LE ( time_date_stamp ),
		FT_FRAME_USHORT_LE( major_version ),
		FT_FRAME_USHORT_LE( minor_version ),
		FT_FRAME_USHORT_LE( number_of_named_entries ),
		FT_FRAME_USHORT_LE( number_of_id_entries ),
		FT_FRAME_END
	};
	static const ft_structs_h::FT_Frame_Field  winpe_rsrc_dir_entry_fields[] =
	{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  WinPE_RsrcDirEntryRec
		FT_FRAME_START( 8 ),
		FT_FRAME_ULONG_LE( name ),
		FT_FRAME_ULONG_LE( offset ),
		FT_FRAME_END
  };
	static const ft_structs_h::FT_Frame_Field  winpe_rsrc_data_entry_fields[] =
	{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  WinPE_RsrcDataEntryRec
		FT_FRAME_START( 16 ),
		FT_FRAME_ULONG_LE( offset_to_data ),
		FT_FRAME_ULONG_LE( size ),
		FT_FRAME_ULONG_LE( code_page ),
		FT_FRAME_ULONG_LE( reserved ),
		FT_FRAME_END
  };
	static ft_structs_h::FT_Error
		fnt_cmap_init( FNT_CMap  cmap )
	{
		FNT_Face  face = (FNT_Face)FT_CMAP_FACE( cmap );
		FNT_Font  font = face->font;
		cmap->first = (ft_structs_h::FT_UInt32)  font->header.first_char;
		cmap->count = (ft_structs_h::FT_UInt32)( font->header.last_char - cmap->first + 1 );
    return 0;
  };
	static ft_structs_h::FT_UInt
		fnt_cmap_char_index( FNT_CMap   cmap,
		ft_structs_h::FT_UInt32  char_code )
	{
		ft_structs_h::FT_UInt  gindex = 0;
		char_code -= cmap->first;
		if ( char_code < cmap->count )
			/* we artificially increase the glyph index; */
			/* FNT_Load_Glyph reverts to the right one   */
			gindex = (ft_structs_h::FT_UInt)( char_code + 1 );
		return gindex;
	}
	static ft_structs_h::FT_UInt32
		fnt_cmap_char_next( FNT_CMap    cmap,
		ft_structs_h::FT_UInt32  *pchar_code )
	{
		ft_structs_h::FT_UInt    gindex = 0;
		ft_structs_h::FT_UInt32  result = 0;
		ft_structs_h::FT_UInt32  char_code = *pchar_code + 1;
		if ( char_code <= cmap->first )
		{
			result = cmap->first;
			gindex = 1;
		}
		else
		{
			char_code -= cmap->first;
			if ( char_code < cmap->count )
			{
				result = cmap->first + char_code;
				gindex = (ft_structs_h::FT_UInt)( char_code + 1 );
			}
    }
		*pchar_code = result;
		return gindex;
	};
	static const ft_structs_h::FT_CMap_ClassRec  fnt_cmap_class_rec =
	{
		sizeof ( FNT_CMapRec ),
		(ft_structs_h::FT_CMap_InitFunc)     fnt_cmap_init,
		(ft_structs_h::FT_CMap_DoneFunc)     NULL,
		(ft_structs_h::FT_CMap_CharIndexFunc)fnt_cmap_char_index,
		(ft_structs_h::FT_CMap_CharNextFunc) fnt_cmap_char_next,
		NULL, NULL, NULL, NULL, NULL
	};
	static ft_structs_h::FT_CMap_Class const  fnt_cmap_class = &fnt_cmap_class_rec;
	static ft_structs_h::FT_Error
		winfnt_get_header( ft_structs_h::FT_Face               face,
		FT_WinFNT_HeaderRec  *aheader )
	{
		FNT_Font  font = ((FNT_Face)face)->font;
		*aheader = font->header;
		return 0;
	};
	static const FT_Service_WinFntRec  winfnt_service_rec =
	{
		winfnt_get_header
	};
	static const ft_structs_h::FT_ServiceDescRec  winfnt_services[] =
	{
		{ FT_SERVICE_ID_XF86_NAME, FT_XF86_FORMAT_WINFNT },
		{ FT_SERVICE_ID_WINFNT,    &winfnt_service_rec },
		{ NULL, NULL }
	};
	static void
		fnt_font_done( FNT_Face face )
	{
		ft_structs_h::FT_Memory  memory = FT_FACE( face )->memory;
		ft_structs_h::FT_Stream  stream = FT_FACE( face )->stream;
		FNT_Font   font   = face->font;
		if ( !font )
			return;
		if ( font->fnt_frame )
			FT_FRAME_RELEASE( font->fnt_frame );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( font->family_name );
		FT_FREE( font );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		face->font = 0;
	};
	static ft_structs_h::FT_Error
		fnt_font_load( FNT_Font   font,
		ft_structs_h::FT_Stream  stream )
	{
		ft_structs_h::FT_Error          error;
		FT_WinFNT_Header  header = &font->header;
		ft_structs_h::FT_Bool           new_format;
		ft_structs_h::FT_UInt           size;
		/* first of all, read the FNT header */
		if ( FT_STREAM_SEEK( font->offset )                        ||
			FT_STREAM_READ_FIELDS( winfnt_header_fields, header ) )
			goto Exit;
		/* check header */
		if ( header->version != 0x200 &&
			header->version != 0x300 )
		{
			FT_TRACE2(( "  not a Windows FNT file\n" ));
			error = ft_error_h::FNT_Err_Unknown_File_Format;
			goto Exit;
		}
		new_format = FT_BOOL( font->header.version == 0x300 );
		size       = new_format ? 148 : 118;
		if ( header->file_size < size )
		{
			FT_TRACE2(( "  not a Windows FNT file\n" ));
			error = ft_error_h::FNT_Err_Unknown_File_Format;
			goto Exit;
		}
		/* Version 2 doesn't have these fields */
		if ( header->version == 0x200 )
		{
			header->flags   = 0;
			header->A_space = 0;
			header->B_space = 0;
			header->C_space = 0;
			header->color_table_offset = 0;
		}
		if ( header->file_type & 1 )
		{
			FT_TRACE2(( "[can't handle vector FNT fonts]\n" ));
			error = ft_error_h::FNT_Err_Unknown_File_Format;
			goto Exit;
		}
		/* this is a FNT file/table; extract its frame */
		if ( FT_STREAM_SEEK( font->offset )                         ||
			FT_FRAME_EXTRACT( header->file_size, font->fnt_frame ) )
			goto Exit;
Exit:
		return error;
	};
	static ft_structs_h::FT_Module_Interface
		winfnt_get_service( ft_structs_h::FT_Module         module,
		const ft_structs_h::FT_String*  service_id )
	{
		FT_UNUSED( module );
		return ft_service_list_lookup( winfnt_services, service_id );
	};
	static ft_structs_h::FT_Error
		fnt_face_get_dll_font( FNT_Face  face,
		ft_structs_h::FT_Int    face_index )
	{
		ft_structs_h::FT_Error         error;
		ft_structs_h::FT_Stream        stream = FT_FACE( face )->stream;
		ft_structs_h::FT_Memory        memory = FT_FACE( face )->memory;
		WinMZ_HeaderRec  mz_header;
		face->font = 0;
		/* does it begin with an MZ header? */
		if ( FT_STREAM_SEEK( 0 )                                      ||
			FT_STREAM_READ_FIELDS( winmz_header_fields, &mz_header ) )
			goto Exit;
		error = ft_error_h::FNT_Err_Unknown_File_Format;
		if ( mz_header.magic == WINFNT_MZ_MAGIC )
		{
			/* yes, now look for an NE header in the file */
			WinNE_HeaderRec  ne_header;
			FT_TRACE2(( "MZ signature found\n" ));
			if ( FT_STREAM_SEEK( mz_header.lfanew )                       ||
				FT_STREAM_READ_FIELDS( winne_header_fields, &ne_header ) )
				goto Exit;
			error = ft_error_h::FNT_Err_Unknown_File_Format;
			if ( ne_header.magic == WINFNT_NE_MAGIC )
			{
				/* good, now look into the resource table for each FNT resource */
				ft_structs_h::FT_ULong   res_offset  = mz_header.lfanew +
					ne_header.resource_tab_offset;
				ft_structs_h::FT_UShort  size_shift;
				ft_structs_h::FT_UShort  font_count  = 0;
				ft_structs_h::FT_ULong   font_offset = 0;
				FT_TRACE2(( "NE signature found\n" ));
				if ( FT_STREAM_SEEK( res_offset )                    ||
					FT_FRAME_ENTER( ne_header.rname_tab_offset -
					ne_header.resource_tab_offset ) )
					goto Exit;
				size_shift = FT_GET_USHORT_LE();
				for (;;)
				{
					ft_structs_h::FT_UShort  type_id, count;
					type_id = FT_GET_USHORT_LE();
					if ( !type_id )
						break;
					count = FT_GET_USHORT_LE();
					if ( type_id == 0x8008U )
					{
						font_count  = count;
            font_offset = (ft_structs_h::FT_ULong)( FT_STREAM_POS() + 4 +
							( stream->cursor - stream->limit ) );
						break;
					}
					stream->cursor += 4 + count * 12;
				}
				FT_FRAME_EXIT();
				if ( !font_count || !font_offset )
				{
					FT_TRACE2(( "this file doesn't contain any FNT resources\n" ));
          error = ft_error_h::FNT_Err_Invalid_File_Format;
          goto Exit;
        }
				/* loading `winfnt_header_fields' needs at least 118 bytes;    */
				/* use this as a rough measure to check the expected font size */
				if ( font_count * 118UL > stream->size )
        {
          FT_TRACE2(( "invalid number of faces\n" ));
          error = ft_error_h::FNT_Err_Invalid_File_Format;
          goto Exit;
        }
				face->root.num_faces = font_count;
				if ( face_index >= font_count )
        {
          error = ft_error_h::FNT_Err_Invalid_Argument;
          goto Exit;
        }
        else if ( face_index < 0 )
          goto Exit;
				if ( FT_NEW( face->font ) )
					goto Exit;
				if ( FT_STREAM_SEEK( font_offset + face_index * 12 ) ||
             FT_FRAME_ENTER( 12 )                            )
          goto Fail;
				face->font->offset   = (ft_structs_h::FT_ULong)FT_GET_USHORT_LE() << size_shift;
        face->font->fnt_size = (ft_structs_h::FT_ULong)FT_GET_USHORT_LE() << size_shift;
				stream->cursor += 8;
				FT_FRAME_EXIT();
				error = fnt_font_load( face->font, stream );
			}
			else if ( ne_header.magic == WINFNT_PE_MAGIC )
      {
				WinPE32_HeaderRec       pe32_header;
        WinPE32_SectionRec      pe32_section;
        WinPE_RsrcDirRec        root_dir, name_dir, lang_dir;
        WinPE_RsrcDirEntryRec   dir_entry1, dir_entry2, dir_entry3;
        WinPE_RsrcDataEntryRec  data_entry;
				ft_structs_h::FT_Long    root_dir_offset, name_dir_offset, lang_dir_offset;
				ft_structs_h::FT_UShort  i, j, k;
				FT_TRACE2(( "PE signature found\n" ));
				if ( FT_STREAM_SEEK( mz_header.lfanew )                           ||
					FT_STREAM_READ_FIELDS( winpe32_header_fields, &pe32_header ) )
					goto Exit;
				FT_TRACE2(( "magic %04lx, machine %02x, number_of_sections %u, "
					"size_of_optional_header %02x\n"
					"magic32 %02x, rsrc_virtual_address %04lx, "
					"rsrc_size %04lx\n",
					pe32_header.magic, pe32_header.machine,
					pe32_header.number_of_sections,
					pe32_header.size_of_optional_header,
					pe32_header.magic32, pe32_header.rsrc_virtual_address,
					pe32_header.rsrc_size ));
				if ( pe32_header.magic != WINFNT_PE_MAGIC /* check full signature */ ||
					pe32_header.machine != 0x014c /* i386 */                        ||
					pe32_header.size_of_optional_header != 0xe0 /* FIXME */         ||
					pe32_header.magic32 != 0x10b                                    )
				{
					FT_TRACE2(( "this file has an invalid PE header\n" ));
					error = ft_error_h::FNT_Err_Invalid_File_Format;
					goto Exit;
				}
				face->root.num_faces = 0;
				for ( i = 0; i < pe32_header.number_of_sections; i++ )
        {
          if ( FT_STREAM_READ_FIELDS( winpe32_section_fields,
						&pe32_section ) )
						goto Exit;
					FT_TRACE2(( "name %.8s, va %04lx, size %04lx, offset %04lx\n",
						pe32_section.name, pe32_section.virtual_address,
						pe32_section.size_of_raw_data,
						pe32_section.pointer_to_raw_data ));
					if ( pe32_header.rsrc_virtual_address ==
						pe32_section.virtual_address )
						goto Found_rsrc_section;
				}
				FT_TRACE2(( "this file doesn't contain any resources\n" ));
				error = ft_error_h::FNT_Err_Invalid_File_Format;
				goto Exit;
Found_rsrc_section:
				FT_TRACE2(( "found resources section %.8s\n", pe32_section.name ));
				if ( FT_STREAM_SEEK( pe32_section.pointer_to_raw_data )        ||
					FT_STREAM_READ_FIELDS( winpe_rsrc_dir_fields, &root_dir ) )
					goto Exit;
				root_dir_offset = pe32_section.pointer_to_raw_data;
				for ( i = 0; i < root_dir.number_of_named_entries +
					root_dir.number_of_id_entries; i++ )
				{
					if ( FT_STREAM_SEEK( root_dir_offset + 16 + i * 8 )      ||
						FT_STREAM_READ_FIELDS( winpe_rsrc_dir_entry_fields,
						&dir_entry1 )                )
						goto Exit;
					if ( !(dir_entry1.offset & 0x80000000UL ) /* DataIsDirectory */ )
					{
            error = ft_error_h::FNT_Err_Invalid_File_Format;
            goto Exit;
          }
					dir_entry1.offset &= ~0x80000000UL;
					name_dir_offset = pe32_section.pointer_to_raw_data +
						dir_entry1.offset;
					if ( FT_STREAM_SEEK( pe32_section.pointer_to_raw_data +
						dir_entry1.offset )                       ||
						FT_STREAM_READ_FIELDS( winpe_rsrc_dir_fields, &name_dir ) )
						goto Exit;
					for ( j = 0; j < name_dir.number_of_named_entries +
						name_dir.number_of_id_entries; j++ )
					{
						if ( FT_STREAM_SEEK( name_dir_offset + 16 + j * 8 )      ||
							FT_STREAM_READ_FIELDS( winpe_rsrc_dir_entry_fields,
							&dir_entry2 )                )
							goto Exit;
						if ( !(dir_entry2.offset & 0x80000000UL ) /* DataIsDirectory */ )
            {
              error = ft_error_h::FNT_Err_Invalid_File_Format;
              goto Exit;
            }
						dir_entry2.offset &= ~0x80000000UL;
						lang_dir_offset = pe32_section.pointer_to_raw_data +
							dir_entry2.offset;
						if ( FT_STREAM_SEEK( pe32_section.pointer_to_raw_data +
							dir_entry2.offset )                     ||
							FT_STREAM_READ_FIELDS( winpe_rsrc_dir_fields, &lang_dir ) )
							goto Exit;
						for ( k = 0; k < lang_dir.number_of_named_entries +
							lang_dir.number_of_id_entries; k++ )
						{
							if ( FT_STREAM_SEEK( lang_dir_offset + 16 + k * 8 )      ||
								FT_STREAM_READ_FIELDS( winpe_rsrc_dir_entry_fields,
								&dir_entry3 )                )
								goto Exit;
							if ( dir_entry2.offset & 0x80000000UL /* DataIsDirectory */ )
							{
								error = ft_error_h::FNT_Err_Invalid_File_Format;
								goto Exit;
							}
							if ( dir_entry1.name == 8 /* RT_FONT */ )
							{
								if ( FT_STREAM_SEEK( root_dir_offset + dir_entry3.offset ) ||
									FT_STREAM_READ_FIELDS( winpe_rsrc_data_entry_fields,
									&data_entry )                  )
									goto Exit;
								FT_TRACE2(( "found font #%lu, offset %04lx, "
									"size %04lx, cp %lu\n",
									dir_entry2.name,
									pe32_section.pointer_to_raw_data +
									data_entry.offset_to_data -
									pe32_section.virtual_address,
									data_entry.size, data_entry.code_page ));
								if ( face_index == face->root.num_faces )
								{
									if ( FT_NEW( face->font ) )
										goto Exit;
									face->font->offset   = pe32_section.pointer_to_raw_data +
										data_entry.offset_to_data -
										pe32_section.virtual_address;
									face->font->fnt_size = data_entry.size;
									error = fnt_font_load( face->font, stream );
									if ( error )
									{
										FT_TRACE2(( "font #%lu load error %d\n",
											dir_entry2.name, error ));
										goto Fail;
									}
									else
										FT_TRACE2(( "font #%lu successfully loaded\n",
										dir_entry2.name ));
								}
								face->root.num_faces++;
              }
            }
          }
        }
      }
			if ( !face->root.num_faces )
			{
				FT_TRACE2(( "this file doesn't contain any RT_FONT resources\n" ));
				error = ft_error_h::FNT_Err_Invalid_File_Format;
				goto Exit;
			}
			if ( face_index >= face->root.num_faces )
			{
				error = ft_error_h::FNT_Err_Invalid_Argument;
				goto Exit;
			}
		}
Fail:
		if ( error )
			fnt_font_done( face );
Exit:
		return error;
	}
	static ft_structs_h::FT_Error
		FNT_Load_Glyph( ft_structs_h::FT_GlyphSlot  slot,
		ft_structs_h::FT_Size       size,
		ft_structs_h::FT_UInt       glyph_index,
		ft_structs_h::FT_Int32      load_flags )
	{
		FNT_Face    face   = (FNT_Face)FT_SIZE_FACE( size );
    FNT_Font    font;
    ft_structs_h::FT_Error    error  = ft_error_h::FNT_Err_Ok;
    ft_structs_h::FT_Byte*    p;
    ft_structs_h::FT_Int      len;
    ft_structs_h::FT_Bitmap*  bitmap = &slot->bitmap;
    ft_structs_h::FT_ULong    offset;
    ft_structs_h::FT_Bool     new_format;
		FT_UNUSED( load_flags );
		if ( !face )
		{
			error = ft_error_h::FNT_Err_Invalid_Argument;
      goto Exit;
    }
		font = face->font;
		if ( !font ||
			glyph_index >= (ft_structs_h::FT_UInt)( FT_FACE( face )->num_glyphs ) )
		{
			error = ft_error_h::FNT_Err_Invalid_Argument;
			goto Exit;
		}
		if ( glyph_index > 0 )
			glyph_index--;                           /* revert to real index */
		else
			glyph_index = font->header.default_char; /* the .notdef glyph */
		new_format = FT_BOOL( font->header.version == 0x300 );
		len        = new_format ? 6 : 4;
		/* jump to glyph entry */
		p = font->fnt_frame + ( new_format ? 148 : 118 ) + len * glyph_index;
		bitmap->width = FT_NEXT_SHORT_LE( p );
		if ( new_format )
			offset = FT_NEXT_ULONG_LE( p );
    else
      offset = FT_NEXT_USHORT_LE( p );
		if ( offset >= font->header.file_size )
    {
      FT_TRACE2(( "invalid FNT offset\n" ));
      error = ft_error_h::FNT_Err_Invalid_File_Format;
      goto Exit;
    }
		/* jump to glyph data */
		p = font->fnt_frame + /* font->header.bits_offset */ + offset;
		/* allocate and build bitmap */
		{
      ft_structs_h::FT_Memory  memory = FT_FACE_MEMORY( slot->face );
      ft_structs_h::FT_Int     pitch  = ( bitmap->width + 7 ) >> 3;
      ft_structs_h::FT_Byte*   column;
      ft_structs_h::FT_Byte*   write;
			bitmap->pitch      = pitch;
      bitmap->rows       = font->header.pixel_height;
      bitmap->pixel_mode = ft_structs_h::FT_PIXEL_MODE_MONO;
			if ( offset + pitch * bitmap->rows >= font->header.file_size )
      {
        FT_TRACE2(( "invalid bitmap width\n" ));
        error = ft_error_h::FNT_Err_Invalid_File_Format;
        goto Exit;
      }
      /* note: since glyphs are stored in columns and not in rows we */
      /*       can't use ft_glyphslot_set_bitmap                     */
      if ( FT_ALLOC_MULT( bitmap->buffer, pitch, bitmap->rows ) )
				goto Exit;
			column = (ft_structs_h::FT_Byte*)bitmap->buffer;
      for ( ; pitch > 0; pitch--, column++ )
      {
        ft_structs_h::FT_Byte*  limit = p + bitmap->rows;
				for ( write = column; p < limit; p++, write += bitmap->pitch )
					*write = *p;
      }
    }
		slot->internal->flags = FT_GLYPH_OWN_BITMAP;
    slot->bitmap_left     = 0;
    slot->bitmap_top      = font->header.ascent;
    slot->format          = ft_structs_h::FT_GLYPH_FORMAT_BITMAP;
		/* now set up metrics */
    slot->metrics.width        = bitmap->width << 6;
    slot->metrics.height       = bitmap->rows << 6;
    slot->metrics.horiAdvance  = bitmap->width << 6;
    slot->metrics.horiBearingX = 0;
    slot->metrics.horiBearingY = slot->bitmap_top << 6;
		ft_synthesize_vertical_metrics( &slot->metrics,
			bitmap->rows << 6 );
Exit:
		return error;
  }
	static ft_structs_h::FT_Error
		FNT_Size_Select( ft_structs_h::FT_Size   size,
		ft_structs_h::FT_ULong  strike_index )
	{
		FNT_Face          face   = (FNT_Face)size->face;
		FT_WinFNT_Header  header = &face->font->header;
		FT_UNUSED( strike_index );
		FT_Select_Metrics( size->face, 0 );
		size->metrics.ascender    = header->ascent * 64;
		size->metrics.descender   = -( header->pixel_height -
			header->ascent ) * 64;
		size->metrics.max_advance = header->max_width * 64;
    return ft_error_h::FNT_Err_Ok;
  }
	static ft_structs_h::FT_Error
		FNT_Size_Request( ft_structs_h::FT_Size          size,
		ft_structs_h::FT_Size_Request  req )
	{
		FNT_Face          face    = (FNT_Face)size->face;
		FT_WinFNT_Header  header  = &face->font->header;
		ft_structs_h::FT_Bitmap_Size*   bsize   = size->face->available_sizes;
		ft_structs_h::FT_Error          error   = ft_error_h::FNT_Err_Invalid_Pixel_Size;
		ft_structs_h::FT_Long           height;
		height = FT_REQUEST_HEIGHT( req );
		height = ( height + 32 ) >> 6;
		switch ( req->type )
		{
		case ft_structs_h::FT_SIZE_REQUEST_TYPE_NOMINAL:
			if ( height == ( ( bsize->y_ppem + 32 ) >> 6 ) )
				error = ft_error_h::FNT_Err_Ok;
			break;
		case ft_structs_h::FT_SIZE_REQUEST_TYPE_REAL_DIM:
			if ( height == header->pixel_height )
				error = ft_error_h::FNT_Err_Ok;
			break;
		default:
			error = ft_error_h::FNT_Err_Unimplemented_Feature;
			break;
		}
		if ( error )
			return error;
		else
			return FNT_Size_Select( size, 0 );
	}
	static void
		FNT_Face_Done( ft_structs_h::FT_Face  fntface )       /* FNT_Face */
	{
		FNT_Face   face = (FNT_Face)fntface;
		ft_structs_h::FT_Memory  memory;
		if ( !face )
			return;
		memory = FT_FACE_MEMORY( face );
		fnt_font_done( face );
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
    FT_FREE( fntface->available_sizes );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
    fntface->num_fixed_sizes = 0;
  }
	static ft_structs_h::FT_Error
		FNT_Face_Init( ft_structs_h::FT_Stream      stream,
		ft_structs_h::FT_Face        fntface,        /* FNT_Face */
		ft_structs_h::FT_Int         face_index,
		ft_structs_h::FT_Int         num_params,
		ft_structs_h::FT_Parameter*  params )
	{
		FNT_Face   face   = (FNT_Face)fntface;
		ft_structs_h::FT_Error   error;
		ft_structs_h::FT_Memory  memory = FT_FACE_MEMORY( face );
		FT_UNUSED( num_params );
		FT_UNUSED( params );
		FT_TRACE2(( "Windows FNT driver\n" ));
		/* try to load font from a DLL */
		error = fnt_face_get_dll_font( face, face_index );
		if ( !error && face_index < 0 )
			goto Exit;
		if ( error == ft_error_h::FNT_Err_Unknown_File_Format )
		{
			/* this didn't work; try to load a single FNT font */
			FNT_Font  font;
			if ( FT_NEW( face->font ) )
				goto Exit;
			fntface->num_faces = 1;
			font           = face->font;
			font->offset   = 0;
			font->fnt_size = stream->size;
			error = fnt_font_load( font, stream );
			if ( !error )
			{
				if ( face_index > 0 )
					error = ft_error_h::FNT_Err_Invalid_Argument;
				else if ( face_index < 0 )
					goto Exit;
			}
		}
		if ( error )
			goto Fail;
		/* we now need to fill the root FT_Face fields */
		/* with relevant information                   */
		{
			ft_structs_h::FT_Face     root = FT_FACE( face );
			FNT_Font    font = face->font;
			ft_structs_h::FT_PtrDist  family_size;
			root->face_index = face_index;
			root->face_flags = FT_FACE_FLAG_FIXED_SIZES |
				FT_FACE_FLAG_HORIZONTAL;
			if ( font->header.avg_width == font->header.max_width )
				root->face_flags |= FT_FACE_FLAG_FIXED_WIDTH;
			if ( font->header.italic )
				root->style_flags |= FT_STYLE_FLAG_ITALIC;
			if ( font->header.weight >= 800 )
				root->style_flags |= FT_STYLE_FLAG_BOLD;
			/* set up the `fixed_sizes' array */
			if ( FT_NEW_ARRAY( root->available_sizes, 1 ) )
				goto Fail;
			root->num_fixed_sizes = 1;
			{
				ft_structs_h::FT_Bitmap_Size*  bsize = root->available_sizes;
				ft_structs_h::FT_UShort        x_res, y_res;
				bsize->width  = font->header.avg_width;
				bsize->height = (ft_structs_h::FT_Short)( font->header.pixel_height + font->header.external_leading );
				bsize->size   = font->header.nominal_point_size << 6;
				x_res = font->header.horizontal_resolution;
				if ( !x_res )
					x_res = 72;
				y_res = font->header.vertical_resolution;
				if ( !y_res )
					y_res = 72;
				bsize->y_ppem = FT_MulDiv( bsize->size, y_res, 72 );
				bsize->y_ppem = FT_PIX_ROUND( bsize->y_ppem );
				/*
				* this reads:
				*
				* the nominal height is larger than the bbox's height
				*
				* => nominal_point_size contains incorrect value;
				*    use pixel_height as the nominal height
				*/
				if ( bsize->y_ppem > ( font->header.pixel_height << 6 ) )
				{
					FT_TRACE2(( "use pixel_height as the nominal height\n" ));
					bsize->y_ppem = font->header.pixel_height << 6;
					bsize->size   = FT_MulDiv( bsize->y_ppem, 72, y_res );
				}
				bsize->x_ppem = FT_MulDiv( bsize->size, x_res, 72 );
				bsize->x_ppem = FT_PIX_ROUND( bsize->x_ppem );
			}
			{
				ft_structs_h::FT_CharMapRec  charmap;
				charmap.encoding    = ft_structs_h::FT_ENCODING_NONE;
				/* initial platform/encoding should indicate unset status? */
				charmap.platform_id = TT_PLATFORM_APPLE_UNICODE;
				charmap.encoding_id = TT_APPLE_ID_DEFAULT;
				charmap.face        = root;
				if ( font->header.charset == FT_WinFNT_ID_MAC )
				{
					charmap.encoding    = ft_structs_h::FT_ENCODING_APPLE_ROMAN;
					charmap.platform_id = TT_PLATFORM_MACINTOSH;
					/*        charmap.encoding_id = TT_MAC_ID_ROMAN; */
				}
				error = FT_CMap_New( fnt_cmap_class,
					NULL,
					&charmap,
					NULL );
				if ( error )
					goto Fail;
				/* Select default charmap */
				if ( root->num_charmaps )
					root->charmap = root->charmaps[0];
			}
			/* set up remaining flags */
			if ( font->header.last_char < font->header.first_char )
			{
				FT_TRACE2(( "invalid number of glyphs\n" ));
				error = ft_error_h::FNT_Err_Invalid_File_Format;
				goto Fail;
			}
			/* reserve one slot for the .notdef glyph at index 0 */
			root->num_glyphs = font->header.last_char -
				font->header.first_char + 1 + 1;
			if ( font->header.face_name_offset >= font->header.file_size )
			{
				FT_TRACE2(( "invalid family name offset\n" ));
				error = ft_error_h::FNT_Err_Invalid_File_Format;
				goto Fail;
			}
			family_size = font->header.file_size - font->header.face_name_offset;
			/* Some broken fonts don't delimit the face name with a final */
			/* NULL byte -- the frame is erroneously one byte too small.  */
			/* We thus allocate one more byte, setting it explicitly to   */
			/* zero.                                                      */
			if ( FT_ALLOC( font->family_name, family_size + 1 ) )
				goto Fail;
			FT_MEM_COPY( font->family_name,
				font->fnt_frame + font->header.face_name_offset,
				family_size );
			font->family_name[family_size] = '\0';
			if ( FT_REALLOC( font->family_name,
				family_size,
				ft_strlen( font->family_name ) + 1 ) )
				goto Fail;
			root->family_name = font->family_name;
			root->style_name  = (char *)"Regular";
			if ( root->style_flags & FT_STYLE_FLAG_BOLD )
			{
				if ( root->style_flags & FT_STYLE_FLAG_ITALIC )
					root->style_name = (char *)"Bold Italic";
				else
					root->style_name = (char *)"Bold";
			}
			else if ( root->style_flags & FT_STYLE_FLAG_ITALIC )
				root->style_name = (char *)"Italic";
			}
			goto Exit;
	Fail:
			FNT_Face_Done( fntface );
	Exit:
			return error;
	};	
};
namespace tt_h
{
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_driver_init( ft_structs_h::FT_Module  ttdriver )     /* TT_Driver */
	{
#ifdef TT_USE_BYTECODE_INTERPRETER
		TT_Driver  driver = (TT_Driver)ttdriver;
		if ( !TT_New_Context( driver ) )
			return ft_error_h::TT_Err_Could_Not_Find_Context;
#else
		FT_UNUSED( ttdriver );
#endif
		return ft_error_h::TT_Err_Ok;
	};
	FT_LOCAL_DEF( void )
		tt_face_free_hdmx( TT_Face  face )
	{
		ft_structs_h::FT_Stream  stream = face->root.stream;
		ft_structs_h::FT_Memory  memory = stream->memory;
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( face->hdmx_record_sizes );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		FT_FRAME_RELEASE( face->hdmx_table );
	};
	FT_LOCAL_DEF( void )
		tt_face_done( ft_structs_h::FT_Face  ttface )           /* TT_Face */
	{
		TT_Face       face = (TT_Face)ttface;
		ft_structs_h::FT_Memory     memory;
		ft_structs_h::FT_Stream     stream;
		SFNT_Service  sfnt;
		if ( !face )
			return;
		memory = ttface->memory;
		stream = ttface->stream;
		sfnt   = (SFNT_Service)face->sfnt;
		/* for `extended TrueType formats' (i.e. compressed versions) */
		if ( face->extra.finalizer )
			face->extra.finalizer( face->extra.data );
		if ( sfnt )
			sfnt->done_face( face );
		/* freeing the locations table */
		tt_face_done_loca( face );
		tt_face_free_hdmx( face );
		/* freeing the CVT */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( face->cvt );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		face->cvt_size = 0;
		/* freeing the programs */
		FT_FRAME_RELEASE( face->font_program );
		FT_FRAME_RELEASE( face->cvt_program );
		face->font_program_size = 0;
		face->cvt_program_size  = 0;
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
		tt_done_blend( memory, face->blend );
		face->blend = NULL;
#endif
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_size_init( ft_structs_h::FT_Size  ttsize )           /* TT_Size */
	{
		TT_Size   size  = (TT_Size)ttsize;
		ft_structs_h::FT_Error  error = ft_error_h::TT_Err_Ok;
#ifdef TT_USE_BYTECODE_INTERPRETER
		size->bytecode_ready = 0;
		size->cvt_ready      = 0;
#endif
		size->ttmetrics.valid = FALSE;
		size->strike_index    = 0xFFFFFFFFUL;
		return error;
	};
	FT_LOCAL_DEF( void )
		tt_size_done( ft_structs_h::FT_Size  ttsize )           /* TT_Size */
	{
		TT_Size  size = (TT_Size)ttsize;
#ifdef TT_USE_BYTECODE_INTERPRETER
		if ( size->bytecode_ready )
			tt_size_done_bytecode( ttsize );
#endif
		size->ttmetrics.valid = FALSE;
	};
	static ft_structs_h::FT_Error
		tt_get_kerning( ft_structs_h::FT_Face     ttface,          /* TT_Face */
		ft_structs_h::FT_UInt     left_glyph,
		ft_structs_h::FT_UInt     right_glyph,
		ft_structs_h::FT_Vector*  kerning )
	{
		TT_Face       face = (TT_Face)ttface;
		SFNT_Service  sfnt = (SFNT_Service)face->sfnt;
		kerning->x = 0;
		kerning->y = 0;
		if ( sfnt )
			kerning->x = sfnt->get_kerning( face, left_glyph, right_glyph );
		return 0;
	};
	static ft_structs_h::FT_Error
		tt_get_advances( ft_structs_h::FT_Face    ttface,
		ft_structs_h::FT_UInt    start,
		ft_structs_h::FT_UInt    count,
		ft_structs_h::FT_Int32   flags,
		ft_structs_h::FT_Fixed  *advances )
	{
		ft_structs_h::FT_UInt  nn;
		TT_Face  face  = (TT_Face) ttface;
		/* XXX: TODO: check for sbits */
		if ( flags & FT_LOAD_VERTICAL_LAYOUT )
		{
			for ( nn = 0; nn < count; nn++ )
			{
				ft_structs_h::FT_Short   tsb;
				ft_structs_h::FT_UShort  ah;
				TT_Get_VMetrics( face, start + nn, &tsb, &ah );
				advances[nn] = ah;
			}
		}
		else
		{
      for ( nn = 0; nn < count; nn++ )
      {
        ft_structs_h::FT_Short   lsb;
        ft_structs_h::FT_UShort  aw;
				TT_Get_HMetrics( face, start + nn, &lsb, &aw );
        advances[nn] = aw;
      }
    }
		return ft_error_h::TT_Err_Ok;
  }
	static ft_structs_h::FT_Error
		tt_size_request( ft_structs_h::FT_Size          size,
		ft_structs_h::FT_Size_Request  req )
	{
		TT_Size   ttsize = (TT_Size)size;
		ft_structs_h::FT_Error  error  = ft_error_h::TT_Err_Ok;
#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
		if ( FT_HAS_FIXED_SIZES( size->face ) )
		{
			TT_Face       ttface = (TT_Face)size->face;
			SFNT_Service  sfnt   = (SFNT_Service) ttface->sfnt;
			FT_ULong      strike_index;
			error = sfnt->set_sbit_strike( ttface, req, &strike_index );
			if ( error )
				ttsize->strike_index = 0xFFFFFFFFUL;
			else
				return tt_size_select( size, strike_index );
		}
#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */
		FT_Request_Metrics( size->face, req );
		if ( FT_IS_SCALABLE( size->face ) )
		{
			error = tt_size_reset( ttsize );
			ttsize->root.metrics = ttsize->metrics;
		}
		return error;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_size_reset( TT_Size  size )
	{
		TT_Face           face;
		ft_structs_h::FT_Error          error = ft_error_h::TT_Err_Ok;
		ft_structs_h::FT_Size_Metrics*  metrics;
		size->ttmetrics.valid = FALSE;
		face = (TT_Face)size->root.face;
		metrics = &size->metrics;
		/* copy the result from base layer */
		*metrics = size->root.metrics;
		if ( metrics->x_ppem < 1 || metrics->y_ppem < 1 )
			return ft_error_h::TT_Err_Invalid_PPem;
    /* This bit flag, if set, indicates that the ppems must be       */
    /* rounded to integers.  Nearly all TrueType fonts have this bit */
    /* set, as hinting won't work really well otherwise.             */
    /*                                                               */
    if ( face->header.Flags & 8 )
    {
			metrics->x_scale = FT_DivFix( metrics->x_ppem << 6,
				face->root.units_per_EM );
			metrics->y_scale = FT_DivFix( metrics->y_ppem << 6,
				face->root.units_per_EM );
			metrics->ascender =
				FT_PIX_ROUND( FT_MulFix( face->root.ascender, metrics->y_scale ) );
      metrics->descender =
				FT_PIX_ROUND( FT_MulFix( face->root.descender, metrics->y_scale ) );
			metrics->height =
				FT_PIX_ROUND( FT_MulFix( face->root.height, metrics->y_scale ) );
			metrics->max_advance =
				FT_PIX_ROUND( FT_MulFix( face->root.max_advance_width,
				metrics->x_scale ) );
		}
		/* compute new transformation */
		if ( metrics->x_ppem >= metrics->y_ppem )
		{
			size->ttmetrics.scale   = metrics->x_scale;
			size->ttmetrics.ppem    = metrics->x_ppem;
			size->ttmetrics.x_ratio = 0x10000L;
			size->ttmetrics.y_ratio = FT_MulDiv( metrics->y_ppem,
				0x10000L,
				metrics->x_ppem );
		}
		else
		{
			size->ttmetrics.scale   = metrics->y_scale;
			size->ttmetrics.ppem    = metrics->y_ppem;
			size->ttmetrics.x_ratio = FT_MulDiv( metrics->x_ppem,
				0x10000L,
				metrics->y_ppem );
			size->ttmetrics.y_ratio = 0x10000L;
    }
#ifdef TT_USE_BYTECODE_INTERPRETER
		size->cvt_ready = 0;
#endif /* TT_USE_BYTECODE_INTERPRETER */
		if ( !error )
			size->ttmetrics.valid = TRUE;
		return error;
	};
	FT_LOCAL_DEF( void )
		TT_Get_HMetrics( TT_Face     face,
		ft_structs_h::FT_UInt     idx,
		ft_structs_h::FT_Short*   lsb,
		ft_structs_h::FT_UShort*  aw )
	{
		( (SFNT_Service)face->sfnt )->get_metrics( face, 0, idx, lsb, aw );
		FT_TRACE5(( "  advance width (font units): %d\n", *aw ));
		FT_TRACE5(( "  left side bearing (font units): %d\n", *lsb ));
  }
	FT_LOCAL_DEF( void )
		TT_Get_VMetrics( TT_Face     face,
		ft_structs_h::FT_UInt     idx,
		ft_structs_h::FT_Short*   tsb,
		ft_structs_h::FT_UShort*  ah )
	{
		if ( face->vertical_info )
			( (SFNT_Service)face->sfnt )->get_metrics( face, 1, idx, tsb, ah );
#if 1             /* Empirically determined, at variance with what MS said */
		else
		{
			*tsb = 0;
			*ah  = face->root.units_per_EM;
		}
#else      /* This is what MS said to do.  It isn't what they do, however. */
		else if ( face->os2.version != 0xFFFFU )
		{
			*tsb = face->os2.sTypoAscender;
      *ah  = face->os2.sTypoAscender - face->os2.sTypoDescender;
    }
    else
    {
      *tsb = face->horizontal.Ascender;
      *ah  = face->horizontal.Ascender - face->horizontal.Descender;
    }
#endif
		FT_TRACE5(( "  advance height (font units): %d\n", *ah ));
    FT_TRACE5(( "  top side bearing (font units): %d\n", *tsb ));
  }
	static ft_structs_h::FT_Error
		tt_glyph_load( ft_structs_h::FT_GlyphSlot  ttslot,      /* TT_GlyphSlot */
		ft_structs_h::FT_Size       ttsize,      /* TT_Size      */
		ft_structs_h::FT_UInt       glyph_index,
		ft_structs_h::FT_Int32      load_flags )
	{
		TT_GlyphSlot  slot = (TT_GlyphSlot)ttslot;
		TT_Size       size = (TT_Size)ttsize;
		ft_structs_h::FT_Face       face = ttslot->face;
		ft_structs_h::FT_Error      error;
		if ( !slot )
			return ft_error_h::TT_Err_Invalid_Slot_Handle;
		if ( !size )
			return ft_error_h::TT_Err_Invalid_Size_Handle;
		if ( !face )
			return ft_error_h::TT_Err_Invalid_Argument;
#ifdef FT_CONFIG_OPTION_INCREMENTAL
		if ( glyph_index >= (FT_UInt)face->num_glyphs &&
			!face->internal->incremental_interface   )
#else
		if ( glyph_index >= (ft_structs_h::FT_UInt)face->num_glyphs )
#endif
			return ft_error_h::TT_Err_Invalid_Argument;
		if ( load_flags & FT_LOAD_NO_HINTING )
		{
			/* both FT_LOAD_NO_HINTING and FT_LOAD_NO_AUTOHINT   */
			/* are necessary to disable hinting for tricky fonts */
			if ( FT_IS_TRICKY( face ) )
				load_flags &= ~FT_LOAD_NO_HINTING;
			if ( load_flags & FT_LOAD_NO_AUTOHINT )
				load_flags |= FT_LOAD_NO_HINTING;
		}
		if ( load_flags & ( FT_LOAD_NO_RECURSE | FT_LOAD_NO_SCALE ) )
		{
			load_flags |= FT_LOAD_NO_BITMAP | FT_LOAD_NO_SCALE;
			if ( !FT_IS_TRICKY( face ) )
				load_flags |= FT_LOAD_NO_HINTING;
		}
		/* now load the glyph outline if necessary */
		error = TT_Load_Glyph( size, slot, glyph_index, load_flags );
		/* force drop-out mode to 2 - irrelevant now */
		/* slot->outline.dropout_mode = 2; */
		return error;
	};
	static ft_structs_h::FT_Error
		tt_loader_init( TT_Loader     loader,
		TT_Size       size,
		TT_GlyphSlot  glyph,
		ft_structs_h::FT_Int32      load_flags,
		ft_structs_h::FT_Bool       glyf_table_only )
	{
		TT_Face    face;
		ft_structs_h::FT_Stream  stream;
		ft_structs_h::FT_Bool    pedantic = FT_BOOL( load_flags & FT_LOAD_PEDANTIC );
		face   = (TT_Face)glyph->face;
		stream = face->root.stream;
		FT_MEM_ZERO( loader, sizeof ( TT_LoaderRec ) );
#ifdef TT_USE_BYTECODE_INTERPRETER
		/* load execution context */
		if ( IS_HINTED( load_flags ) && !glyf_table_only )
    {
      TT_ExecContext  exec;
      ft_structs_h::FT_Bool         grayscale;
			if ( !size->cvt_ready )
			{
				ft_structs_h::FT_Error  error = tt_size_ready_bytecode( size, pedantic );
				if ( error )
					return error;
			}
			/* query new execution context */
			exec = size->debug ? size->context
				: ( (TT_Driver)FT_FACE_DRIVER( face ) )->context;
			if ( !exec )
				return ft_error_h::TT_Err_Could_Not_Find_Context;
			grayscale =
				FT_BOOL( FT_LOAD_TARGET_MODE( load_flags ) != FT_RENDER_MODE_MONO );
			TT_Load_Context( exec, face, size );
			/* a change from mono to grayscale rendering (and vice versa) */
			/* requires a re-execution of the CVT program                 */
			if ( grayscale != exec->grayscale )
			{
				FT_UInt  i;
				FT_TRACE4(( "tt_loader_init: grayscale change,"
					" re-executing `prep' table\n" ));
				exec->grayscale = grayscale;
				for ( i = 0; i < size->cvt_size; i++ )
					size->cvt[i] = FT_MulFix( face->cvt[i], size->ttmetrics.scale );
				tt_size_run_prep( size, pedantic );
      }
			/* see whether the cvt program has disabled hinting */
			if ( exec->GS.instruct_control & 1 )
				load_flags |= FT_LOAD_NO_HINTING;
			/* load default graphics state -- if needed */
			if ( exec->GS.instruct_control & 2 )
				exec->GS = tt_default_graphics_state;
			exec->pedantic_hinting = FT_BOOL( load_flags & FT_LOAD_PEDANTIC );
			loader->exec = exec;
			loader->instructions = exec->glyphIns;
		}
#endif /* TT_USE_BYTECODE_INTERPRETER */
		/* seek to the beginning of the glyph table -- for Type 42 fonts     */
		/* the table might be accessed from a Postscript stream or something */
		/* else...                                                           */
#ifdef FT_CONFIG_OPTION_INCREMENTAL
		if ( face->root.internal->incremental_interface )
			loader->glyf_offset = 0;
		else
#endif
		{
			ft_structs_h::FT_Error  error = face->goto_table( face, TTAG_glyf, stream, 0 );
			if ( error == ft_error_h::TT_Err_Table_Missing )
				loader->glyf_offset = 0;
			else if ( error )
			{
        FT_ERROR(( "tt_loader_init: could not access glyph table\n" ));
        return error;
      }
      else
        loader->glyf_offset = FT_STREAM_POS();
    }
		/* get face's glyph loader */
		if ( !glyf_table_only )
		{
			ft_structs_h::FT_GlyphLoader  gloader = glyph->internal->loader;
			FT_GlyphLoader_Rewind( gloader );
			loader->gloader = gloader;
    }
		loader->load_flags = load_flags;
		loader->face   = (ft_structs_h::FT_Face)face;
    loader->size   = (ft_structs_h::FT_Size)size;
    loader->glyph  = (ft_structs_h::FT_GlyphSlot)glyph;
    loader->stream = stream;
		return ft_error_h::TT_Err_Ok;
  }
	static void
		tt_get_metrics( TT_Loader  loader,
		ft_structs_h::FT_UInt    glyph_index )
	{
		TT_Face  face = (TT_Face)loader->face;
		ft_structs_h::FT_Short   left_bearing = 0, top_bearing = 0;
		ft_structs_h::FT_UShort  advance_width = 0, advance_height = 0;
		TT_Get_HMetrics( face, glyph_index,
			&left_bearing,
			&advance_width );
		TT_Get_VMetrics( face, glyph_index,
			&top_bearing,
			&advance_height );
		loader->left_bearing = left_bearing;
    loader->advance      = advance_width;
    loader->top_bearing  = top_bearing;
    loader->vadvance     = advance_height;
		if ( !loader->linear_def )
    {
      loader->linear_def = 1;
      loader->linear     = advance_width;
    }
  }
	FT_LOCAL_DEF( ft_structs_h::FT_ULong )
		tt_face_get_location( TT_Face   face,
		ft_structs_h::FT_UInt   gindex,
		ft_structs_h::FT_UInt  *asize )
	{
		ft_structs_h::FT_ULong  pos1, pos2;
		ft_structs_h::FT_Byte*  p;
		ft_structs_h::FT_Byte*  p_limit;
		pos1 = pos2 = 0;
		if ( gindex < face->num_locations )
    {
      if ( face->header.Index_To_Loc_Format != 0 )
      {
        p       = face->glyph_locations + gindex * 4;
        p_limit = face->glyph_locations + face->num_locations * 4;
				pos1 = FT_NEXT_ULONG( p );
				pos2 = pos1;
				if ( p + 4 <= p_limit )
					pos2 = FT_NEXT_ULONG( p );
      }
      else
      {
        p       = face->glyph_locations + gindex * 2;
        p_limit = face->glyph_locations + face->num_locations * 2;
				pos1 = FT_NEXT_USHORT( p );
				pos2 = pos1;
				if ( p + 2 <= p_limit )
					pos2 = FT_NEXT_USHORT( p );
				pos1 <<= 1;
        pos2 <<= 1;
      }
    }
		/* Check broken location data */
		if ( pos1 > face->glyf_len )
    {
			FT_TRACE1(( "tt_face_get_location:"
				" too large offset=0x%08lx found for gid=0x%04lx,"
				" exceeding the end of glyf table (0x%08lx)\n",
				pos1, gindex, face->glyf_len ));
			*asize = 0;
			return 0;
		}
		if ( pos2 > face->glyf_len )
    {
			FT_TRACE1(( "tt_face_get_location:"
				" too large offset=0x%08lx found for gid=0x%04lx,"
				" truncate at the end of glyf table (0x%08lx)\n",
				pos2, gindex + 1, face->glyf_len ));
			pos2 = face->glyf_len;
		}
		/* The `loca' table must be ordered; it refers to the length of */
    /* an entry as the difference between the current and the next  */
    /* position.  However, there do exist (malformed) fonts which   */
    /* don't obey this rule, so we are only able to provide an      */
    /* upper bound for the size.                                    */
    /*                                                              */
    /* We get (intentionally) a wrong, non-zero result in case the  */
    /* `glyf' table is missing.                                     */
		if ( pos2 >= pos1 )
			*asize = (ft_structs_h::FT_UInt)( pos2 - pos1 );
		else
			*asize = (ft_structs_h::FT_UInt)( face->glyf_len - pos1 );
		return pos1;
  }
	static void
		tt_prepare_zone( TT_GlyphZone  zone,
		ft_structs_h::FT_GlyphLoad  load,
		ft_structs_h::FT_UInt       start_point,
		ft_structs_h::FT_UInt       start_contour )
	{
		zone->n_points    = (ft_structs_h::FT_UShort)( load->outline.n_points - start_point );
		zone->n_contours  = (ft_structs_h::FT_Short) ( load->outline.n_contours -
			start_contour );
		zone->org         = load->extra_points + start_point;
		zone->cur         = load->outline.points + start_point;
		zone->orus        = load->extra_points2 + start_point;
		zone->tags        = (ft_structs_h::FT_Byte*)load->outline.tags + start_point;
		zone->contours    = (ft_structs_h::FT_UShort*)load->outline.contours + start_contour;
		zone->first_point = (ft_structs_h::FT_UShort)start_point;
	};
	static void
		translate_array( ft_structs_h::FT_UInt     n,
		ft_structs_h::FT_Vector*  coords,
		ft_structs_h::FT_Pos      delta_x,
		ft_structs_h::FT_Pos      delta_y )
	{
		ft_structs_h::FT_UInt  k;
		if ( delta_x )
			for ( k = 0; k < n; k++ )
				coords[k].x += delta_x;
		if ( delta_y )
			for ( k = 0; k < n; k++ )
				coords[k].y += delta_y;
	};
	static ft_structs_h::FT_Error
		TT_Hint_Glyph( TT_Loader  loader,
		ft_structs_h::FT_Bool    is_composite )
	{
		TT_GlyphZone  zone = &loader->zone;
		ft_structs_h::FT_Pos        origin;
#ifdef TT_USE_BYTECODE_INTERPRETER
		FT_UInt       n_ins;
#else
		FT_UNUSED( is_composite );
#endif
#ifdef TT_USE_BYTECODE_INTERPRETER
		if ( loader->glyph->control_len > 0xFFFFL )
		{
      FT_TRACE1(( "TT_Hint_Glyph: too long instructions " ));
      FT_TRACE1(( "(0x%lx byte) is truncated\n",
                 loader->glyph->control_len ));
    }
    n_ins = (FT_UInt)( loader->glyph->control_len );
#endif
		origin = zone->cur[zone->n_points - 4].x;
		origin = FT_PIX_ROUND( origin ) - origin;
		if ( origin )
			translate_array( zone->n_points, zone->cur, origin, 0 );
#ifdef TT_USE_BYTECODE_INTERPRETER
    /* save original point position in org */
    if ( n_ins > 0 )
      FT_ARRAY_COPY( zone->org, zone->cur, zone->n_points );
		/* Reset graphics state. */
		loader->exec->GS = ((TT_Size)loader->size)->GS;
		/* XXX: UNDOCUMENTED! Hinting instructions of a composite glyph */
    /*      completely refer to the (already) hinted subglyphs.     */
    if ( is_composite )
    {
      loader->exec->metrics.x_scale = 1 << 16;
      loader->exec->metrics.y_scale = 1 << 16;
			FT_ARRAY_COPY( zone->orus, zone->cur, zone->n_points );
    }
    else
    {
      loader->exec->metrics.x_scale =
        ((TT_Size)loader->size)->metrics.x_scale;
      loader->exec->metrics.y_scale =
        ((TT_Size)loader->size)->metrics.y_scale;
    }
#endif
		/* round pp2 and pp4 */
		zone->cur[zone->n_points - 3].x =
			FT_PIX_ROUND( zone->cur[zone->n_points - 3].x );
		zone->cur[zone->n_points - 1].y =
			FT_PIX_ROUND( zone->cur[zone->n_points - 1].y );
#ifdef TT_USE_BYTECODE_INTERPRETER
		if ( n_ins > 0 )
    {
      FT_Bool   debug;
      FT_Error  error;
			FT_GlyphLoader  gloader         = loader->gloader;
      FT_Outline      current_outline = gloader->current.outline;
			error = TT_Set_CodeRange( loader->exec, tt_coderange_glyph,
				loader->exec->glyphIns, n_ins );
			if ( error )
				return error;
			loader->exec->is_composite = is_composite;
			loader->exec->pts          = *zone;
			debug = FT_BOOL( !( loader->load_flags & FT_LOAD_NO_SCALE ) &&
				((TT_Size)loader->size)->debug             );
			error = TT_Run_Context( loader->exec, debug );
      if ( error && loader->exec->pedantic_hinting )
        return error;
			/* store drop-out mode in bits 5-7; set bit 2 also as a marker */
			current_outline.tags[0] |=
				( loader->exec->GS.scan_type << 5 ) | FT_CURVE_TAG_HAS_SCANMODE;
		}
#endif
		/* save glyph phantom points */
		if ( !loader->preserve_pps )
		{
      loader->pp1 = zone->cur[zone->n_points - 4];
      loader->pp2 = zone->cur[zone->n_points - 3];
      loader->pp3 = zone->cur[zone->n_points - 2];
      loader->pp4 = zone->cur[zone->n_points - 1];
    }
		return ft_error_h::TT_Err_Ok;
  }
	static ft_structs_h::FT_Error
		TT_Process_Simple_Glyph( TT_Loader  loader )
	{
		ft_structs_h::FT_GlyphLoader  gloader = loader->gloader;
		ft_structs_h::FT_Error        error   = ft_error_h::TT_Err_Ok;
		ft_structs_h::FT_Outline*     outline;
		ft_structs_h::FT_Int          n_points;
		outline  = &gloader->current.outline;
		n_points = outline->n_points;
		/* set phantom points */
		outline->points[n_points    ] = loader->pp1;
    outline->points[n_points + 1] = loader->pp2;
    outline->points[n_points + 2] = loader->pp3;
    outline->points[n_points + 3] = loader->pp4;
		outline->tags[n_points    ] = 0;
    outline->tags[n_points + 1] = 0;
    outline->tags[n_points + 2] = 0;
    outline->tags[n_points + 3] = 0;
		n_points += 4;
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
    if ( ((TT_Face)loader->face)->doblend )
    {
      /* Deltas apply to the unscaled data. */
      FT_Vector*  deltas;
      FT_Memory   memory = loader->face->memory;
      FT_Int      i;
			error = TT_Vary_Get_Glyph_Deltas( (TT_Face)(loader->face),
				loader->glyph_index,
				&deltas,
				n_points );
			if ( error )
				return error;
			for ( i = 0; i < n_points; ++i )
      {
        outline->points[i].x += deltas[i].x;
        outline->points[i].y += deltas[i].y;
      }
			FT_FREE( deltas );
		}
#endif /* TT_CONFIG_OPTION_GX_VAR_SUPPORT */
		if ( IS_HINTED( loader->load_flags ) )
		{
			tt_prepare_zone( &loader->zone, &gloader->current, 0, 0 );
			FT_ARRAY_COPY( loader->zone.orus, loader->zone.cur,
				loader->zone.n_points + 4 );
		}
		/* scale the glyph */
		if ( ( loader->load_flags & FT_LOAD_NO_SCALE ) == 0 )
		{
			ft_structs_h::FT_Vector*  vec     = outline->points;
			ft_structs_h::FT_Vector*  limit   = outline->points + n_points;
			ft_structs_h::FT_Fixed    x_scale = ((TT_Size)loader->size)->metrics.x_scale;
			ft_structs_h::FT_Fixed    y_scale = ((TT_Size)loader->size)->metrics.y_scale;
			for ( ; vec < limit; vec++ )
			{
        vec->x = FT_MulFix( vec->x, x_scale );
        vec->y = FT_MulFix( vec->y, y_scale );
      }
			loader->pp1 = outline->points[n_points - 4];
      loader->pp2 = outline->points[n_points - 3];
      loader->pp3 = outline->points[n_points - 2];
      loader->pp4 = outline->points[n_points - 1];
    }
		if ( IS_HINTED( loader->load_flags ) )
		{
			loader->zone.n_points += 4;
			error = TT_Hint_Glyph( loader, 0 );
		}
		return error;
  }
	static ft_structs_h::FT_Error
		TT_Process_Composite_Component( TT_Loader    loader,
		ft_structs_h::FT_SubGlyph  subglyph,
		ft_structs_h::FT_UInt      start_point,
		ft_structs_h::FT_UInt      num_base_points )
	{
		ft_structs_h::FT_GlyphLoader  gloader    = loader->gloader;
    ft_structs_h::FT_Vector*      base_vec   = gloader->base.outline.points;
    ft_structs_h::FT_UInt         num_points = gloader->base.outline.n_points;
    ft_structs_h::FT_Bool         have_scale;
    ft_structs_h::FT_Pos          x, y;
		have_scale = FT_BOOL( subglyph->flags & ( WE_HAVE_A_SCALE     |
			WE_HAVE_AN_XY_SCALE |
			WE_HAVE_A_2X2       ) );
		/* perform the transform required for this subglyph */
		if ( have_scale )
		{
			ft_structs_h::FT_UInt  i;
			for ( i = num_base_points; i < num_points; i++ )
				FT_Vector_Transform( base_vec + i, &subglyph->transform );
		}
		/* get offset */
		if ( !( subglyph->flags & ARGS_ARE_XY_VALUES ) )
		{
			ft_structs_h::FT_UInt     k = subglyph->arg1;
			ft_structs_h::FT_UInt     l = subglyph->arg2;
			ft_structs_h::FT_Vector*  p1;
			ft_structs_h::FT_Vector*  p2;
			/* match l-th point of the newly loaded component to the k-th point */
      /* of the previously loaded components.                             */
      /* change to the point numbers used by our outline */
      k += start_point;
      l += num_base_points;
			if ( k >= num_base_points ||
				l >= num_points      )
				return ft_error_h::TT_Err_Invalid_Composite;
			p1 = gloader->base.outline.points + k;
			p2 = gloader->base.outline.points + l;
			x = p1->x - p2->x;
      y = p1->y - p2->y;
    }
    else
    {
      x = subglyph->arg1;
      y = subglyph->arg2;
			if ( !x && !y )
				return ft_error_h::TT_Err_Ok;
			/* Use a default value dependent on                                     */
			/* TT_CONFIG_OPTION_COMPONENT_OFFSET_SCALED.  This is useful for old TT */
			/* fonts which don't set the xxx_COMPONENT_OFFSET bit.                  */
			if ( have_scale &&
#ifdef TT_CONFIG_OPTION_COMPONENT_OFFSET_SCALED
				!( subglyph->flags & UNSCALED_COMPONENT_OFFSET ) )
#else
				( subglyph->flags & SCALED_COMPONENT_OFFSET ) )
#endif
			{
#if 0
  /*************************************************************************/
  /*                                                                       */
  /* This algorithm is what Apple documents.  But it doesn't work.         */
  /*                                                                       */
				int  a = subglyph->transform.xx > 0 ?  subglyph->transform.xx
					: -subglyph->transform.xx;
				int  b = subglyph->transform.yx > 0 ?  subglyph->transform.yx
					: -subglyph->transform.yx;
				int  c = subglyph->transform.xy > 0 ?  subglyph->transform.xy
					: -subglyph->transform.xy;
				int  d = subglyph->transform.yy > 0 ? subglyph->transform.yy
					: -subglyph->transform.yy;
				int  m = a > b ? a : b;
				int  n = c > d ? c : d;
				if ( a - b <= 33 && a - b >= -33 )
					m *= 2;
				if ( c - d <= 33 && c - d >= -33 )
					n *= 2;
				x = FT_MulFix( x, m );
				y = FT_MulFix( y, n );
#else /* 0 */
  /*************************************************************************/
  /*                                                                       */
  /* This algorithm is a guess and works much better than the above.       */
  /*                                                                      */
				ft_structs_h::FT_Fixed  mac_xscale = FT_SqrtFixed(
					(ft_structs_h::FT_Int32)FT_MulFix( subglyph->transform.xx,
					subglyph->transform.xx ) +
					(ft_structs_h::FT_Int32)FT_MulFix( subglyph->transform.xy,
					subglyph->transform.xy ) );
				ft_structs_h::FT_Fixed  mac_yscale = FT_SqrtFixed(
					(ft_structs_h::FT_Int32)FT_MulFix( subglyph->transform.yy,
					subglyph->transform.yy ) +
					(ft_structs_h::FT_Int32)FT_MulFix( subglyph->transform.yx,
					subglyph->transform.yx ) );
				x = FT_MulFix( x, mac_xscale );
				y = FT_MulFix( y, mac_yscale );
#endif /* 0 */
			}
			if ( !( loader->load_flags & FT_LOAD_NO_SCALE ) )
			{
				ft_structs_h::FT_Fixed  x_scale = ((TT_Size)loader->size)->metrics.x_scale;
				ft_structs_h::FT_Fixed  y_scale = ((TT_Size)loader->size)->metrics.y_scale;
				x = FT_MulFix( x, x_scale );
				y = FT_MulFix( y, y_scale );
				if ( subglyph->flags & ROUND_XY_TO_GRID )
				{
					x = FT_PIX_ROUND( x );
					y = FT_PIX_ROUND( y );
				}
      }
    }
		if ( x || y )
			translate_array( num_points - num_base_points,
			base_vec + num_base_points,
			x, y );
		return ft_error_h::TT_Err_Ok;
  }
	static ft_structs_h::FT_Error
		TT_Process_Composite_Glyph( TT_Loader  loader,
		ft_structs_h::FT_UInt    start_point,
		ft_structs_h::FT_UInt    start_contour )
	{
		ft_structs_h::FT_Error     error;
		ft_structs_h::FT_Outline*  outline;
		ft_structs_h::FT_UInt      i;
		outline = &loader->gloader->base.outline;
		/* make room for phantom points */
		error = FT_GLYPHLOADER_CHECK_POINTS( loader->gloader,
			outline->n_points + 4,
			0 );
		if ( error )
			return error;
		outline->points[outline->n_points    ] = loader->pp1;
		outline->points[outline->n_points + 1] = loader->pp2;
    outline->points[outline->n_points + 2] = loader->pp3;
    outline->points[outline->n_points + 3] = loader->pp4;
		outline->tags[outline->n_points    ] = 0;
    outline->tags[outline->n_points + 1] = 0;
    outline->tags[outline->n_points + 2] = 0;
    outline->tags[outline->n_points + 3] = 0;
#ifdef TT_USE_BYTECODE_INTERPRETER
		{
			FT_Stream  stream = loader->stream;
      FT_UShort  n_ins, max_ins;
      FT_ULong   tmp;
			/* TT_Load_Composite_Glyph only gives us the offset of instructions */
			/* so we read them here                                             */
			if ( FT_STREAM_SEEK( loader->ins_pos ) ||
				FT_READ_USHORT( n_ins )           )
				return error;
			FT_TRACE5(( "  Instructions size = %d\n", n_ins ));
			/* check it */
      max_ins = ((TT_Face)loader->face)->max_profile.maxSizeOfInstructions;
			if ( n_ins > max_ins )
			{
				/* acroread ignores this field, so we only do a rough safety check */
				if ( (FT_Int)n_ins > loader->byte_len )
				{
					FT_TRACE1(( "TT_Process_Composite_Glyph: "
						"too many instructions (%d) for glyph with length %d\n",
						n_ins, loader->byte_len ));
					return ft_error_h::TT_Err_Too_Many_Hints;
				}
				tmp = loader->exec->glyphSize;
				error = Update_Max( loader->exec->memory,
					&tmp,
					sizeof ( FT_Byte ),
					(void*)&loader->exec->glyphIns,
					n_ins );
				loader->exec->glyphSize = (FT_UShort)tmp;
				if ( error )
					return error;
			}
			else if ( n_ins == 0 )
				return ft_error_h::TT_Err_Ok;
			if ( FT_STREAM_READ( loader->exec->glyphIns, n_ins ) )
				return error;
			loader->glyph->control_data = loader->exec->glyphIns;
			loader->glyph->control_len  = n_ins;
    }
#endif
		tt_prepare_zone( &loader->zone, &loader->gloader->base,
			start_point, start_contour );
		/* Some points are likely touched during execution of  */
		/* instructions on components.  So let's untouch them. */
		for ( i = start_point; i < loader->zone.n_points; i++ )
			loader->zone.tags[i] &= ~FT_CURVE_TAG_TOUCH_BOTH;
		loader->zone.n_points += 4;
		return TT_Hint_Glyph( loader, 1 );
  }
	static ft_structs_h::FT_Error
		load_truetype_glyph( TT_Loader  loader,
		ft_structs_h::FT_UInt    glyph_index,
		ft_structs_h::FT_UInt    recurse_count,
		ft_structs_h::FT_Bool    header_only )
	{
		ft_structs_h::FT_Error        error        = ft_error_h::TT_Err_Ok;
    ft_structs_h::FT_Fixed        x_scale, y_scale;
    ft_structs_h::FT_ULong        offset;
    TT_Face         face         = (TT_Face)loader->face;
    ft_structs_h::FT_GlyphLoader  gloader      = loader->gloader;
    ft_structs_h::FT_Bool         opened_frame = 0;
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
		ft_structs_h::FT_Vector*      deltas       = NULL;
#endif
#ifdef FT_CONFIG_OPTION_INCREMENTAL
		ft_structs_h::FT_StreamRec    inc_stream;
		FT_Data         glyph_data;
		FT_Bool         glyph_data_loaded = 0;
#endif
		/* some fonts have an incorrect value of `maxComponentDepth', */
		/* thus we allow depth 1 to catch the majority of them        */
		if ( recurse_count > 1                                   &&
			recurse_count > face->max_profile.maxComponentDepth )
		{
			error = ft_error_h::TT_Err_Invalid_Composite;
			goto Exit;
		}
		/* check glyph index */
		if ( glyph_index >= (ft_structs_h::FT_UInt)face->root.num_glyphs )
		{
			error = ft_error_h::TT_Err_Invalid_Glyph_Index;
			goto Exit;
		}
		loader->glyph_index = glyph_index;
		if ( ( loader->load_flags & FT_LOAD_NO_SCALE ) == 0 )
		{
      x_scale = ((TT_Size)loader->size)->metrics.x_scale;
      y_scale = ((TT_Size)loader->size)->metrics.y_scale;
    }
    else
    {
      x_scale = 0x10000L;
      y_scale = 0x10000L;
    }
		tt_get_metrics( loader, glyph_index );
		/* Set `offset' to the start of the glyph relative to the start of */
    /* the `glyf' table, and `byte_len' to the length of the glyph in  */
    /* bytes.                                                          */
#ifdef FT_CONFIG_OPTION_INCREMENTAL
    /* If we are loading glyph data via the incremental interface, set */
    /* the loader stream to a memory stream reading the data returned  */
    /* by the interface.                                               */
    if ( face->root.internal->incremental_interface )
    {
      error = face->root.internal->incremental_interface->funcs->get_glyph_data(
                face->root.internal->incremental_interface->object,
                glyph_index, &glyph_data );
      if ( error )
        goto Exit;
			glyph_data_loaded = 1;
      offset            = 0;
      loader->byte_len  = glyph_data.length;
			FT_MEM_ZERO( &inc_stream, sizeof ( inc_stream ) );
      FT_Stream_OpenMemory( &inc_stream,
                            glyph_data.pointer, glyph_data.length );
			loader->stream = &inc_stream;
    }
    else
#endif /* FT_CONFIG_OPTION_INCREMENTAL */
			offset = tt_face_get_location( face, glyph_index,
			(ft_structs_h::FT_UInt*)&loader->byte_len );
		if ( loader->byte_len > 0 )
		{
#ifdef FT_CONFIG_OPTION_INCREMENTAL
      /* for the incremental interface, `glyf_offset' is always zero */
      if ( !loader->glyf_offset                        &&
           !face->root.internal->incremental_interface )
#else
      if ( !loader->glyf_offset )
#endif /* FT_CONFIG_OPTION_INCREMENTAL */
      {
        FT_TRACE2(( "no `glyf' table but non-zero `loca' entry\n" ));
        error = ft_error_h::TT_Err_Invalid_Table;
        goto Exit;
      }
			error = face->access_glyph_frame( loader, glyph_index,
				loader->glyf_offset + offset,
				loader->byte_len );
			if ( error )
				goto Exit;
			opened_frame = 1;
			/* read glyph header first */
			error = face->read_glyph_header( loader );
			if ( error || header_only )
				goto Exit;
		}
		if ( loader->byte_len == 0 || loader->n_contours == 0 )
    {
      loader->bbox.xMin = 0;
      loader->bbox.xMax = 0;
      loader->bbox.yMin = 0;
      loader->bbox.yMax = 0;
			if ( header_only )
				goto Exit;
			/* must initialize points before (possibly) overriding */
			/* glyph metrics from the incremental interface        */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
			TT_LOADER_SET_PP( loader );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
#ifdef FT_CONFIG_OPTION_INCREMENTAL
      tt_get_metrics_incr_overrides( loader, glyph_index );
#endif
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
			if ( ((TT_Face)(loader->face))->doblend )
      {
        /* this must be done before scaling */
        FT_Memory  memory = loader->face->memory;
				error = TT_Vary_Get_Glyph_Deltas( (TT_Face)(loader->face),
					glyph_index, &deltas, 4 );
				if ( error )
					goto Exit;
				loader->pp1.x += deltas[0].x; loader->pp1.y += deltas[0].y;
        loader->pp2.x += deltas[1].x; loader->pp2.y += deltas[1].y;
        loader->pp3.x += deltas[2].x; loader->pp3.y += deltas[2].y;
        loader->pp4.x += deltas[3].x; loader->pp4.y += deltas[3].y;
				FT_FREE( deltas );
			}
#endif
			if ( ( loader->load_flags & FT_LOAD_NO_SCALE ) == 0 )
			{
				loader->pp1.x = FT_MulFix( loader->pp1.x, x_scale );
        loader->pp2.x = FT_MulFix( loader->pp2.x, x_scale );
        loader->pp3.y = FT_MulFix( loader->pp3.y, y_scale );
        loader->pp4.y = FT_MulFix( loader->pp4.y, y_scale );
      }
			error = ft_error_h::TT_Err_Ok;
      goto Exit;
    }
		/* must initialize points before (possibly) overriding */
    /* glyph metrics from the incremental interface        */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
    TT_LOADER_SET_PP( loader );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
#ifdef FT_CONFIG_OPTION_INCREMENTAL
		tt_get_metrics_incr_overrides( loader, glyph_index );
#endif
		/***********************************************************************/
    /***********************************************************************/
    /***********************************************************************/
		/* if it is a simple glyph, load it */
		if ( loader->n_contours > 0 )
		{
			error = face->read_simple_glyph( loader );
      if ( error )
				goto Exit;
			/* all data have been read */
			face->forget_glyph_frame( loader );
			opened_frame = 0;
			error = TT_Process_Simple_Glyph( loader );
			if ( error )
				goto Exit;
			FT_GlyphLoader_Add( gloader );
		}
		/***********************************************************************/
    /***********************************************************************/
    /***********************************************************************/
		/* otherwise, load a composite! */
		else if ( loader->n_contours == -1 )
    {
      ft_structs_h::FT_UInt   start_point;
      ft_structs_h::FT_UInt   start_contour;
      ft_structs_h::FT_ULong  ins_pos;  /* position of composite instructions, if any */
			start_point   = gloader->base.outline.n_points;
			start_contour = gloader->base.outline.n_contours;
			/* for each subglyph, read composite header */
      error = face->read_composite_glyph( loader );
			if ( error )
				goto Exit;
			/* store the offset of instructions */
			ins_pos = loader->ins_pos;
			/* all data we need are read */
			face->forget_glyph_frame( loader );
			opened_frame = 0;
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
			if ( face->doblend )
      {
        FT_Int       i, limit;
        FT_SubGlyph  subglyph;
        FT_Memory    memory = face->root.memory;
				/* this provides additional offsets */
				/* for each component's translation */
				if ( ( error = TT_Vary_Get_Glyph_Deltas(
					face,
					glyph_index,
					&deltas,
					gloader->current.num_subglyphs + 4 )) != 0 )
					goto Exit;
				subglyph = gloader->current.subglyphs + gloader->base.num_subglyphs;
				limit    = gloader->current.num_subglyphs;
				for ( i = 0; i < limit; ++i, ++subglyph )
        {
          if ( subglyph->flags & ARGS_ARE_XY_VALUES )
          {
            /* XXX: overflow check for subglyph->{arg1,arg2}.   */
            /* deltas[i].{x,y} must be within signed 16-bit,    */
            /* but the restriction of summed delta is not clear */
            subglyph->arg1 += (FT_Int16)deltas[i].x;
            subglyph->arg2 += (FT_Int16)deltas[i].y;
          }
        }
				loader->pp1.x += deltas[i + 0].x; loader->pp1.y += deltas[i + 0].y;
        loader->pp2.x += deltas[i + 1].x; loader->pp2.y += deltas[i + 1].y;
        loader->pp3.x += deltas[i + 2].x; loader->pp3.y += deltas[i + 2].y;
        loader->pp4.x += deltas[i + 3].x; loader->pp4.y += deltas[i + 3].y;
				FT_FREE( deltas );
      }
#endif /* TT_CONFIG_OPTION_GX_VAR_SUPPORT */
			if ( ( loader->load_flags & FT_LOAD_NO_SCALE ) == 0 )
      {
        loader->pp1.x = FT_MulFix( loader->pp1.x, x_scale );
        loader->pp2.x = FT_MulFix( loader->pp2.x, x_scale );
        loader->pp3.y = FT_MulFix( loader->pp3.y, y_scale );
        loader->pp4.y = FT_MulFix( loader->pp4.y, y_scale );
      }
			/* if the flag FT_LOAD_NO_RECURSE is set, we return the subglyph */
      /* `as is' in the glyph slot (the client application will be     */
      /* responsible for interpreting these data)...                   */
      if ( loader->load_flags & FT_LOAD_NO_RECURSE )
      {
        FT_GlyphLoader_Add( gloader );
        loader->glyph->format = ft_structs_h::FT_GLYPH_FORMAT_COMPOSITE;
				goto Exit;
      }
      /*********************************************************************/
      /*********************************************************************/
      /*********************************************************************/
			{
				ft_structs_h::FT_UInt      n, num_base_points;
				ft_structs_h::FT_SubGlyph  subglyph       = 0;
				ft_structs_h::FT_UInt      num_points     = start_point;
        ft_structs_h::FT_UInt      num_subglyphs  = gloader->current.num_subglyphs;
        ft_structs_h::FT_UInt      num_base_subgs = gloader->base.num_subglyphs;
				ft_structs_h::FT_Stream    old_stream     = loader->stream;
        ft_structs_h::FT_Int       old_byte_len   = loader->byte_len;
				FT_GlyphLoader_Add( gloader );
				/* read each subglyph independently */
				for ( n = 0; n < num_subglyphs; n++ )
				{
					ft_structs_h::FT_Vector  pp[4];
					/* Each time we call load_truetype_glyph in this loop, the   */
          /* value of `gloader.base.subglyphs' can change due to table */
          /* reallocations.  We thus need to recompute the subglyph    */
          /* pointer on each iteration.                                */
          subglyph = gloader->base.subglyphs + num_base_subgs + n;
					pp[0] = loader->pp1;
          pp[1] = loader->pp2;
          pp[2] = loader->pp3;
          pp[3] = loader->pp4;
					num_base_points = gloader->base.outline.n_points;
					error = load_truetype_glyph( loader, subglyph->index,
						recurse_count + 1, FALSE );
					if ( error )
						goto Exit;
					/* restore subglyph pointer */
					subglyph = gloader->base.subglyphs + num_base_subgs + n;
					if ( !( subglyph->flags & USE_MY_METRICS ) )
          {
            loader->pp1 = pp[0];
            loader->pp2 = pp[1];
            loader->pp3 = pp[2];
            loader->pp4 = pp[3];
          }
					num_points = gloader->base.outline.n_points;
					if ( num_points == num_base_points )
						continue;
					/* gloader->base.outline consists of three parts:               */
          /* 0 -(1)-> start_point -(2)-> num_base_points -(3)-> n_points. */
          /*                                                              */
          /* (1): exists from the beginning                               */
          /* (2): components that have been loaded so far                 */
          /* (3): the newly loaded component                              */
          TT_Process_Composite_Component( loader, subglyph, start_point,
						num_base_points );
        }
				loader->stream   = old_stream;
				loader->byte_len = old_byte_len;
				/* process the glyph */
				loader->ins_pos = ins_pos;
				if ( IS_HINTED( loader->load_flags ) &&
#ifdef TT_USE_BYTECODE_INTERPRETER
					subglyph->flags & WE_HAVE_INSTR &&
#endif
					num_points > start_point )
					TT_Process_Composite_Glyph( loader, start_point, start_contour );
			}
		}
    else
    {
      /* invalid composite count (negative but not -1) */
      error = ft_error_h::TT_Err_Invalid_Outline;
      goto Exit;
    }
		/***********************************************************************/
    /***********************************************************************/
    /***********************************************************************/
Exit:
if ( opened_frame )
	face->forget_glyph_frame( loader );
#ifdef FT_CONFIG_OPTION_INCREMENTAL
if ( glyph_data_loaded )
	face->root.internal->incremental_interface->funcs->free_glyph_data(
	face->root.internal->incremental_interface->object,
	&glyph_data );
#endif
return error;
}
FT_LOCAL_DEF( ft_structs_h::FT_Byte* )
	tt_face_get_device_metrics( TT_Face  face,
	ft_structs_h::FT_UInt  ppem,
	ft_structs_h::FT_UInt  gindex )
{
	ft_structs_h::FT_UInt   nn;
	ft_structs_h::FT_Byte*  result      = NULL;
	ft_structs_h::FT_ULong  record_size = face->hdmx_record_size;
	ft_structs_h::FT_Byte*  record      = face->hdmx_table + 8;
	for ( nn = 0; nn < face->hdmx_record_count; nn++ )
		if ( face->hdmx_record_sizes[nn] == ppem )
		{
			gindex += 2;
			if ( gindex < record_size )
				result = record + nn * record_size + gindex;
			break;
		}
	return result;
}
static ft_structs_h::FT_Error
	compute_glyph_metrics( TT_Loader  loader,
	ft_structs_h::FT_UInt    glyph_index )
{
	ft_structs_h::FT_BBox       bbox;
	TT_Face       face = (TT_Face)loader->face;
	ft_structs_h::FT_Fixed      y_scale;
	TT_GlyphSlot  glyph = loader->glyph;
	TT_Size       size = (TT_Size)loader->size;
	y_scale = 0x10000L;
	if ( ( loader->load_flags & FT_LOAD_NO_SCALE ) == 0 )
		y_scale = size->root.metrics.y_scale;
	if ( glyph->format != ft_structs_h::FT_GLYPH_FORMAT_COMPOSITE )
		FT_Outline_Get_CBox( &glyph->outline, &bbox );
	else
		bbox = loader->bbox;
	/* get the device-independent horizontal advance; it is scaled later */
	/* by the base layer.                                                */
	glyph->linearHoriAdvance = loader->linear;
	glyph->metrics.horiBearingX = bbox.xMin;
	glyph->metrics.horiBearingY = bbox.yMax;
	glyph->metrics.horiAdvance  = loader->pp2.x - loader->pp1.x;
	/* adjust advance width to the value contained in the hdmx table */
	if ( !face->postscript.isFixedPitch  &&
		IS_HINTED( loader->load_flags ) )
	{
		ft_structs_h::FT_Byte*  widthp;
		widthp = tt_face_get_device_metrics( face,
			size->root.metrics.x_ppem,
			glyph_index );
		if ( widthp )
			glyph->metrics.horiAdvance = *widthp << 6;
	}
	/* set glyph dimensions */
	glyph->metrics.width  = bbox.xMax - bbox.xMin;
	glyph->metrics.height = bbox.yMax - bbox.yMin;
	/* Now take care of vertical metrics.  In the case where there is */
	/* no vertical information within the font (relatively common),   */
	/* create some metrics manually                                   */
	{
		ft_structs_h::FT_Pos  top;      /* scaled vertical top side bearing  */
		ft_structs_h::FT_Pos  advance;  /* scaled vertical advance height    */
		/* Get the unscaled top bearing and advance height. */
		if ( face->vertical_info                   &&
			face->vertical.number_Of_VMetrics > 0 )
		{
			top = (ft_structs_h::FT_Short)FT_DivFix( loader->pp3.y - bbox.yMax,
				y_scale );
			if ( loader->pp3.y <= loader->pp4.y )
				advance = 0;
			else
				advance = (ft_structs_h::FT_UShort)FT_DivFix( loader->pp3.y - loader->pp4.y,
				y_scale );
		}
		else
		{
			ft_structs_h::FT_Pos  height;
			/* XXX Compute top side bearing and advance height in  */
			/*     Get_VMetrics instead of here.                   */
			/* NOTE: The OS/2 values are the only `portable' ones, */
			/*       which is why we use them, if there is an OS/2 */
			/*       table in the font.  Otherwise, we use the     */
			/*       values defined in the horizontal header.      */
			height = (ft_structs_h::FT_Short)FT_DivFix( bbox.yMax - bbox.yMin,
				y_scale );
			if ( face->os2.version != 0xFFFFU )
				advance = (ft_structs_h::FT_Pos)( face->os2.sTypoAscender -
				face->os2.sTypoDescender );
			else
				advance = (ft_structs_h::FT_Pos)( face->horizontal.Ascender -
				face->horizontal.Descender );
			top = ( advance - height ) / 2;
		}
#ifdef FT_CONFIG_OPTION_INCREMENTAL
		{
			FT_Incremental_InterfaceRec*  incr;
			FT_Incremental_MetricsRec     metrics;
			FT_Error                      error;
			incr = face->root.internal->incremental_interface;
			/* If this is an incrementally loaded font see if there are */
			/* overriding metrics for this glyph.                       */
			if ( incr && incr->funcs->get_glyph_metrics )
			{
				metrics.bearing_x = 0;
				metrics.bearing_y = top;
				metrics.advance   = advance;
				error = incr->funcs->get_glyph_metrics( incr->object,
					glyph_index,
					TRUE,
					&metrics );
				if ( error )
					return error;
				top     = metrics.bearing_y;
				advance = metrics.advance;
			}
		}
		/* GWW: Do vertical metrics get loaded incrementally too? */
#endif /* FT_CONFIG_OPTION_INCREMENTAL */
		glyph->linearVertAdvance = advance;
		/* scale the metrics */
		if ( !( loader->load_flags & FT_LOAD_NO_SCALE ) )
		{
			top     = FT_MulFix( top,     y_scale );
			advance = FT_MulFix( advance, y_scale );
		}
		/* XXX: for now, we have no better algorithm for the lsb, but it */
		/*      should work fine.                                        */
		/*                                                               */
		glyph->metrics.vertBearingX = glyph->metrics.horiBearingX -
			glyph->metrics.horiAdvance / 2;
		glyph->metrics.vertBearingY = top;
		glyph->metrics.vertAdvance  = advance;
	}
	return 0;
};
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
  FT_DEFINE_SERVICE_MULTIMASTERSREC(tt_service_gx_multi_masters,
    (FT_Get_MM_Func)        NULL,
    (FT_Set_MM_Design_Func) NULL,
    (FT_Set_MM_Blend_Func)  TT_Set_MM_Blend,
    (FT_Get_MM_Var_Func)    TT_Get_MM_Var,
    (FT_Set_Var_Design_Func)TT_Set_Var_Design
  )
#endif

  static const FT_Service_TrueTypeEngineRec  tt_service_truetype_engine =
  {
#ifdef TT_USE_BYTECODE_INTERPRETER

#ifdef TT_CONFIG_OPTION_UNPATENTED_HINTING
    FT_TRUETYPE_ENGINE_TYPE_UNPATENTED
#else
    FT_TRUETYPE_ENGINE_TYPE_PATENTED
#endif

#else /* !TT_USE_BYTECODE_INTERPRETER */

    FT_TRUETYPE_ENGINE_TYPE_NONE

#endif /* TT_USE_BYTECODE_INTERPRETER */
  };

  FT_DEFINE_SERVICE_TTGLYFREC(tt_service_truetype_glyf,
    (TT_Glyf_GetLocationFunc)tt_face_get_location
  )

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
  FT_DEFINE_SERVICEDESCREC4(tt_services,
    FT_SERVICE_ID_XF86_NAME,       FT_XF86_FORMAT_TRUETYPE,
    FT_SERVICE_ID_MULTI_MASTERS,   &FT_TT_SERVICE_GX_MULTI_MASTERS_GET,
    FT_SERVICE_ID_TRUETYPE_ENGINE, &tt_service_truetype_engine,
    FT_SERVICE_ID_TT_GLYF,         &FT_TT_SERVICE_TRUETYPE_GLYF_GET
  )
#else
  FT_DEFINE_SERVICEDESCREC3(tt_services,
    FT_SERVICE_ID_XF86_NAME,       FT_XF86_FORMAT_TRUETYPE,
    FT_SERVICE_ID_TRUETYPE_ENGINE, &tt_service_truetype_engine,
    FT_SERVICE_ID_TT_GLYF,         &FT_TT_SERVICE_TRUETYPE_GLYF_GET
  )
#endif
FT_CALLBACK_DEF( ft_structs_h::FT_Module_Interface )
	tt_get_interface( ft_structs_h::FT_Module    driver,    /* TT_Driver */
	const char*  tt_interface )
{
	ft_structs_h::FT_Library           library;
	ft_structs_h::FT_Module_Interface  result;
	ft_structs_h::FT_Module            sfntd;
	SFNT_Service         sfnt;
	/* FT_TT_SERVICES_GET derefers `library' in PIC mode */
#ifdef FT_CONFIG_OPTION_PIC
	if ( !driver )
		return NULL;
	library = driver->library;
	if ( !library )
		return NULL;
#endif
	result = ft_service_list_lookup( FT_TT_SERVICES_GET, tt_interface );
	if ( result != NULL )
		return result;
#ifndef FT_CONFIG_OPTION_PIC
	if ( !driver )
		return NULL;
	library = driver->library;
	if ( !library )
		return NULL;
#endif
	/* only return the default interface from the SFNT module */
	sfntd = FT_Get_Module( library, "sfnt" );
	if ( sfntd )
	{
		sfnt = (SFNT_Service)( sfntd->clazz->module_interface );
		if ( sfnt )
			return sfnt->get_interface( driver, tt_interface );
	}
	return 0;
};
	static ft_structs_h::FT_Bool
		tt_check_trickyness_family( ft_structs_h::FT_String*  name )
	{
#define TRICK_NAMES_MAX_CHARACTERS  16
#define TRICK_NAMES_COUNT            8
		static const char trick_names[TRICK_NAMES_COUNT]
		[TRICK_NAMES_MAX_CHARACTERS + 1] =
		{
			"DFKaiSho-SB",     /* dfkaisb.ttf */
			"DFKaiShu",
			"DFKai-SB",        /* kaiu.ttf */
			"HuaTianKaiTi?",   /* htkt2.ttf */
			"HuaTianSongTi?",  /* htst3.ttf */
			"MingLiU",         /* mingliu.ttf & mingliu.ttc */
			"PMingLiU",        /* mingliu.ttc */
			"MingLi43",        /* mingli.ttf */
		};
		int  nn;
		for ( nn = 0; nn < TRICK_NAMES_COUNT; nn++ )
			if ( ft_strstr( name, trick_names[nn] ) )
				return TRUE;
		return FALSE;
	};
	static ft_structs_h::FT_UInt32
		tt_synth_sfnt_checksum( ft_structs_h::FT_Stream  stream,
		ft_structs_h::FT_ULong   length )
	{
		ft_structs_h::FT_Error   error;
		ft_structs_h::FT_UInt32  checksum = 0;
		int        i;
		if ( FT_FRAME_ENTER( length ) )
			return 0;
		for ( ; length > 3; length -= 4 )
			checksum += (ft_structs_h::FT_UInt32)FT_GET_ULONG();
		for ( i = 3; length > 0; length --, i-- )
			checksum += (ft_structs_h::FT_UInt32)( FT_GET_BYTE() << ( i * 8 ) );
		FT_FRAME_EXIT();
		return checksum;
	};
	static ft_structs_h::FT_ULong
		tt_get_sfnt_checksum( TT_Face    face,
		ft_structs_h::FT_UShort  i )
	{
#if 0 /* if we believe the written value, use following part. */
		if ( face->dir_tables[i].CheckSum )
			return face->dir_tables[i].CheckSum;
#endif
		if ( !face->goto_table )
			return 0;
		if ( face->goto_table( face,
			face->dir_tables[i].Tag,                      
			face->root.stream,
			NULL ) )
			return 0;
		return (ft_structs_h::FT_ULong)tt_synth_sfnt_checksum( face->root.stream,
			face->dir_tables[i].Length );
	};
	typedef struct tt_sfnt_id_rec_
	{
		ft_structs_h::FT_ULong  CheckSum;
		ft_structs_h::FT_ULong  Length;
	} tt_sfnt_id_rec;
	static ft_structs_h::FT_Bool
		tt_check_trickyness_sfnt_ids( TT_Face  face )
	{
#define TRICK_SFNT_IDS_PER_FACE   3
#define TRICK_SFNT_IDS_NUM_FACES  17
		static const tt_sfnt_id_rec sfnt_id[TRICK_SFNT_IDS_NUM_FACES]
		[TRICK_SFNT_IDS_PER_FACE] = {
#define TRICK_SFNT_ID_cvt   0
#define TRICK_SFNT_ID_fpgm  1
#define TRICK_SFNT_ID_prep  2
			{ /* MingLiU 1995 */
				{ 0x05bcf058, 0x000002e4 }, /* cvt  */
        { 0x28233bf1, 0x000087c4 }, /* fpgm */
        { 0xa344a1ea, 0x000001e1 }  /* prep */
      },
      { /* MingLiU 1996- */
        { 0x05bcf058, 0x000002e4 }, /* cvt  */
        { 0x28233bf1, 0x000087c4 }, /* fpgm */
        { 0xa344a1eb, 0x000001e1 }  /* prep */
      },
      { /* DFKaiShu */
        { 0x11e5ead4, 0x00000350 }, /* cvt  */
        { 0x5a30ca3b, 0x00009063 }, /* fpgm */
        { 0x13a42602, 0x0000007e }  /* prep */
      },
      { /* HuaTianKaiTi */
        { 0xfffbfffc, 0x00000008 }, /* cvt  */
        { 0x9c9e48b8, 0x0000bea2 }, /* fpgm */
        { 0x70020112, 0x00000008 }  /* prep */
      },
      { /* HuaTianSongTi */
        { 0xfffbfffc, 0x00000008 }, /* cvt  */
        { 0x0a5a0483, 0x00017c39 }, /* fpgm */
        { 0x70020112, 0x00000008 }  /* prep */
      },
      { /* NEC fadpop7.ttf */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x40c92555, 0x000000e5 }, /* fpgm */
        { 0xa39b58e3, 0x0000117c }  /* prep */
      },
      { /* NEC fadrei5.ttf */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x33c41652, 0x000000e5 }, /* fpgm */
        { 0x26d6c52a, 0x00000f6a }  /* prep */
      },
      { /* NEC fangot7.ttf */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x6db1651d, 0x0000019d }, /* fpgm */
        { 0x6c6e4b03, 0x00002492 }  /* prep */
      },
      { /* NEC fangyo5.ttf */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x40c92555, 0x000000e5 }, /* fpgm */
        { 0xde51fad0, 0x0000117c }  /* prep */
      },
      { /* NEC fankyo5.ttf */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x85e47664, 0x000000e5 }, /* fpgm */
        { 0xa6c62831, 0x00001caa }  /* prep */
      },
      { /* NEC fanrgo5.ttf */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x2d891cfd, 0x0000019d }, /* fpgm */
        { 0xa0604633, 0x00001de8 }  /* prep */
      },
      { /* NEC fangot5.ttc */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x40aa774c, 0x000001cb }, /* fpgm */
        { 0x9b5caa96, 0x00001f9a }  /* prep */
      },
      { /* NEC fanmin3.ttc */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x0d3de9cb, 0x00000141 }, /* fpgm */
        { 0xd4127766, 0x00002280 }  /* prep */
      },
      { /* NEC FA-Gothic, 1996 */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x4a692698, 0x000001f0 }, /* fpgm */
        { 0x340d4346, 0x00001fca }  /* prep */
      },
      { /* NEC FA-Minchou, 1996 */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0xcd34c604, 0x00000166 }, /* fpgm */
        { 0x6cf31046, 0x000022b0 }  /* prep */
      },
      { /* NEC FA-RoundGothicB, 1996 */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0x5da75315, 0x0000019d }, /* fpgm */
        { 0x40745a5f, 0x000022e0 }  /* prep */
      },
      { /* NEC FA-RoundGothicM, 1996 */
        { 0x00000000, 0x00000000 }, /* cvt  */
        { 0xf055fc48, 0x000001c2 }, /* fpgm */
        { 0x3900ded3, 0x00001e18 }  /* prep */
      }
    };
		ft_structs_h::FT_ULong   checksum;
		int        num_matched_ids[TRICK_SFNT_IDS_NUM_FACES];
		ft_structs_h::FT_Bool    has_cvt, has_fpgm, has_prep;
		ft_structs_h::FT_UShort  i;
    int        j, k;
		FT_MEM_SET( num_matched_ids, 0,
			sizeof ( int ) * TRICK_SFNT_IDS_NUM_FACES );
		has_cvt  = FALSE;
		has_fpgm = FALSE;
		has_prep = FALSE;
		for ( i = 0; i < face->num_tables; i++ )
		{
			checksum = 0;
      switch( face->dir_tables[i].Tag )
      {
      case TTAG_cvt:
        k = TRICK_SFNT_ID_cvt;
        has_cvt  = TRUE;
        break;
			case TTAG_fpgm:
        k = TRICK_SFNT_ID_fpgm;
        has_fpgm = TRUE;
        break;
			case TTAG_prep:
        k = TRICK_SFNT_ID_prep;
        has_prep = TRUE;
        break;
			default:
        continue;
      }
			for ( j = 0; j < TRICK_SFNT_IDS_NUM_FACES; j++ )
				if ( face->dir_tables[i].Length == sfnt_id[j][k].Length )
        {
          if ( !checksum )
            checksum = tt_get_sfnt_checksum( face, i );
					if ( sfnt_id[j][k].CheckSum == checksum )
						num_matched_ids[j]++;
					if ( num_matched_ids[j] == TRICK_SFNT_IDS_PER_FACE )
            return TRUE;
        }
    }
		for ( j = 0; j < TRICK_SFNT_IDS_NUM_FACES; j++ )
    {
      if ( !has_cvt  && !sfnt_id[j][TRICK_SFNT_ID_cvt].Length )
        num_matched_ids[j] ++;
      if ( !has_fpgm && !sfnt_id[j][TRICK_SFNT_ID_fpgm].Length )
        num_matched_ids[j] ++;
      if ( !has_prep && !sfnt_id[j][TRICK_SFNT_ID_prep].Length )
        num_matched_ids[j] ++;
      if ( num_matched_ids[j] == TRICK_SFNT_IDS_PER_FACE )
        return TRUE;
    }
		return FALSE;
	};
	static ft_structs_h::FT_Bool
		tt_check_trickyness( ft_structs_h::FT_Face  face )
	{
		if ( !face )
			return FALSE;
		/* For first, check the face name for quick check. */
		if ( face->family_name                               &&
			tt_check_trickyness_family( face->family_name ) )
			return TRUE;
		/* Type42 fonts may lack `name' tables, we thus try to identify */
		/* tricky fonts by checking the checksums of Type42-persistent  */
		/* sfnt tables (`cvt', `fpgm', and `prep').                     */
		if ( tt_check_trickyness_sfnt_ids( (TT_Face)face ) )
			return TRUE;
		return FALSE;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_hdmx( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		ft_structs_h::FT_Error   error;
		ft_structs_h::FT_Memory  memory = stream->memory;
		ft_structs_h::FT_UInt    version, nn, num_records;
		ft_structs_h::FT_ULong   table_size, record_size;
		ft_structs_h::FT_Byte*   p;
		ft_structs_h::FT_Byte*   limit;
		/* this table is optional */
		error = face->goto_table( face, TTAG_hdmx, stream, &table_size );
		if ( error || table_size < 8 )
			return ft_error_h::TT_Err_Ok;
		if ( FT_FRAME_EXTRACT( table_size, face->hdmx_table ) )
			goto Exit;
		p     = face->hdmx_table;
		limit = p + table_size;
		version     = FT_NEXT_USHORT( p );
		num_records = FT_NEXT_USHORT( p );
		record_size = FT_NEXT_ULONG( p );
		/* The maximum number of bytes in an hdmx device record is the */
    /* maximum number of glyphs + 2; this is 0xFFFF + 2; this is   */
    /* the reason why `record_size' is a long (which we read as    */
    /* unsigned long for convenience).  In practice, two bytes     */
    /* sufficient to hold the size value.                          */
    /*                                                             */
    /* There are at least two fonts, HANNOM-A and HANNOM-B version */
    /* 2.0 (2005), which get this wrong: The upper two bytes of    */
    /* the size value are set to 0xFF instead of 0x00.  We catch   */
    /* and fix this.                                               */
		if ( record_size >= 0xFFFF0000UL )
			record_size &= 0xFFFFU;
		/* The limit for `num_records' is a heuristic value. */
		if ( version != 0 || num_records > 255 || record_size > 0x10001L )
		{
			error = ft_error_h::TT_Err_Invalid_File_Format;
			goto Fail;
		}
		if ( FT_NEW_ARRAY( face->hdmx_record_sizes, num_records ) )
			goto Fail;
		for ( nn = 0; nn < num_records; nn++ )
		{
			if ( p + record_size > limit )
				break;
			face->hdmx_record_sizes[nn] = p[0];
			p                          += record_size;
		}
		face->hdmx_record_count = nn;
    face->hdmx_table_size   = table_size;
    face->hdmx_record_size  = record_size;
Exit:
		return error;
Fail:
		FT_FRAME_RELEASE( face->hdmx_table );
		face->hdmx_table_size = 0;
		goto Exit;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_fpgm( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
#ifdef TT_USE_BYTECODE_INTERPRETER
		FT_Error  error;
		FT_ULong  table_len;
		FT_TRACE2(( "Font program " ));
		/* The font program is optional */
		error = face->goto_table( face, TTAG_fpgm, stream, &table_len );
    if ( error )
    {
      face->font_program      = NULL;
      face->font_program_size = 0;
      error                   = TT_Err_Ok;
			FT_TRACE2(( "is missing\n" ));
    }
    else
    {
      face->font_program_size = table_len;
      if ( FT_FRAME_EXTRACT( table_len, face->font_program ) )
        goto Exit;
			FT_TRACE2(( "loaded, %12d bytes\n", face->font_program_size ));
		}
Exit:
		return error;
#else /* !TT_USE_BYTECODE_INTERPRETER */
		FT_UNUSED( face   );
		FT_UNUSED( stream );
		return ft_error_h::TT_Err_Ok;
#endif
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_prep( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
#ifdef TT_USE_BYTECODE_INTERPRETER
		FT_Error  error;
		FT_ULong  table_len;
		FT_TRACE2(( "Prep program " ));
		error = face->goto_table( face, TTAG_prep, stream, &table_len );
		if ( error )
		{
			face->cvt_program      = NULL;
			face->cvt_program_size = 0;
			error                  = TT_Err_Ok;
			FT_TRACE2(( "is missing\n" ));
		}
    else
    {
      face->cvt_program_size = table_len;
      if ( FT_FRAME_EXTRACT( table_len, face->cvt_program ) )
        goto Exit;
			FT_TRACE2(( "loaded, %12d bytes\n", face->cvt_program_size ));
		}
Exit:
		return error;
#else /* !TT_USE_BYTECODE_INTERPRETER */
		FT_UNUSED( face   );
		FT_UNUSED( stream );
		return ft_error_h::TT_Err_Ok;
#endif
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_cvt( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
#ifdef TT_USE_BYTECODE_INTERPRETER
		FT_Error   error;
		FT_Memory  memory = stream->memory;
		FT_ULong   table_len;
		FT_TRACE2(( "CVT " ));
		error = face->goto_table( face, TTAG_cvt, stream, &table_len );
		if ( error )
		{
			FT_TRACE2(( "is missing\n" ));
			face->cvt_size = 0;
			face->cvt      = NULL;
      error          = TT_Err_Ok;
			goto Exit;
    }
		face->cvt_size = table_len / 2;
		if ( FT_NEW_ARRAY( face->cvt, face->cvt_size ) )
			goto Exit;
		if ( FT_FRAME_ENTER( face->cvt_size * 2L ) )
			goto Exit;
		{
			FT_Short*  cur   = face->cvt;
			FT_Short*  limit = cur + face->cvt_size;
			for ( ; cur < limit; cur++ )
				*cur = FT_GET_SHORT();
		}
		FT_FRAME_EXIT();
    FT_TRACE2(( "loaded\n" ));
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
		if ( face->doblend )
			error = tt_face_vary_cvt( face, stream );
#endif
Exit:
		return error;
#else /* !TT_USE_BYTECODE_INTERPRETER */
		FT_UNUSED( face   );
		FT_UNUSED( stream );
		return ft_error_h::TT_Err_Ok;
#endif
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_loca( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		ft_structs_h::FT_Error  error;
		ft_structs_h::FT_ULong  table_len;
		ft_structs_h::FT_Int    shift;
		/* we need the size of the `glyf' table for malformed `loca' tables */
		error = face->goto_table( face, TTAG_glyf, stream, &face->glyf_len );
		/* it is possible that a font doesn't have a glyf table at all */
		/* or its size is zero                                         */
		if ( error == ft_error_h::TT_Err_Table_Missing )
			face->glyf_len = 0;
		else if ( error )
			goto Exit;
		FT_TRACE2(( "Locations " ));
		error = face->goto_table( face, TTAG_loca, stream, &table_len );
		if ( error )
    {
      error = ft_error_h::TT_Err_Locations_Missing;
      goto Exit;
    }
		if ( face->header.Index_To_Loc_Format != 0 )
		{
			shift = 2;
			if ( table_len >= 0x40000L )
			{
				FT_TRACE2(( "table too large\n" ));
				error = ft_error_h::TT_Err_Invalid_Table;
				goto Exit;
			}
			face->num_locations = table_len >> shift;
    }
    else
    {
      shift = 1;
			if ( table_len >= 0x20000L )
      {
        FT_TRACE2(( "table too large\n" ));
        error = ft_error_h::TT_Err_Invalid_Table;
        goto Exit;
      }
      face->num_locations = table_len >> shift;
    }
		if ( face->num_locations != (ft_structs_h::FT_ULong)face->root.num_glyphs + 1 )
		{
			FT_TRACE2(( "glyph count mismatch!  loca: %d, maxp: %d\n",
				face->num_locations - 1, face->root.num_glyphs ));
			/* we only handle the case where `maxp' gives a larger value */
			if ( face->num_locations <= (ft_structs_h::FT_ULong)face->root.num_glyphs )
			{
				ft_structs_h::FT_Long   new_loca_len =
					( (ft_structs_h::FT_Long)( face->root.num_glyphs ) + 1 ) << shift;
				TT_Table  entry = face->dir_tables;
				TT_Table  limit = entry + face->num_tables;
				ft_structs_h::FT_Long   pos  = FT_Stream_Pos( stream );
        ft_structs_h::FT_Long   dist = 0x7FFFFFFFL;
				/* compute the distance to next table in font file */
				for ( ; entry < limit; entry++ )
				{
					ft_structs_h::FT_Long  diff = entry->Offset - pos;
					if ( diff > 0 && diff < dist )
						dist = diff;
				}
				if ( entry == limit )
        {
          /* `loca' is the last table */
          dist = stream->size - pos;
        }
				if ( new_loca_len <= dist )
        {
          face->num_locations = face->root.num_glyphs + 1;
          table_len           = new_loca_len;
					FT_TRACE2(( "adjusting num_locations to %d\n",
						face->num_locations ));
				}
      }
    }
		/*
		* Extract the frame.  We don't need to decompress it since
		* we are able to parse it directly.
		*/
		if ( FT_FRAME_EXTRACT( table_len, face->glyph_locations ) )
			goto Exit;
		FT_TRACE2(( "loaded\n" ));
Exit:
		return error;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		TT_Access_Glyph_Frame( TT_Loader  loader,
		ft_structs_h::FT_UInt    glyph_index,
		ft_structs_h::FT_ULong   offset,
		ft_structs_h::FT_UInt    byte_count )
	{
		ft_structs_h::FT_Error   error;
		ft_structs_h::FT_Stream  stream = loader->stream;
		/* for non-debug mode */
		FT_UNUSED( glyph_index );
		FT_TRACE4(( "Glyph %ld\n", glyph_index ));
		/* the following line sets the `error' variable through macros! */
		if ( FT_STREAM_SEEK( offset ) || FT_FRAME_ENTER( byte_count ) )
			return error;
		loader->cursor = stream->cursor;
		loader->limit  = stream->limit;
		return ft_error_h::TT_Err_Ok;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		TT_Load_Glyph_Header( TT_Loader  loader )
	{
		ft_structs_h::FT_Byte*  p     = loader->cursor;
		ft_structs_h::FT_Byte*  limit = loader->limit;
		if ( p + 10 > limit )
			return ft_error_h::TT_Err_Invalid_Outline;
		loader->n_contours = FT_NEXT_SHORT( p );
		loader->bbox.xMin = FT_NEXT_SHORT( p );
    loader->bbox.yMin = FT_NEXT_SHORT( p );
    loader->bbox.xMax = FT_NEXT_SHORT( p );
    loader->bbox.yMax = FT_NEXT_SHORT( p );
		FT_TRACE5(( "  # of contours: %d\n", loader->n_contours ));
		FT_TRACE5(( "  xMin: %4d  xMax: %4d\n", loader->bbox.xMin,
			loader->bbox.xMax ));
		FT_TRACE5(( "  yMin: %4d  yMax: %4d\n", loader->bbox.yMin,
			loader->bbox.yMax ));
		loader->cursor = p;
		return ft_error_h::TT_Err_Ok;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		TT_Load_Simple_Glyph( TT_Loader  load )
	{
		ft_structs_h::FT_Error        error;
		ft_structs_h::FT_Byte*        p          = load->cursor;
		ft_structs_h::FT_Byte*        limit      = load->limit;
		ft_structs_h::FT_GlyphLoader  gloader    = load->gloader;
		ft_structs_h::FT_Int          n_contours = load->n_contours;
		ft_structs_h::FT_Outline*     outline;
		TT_Face         face       = (TT_Face)load->face;
		ft_structs_h::FT_UShort       n_ins;
		ft_structs_h::FT_Int          n_points;
		ft_structs_h::FT_Byte         *flag, *flag_limit;
    ft_structs_h::FT_Byte         c, count;
    ft_structs_h::FT_Vector       *vec, *vec_limit;
    ft_structs_h::FT_Pos          x;
    ft_structs_h::FT_Short        *cont, *cont_limit, prev_cont;
    ft_structs_h::FT_Int          xy_size = 0;
		/* check that we can add the contours to the glyph */
    error = FT_GLYPHLOADER_CHECK_POINTS( gloader, 0, n_contours );
    if ( error )
      goto Fail;
    /* reading the contours' endpoints & number of points */
    cont       = gloader->current.outline.contours;
    cont_limit = cont + n_contours;
		/* check space for contours array + instructions count */
    if ( n_contours >= 0xFFF || p + ( n_contours + 1 ) * 2 > limit )
      goto Invalid_Outline;
		prev_cont = FT_NEXT_SHORT( p );
		if ( n_contours > 0 )
      cont[0] = prev_cont;
		if ( prev_cont < 0 )
      goto Invalid_Outline;
		for ( cont++; cont < cont_limit; cont++ )
    {
      cont[0] = FT_NEXT_SHORT( p );
      if ( cont[0] <= prev_cont )
      {
        /* unordered contours: this is invalid */
        goto Invalid_Outline;
      }
      prev_cont = cont[0];
    }
		n_points = 0;
    if ( n_contours > 0 )
    {
      n_points = cont[-1] + 1;
      if ( n_points < 0 )
        goto Invalid_Outline;
    }
		/* note that we will add four phantom points later */
    error = FT_GLYPHLOADER_CHECK_POINTS( gloader, n_points + 4, 0 );
    if ( error )
      goto Fail;
		/* reading the bytecode instructions */
    load->glyph->control_len  = 0;
    load->glyph->control_data = 0;
		if ( p + 2 > limit )
      goto Invalid_Outline;
		n_ins = FT_NEXT_USHORT( p );
		FT_TRACE5(( "  Instructions size: %u\n", n_ins ));
		if ( n_ins > face->max_profile.maxSizeOfInstructions )
    {
      FT_TRACE0(( "TT_Load_Simple_Glyph: too many instructions (%d)\n",
                  n_ins ));
      error = ft_error_h::TT_Err_Too_Many_Hints;
      goto Fail;
    }
		if ( ( limit - p ) < n_ins )
    {
      FT_TRACE0(( "TT_Load_Simple_Glyph: instruction count mismatch\n" ));
      error = ft_error_h::TT_Err_Too_Many_Hints;
      goto Fail;
    }
#ifdef TT_USE_BYTECODE_INTERPRETER
		if ( IS_HINTED( load->load_flags ) )
    {
      load->glyph->control_len  = n_ins;
      load->glyph->control_data = load->exec->glyphIns;

      FT_MEM_COPY( load->exec->glyphIns, p, (FT_Long)n_ins );
    }
#endif /* TT_USE_BYTECODE_INTERPRETER */
		p += n_ins;
		outline = &gloader->current.outline;
		/* reading the point tags */
		flag       = (ft_structs_h::FT_Byte*)outline->tags;
		flag_limit = flag + n_points;
		FT_ASSERT( flag != NULL );
		while ( flag < flag_limit )
    {
      if ( p + 1 > limit )
        goto Invalid_Outline;
			*flag++ = c = FT_NEXT_BYTE( p );
      if ( c & 8 )
      {
        if ( p + 1 > limit )
          goto Invalid_Outline;
				count = FT_NEXT_BYTE( p );
				if ( flag + (ft_structs_h::FT_Int)count > flag_limit )
					goto Invalid_Outline;
				for ( ; count > 0; count-- )
					*flag++ = c;
      }
    }
		/* reading the X coordinates */
		vec       = outline->points;
		vec_limit = vec + n_points;
		flag      = (ft_structs_h::FT_Byte*)outline->tags;
		x         = 0;
		if ( p + xy_size > limit )
			goto Invalid_Outline;
		for ( ; vec < vec_limit; vec++, flag++ )
    {
      ft_structs_h::FT_Pos   y = 0;
      ft_structs_h::FT_Byte  f = *flag;
			if ( f & 2 )
      {
        if ( p + 1 > limit )
          goto Invalid_Outline;
				y = (ft_structs_h::FT_Pos)FT_NEXT_BYTE( p );
        if ( ( f & 16 ) == 0 )
          y = -y;
      }
      else if ( ( f & 16 ) == 0 )
      {
        if ( p + 2 > limit )
          goto Invalid_Outline;
				y = (ft_structs_h::FT_Pos)FT_NEXT_SHORT( p );
      }
			x     += y;
      vec->x = x;
      /* the cast is for stupid compilers */
      *flag  = (ft_structs_h::FT_Byte)( f & ~( 2 | 16 ) );
    }
		/* reading the Y coordinates */
		vec       = gloader->current.outline.points;
    vec_limit = vec + n_points;
    flag      = (ft_structs_h::FT_Byte*)outline->tags;
    x         = 0;
		for ( ; vec < vec_limit; vec++, flag++ )
    {
      ft_structs_h::FT_Pos   y = 0;
      ft_structs_h::FT_Byte  f = *flag;
			if ( f & 4 )
      {
        if ( p + 1 > limit )
          goto Invalid_Outline;
				y = (ft_structs_h::FT_Pos)FT_NEXT_BYTE( p );
        if ( ( f & 32 ) == 0 )
          y = -y;
      }
      else if ( ( f & 32 ) == 0 )
      {
        if ( p + 2 > limit )
          goto Invalid_Outline;
				y = (ft_structs_h::FT_Pos)FT_NEXT_SHORT( p );
      }
			x     += y;
      vec->y = x;
      /* the cast is for stupid compilers */
      *flag  = (ft_structs_h::FT_Byte)( f & FT_CURVE_TAG_ON );
    }
		outline->n_points   = (ft_structs_h::FT_UShort)n_points;
		outline->n_contours = (ft_structs_h::FT_Short) n_contours;
		load->cursor = p;
Fail:
		return error;
Invalid_Outline:
		error = ft_error_h::TT_Err_Invalid_Outline;
		goto Fail;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		TT_Load_Composite_Glyph( TT_Loader  loader )
	{
		ft_structs_h::FT_Error        error;
    ft_structs_h::FT_Byte*        p       = loader->cursor;
    ft_structs_h::FT_Byte*        limit   = loader->limit;
    ft_structs_h::FT_GlyphLoader  gloader = loader->gloader;
    ft_structs_h::FT_SubGlyph     subglyph;
    ft_structs_h::FT_UInt         num_subglyphs;
		num_subglyphs = 0;
		do
    {
      ft_structs_h::FT_Fixed  xx, xy, yy, yx;
      ft_structs_h::FT_UInt   count;
			/* check that we can load a new subglyph */
      error = FT_GlyphLoader_CheckSubGlyphs( gloader, num_subglyphs + 1 );
      if ( error )
        goto Fail;
			/* check space */
      if ( p + 4 > limit )
        goto Invalid_Composite;
			subglyph = gloader->current.subglyphs + num_subglyphs;
			subglyph->arg1 = subglyph->arg2 = 0;
			subglyph->flags = FT_NEXT_USHORT( p );
      subglyph->index = FT_NEXT_USHORT( p );
			/* check space */
      count = 2;
      if ( subglyph->flags & ARGS_ARE_WORDS )
        count += 2;
      if ( subglyph->flags & WE_HAVE_A_SCALE )
        count += 2;
      else if ( subglyph->flags & WE_HAVE_AN_XY_SCALE )
        count += 4;
      else if ( subglyph->flags & WE_HAVE_A_2X2 )
        count += 8;
			if ( p + count > limit )
				goto Invalid_Composite;
			/* read arguments */
      if ( subglyph->flags & ARGS_ARE_WORDS )
      {
        subglyph->arg1 = FT_NEXT_SHORT( p );
        subglyph->arg2 = FT_NEXT_SHORT( p );
      }
      else
      {
        subglyph->arg1 = FT_NEXT_CHAR( p );
        subglyph->arg2 = FT_NEXT_CHAR( p );
      }
			/* read transform */
			xx = yy = 0x10000L;
			xy = yx = 0;
			if ( subglyph->flags & WE_HAVE_A_SCALE )
      {
        xx = (ft_structs_h::FT_Fixed)FT_NEXT_SHORT( p ) << 2;
        yy = xx;
      }
      else if ( subglyph->flags & WE_HAVE_AN_XY_SCALE )
      {
        xx = (ft_structs_h::FT_Fixed)FT_NEXT_SHORT( p ) << 2;
        yy = (ft_structs_h::FT_Fixed)FT_NEXT_SHORT( p ) << 2;
      }
      else if ( subglyph->flags & WE_HAVE_A_2X2 )
      {
        xx = (ft_structs_h::FT_Fixed)FT_NEXT_SHORT( p ) << 2;
        yx = (ft_structs_h::FT_Fixed)FT_NEXT_SHORT( p ) << 2;
        xy = (ft_structs_h::FT_Fixed)FT_NEXT_SHORT( p ) << 2;
        yy = (ft_structs_h::FT_Fixed)FT_NEXT_SHORT( p ) << 2;
      }
			subglyph->transform.xx = xx;
      subglyph->transform.xy = xy;
      subglyph->transform.yx = yx;
      subglyph->transform.yy = yy;
			num_subglyphs++;
		} while ( subglyph->flags & MORE_COMPONENTS );
		gloader->current.num_subglyphs = num_subglyphs;
#ifdef TT_USE_BYTECODE_INTERPRETER
		{
			FT_Stream  stream = loader->stream;
			/* we must undo the FT_FRAME_ENTER in order to point */
      /* to the composite instructions, if we find some.   */
      /* We will process them later.                       */
      /*                                                   */
      loader->ins_pos = (FT_ULong)( FT_STREAM_POS() +
				p - limit );
		}
#endif
		loader->cursor = p;
Fail:
		return error;
Invalid_Composite:
		error = ft_error_h::TT_Err_Invalid_Composite;
		goto Fail;
	};
	FT_CALLBACK_DEF( void )
		TT_Forget_Glyph_Frame( TT_Loader  loader )
	{
		ft_structs_h::FT_Stream  stream = loader->stream;
		FT_FRAME_EXIT();
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_slot_init( ft_structs_h::FT_GlyphSlot  slot )
	{
		return FT_GlyphLoader_CreateExtra( slot->internal->loader );
	};
	FT_LOCAL_DEF( void )
		tt_driver_done( ft_structs_h::FT_Module  ttdriver )     /* TT_Driver */
	{
#ifdef TT_USE_BYTECODE_INTERPRETER
		TT_Driver  driver = (TT_Driver)ttdriver;
		/* destroy the execution context */
		if ( driver->context )
		{
			TT_Done_Context( driver->context );
			driver->context = NULL;
		}
#else
    FT_UNUSED( ttdriver );
#endif
	};
	FT_LOCAL_DEF( void )
		tt_face_done_loca( TT_Face  face )
	{
		ft_structs_h::FT_Stream  stream = face->root.stream;
		FT_FRAME_RELEASE( face->glyph_locations );
		face->num_locations = 0;
	};
	FT_LOCAL_DEF( void )
		TT_Init_Glyph_Loading( TT_Face  face )
	{
		face->access_glyph_frame   = TT_Access_Glyph_Frame;
		face->read_glyph_header    = TT_Load_Glyph_Header;
		face->read_simple_glyph    = TT_Load_Simple_Glyph;
		face->read_composite_glyph = TT_Load_Composite_Glyph;
		face->forget_glyph_frame   = TT_Forget_Glyph_Frame;
	};
	static ft_structs_h::FT_Bool
		tt_check_single_notdef( ft_structs_h::FT_Face  ttface )
	{
		ft_structs_h::FT_Bool   result = FALSE;
		TT_Face   face = (TT_Face)ttface;
    ft_structs_h::FT_UInt   asize;
    ft_structs_h::FT_ULong  i;
    ft_structs_h::FT_ULong  glyph_index = 0;
    ft_structs_h::FT_UInt   count       = 0;
		for( i = 0; i < face->num_locations; i++ )
    {
      tt_face_get_location( face, i, &asize );
      if ( asize > 0 )
      {
        count += 1;
        if ( count > 1 )
          break;
        glyph_index = i;
      }
    }
		/* Only have a single outline. */
		if ( count == 1 )
    {
      if ( glyph_index == 0 )
        result = TRUE;
      else
      {
        /* FIXME: Need to test glyphname == .notdef ? */
        ft_structs_h::FT_Error error;
        char buf[8];
				error = FT_Get_Glyph_Name( ttface, glyph_index, buf, 8 );
				if ( !error                                            &&
					buf[0] == '.' && !ft_strncmp( buf, ".notdef", 8 ) )
					result = TRUE;
			}
		}
		return result;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_init( ft_structs_h::FT_Stream      stream,
		ft_structs_h::FT_Face        ttface,      /* TT_Face */
		ft_structs_h::FT_Int         face_index,
		ft_structs_h::FT_Int         num_params,
		ft_structs_h::FT_Parameter*  params )
	{
		ft_structs_h::FT_Error      error;
		ft_structs_h::FT_Library    library;
		SFNT_Service  sfnt;
		TT_Face       face = (TT_Face)ttface;
		FT_TRACE2(( "TTF driver\n" ));
		library = ttface->driver->root.library;
		sfnt = (SFNT_Service)FT_Get_Module_Interface( library, "sfnt" );
		if ( !sfnt )
    {
			FT_ERROR(( "tt_face_init: cannot access `sfnt' module\n" ));
			error = ft_error_h::TT_Err_Missing_Module;
			goto Exit;
		}
		/* create input stream from resource */
		if ( FT_STREAM_SEEK( 0 ) )
			goto Exit;
		/* check that we have a valid TrueType file */
		error = sfnt->init_face( stream, face, face_index, num_params, params );
		if ( error )
			goto Exit;
		/* We must also be able to accept Mac/GX fonts, as well as OT ones. */
    /* The 0x00020000 tag is completely undocumented; some fonts from   */
    /* Arphic made for Chinese Windows 3.1 have this.                   */
    if ( face->format_tag != 0x00010000L &&    /* MS fonts  */
         face->format_tag != 0x00020000L &&    /* CJK fonts for Win 3.1 */
         face->format_tag != TTAG_true   )     /* Mac fonts */
    {
      FT_TRACE2(( "  not a TTF font\n" ));
      goto Bad_Format;
    }
#ifdef TT_USE_BYTECODE_INTERPRETER
    ttface->face_flags |= FT_FACE_FLAG_HINTER;
#endif
		/* If we are performing a simple font format check, exit immediately. */
    if ( face_index < 0 )
      return ft_error_h::TT_Err_Ok;
		/* Load font directory */
    error = sfnt->load_face( stream, face, face_index, num_params, params );
    if ( error )
      goto Exit;
		if ( tt_check_trickyness( ttface ) )
			ttface->face_flags |= FT_FACE_FLAG_TRICKY;
		error = tt_face_load_hdmx( face, stream );
		if ( error )
			goto Exit;
		if ( FT_IS_SCALABLE( ttface ) )
		{
#ifdef FT_CONFIG_OPTION_INCREMENTAL
			if ( !ttface->internal->incremental_interface )
        error = tt_face_load_loca( face, stream );
      if ( !error )
        error = tt_face_load_cvt( face, stream );
      if ( !error )
        error = tt_face_load_fpgm( face, stream );
      if ( !error )
        error = tt_face_load_prep( face, stream );
			/* Check the scalable flag based on `loca'. */
			if ( !ttface->internal->incremental_interface &&
				ttface->num_fixed_sizes                  &&
				face->glyph_locations                    &&
				tt_check_single_notdef( ttface )         )
			{
				FT_TRACE5(( "tt_face_init:"
					" Only the `.notdef' glyph has an outline.\n"
					"             "
					" Resetting scalable flag to FALSE.\n" ));
				ttface->face_flags &= ~FT_FACE_FLAG_SCALABLE;
			}
#else
			if ( !error )
				error = tt_face_load_loca( face, stream );
			if ( !error )
        error = tt_face_load_cvt( face, stream );
      if ( !error )
        error = tt_face_load_fpgm( face, stream );
      if ( !error )
        error = tt_face_load_prep( face, stream );
			/* Check the scalable flag based on `loca'. */
      if ( ttface->num_fixed_sizes          &&
				face->glyph_locations            &&
				tt_check_single_notdef( ttface ) )
			{
				FT_TRACE5(( "tt_face_init:"
					" Only the `.notdef' glyph has an outline.\n"
					"             "
					" Resetting scalable flag to FALSE.\n" ));
				ttface->face_flags &= ~FT_FACE_FLAG_SCALABLE;
      }
#endif
		}
#if defined( TT_CONFIG_OPTION_UNPATENTED_HINTING    ) && \
	!defined( TT_CONFIG_OPTION_BYTECODE_INTERPRETER )
		{
			FT_Bool  unpatented_hinting;
			int      i;
			/* Determine whether unpatented hinting is to be used for this face. */
			unpatented_hinting = FT_BOOL
				( library->debug_hooks[FT_DEBUG_HOOK_UNPATENTED_HINTING] != NULL );
			for ( i = 0; i < num_params && !face->unpatented_hinting; i++ )
				if ( params[i].tag == FT_PARAM_TAG_UNPATENTED_HINTING )
					unpatented_hinting = TRUE;
			if ( !unpatented_hinting )
				ttface->internal->ignore_unpatented_hinter = TRUE;
		}
#endif /* TT_CONFIG_OPTION_UNPATENTED_HINTING &&
		!TT_CONFIG_OPTION_BYTECODE_INTERPRETER */
		/* initialize standard glyph loading routines */
		TT_Init_Glyph_Loading( face );
Exit:
		return error;
Bad_Format:
		error = ft_error_h::TT_Err_Unknown_File_Format;
    goto Exit;
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_goto_table( TT_Face    face,
		ft_structs_h::FT_ULong   tag,
		ft_structs_h::FT_Stream  stream,
		ft_structs_h::FT_ULong*  length )
	{
		TT_Table  table;
		ft_structs_h::FT_Error  error;
		table = tt_face_lookup_table( face, tag );
		if ( table )
		{
			if ( length )
				*length = table->Length;
			if ( FT_STREAM_SEEK( table->Offset ) )
				goto Exit;
		}
		else
			error = ft_error_h::SFNT_Err_Table_Missing;
Exit:
		return error;
	}
	FT_LOCAL_DEF( TT_Table  )
		tt_face_lookup_table( TT_Face   face,
		ft_structs_h::FT_ULong  tag  )
	{
		TT_Table  entry;
		TT_Table  limit;
#ifdef FT_DEBUG_LEVEL_TRACE
		FT_Bool   zero_length = FALSE;
#endif
		FT_TRACE4(( "tt_face_lookup_table: %08p, `%c%c%c%c' -- ",
			face,
			(ft_structs_h::FT_Char)( tag >> 24 ),
			(ft_structs_h::FT_Char)( tag >> 16 ),
			(ft_structs_h::FT_Char)( tag >> 8  ),
			(ft_structs_h::FT_Char)( tag       ) ));
		entry = face->dir_tables;
		limit = entry + face->num_tables;
		for ( ; entry < limit; entry++ )
		{
			/* For compatibility with Windows, we consider    */
      /* zero-length tables the same as missing tables. */
      if ( entry->Tag == tag )
      {
        if ( entry->Length != 0 )
        {
          FT_TRACE4(( "found table.\n" ));
          return entry;
        }
#ifdef FT_DEBUG_LEVEL_TRACE
        zero_length = TRUE;
#endif
      }
    }
#ifdef FT_DEBUG_LEVEL_TRACE
		if ( zero_length )
      FT_TRACE4(( "ignoring empty table\n" ));
    else
      FT_TRACE4(( "could not find table\n" ));
#endif
		return NULL;
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_any( TT_Face    face,
		ft_structs_h::FT_ULong   tag,
		ft_structs_h::FT_Long    offset,
		ft_structs_h::FT_Byte*   buffer,
		ft_structs_h::FT_ULong*  length )
	{
		ft_structs_h::FT_Error   error;
    ft_structs_h::FT_Stream  stream;
    TT_Table   table;
    ft_structs_h::FT_ULong   size;
		if ( tag != 0 )
    {
      /* look for tag in font directory */
      table = tt_face_lookup_table( face, tag );
      if ( !table )
      {
        error = ft_error_h::SFNT_Err_Table_Missing;
        goto Exit;
      }
      offset += table->Offset;
      size    = table->Length;
    }
    else
      /* tag == 0 -- the user wants to access the font file directly */
      size = face->root.stream->size;
    if ( length && *length == 0 )
    {
      *length = size;
      return ft_error_h::SFNT_Err_Ok;
    }
    if ( length )
      size = *length;
    stream = face->root.stream;
    /* the `if' is syntactic sugar for picky compilers */
    if ( FT_STREAM_READ_AT( offset, buffer, size ) )
      goto Exit;
Exit:
		return error;
  };
	static ft_structs_h::FT_Error
		tt_face_load_generic_header( TT_Face    face,
		ft_structs_h::FT_Stream  stream,
		ft_structs_h::FT_ULong   tag )
	{
		ft_structs_h::FT_Error    error;
    TT_Header*  header;
		static const ft_structs_h::FT_Frame_Field  header_fields[] =
    {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TT_Header
			FT_FRAME_START( 54 ),
			FT_FRAME_ULONG ( Table_Version ),
			FT_FRAME_ULONG ( Font_Revision ),
			FT_FRAME_LONG  ( CheckSum_Adjust ),
			FT_FRAME_LONG  ( Magic_Number ),
			FT_FRAME_USHORT( Flags ),
			FT_FRAME_USHORT( Units_Per_EM ),
			FT_FRAME_LONG  ( Created[0] ),
			FT_FRAME_LONG  ( Created[1] ),
			FT_FRAME_LONG  ( Modified[0] ),
			FT_FRAME_LONG  ( Modified[1] ),
			FT_FRAME_SHORT ( xMin ),
			FT_FRAME_SHORT ( yMin ),
			FT_FRAME_SHORT ( xMax ),
			FT_FRAME_SHORT ( yMax ),
			FT_FRAME_USHORT( Mac_Style ),
			FT_FRAME_USHORT( Lowest_Rec_PPEM ),
			FT_FRAME_SHORT ( Font_Direction ),
			FT_FRAME_SHORT ( Index_To_Loc_Format ),
			FT_FRAME_SHORT ( Glyph_Data_Format ),
			FT_FRAME_END
		};
		error = face->goto_table( face, tag, stream, 0 );
    if ( error )
      goto Exit;
		header = &face->header;
		if ( FT_STREAM_READ_FIELDS( header_fields, header ) )
			goto Exit;
		FT_TRACE3(( "Units per EM: %4u\n", header->Units_Per_EM ));
		FT_TRACE3(( "IndexToLoc:   %4d\n", header->Index_To_Loc_Format ));
Exit:
		return error;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_head( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		return tt_face_load_generic_header( face, stream, TTAG_head );
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_hhea( TT_Face    face,
		ft_structs_h::FT_Stream  stream,
		ft_structs_h::FT_Bool    vertical )
	{
		ft_structs_h::FT_Error        error;
		TT_HoriHeader*  header;
		static const ft_structs_h::FT_Frame_Field  metrics_header_fields[] =
		{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TT_HoriHeader
			FT_FRAME_START( 36 ),
			FT_FRAME_ULONG ( Version ),
			FT_FRAME_SHORT ( Ascender ),
			FT_FRAME_SHORT ( Descender ),
			FT_FRAME_SHORT ( Line_Gap ),
			FT_FRAME_USHORT( advance_Width_Max ),
			FT_FRAME_SHORT ( min_Left_Side_Bearing ),
			FT_FRAME_SHORT ( min_Right_Side_Bearing ),
			FT_FRAME_SHORT ( xMax_Extent ),
			FT_FRAME_SHORT ( caret_Slope_Rise ),
			FT_FRAME_SHORT ( caret_Slope_Run ),
			FT_FRAME_SHORT ( caret_Offset ),
			FT_FRAME_SHORT ( Reserved[0] ),
			FT_FRAME_SHORT ( Reserved[1] ),
			FT_FRAME_SHORT ( Reserved[2] ),
			FT_FRAME_SHORT ( Reserved[3] ),
			FT_FRAME_SHORT ( metric_Data_Format ),
			FT_FRAME_USHORT( number_Of_HMetrics ),
			FT_FRAME_END
		};
		if ( vertical )
		{
			void  *v = &face->vertical;
			error = face->goto_table( face, TTAG_vhea, stream, 0 );
      if ( error )
        goto Fail;
			header = (TT_HoriHeader*)v;
    }
    else
    {
      error = face->goto_table( face, TTAG_hhea, stream, 0 );
      if ( error )
        goto Fail;
			header = &face->horizontal;
    }
		if ( FT_STREAM_READ_FIELDS( metrics_header_fields, header ) )
			goto Fail;
		FT_TRACE3(( "Ascender:          %5d\n", header->Ascender ));
    FT_TRACE3(( "Descender:         %5d\n", header->Descender ));
    FT_TRACE3(( "number_Of_Metrics: %5u\n", header->number_Of_HMetrics ));
		header->long_metrics  = NULL;
		header->short_metrics = NULL;
Fail:
		return error;
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_cmap( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		ft_structs_h::FT_Error  error;
		error = face->goto_table( face, TTAG_cmap, stream, &face->cmap_size );
		if ( error )
			goto Exit;
		if ( FT_FRAME_EXTRACT( face->cmap_size, face->cmap_table ) )
			face->cmap_size = 0;
Exit:
		return error;
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_pclt( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		static const ft_structs_h::FT_Frame_Field  pclt_fields[] =
    {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TT_PCLT
			FT_FRAME_START( 54 ),
			FT_FRAME_ULONG ( Version ),
			FT_FRAME_ULONG ( FontNumber ),
			FT_FRAME_USHORT( Pitch ),
			FT_FRAME_USHORT( xHeight ),
			FT_FRAME_USHORT( Style ),
			FT_FRAME_USHORT( TypeFamily ),
			FT_FRAME_USHORT( CapHeight ),
			FT_FRAME_BYTES ( TypeFace, 16 ),
			FT_FRAME_BYTES ( CharacterComplement, 8 ),
			FT_FRAME_BYTES ( FileName, 6 ),
			FT_FRAME_CHAR  ( StrokeWeight ),
			FT_FRAME_CHAR  ( WidthType ),
			FT_FRAME_BYTE  ( SerifStyle ),
			FT_FRAME_BYTE  ( Reserved ),
			FT_FRAME_END
		};
		ft_structs_h::FT_Error  error;
		TT_PCLT*  pclt = &face->pclt;
		/* optional table */
		error = face->goto_table( face, TTAG_PCLT, stream, 0 );
		if ( error )
			goto Exit;
		if ( FT_STREAM_READ_FIELDS( pclt_fields, pclt ) )
			goto Exit;
Exit:
		return error;
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_get_metrics( TT_Face     face,
		ft_structs_h::FT_Bool     vertical,
		ft_structs_h::FT_UInt     gindex,
		ft_structs_h::FT_Short   *abearing,
		ft_structs_h::FT_UShort  *aadvance )
	{
		ft_structs_h::FT_Error        error;
    ft_structs_h::FT_Stream       stream = face->root.stream;
    TT_HoriHeader*  header;
    ft_structs_h::FT_ULong        table_pos, table_size, table_end;
    ft_structs_h::FT_UShort       k;
		if ( vertical )
		{
			void*  v = &face->vertical;
			header     = (TT_HoriHeader*)v;
      table_pos  = face->vert_metrics_offset;
      table_size = face->vert_metrics_size;
    }
    else
    {
      header     = &face->horizontal;
      table_pos  = face->horz_metrics_offset;
      table_size = face->horz_metrics_size;
    }
		table_end = table_pos + table_size;
		k = header->number_Of_HMetrics;
		if ( k > 0 )
    {
      if ( gindex < (ft_structs_h::FT_UInt)k )
      {
        table_pos += 4 * gindex;
        if ( table_pos + 4 > table_end )
          goto NoData;
				if ( FT_STREAM_SEEK( table_pos ) ||
					FT_READ_USHORT( *aadvance ) ||
					FT_READ_SHORT( *abearing )  )
					goto NoData;
			}
      else
      {
				table_pos += 4 * ( k - 1 );
				if ( table_pos + 4 > table_end )
					goto NoData;
				if ( FT_STREAM_SEEK( table_pos ) ||
					FT_READ_USHORT( *aadvance ) )
					goto NoData;
				table_pos += 4 + 2 * ( gindex - k );
        if ( table_pos + 2 > table_end )
          *abearing = 0;
        else
        {
          if ( !FT_STREAM_SEEK( table_pos ) )
            (void)FT_READ_SHORT( *abearing );
        }
      }
    }
    else
    {
NoData:
			*abearing = 0;
			*aadvance = 0;
    }
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_hmtx( TT_Face    face,
		ft_structs_h::FT_Stream  stream,
		ft_structs_h::FT_Bool    vertical )
	{
		ft_structs_h::FT_Error   error;
		ft_structs_h::FT_ULong   tag, table_size;
		ft_structs_h::FT_ULong*  ptable_offset;
		ft_structs_h::FT_ULong*  ptable_size;
		if ( vertical )
		{
			tag           = TTAG_vmtx;
      ptable_offset = &face->vert_metrics_offset;
      ptable_size   = &face->vert_metrics_size;
    }
    else
    {
      tag           = TTAG_hmtx;
      ptable_offset = &face->horz_metrics_offset;
      ptable_size   = &face->horz_metrics_size;
    }
		error = face->goto_table( face, tag, stream, &table_size );
    if ( error )
      goto Fail;
		*ptable_size   = table_size;
		*ptable_offset = FT_STREAM_POS();
Fail:
		return error;
	};
	static ft_structs_h::FT_Error
		check_table_dir( sfnt_h::SFNT_Header  sfnt,
		ft_structs_h::FT_Stream    stream )
	{
		ft_structs_h::FT_Error   error;
		ft_structs_h::FT_UShort  nn, valid_entries = 0;
		ft_structs_h::FT_UInt    has_head = 0, has_sing = 0, has_meta = 0;
		ft_structs_h::FT_ULong   offset = sfnt->offset + 12;
		static const ft_structs_h::FT_Frame_Field  table_dir_entry_fields[] =
		{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TT_TableRec
			FT_FRAME_START( 16 ),
			FT_FRAME_ULONG( Tag ),
			FT_FRAME_ULONG( CheckSum ),
			FT_FRAME_ULONG( Offset ),
			FT_FRAME_ULONG( Length ),
			FT_FRAME_END
		};
		if ( FT_STREAM_SEEK( offset ) )
			goto Exit;
		for ( nn = 0; nn < sfnt->num_tables; nn++ )
		{
			TT_TableRec  table;
			if ( FT_STREAM_READ_FIELDS( table_dir_entry_fields, &table ) )
			{
        nn--;
        FT_TRACE2(( "check_table_dir:"
					" can read only %d table%s in font (instead of %d)\n",
					nn, nn == 1 ? "" : "s", sfnt->num_tables ));
				sfnt->num_tables = nn;
				break;
			}
			/* we ignore invalid tables */
			if ( table.Offset + table.Length > stream->size )
			{
				FT_TRACE2(( "check_table_dir: table entry %d invalid\n", nn ));
				continue;
			}
			else
				valid_entries++;
			if ( table.Tag == TTAG_head || table.Tag == TTAG_bhed )
			{
				ft_structs_h::FT_UInt32  magic;
#ifndef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
				if ( table.Tag == TTAG_head )
#endif
					has_head = 1;
				/*
         * The table length should be 0x36, but certain font tools make it
         * 0x38, so we will just check that it is greater.
         *
         * Note that according to the specification, the table must be
         * padded to 32-bit lengths, but this doesn't apply to the value of
         * its `Length' field!
         *
         */
        if ( table.Length < 0x36 )
        {
          FT_TRACE2(( "check_table_dir: `head' table too small\n" ));
          error = ft_error_h::SFNT_Err_Table_Missing;
          goto Exit;
        }
				if ( FT_STREAM_SEEK( table.Offset + 12 ) ||
					FT_READ_ULONG( magic )              )
					goto Exit;
				if ( magic != 0x5F0F3CF5UL )
				{
					FT_TRACE2(( "check_table_dir:"
						" no magic number found in `head' table\n"));
					error = ft_error_h::SFNT_Err_Table_Missing;
					goto Exit;
				}
				if ( FT_STREAM_SEEK( offset + ( nn + 1 ) * 16 ) )
					goto Exit;
			}
			else if ( table.Tag == TTAG_SING )
				has_sing = 1;
      else if ( table.Tag == TTAG_META )
        has_meta = 1;
    }
		sfnt->num_tables = valid_entries;
		if ( sfnt->num_tables == 0 )
    {
      FT_TRACE2(( "check_table_dir: no tables found\n" ));
      error = ft_error_h::SFNT_Err_Unknown_File_Format;
      goto Exit;
    }
		/* if `sing' and `meta' tables are present, there is no `head' table */
		if ( has_head || ( has_sing && has_meta ) )
    {
      error = ft_error_h::SFNT_Err_Ok;
      goto Exit;
    }
    else
    {
      FT_TRACE2(( "check_table_dir:" ));
#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
      FT_TRACE2(( " neither `head', `bhed', nor `sing' table found\n" ));
#else
      FT_TRACE2(( " neither `head' nor `sing' table found\n" ));
#endif
      error = ft_error_h::SFNT_Err_Table_Missing;
    }
Exit:
		return error;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_font_dir( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		sfnt_h::SFNT_HeaderRec  sfnt;
    ft_structs_h::FT_Error        error;
    ft_structs_h::FT_Memory       memory = stream->memory;
    TT_TableRec*    entry;
    ft_structs_h::FT_Int          nn;
		static const ft_structs_h::FT_Frame_Field  offset_table_fields[] =
		{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  sfnt_h::SFNT_HeaderRec
			FT_FRAME_START( 8 ),
			FT_FRAME_USHORT( num_tables ),
			FT_FRAME_USHORT( search_range ),
			FT_FRAME_USHORT( entry_selector ),
			FT_FRAME_USHORT( range_shift ),
			FT_FRAME_END
		};
    FT_TRACE2(( "tt_face_load_font_dir: %08p\n", face ));
		/* read the offset table */
		sfnt.offset = FT_STREAM_POS();
		if ( FT_READ_ULONG( sfnt.format_tag )                    ||
			FT_STREAM_READ_FIELDS( offset_table_fields, &sfnt ) )
			goto Exit;
		/* many fonts don't have these fields set correctly */
#if 0
		if ( sfnt.search_range != 1 << ( sfnt.entry_selector + 4 )        ||
			sfnt.search_range + sfnt.range_shift != sfnt.num_tables << 4 )
			return ft_error_h::SFNT_Err_Unknown_File_Format;
#endif
		/* load the table directory */
		FT_TRACE2(( "-- Number of tables: %10u\n",    sfnt.num_tables ));
		FT_TRACE2(( "-- Format version:   0x%08lx\n", sfnt.format_tag ));
		/* check first */
		error = check_table_dir( &sfnt, stream );
		if ( error )
		{
			FT_TRACE2(( "tt_face_load_font_dir:"
				" invalid table directory for TrueType\n" ));
			goto Exit;
		}
		face->num_tables = sfnt.num_tables;
    face->format_tag = sfnt.format_tag;
		if ( FT_QNEW_ARRAY( face->dir_tables, face->num_tables ) )
			goto Exit;
		if ( FT_STREAM_SEEK( sfnt.offset + 12 )       ||
			FT_FRAME_ENTER( face->num_tables * 16L ) )
			goto Exit;
		entry = face->dir_tables;
		FT_TRACE2(( "\n"
			"  tag    offset    length   checksum\n"
			"  ----------------------------------\n" ));
		for ( nn = 0; nn < sfnt.num_tables; nn++ )
		{
      entry->Tag      = FT_GET_TAG4();
      entry->CheckSum = FT_GET_ULONG();
      entry->Offset   = FT_GET_LONG();
      entry->Length   = FT_GET_LONG();
			/* ignore invalid tables */
			if ( entry->Offset + entry->Length > stream->size )
        continue;
      else
      {
        FT_TRACE2(( "  %c%c%c%c  %08lx  %08lx  %08lx\n",
					(ft_structs_h::FT_Char)( entry->Tag >> 24 ),
					(ft_structs_h::FT_Char)( entry->Tag >> 16 ),
					(ft_structs_h::FT_Char)( entry->Tag >> 8  ),
					(ft_structs_h::FT_Char)( entry->Tag       ),
					entry->Offset,
					entry->Length,
					entry->CheckSum ));
				entry++;
			}
		}
		FT_FRAME_EXIT();
		FT_TRACE2(( "table directory loaded\n\n" ));
Exit:
		return error;
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Int )
		tt_face_get_kerning( TT_Face  face,
		ft_structs_h::FT_UInt  left_glyph,
		ft_structs_h::FT_UInt  right_glyph )
	{
		ft_structs_h::FT_Int    result = 0;
    ft_structs_h::FT_UInt   count, mask = 1;
    ft_structs_h::FT_Byte*  p       = face->kern_table;
    ft_structs_h::FT_Byte*  p_limit = p + face->kern_table_size;
		p   += 4;
		mask = 0x0001;
		for ( count = face->num_kern_tables;
			count > 0 && p + 6 <= p_limit;
			count--, mask <<= 1 )
		{
      ft_structs_h::FT_Byte* base     = p;
      ft_structs_h::FT_Byte* next     = base;
      ft_structs_h::FT_UInt  version  = FT_NEXT_USHORT( p );
      ft_structs_h::FT_UInt  length   = FT_NEXT_USHORT( p );
      ft_structs_h::FT_UInt  coverage = FT_NEXT_USHORT( p );
      ft_structs_h::FT_UInt  num_pairs;
      ft_structs_h::FT_Int   value    = 0;
			FT_UNUSED( version );
			next = base + length;
			if ( next > p_limit )  /* handle broken table */
				next = p_limit;
			if ( ( face->kern_avail_bits & mask ) == 0 )
				goto NextTable;
			if ( p + 8 > next )
				goto NextTable;
			num_pairs = FT_NEXT_USHORT( p );
      p        += 6;
			if ( ( next - p ) < 6 * (int)num_pairs )  /* handle broken count  */
				num_pairs = (ft_structs_h::FT_UInt)( ( next - p ) / 6 );
			switch ( coverage >> 8 )
			{
      case 0:
        {
          ft_structs_h::FT_ULong  key0 = TT_KERN_INDEX( left_glyph, right_glyph );
					if ( face->kern_order_bits & mask )   /* binary search */
					{
						ft_structs_h::FT_UInt   min = 0;
						ft_structs_h::FT_UInt   max = num_pairs;
						while ( min < max )
						{
							ft_structs_h::FT_UInt   mid = ( min + max ) >> 1;
              ft_structs_h::FT_Byte*  q   = p + 6 * mid;
              ft_structs_h::FT_ULong  key;
							key = FT_NEXT_ULONG( q );
							if ( key == key0 )
              {
                value = FT_PEEK_SHORT( q );
                goto Found;
              }
              if ( key < key0 )
                min = mid + 1;
              else
                max = mid;
            }
          }
          else /* linear search */
          {
            ft_structs_h::FT_UInt  count2;
						for ( count2 = num_pairs; count2 > 0; count2-- )
						{
							ft_structs_h::FT_ULong  key = FT_NEXT_ULONG( p );
							if ( key == key0 )
              {
                value = FT_PEEK_SHORT( p );
                goto Found;
              }
              p += 2;
            }
          }
        }
        break;
				/*
        *  We don't support format 2 because we haven't seen a single font
        *  using it in real life...
        */
			default:
				;
      }
			goto NextTable;
Found:
			if ( coverage & 8 ) /* override or add */
				result = value;
			else
				result += value;
NextTable:
			p = next;
		}
		return result;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_gasp( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		ft_structs_h::FT_Error   error;
		ft_structs_h::FT_Memory  memory = stream->memory;
		ft_structs_h::FT_UInt        j,num_ranges;
    TT_GaspRange   gaspranges = NULL;
		/* the gasp table is optional */
    error = face->goto_table( face, TTAG_gasp, stream, 0 );
    if ( error )
      goto Exit;
		if ( FT_FRAME_ENTER( 4L ) )
			goto Exit;
		face->gasp.version   = FT_GET_USHORT();
    face->gasp.numRanges = FT_GET_USHORT();
		FT_FRAME_EXIT();
		/* only support versions 0 and 1 of the table */
    if ( face->gasp.version >= 2 )
    {
      face->gasp.numRanges = 0;
      error = ft_error_h::SFNT_Err_Invalid_Table;
      goto Exit;
    }
		num_ranges = face->gasp.numRanges;
    FT_TRACE3(( "numRanges: %u\n", num_ranges ));
		if ( FT_QNEW_ARRAY( gaspranges, num_ranges ) ||
			FT_FRAME_ENTER( num_ranges * 4L )      )
			goto Exit;
		face->gasp.gaspRanges = gaspranges;
		for ( j = 0; j < num_ranges; j++ )
    {
      gaspranges[j].maxPPEM  = FT_GET_USHORT();
      gaspranges[j].gaspFlag = FT_GET_USHORT();
			FT_TRACE3(( "gaspRange %d: rangeMaxPPEM %5d, rangeGaspBehavior 0x%x\n",
				j,
				gaspranges[j].maxPPEM,
				gaspranges[j].gaspFlag ));
		}
		FT_FRAME_EXIT();
Exit:
		return error;
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_kern( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
    ft_structs_h::FT_Error   error;
    ft_structs_h::FT_ULong   table_size;
    ft_structs_h::FT_Byte*   p;
    ft_structs_h::FT_Byte*   p_limit;
    ft_structs_h::FT_UInt    nn, num_tables;
    ft_structs_h::FT_UInt32  avail = 0, ordered = 0;
		/* the kern table is optional; exit silently if it is missing */
		error = face->goto_table( face, TTAG_kern, stream, &table_size );
		if ( error )
			goto Exit;
		if ( table_size < 4 )  /* the case of a malformed table */
    {
			FT_ERROR(( "tt_face_load_kern:"
				" kerning table is too small - ignored\n" ));
			error = ft_error_h::SFNT_Err_Table_Missing;
      goto Exit;
    }
		if ( FT_FRAME_EXTRACT( table_size, face->kern_table ) )
    {
      FT_ERROR(( "tt_face_load_kern:"
				" could not extract kerning table\n" ));
      goto Exit;
    }
		face->kern_table_size = table_size;
		p       = face->kern_table;
    p_limit = p + table_size;
		p         += 2; /* skip version */
    num_tables = FT_NEXT_USHORT( p );
		if ( num_tables > 32 ) /* we only support up to 32 sub-tables */
      num_tables = 32;
		for ( nn = 0; nn < num_tables; nn++ )
    {
      ft_structs_h::FT_UInt    num_pairs, length, coverage;
      ft_structs_h::FT_Byte*   p_next;
      ft_structs_h::FT_UInt32  mask = (ft_structs_h::FT_UInt32)1UL << nn;
			if ( p + 6 > p_limit )
				break;
			p_next = p;
			p += 2; /* skip version */
      length   = FT_NEXT_USHORT( p );
      coverage = FT_NEXT_USHORT( p );
			if ( length <= 6 )
        break;
			p_next += length;
			if ( p_next > p_limit )  /* handle broken table */
				p_next = p_limit;
			/* only use horizontal kerning tables */
      if ( ( coverage & ~8 ) != 0x0001 ||
           p + 8 > p_limit             )
        goto NextTable;
			num_pairs = FT_NEXT_USHORT( p );
			p        += 6;
			if ( ( p_next - p ) < 6 * (int)num_pairs ) /* handle broken count */
				num_pairs = (ft_structs_h::FT_UInt)( ( p_next - p ) / 6 );
			avail |= mask;
			/*
       *  Now check whether the pairs in this table are ordered.
       *  We then can use binary search.
       */
      if ( num_pairs > 0 )
      {
        ft_structs_h::FT_ULong  count;
        ft_structs_h::FT_ULong  old_pair;
				old_pair = FT_NEXT_ULONG( p );
				p       += 2;
				for ( count = num_pairs - 1; count > 0; count-- )
				{
					ft_structs_h::FT_UInt32  cur_pair;
					cur_pair = FT_NEXT_ULONG( p );
          if ( cur_pair <= old_pair )
            break;
					p += 2;
          old_pair = cur_pair;
        }
				if ( count == 0 )
					ordered |= mask;
			}
NextTable:
			p = p_next;
		}
		face->num_kern_tables = nn;
		face->kern_avail_bits = avail;
		face->kern_order_bits = ordered;
Exit:
		return error;
	};
	FT_LOCAL_DEF( void )
		tt_face_free_name( TT_Face  face )
	{
		ft_structs_h::FT_Memory     memory = face->root.driver->root.memory;
    TT_NameTable  table  = &face->name_table;
    TT_NameEntry  entry  = table->names;
    ft_structs_h::FT_UInt       count  = table->numNameRecords;
		if ( table->names )
    {
      for ( ; count > 0; count--, entry++ )
      {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
        FT_FREE( entry->string );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
        entry->stringLength = 0;
      }
			/* free strings table */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
      FT_FREE( table->names );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
    }
    table->numNameRecords = 0;
    table->format         = 0;
    table->storageOffset  = 0;
  }
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_name( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		ft_structs_h::FT_Error      error;
    ft_structs_h::FT_Memory     memory = stream->memory;
    ft_structs_h::FT_ULong      table_pos, table_len;
    ft_structs_h::FT_ULong      storage_start, storage_limit;
    ft_structs_h::FT_UInt       count;
    TT_NameTable  table;
		static const ft_structs_h::FT_Frame_Field  name_table_fields[] =
    {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TT_NameTableRec
			FT_FRAME_START( 6 ),
			FT_FRAME_USHORT( format ),
			FT_FRAME_USHORT( numNameRecords ),
			FT_FRAME_USHORT( storageOffset ),
      FT_FRAME_END
    };
		static const ft_structs_h::FT_Frame_Field  name_record_fields[] =
    {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TT_NameEntryRec
			/* no FT_FRAME_START */
			FT_FRAME_USHORT( platformID ),      
			FT_FRAME_USHORT( encodingID ),
			FT_FRAME_USHORT( languageID ),
			FT_FRAME_USHORT( nameID ),
			FT_FRAME_USHORT( stringLength ),
			FT_FRAME_USHORT( stringOffset ),
			FT_FRAME_END
		};
		table         = &face->name_table;
		table->stream = stream;
		error = face->goto_table( face, TTAG_name, stream, &table_len );
		if ( error )
			goto Exit;
		table_pos = FT_STREAM_POS();
		if ( FT_STREAM_READ_FIELDS( name_table_fields, table ) )
			goto Exit;
    /* Some popular Asian fonts have an invalid `storageOffset' value   */
    /* (it should be at least "6 + 12*num_names").  However, the string */
    /* offsets, computed as "storageOffset + entry->stringOffset", are  */
    /* valid pointers within the name table...                          */
    /*                                                                  */
    /* We thus can't check `storageOffset' right now.                   */
    /*                                                                  */
    storage_start = table_pos + 6 + 12*table->numNameRecords;
    storage_limit = table_pos + table_len;
		if ( storage_start > storage_limit )
    {
      FT_ERROR(( "tt_face_load_name: invalid `name' table\n" ));
      error = ft_error_h::SFNT_Err_Name_Table_Missing;
      goto Exit;
    }
		/* Allocate the array of name records. */
    count                 = table->numNameRecords;
    table->numNameRecords = 0;
		if ( FT_NEW_ARRAY( table->names, count ) ||
			FT_FRAME_ENTER( count * 12 )        )
			goto Exit;
    /* Load the name records and determine how much storage is needed */
    /* to hold the strings themselves.                                */
    {
			TT_NameEntryRec*  entry = table->names;
			for ( ; count > 0; count-- )
      {
        if ( FT_STREAM_READ_FIELDS( name_record_fields, entry ) )
          continue;
				/* check that the name is not empty */
        if ( entry->stringLength == 0 )
          continue;
				/* check that the name string is within the table */
        entry->stringOffset += table_pos + table->storageOffset;
        if ( entry->stringOffset                       < storage_start ||
             entry->stringOffset + entry->stringLength > storage_limit )
        {
          /* invalid entry - ignore it */
          entry->stringOffset = 0;
          entry->stringLength = 0;
          continue;
        }
				entry++;
      }
			table->numNameRecords = (ft_structs_h::FT_UInt)( entry - table->names );
    }
		FT_FRAME_EXIT();
		/* everything went well, update face->num_names */
    face->num_names = (ft_structs_h::FT_UShort) table->numNameRecords;
Exit:
		return error;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_post( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		ft_structs_h::FT_Error        error;
		TT_Postscript*  post = &face->postscript;
		static const ft_structs_h::FT_Frame_Field  post_fields[] =
		{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TT_Postscript
			FT_FRAME_START( 32 ),
			FT_FRAME_ULONG( FormatType ),
			FT_FRAME_ULONG( italicAngle ),
			FT_FRAME_SHORT( underlinePosition ),
			FT_FRAME_SHORT( underlineThickness ),
			FT_FRAME_ULONG( isFixedPitch ),
			FT_FRAME_ULONG( minMemType42 ),
			FT_FRAME_ULONG( maxMemType42 ),
			FT_FRAME_ULONG( minMemType1 ),
			FT_FRAME_ULONG( maxMemType1 ),
			FT_FRAME_END
		};
		error = face->goto_table( face, TTAG_post, stream, 0 );
		if ( error )
			return error;
		if ( FT_STREAM_READ_FIELDS( post_fields, post ) )
			return error;
		/* we don't load the glyph names, we do that in another */
		/* module (ttpost).                                     */
		FT_TRACE3(( "FormatType:   0x%x\n", post->FormatType ));
		FT_TRACE3(( "isFixedPitch:   %s\n", post->isFixedPitch
			? "  yes" : "   no" ));
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_os2( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		ft_structs_h::FT_Error  error;
		TT_OS2*   os2;
		static const ft_structs_h::FT_Frame_Field  os2_fields[] =
    {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TT_OS2
			FT_FRAME_START( 78 ),
			FT_FRAME_USHORT( version ),
			FT_FRAME_SHORT ( xAvgCharWidth ),
			FT_FRAME_USHORT( usWeightClass ),
			FT_FRAME_USHORT( usWidthClass ),
			FT_FRAME_SHORT ( fsType ),
			FT_FRAME_SHORT ( ySubscriptXSize ),
			FT_FRAME_SHORT ( ySubscriptYSize ),
			FT_FRAME_SHORT ( ySubscriptXOffset ),
			FT_FRAME_SHORT ( ySubscriptYOffset ),
			FT_FRAME_SHORT ( ySuperscriptXSize ),
			FT_FRAME_SHORT ( ySuperscriptYSize ),
			FT_FRAME_SHORT ( ySuperscriptXOffset ),
			FT_FRAME_SHORT ( ySuperscriptYOffset ),
			FT_FRAME_SHORT ( yStrikeoutSize ),
			FT_FRAME_SHORT ( yStrikeoutPosition ),
			FT_FRAME_SHORT ( sFamilyClass ),
			FT_FRAME_BYTE  ( panose[0] ),
			FT_FRAME_BYTE  ( panose[1] ),
			FT_FRAME_BYTE  ( panose[2] ),
			FT_FRAME_BYTE  ( panose[3] ),
			FT_FRAME_BYTE  ( panose[4] ),
			FT_FRAME_BYTE  ( panose[5] ),
			FT_FRAME_BYTE  ( panose[6] ),
			FT_FRAME_BYTE  ( panose[7] ),
			FT_FRAME_BYTE  ( panose[8] ),
			FT_FRAME_BYTE  ( panose[9] ),
			FT_FRAME_ULONG ( ulUnicodeRange1 ),
			FT_FRAME_ULONG ( ulUnicodeRange2 ),
			FT_FRAME_ULONG ( ulUnicodeRange3 ),
			FT_FRAME_ULONG ( ulUnicodeRange4 ),
			FT_FRAME_BYTE  ( achVendID[0] ),
			FT_FRAME_BYTE  ( achVendID[1] ),
			FT_FRAME_BYTE  ( achVendID[2] ),
			FT_FRAME_BYTE  ( achVendID[3] ),
			FT_FRAME_USHORT( fsSelection ),
			FT_FRAME_USHORT( usFirstCharIndex ),
			FT_FRAME_USHORT( usLastCharIndex ),
			FT_FRAME_SHORT ( sTypoAscender ),
			FT_FRAME_SHORT ( sTypoDescender ),
			FT_FRAME_SHORT ( sTypoLineGap ),
			FT_FRAME_USHORT( usWinAscent ),
			FT_FRAME_USHORT( usWinDescent ),
      FT_FRAME_END
    };
		static const ft_structs_h::FT_Frame_Field  os2_fields_extra[] =
		{
			FT_FRAME_START( 8 ),
			FT_FRAME_ULONG( ulCodePageRange1 ),
			FT_FRAME_ULONG( ulCodePageRange2 ),
			FT_FRAME_END
    };
		static const ft_structs_h::FT_Frame_Field  os2_fields_extra2[] =
		{
			FT_FRAME_START( 10 ),
			FT_FRAME_SHORT ( sxHeight ),
			FT_FRAME_SHORT ( sCapHeight ),
			FT_FRAME_USHORT( usDefaultChar ),
			FT_FRAME_USHORT( usBreakChar ),
			FT_FRAME_USHORT( usMaxContext ),
      FT_FRAME_END
    };
    /* We now support old Mac fonts where the OS/2 table doesn't  */
    /* exist.  Simply put, we set the `version' field to 0xFFFF   */
    /* and test this value each time we need to access the table. */
    error = face->goto_table( face, TTAG_OS2, stream, 0 );
    if ( error )
      goto Exit;
		os2 = &face->os2;
		if ( FT_STREAM_READ_FIELDS( os2_fields, os2 ) )
			goto Exit;
    os2->ulCodePageRange1 = 0;
    os2->ulCodePageRange2 = 0;
    os2->sxHeight         = 0;
    os2->sCapHeight       = 0;
    os2->usDefaultChar    = 0;
    os2->usBreakChar      = 0;
    os2->usMaxContext     = 0;
		if ( os2->version >= 0x0001 )
    {
      /* only version 1 tables */
      if ( FT_STREAM_READ_FIELDS( os2_fields_extra, os2 ) )
        goto Exit;
			if ( os2->version >= 0x0002 )
      {
        /* only version 2 tables */
        if ( FT_STREAM_READ_FIELDS( os2_fields_extra2, os2 ) )
          goto Exit;
      }
    }
		FT_TRACE3(( "sTypoAscender:  %4d\n",   os2->sTypoAscender ));
    FT_TRACE3(( "sTypoDescender: %4d\n",   os2->sTypoDescender ));
    FT_TRACE3(( "usWinAscent:    %4u\n",   os2->usWinAscent ));
    FT_TRACE3(( "usWinDescent:   %4u\n",   os2->usWinDescent ));
    FT_TRACE3(( "fsSelection:    0x%2x\n", os2->fsSelection ));
Exit:
		return error;
	};
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_load_maxp( TT_Face    face,
		ft_structs_h::FT_Stream  stream )
	{
		ft_structs_h::FT_Error        error;
		TT_MaxProfile*  maxProfile = &face->max_profile;
		static const ft_structs_h::FT_Frame_Field  maxp_fields[] =
		{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TT_MaxProfile
			FT_FRAME_START( 6 ),
			FT_FRAME_LONG  ( version ),
			FT_FRAME_USHORT( numGlyphs ),
			FT_FRAME_END
		};
		static const ft_structs_h::FT_Frame_Field  maxp_fields_extra[] =
		{
			FT_FRAME_START( 26 ),
			FT_FRAME_USHORT( maxPoints ),
			FT_FRAME_USHORT( maxContours ),
			FT_FRAME_USHORT( maxCompositePoints ),
			FT_FRAME_USHORT( maxCompositeContours ),
			FT_FRAME_USHORT( maxZones ),
			FT_FRAME_USHORT( maxTwilightPoints ),
			FT_FRAME_USHORT( maxStorage ),
			FT_FRAME_USHORT( maxFunctionDefs ),
			FT_FRAME_USHORT( maxInstructionDefs ),
			FT_FRAME_USHORT( maxStackElements ),
			FT_FRAME_USHORT( maxSizeOfInstructions ),
			FT_FRAME_USHORT( maxComponentElements ),
			FT_FRAME_USHORT( maxComponentDepth ),
      FT_FRAME_END
    };
		error = face->goto_table( face, TTAG_maxp, stream, 0 );
		if ( error )
			goto Exit;
		if ( FT_STREAM_READ_FIELDS( maxp_fields, maxProfile ) )
			goto Exit;
		maxProfile->maxPoints             = 0;
    maxProfile->maxContours           = 0;
    maxProfile->maxCompositePoints    = 0;
    maxProfile->maxCompositeContours  = 0;
    maxProfile->maxZones              = 0;
    maxProfile->maxTwilightPoints     = 0;
    maxProfile->maxStorage            = 0;
    maxProfile->maxFunctionDefs       = 0;
    maxProfile->maxInstructionDefs    = 0;
    maxProfile->maxStackElements      = 0;
    maxProfile->maxSizeOfInstructions = 0;
    maxProfile->maxComponentElements  = 0;
    maxProfile->maxComponentDepth     = 0;
		if ( maxProfile->version >= 0x10000L )
		{
			if ( FT_STREAM_READ_FIELDS( maxp_fields_extra, maxProfile ) )
				goto Exit;
      /* XXX: an adjustment that is necessary to load certain */
      /*      broken fonts like `Keystrokes MT' :-(           */
      /*                                                      */
      /*   We allocate 64 function entries by default when    */
      /*   the maxFunctionDefs value is smaller.              */
			if ( maxProfile->maxFunctionDefs < 64 )
				maxProfile->maxFunctionDefs = 64;
			/* we add 4 phantom points later */
			if ( maxProfile->maxTwilightPoints > ( 0xFFFFU - 4 ) )
			{
				FT_TRACE0(( "tt_face_load_maxp:"
					" too much twilight points in `maxp' table;\n"
					"                  "
					" some glyphs might be rendered incorrectly\n" ));
				maxProfile->maxTwilightPoints = 0xFFFFU - 4;
			}
			/* we arbitrarily limit recursion to avoid stack exhaustion */
			if ( maxProfile->maxComponentDepth > 100 )
      {
        FT_TRACE0(( "tt_face_load_maxp:" 
					" abnormally large component depth (%d) set to 100\n",
					maxProfile->maxComponentDepth ));
				maxProfile->maxComponentDepth = 100;
			}
		}
		FT_TRACE3(( "numGlyphs: %u\n", maxProfile->numGlyphs ));
Exit:
		return error;
  };
	FT_LOCAL_DEF( void )
		tt_face_done_kern( TT_Face  face )
	{
		ft_structs_h::FT_Stream  stream = face->root.stream;
		FT_FRAME_RELEASE( face->kern_table );
    face->kern_table_size = 0;
    face->num_kern_tables = 0;
    face->kern_avail_bits = 0;
    face->kern_order_bits = 0;
  };
	FT_LOCAL( ft_structs_h::FT_Error )
		tt_get_cmap_info( ft_structs_h::FT_CharMap    charmap,
		TT_CMapInfo  *cmap_info )
	{
		ft_structs_h::FT_CMap        cmap  = (ft_structs_h::FT_CMap)charmap;
		TT_CMap_Class  clazz = (TT_CMap_Class)cmap->clazz;
		return clazz->get_cmap_info( charmap, cmap_info );
  }
	typedef ft_structs_h::FT_String*  (*TT_NameEntry_ConvertFunc)( TT_NameEntry  entry,
		ft_structs_h::FT_Memory     memory );
	static ft_structs_h::FT_String*
		tt_name_entry_ascii_from_utf16( TT_NameEntry  entry,
		ft_structs_h::FT_Memory     memory )
	{
		ft_structs_h::FT_String*  string = NULL;
    ft_structs_h::FT_UInt     len, code, n;
    ft_structs_h::FT_Byte*    read   = (ft_structs_h::FT_Byte*)entry->string;
    ft_structs_h::FT_Error    error;
		len = (ft_structs_h::FT_UInt)entry->stringLength / 2;
		if ( FT_NEW_ARRAY( string, len + 1 ) )
			return NULL;
		for ( n = 0; n < len; n++ )
    {
      code = FT_NEXT_USHORT( read );
			if ( code == 0 )
        break;
			if ( code < 32 || code > 127 )
        code = '?';
			string[n] = (char)code;
    }
		string[n] = 0;
		return string;
  };
	static ft_structs_h::FT_String*
		tt_name_entry_ascii_from_other( TT_NameEntry  entry,
		ft_structs_h::FT_Memory     memory )
	{
		ft_structs_h::FT_String*  string = NULL;
		ft_structs_h::FT_UInt     len, code, n;
		ft_structs_h::FT_Byte*    read   = (ft_structs_h::FT_Byte*)entry->string;
		ft_structs_h::FT_Error    error;
		len = (ft_structs_h::FT_UInt)entry->stringLength;
		if ( FT_NEW_ARRAY( string, len + 1 ) )
			return NULL;
		for ( n = 0; n < len; n++ )
    {
      code = *read++;
			if ( code == 0 )
				break;
			if ( code < 32 || code > 127 )
        code = '?';
			string[n] = (char)code;
    }
    string[n] = 0;
		return string;
  };
	static ft_structs_h::FT_Error
		tt_face_get_name( TT_Face      face,
		ft_structs_h::FT_UShort    nameid,
		ft_structs_h::FT_String**  name )
	{
		ft_structs_h::FT_Memory         memory = face->root.memory;
    ft_structs_h::FT_Error          error  = ft_error_h::SFNT_Err_Ok;
    ft_structs_h::FT_String*        result = NULL;
    ft_structs_h::FT_UShort         n;
    TT_NameEntryRec*  rec;
    ft_structs_h::FT_Int            found_apple         = -1;
    ft_structs_h::FT_Int            found_apple_roman   = -1;
    ft_structs_h::FT_Int            found_apple_english = -1;
    ft_structs_h::FT_Int            found_win           = -1;
    ft_structs_h::FT_Int            found_unicode       = -1;
		ft_structs_h::FT_Bool           is_english = 0;
		TT_NameEntry_ConvertFunc  convert;
		FT_ASSERT( name );
		rec = face->name_table.names;
    for ( n = 0; n < face->num_names; n++, rec++ )
    {    
      if ( rec->nameID == nameid && rec->stringLength > 0 )
      {
        switch ( rec->platformID )
        {
        case TT_PLATFORM_APPLE_UNICODE:
        case TT_PLATFORM_ISO:    
          found_unicode = n;
          break;
        case TT_PLATFORM_MACINTOSH:
					if ( rec->languageID == TT_MAC_LANGID_ENGLISH )
						found_apple_english = n;
					else if ( rec->encodingID == TT_MAC_ID_ROMAN )
            found_apple_roman = n;
          break;
        case TT_PLATFORM_MICROSOFT:
          if ( found_win == -1 || ( rec->languageID & 0x3FF ) == 0x009 )
          {
            switch ( rec->encodingID )
            {
            case TT_MS_ID_SYMBOL_CS:
            case TT_MS_ID_UNICODE_CS:
            case TT_MS_ID_UCS_4:
              is_english = FT_BOOL( ( rec->languageID & 0x3FF ) == 0x009 );
              found_win  = n;
              break;
            default:
              ;
            }
          }
          break;
        default:
          ;
        }
      }
    }
    found_apple = found_apple_roman;
    if ( found_apple_english >= 0 )
      found_apple = found_apple_english;
    convert = NULL;
    if ( found_win >= 0 && !( found_apple >= 0 && !is_english ) )
    {
      rec = face->name_table.names + found_win;
      switch ( rec->encodingID )
      {
      case TT_MS_ID_UNICODE_CS:
      case TT_MS_ID_SYMBOL_CS:
        convert = tt_name_entry_ascii_from_utf16;
        break;
      case TT_MS_ID_UCS_4:
        convert = tt_name_entry_ascii_from_utf16;
        break;
      default:
        ;
      }
    }
    else if ( found_apple >= 0 )
    {
      rec     = face->name_table.names + found_apple;
      convert = tt_name_entry_ascii_from_other;
    }
    else if ( found_unicode >= 0 )
    {
      rec     = face->name_table.names + found_unicode;
      convert = tt_name_entry_ascii_from_utf16;
    }
    if ( rec && convert )
    {
      if ( rec->string == NULL )
      {
        ft_structs_h::FT_Stream  stream = face->name_table.stream;
        if ( FT_QNEW_ARRAY ( rec->string, rec->stringLength ) ||
             FT_STREAM_SEEK( rec->stringOffset )              ||
             FT_STREAM_READ( rec->string, rec->stringLength ) )
        {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
          FT_FREE( rec->string );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
          rec->stringLength = 0;
          result            = NULL;
          goto Exit;
        }
      }
      result = convert( rec, memory );
    }
Exit:
		*name = result;
    return error;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap_init( TT_CMap   cmap,
		ft_structs_h::FT_Byte*  table )
	{
		cmap->data = table;
		return ft_error_h::SFNT_Err_Ok;
	};
#ifdef TT_CONFIG_CMAP_FORMAT_0
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap0_validate( ft_structs_h::FT_Byte*      table,
		ft_structs_h::FT_Validator  valid )
	{
		ft_structs_h::FT_Byte*  p      = table + 2;
		ft_structs_h::FT_UInt   length = TT_NEXT_USHORT( p );
		if ( table + length > valid->limit || length < 262 )
			FT_INVALID_TOO_SHORT;
		/* check glyph indices whenever necessary */
		if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
		{
			ft_structs_h::FT_UInt  n, idx;
			p = table + 6;
			for ( n = 0; n < 256; n++ )
			{
        idx = *p++;
        if ( idx >= TT_VALID_GLYPH_COUNT( valid ) )
          FT_INVALID_GLYPH_ID;
      }
    }
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap0_char_index( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  char_code )
	{
		ft_structs_h::FT_Byte*  table = cmap->data;
		return char_code < 256 ? table[6 + char_code] : 0;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 )
		tt_cmap0_char_next( tt_h::TT_CMap     cmap,
		ft_structs_h::FT_UInt32  *pchar_code )
	{
		ft_structs_h::FT_Byte*   table    = cmap->data;
    ft_structs_h::FT_UInt32  charcode = *pchar_code;
    ft_structs_h::FT_UInt32  result   = 0;
    ft_structs_h::FT_UInt    gindex   = 0;
		table += 6;  /* go to glyph IDs */
		while ( ++charcode < 256 )
		{
      gindex = table[charcode];
      if ( gindex != 0 )
      {
        result = charcode;
        break;
      }
    }
		*pchar_code = result;
		return gindex;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap0_get_info( tt_h::TT_CMap       cmap,
		tt_h::TT_CMapInfo  *cmap_info )
	{
		ft_structs_h::FT_Byte*  p = cmap->data + 4;
		cmap_info->format   = 0;
		cmap_info->language = (ft_structs_h::FT_ULong)TT_PEEK_USHORT( p );
		return ft_error_h::SFNT_Err_Ok;
	};
	FT_DEFINE_TT_CMAP(tt_cmap0_class_rec,
		sizeof ( TT_CMapRec ),
		(FT_CMap_InitFunc)     tt_cmap_init,
		(FT_CMap_DoneFunc)     NULL,
		(FT_CMap_CharIndexFunc)tt_cmap0_char_index,
		(FT_CMap_CharNextFunc) tt_cmap0_char_next,
		NULL, NULL, NULL, NULL, NULL
		,
		0,
		(TT_CMap_ValidateFunc)   tt_cmap0_validate,
		(TT_CMap_Info_GetFunc)   tt_cmap0_get_info
		)
#endif /* TT_CONFIG_CMAP_FORMAT_0 */
#ifdef TT_CONFIG_CMAP_FORMAT_2
FT_CALLBACK_DEF( ft_structs_h::FT_Error )
tt_cmap2_validate( ft_structs_h::FT_Byte*      table,
ft_structs_h::FT_Validator  valid )
{
	ft_structs_h::FT_Byte*  p      = table + 2;           /* skip format */
	ft_structs_h::FT_UInt   length = TT_PEEK_USHORT( p );
	ft_structs_h::FT_UInt   n, max_subs;
	ft_structs_h::FT_Byte*  keys;                         /* keys table */
	ft_structs_h::FT_Byte*  subs;                         /* sub-headers */
	ft_structs_h::FT_Byte*  glyph_ids;                    /* glyph ID array */
	if ( table + length > valid->limit || length < 6 + 512 )
		FT_INVALID_TOO_SHORT;
	keys = table + 6;
	/* parse keys to compute sub-headers count */
	p        = keys;
	max_subs = 0;
	for ( n = 0; n < 256; n++ )
	{
		ft_structs_h::FT_UInt  idx = TT_NEXT_USHORT( p );
		/* value must be multiple of 8 */
		if ( valid->level >= ft_structs_h::FT_VALIDATE_PARANOID && ( idx & 7 ) != 0 )
			FT_INVALID_DATA;
		idx >>= 3;
		if ( idx > max_subs )
			max_subs = idx;
	}
	FT_ASSERT( p == table + 518 );
	subs      = p;
	glyph_ids = subs + (max_subs + 1) * 8;
	if ( glyph_ids > valid->limit )
		FT_INVALID_TOO_SHORT;
	/* parse sub-headers */
	for ( n = 0; n <= max_subs; n++ )
	{
		ft_structs_h::FT_UInt   first_code, code_count, offset;
		ft_structs_h::FT_Int    delta;
		ft_structs_h::FT_Byte*  ids;
		first_code = TT_NEXT_USHORT( p );
		code_count = TT_NEXT_USHORT( p );
		delta      = TT_NEXT_SHORT( p );
		offset     = TT_NEXT_USHORT( p );
		/* many Dynalab fonts have empty sub-headers */
		if ( code_count == 0 )
			continue;
		/* check range within 0..255 */
		if ( valid->level >= ft_structs_h::FT_VALIDATE_PARANOID )
		{
			if ( first_code >= 256 || first_code + code_count > 256 )
				FT_INVALID_DATA;
		}
		/* check offset */
		if ( offset != 0 )
		{
			ids = p - 2 + offset;
			if ( ids < glyph_ids || ids + code_count*2 > table + length )
				FT_INVALID_OFFSET;
			/* check glyph IDs */
			if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
			{
				ft_structs_h::FT_Byte*  limit = p + code_count * 2;
				ft_structs_h::FT_UInt   idx;
				for ( ; p < limit; )
				{
					idx = TT_NEXT_USHORT( p );
					if ( idx != 0 )
					{
						idx = ( idx + delta ) & 0xFFFFU;
						if ( idx >= TT_VALID_GLYPH_COUNT( valid ) )
							FT_INVALID_GLYPH_ID;
					}
				}
			}
		}
	}
	return ft_error_h::SFNT_Err_Ok;
};
	/* return sub header corresponding to a given character code */
	/* NULL on invalid charcode                                  */
	static ft_structs_h::FT_Byte*
		tt_cmap2_get_subheader( ft_structs_h::FT_Byte*   table,
		ft_structs_h::FT_UInt32  char_code )
	{
		ft_structs_h::FT_Byte*  result = NULL;
		if ( char_code < 0x10000UL )
		{
      ft_structs_h::FT_UInt   char_lo = (ft_structs_h::FT_UInt)( char_code & 0xFF );
      ft_structs_h::FT_UInt   char_hi = (ft_structs_h::FT_UInt)( char_code >> 8 );
      ft_structs_h::FT_Byte*  p       = table + 6;    /* keys table */
      ft_structs_h::FT_Byte*  subs    = table + 518;  /* subheaders table */
      ft_structs_h::FT_Byte*  sub;
			if ( char_hi == 0 )
      {
        /* an 8-bit character code -- we use subHeader 0 in this case */
        /* to test whether the character code is in the charmap       */
        /*                                                            */
        sub = subs;  /* jump to first sub-header */
				/* check that the sub-header for this byte is 0, which */
        /* indicates that it is really a valid one-byte value  */
        /* Otherwise, return 0                                 */
        /*                                                     */
        p += char_lo * 2;
        if ( TT_PEEK_USHORT( p ) != 0 )
          goto Exit;
      }
      else
      {
        /* a 16-bit character code */
        /* jump to key entry  */
				p  += char_hi * 2;
				/* jump to sub-header */
        sub = subs + ( FT_PAD_FLOOR( TT_PEEK_USHORT( p ), 8 ) );
				/* check that the high byte isn't a valid one-byte value */
        if ( sub == subs )
          goto Exit;
      }
      result = sub;
    }
Exit:
		return result;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap2_char_index( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  char_code )
	{
    ft_structs_h::FT_Byte*  table   = cmap->data;
    ft_structs_h::FT_UInt   result  = 0;
    ft_structs_h::FT_Byte*  subheader;
		subheader = tt_cmap2_get_subheader( table, char_code );
    if ( subheader )
    {
      ft_structs_h::FT_Byte*  p   = subheader;
      ft_structs_h::FT_UInt   idx = (ft_structs_h::FT_UInt)(char_code & 0xFF);
      ft_structs_h::FT_UInt   start, count;
      ft_structs_h::FT_Int    delta;
      ft_structs_h::FT_UInt   offset;
			start  = TT_NEXT_USHORT( p );
      count  = TT_NEXT_USHORT( p );
      delta  = TT_NEXT_SHORT ( p );
      offset = TT_PEEK_USHORT( p );
      idx -= start;
      if ( idx < count && offset != 0 )
      {
        p  += offset + 2 * idx;
        idx = TT_PEEK_USHORT( p );
        if ( idx != 0 )
          result = (ft_structs_h::FT_UInt)( idx + delta ) & 0xFFFFU;
      }
    }
    return result;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 )
		tt_cmap2_char_next( TT_CMap     cmap,
		ft_structs_h::FT_UInt32  *pcharcode )
  {
    ft_structs_h::FT_Byte*   table    = cmap->data;
    ft_structs_h::FT_UInt    gindex   = 0;
    ft_structs_h::FT_UInt32  result   = 0;
    ft_structs_h::FT_UInt32  charcode = *pcharcode + 1;
    ft_structs_h::FT_Byte*   subheader;
		while ( charcode < 0x10000UL )
    {
      subheader = tt_cmap2_get_subheader( table, charcode );
      if ( subheader )
      {
        ft_structs_h::FT_Byte*  p       = subheader;
        ft_structs_h::FT_UInt   start   = TT_NEXT_USHORT( p );
        ft_structs_h::FT_UInt   count   = TT_NEXT_USHORT( p );
        ft_structs_h::FT_Int    delta   = TT_NEXT_SHORT ( p );
        ft_structs_h::FT_UInt   offset  = TT_PEEK_USHORT( p );
        ft_structs_h::FT_UInt   char_lo = (ft_structs_h::FT_UInt)( charcode & 0xFF );
        ft_structs_h::FT_UInt   pos, idx;
				if ( offset == 0 )
					goto Next_SubHeader;
				if ( char_lo < start )
        {
          char_lo = start;
          pos     = 0;
        }
        else
          pos = (ft_structs_h::FT_UInt)( char_lo - start );
				p       += offset + pos * 2;
				charcode = FT_PAD_FLOOR( charcode, 256 ) + char_lo;
				for ( ; pos < count; pos++, charcode++ )
        {
          idx = TT_NEXT_USHORT( p );
					if ( idx != 0 )
          {
            gindex = ( idx + delta ) & 0xFFFFU;
            if ( gindex != 0 )
            {
              result = charcode;
              goto Exit;
            }
          }
        }
      }
			/* jump to next sub-header, i.e. higher byte value */
Next_SubHeader:
			charcode = FT_PAD_FLOOR( charcode, 256 ) + 256;
		}
Exit:
		*pcharcode = result;
		return gindex;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap2_get_info( TT_CMap       cmap,
		TT_CMapInfo  *cmap_info )
	{
		ft_structs_h::FT_Byte*  p = cmap->data + 4;
		cmap_info->format   = 2;
		cmap_info->language = (ft_structs_h::FT_ULong)TT_PEEK_USHORT( p );
		return ft_error_h::SFNT_Err_Ok;
	};
	FT_DEFINE_TT_CMAP(tt_cmap2_class_rec,
		sizeof ( TT_CMapRec ),
		(FT_CMap_InitFunc)     tt_cmap_init,
		(FT_CMap_DoneFunc)     NULL,
		(FT_CMap_CharIndexFunc)tt_cmap2_char_index,
		(FT_CMap_CharNextFunc) tt_cmap2_char_next,
		NULL, NULL, NULL, NULL, NULL
		,
		2,
    (TT_CMap_ValidateFunc)   tt_cmap2_validate,
    (TT_CMap_Info_GetFunc)   tt_cmap2_get_info
  )
#endif /* TT_CONFIG_CMAP_FORMAT_2 */
	#ifdef TT_CONFIG_CMAP_FORMAT_4
	typedef struct  TT_CMap4Rec_
	{
		TT_CMapRec  cmap;
    ft_structs_h::FT_UInt32   cur_charcode;   /* current charcode */
    ft_structs_h::FT_UInt     cur_gindex;     /* current glyph index */
		ft_structs_h::FT_UInt     num_ranges;
    ft_structs_h::FT_UInt     cur_range;
    ft_structs_h::FT_UInt     cur_start;
    ft_structs_h::FT_UInt     cur_end;
    ft_structs_h::FT_Int      cur_delta;
    ft_structs_h::FT_Byte*    cur_values;
	} TT_CMap4Rec, *TT_CMap4;
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap4_init( TT_CMap4  cmap,
		ft_structs_h::FT_Byte*  table )
	{
		ft_structs_h::FT_Byte*  p;
		cmap->cmap.data    = table;
		p                  = table + 6;
    cmap->num_ranges   = FT_PEEK_USHORT( p ) >> 1;
    cmap->cur_charcode = (ft_structs_h::FT_UInt32)0xFFFFFFFFUL;
    cmap->cur_gindex   = 0;
		return ft_error_h::SFNT_Err_Ok;
	};
	static ft_structs_h::FT_Int
		tt_cmap4_set_range( TT_CMap4  cmap,
		ft_structs_h::FT_UInt   range_index )
	{
    ft_structs_h::FT_Byte*  table = cmap->cmap.data;
    ft_structs_h::FT_Byte*  p;
    ft_structs_h::FT_UInt   num_ranges = cmap->num_ranges;
		while ( range_index < num_ranges )
		{
			ft_structs_h::FT_UInt  offset;
			p             = table + 14 + range_index * 2;
			cmap->cur_end = FT_PEEK_USHORT( p );
      p              += 2 + num_ranges * 2;
      cmap->cur_start = FT_PEEK_USHORT( p );
			p              += num_ranges * 2;
      cmap->cur_delta = FT_PEEK_SHORT( p );
			p     += num_ranges * 2;
      offset = FT_PEEK_USHORT( p );
			/* some fonts have an incorrect last segment; */
      /* we have to catch it                        */
      if ( range_index     >= num_ranges - 1 &&
           cmap->cur_start == 0xFFFFU        &&
           cmap->cur_end   == 0xFFFFU        )
      {
        TT_Face   face  = (TT_Face)cmap->cmap.cmap.charmap.face;
        ft_structs_h::FT_Byte*  limit = face->cmap_table + face->cmap_size;
        if ( offset && p + offset + 2 > limit )
        {
          cmap->cur_delta = 1;
          offset          = 0;
        }
      }
      if ( offset != 0xFFFFU )
      {
        cmap->cur_values = offset ? p + offset : NULL;
        cmap->cur_range  = range_index;
        return 0;
      }
			/* we skip empty segments */
      range_index++;
    }
		return -1;
	};
  static void
	tt_cmap4_next( TT_CMap4  cmap )
	{
		ft_structs_h::FT_UInt  charcode;
		if ( cmap->cur_charcode >= 0xFFFFUL )
			goto Fail;
		charcode = (ft_structs_h::FT_UInt)cmap->cur_charcode + 1;
		if ( charcode < cmap->cur_start )
			charcode = cmap->cur_start;
		for ( ;; )
    {
      ft_structs_h::FT_Byte*  values = cmap->cur_values;
      ft_structs_h::FT_UInt   end    = cmap->cur_end;
      ft_structs_h::FT_Int    delta  = cmap->cur_delta;
			if ( charcode <= end )
      {
        if ( values )
        {
          ft_structs_h::FT_Byte*  p = values + 2 * ( charcode - cmap->cur_start );
					do
          {
            ft_structs_h::FT_UInt  gindex = FT_NEXT_USHORT( p );
						if ( gindex != 0 )
            {
              gindex = (ft_structs_h::FT_UInt)( ( gindex + delta ) & 0xFFFFU );
              if ( gindex != 0 )
              {
                cmap->cur_charcode = charcode;
                cmap->cur_gindex   = gindex;
                return;
              }
            }
          } while ( ++charcode <= end );
        }
        else
        {
          do
          {
            ft_structs_h::FT_UInt  gindex = (ft_structs_h::FT_UInt)( ( charcode + delta ) & 0xFFFFU );
						if ( gindex != 0 )
            {
              cmap->cur_charcode = charcode;
              cmap->cur_gindex   = gindex;
              return;
            }
          } while ( ++charcode <= end );
        }
      }
			/* we need to find another range */
			if ( tt_cmap4_set_range( cmap, cmap->cur_range + 1 ) < 0 )
				break;
			if ( charcode < cmap->cur_start )
        charcode = cmap->cur_start;
    }
Fail:
		cmap->cur_charcode = (ft_structs_h::FT_UInt32)0xFFFFFFFFUL;
		cmap->cur_gindex   = 0;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap4_validate( ft_structs_h::FT_Byte*      table,
		ft_structs_h::FT_Validator  valid )
	{
    ft_structs_h::FT_Byte*  p      = table + 2;               /* skip format */
    ft_structs_h::FT_UInt   length = TT_NEXT_USHORT( p );
    ft_structs_h::FT_Byte   *ends, *starts, *offsets, *deltas, *glyph_ids;
    ft_structs_h::FT_UInt   num_segs;
    ft_structs_h::FT_Error  error = ft_error_h::SFNT_Err_Ok;
		if ( length < 16 )
			FT_INVALID_TOO_SHORT;
		/* in certain fonts, the `length' field is invalid and goes */
    /* out of bound.  We try to correct this here...            */
    if ( table + length > valid->limit )
    {
      if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
        FT_INVALID_TOO_SHORT;
			length = (ft_structs_h::FT_UInt)( valid->limit - table );
    }
		p        = table + 6;
		num_segs = TT_NEXT_USHORT( p );   /* read segCountX2 */
		if ( valid->level >= ft_structs_h::FT_VALIDATE_PARANOID )
    {
      /* check that we have an even value here */
      if ( num_segs & 1 )
        FT_INVALID_DATA;
    }
		num_segs /= 2;
		if ( length < 16 + num_segs * 2 * 4 )
			FT_INVALID_TOO_SHORT;
		/* check the search parameters - even though we never use them */
    /*                                                             */
    if ( valid->level >= ft_structs_h::FT_VALIDATE_PARANOID )
    {
      /* check the values of `searchRange', `entrySelector', `rangeShift' */
      ft_structs_h::FT_UInt  search_range   = TT_NEXT_USHORT( p );
      ft_structs_h::FT_UInt  entry_selector = TT_NEXT_USHORT( p );
      ft_structs_h::FT_UInt  range_shift    = TT_NEXT_USHORT( p );
			if ( ( search_range | range_shift ) & 1 )  /* must be even values */
				FT_INVALID_DATA;
			search_range /= 2;
			range_shift  /= 2;
			/* `search range' is the greatest power of 2 that is <= num_segs */
			if ( search_range                > num_segs                 ||
				search_range * 2            < num_segs                 ||
				search_range + range_shift != num_segs                 ||
				search_range               != ( 1U << entry_selector ) )
				FT_INVALID_DATA;
		}
		ends      = table   + 14;
    starts    = table   + 16 + num_segs * 2;
    deltas    = starts  + num_segs * 2;
    offsets   = deltas  + num_segs * 2;
    glyph_ids = offsets + num_segs * 2;
		/* check last segment; its end count value must be 0xFFFF */
    if ( valid->level >= ft_structs_h::FT_VALIDATE_PARANOID )
    {
      p = ends + ( num_segs - 1 ) * 2;
      if ( TT_PEEK_USHORT( p ) != 0xFFFFU )
        FT_INVALID_DATA;
    }
		{
      ft_structs_h::FT_UInt   start, end, offset, n;
      ft_structs_h::FT_UInt   last_start = 0, last_end = 0;
      ft_structs_h::FT_Int    delta;
      ft_structs_h::FT_Byte*  p_start   = starts;
      ft_structs_h::FT_Byte*  p_end     = ends;
      ft_structs_h::FT_Byte*  p_delta   = deltas;
      ft_structs_h::FT_Byte*  p_offset  = offsets;
			for ( n = 0; n < num_segs; n++ )
      {
        p      = p_offset;
        start  = TT_NEXT_USHORT( p_start );
        end    = TT_NEXT_USHORT( p_end );
        delta  = TT_NEXT_SHORT( p_delta );
        offset = TT_NEXT_USHORT( p_offset );
				if ( start > end )
					FT_INVALID_DATA;
				if ( start <= last_end && n > 0 )
        {
          if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
            FT_INVALID_DATA;
          else
          {
            /* allow overlapping segments, provided their start points */
            /* and end points, respectively, are in ascending order    */
            /*                                                         */
            if ( last_start > start || last_end > end )
              error |= TT_CMAP_FLAG_UNSORTED;
            else
              error |= TT_CMAP_FLAG_OVERLAPPING;
          }
        }
				if ( offset && offset != 0xFFFFU )
        {
          p += offset;  /* start of glyph ID array */
					/* check that we point within the glyph IDs table only */
          if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
          {
            if ( p < glyph_ids                                ||
                 p + ( end - start + 1 ) * 2 > table + length )
              FT_INVALID_DATA;
          }
          else if ( n != num_segs - 1                       ||
						!( start == 0xFFFFU && end == 0xFFFFU ) )
					{
            if ( p < glyph_ids                              ||
                 p + ( end - start + 1 ) * 2 > valid->limit )
              FT_INVALID_DATA;
          }
          if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
          {
            ft_structs_h::FT_UInt  i, idx;
						for ( i = start; i < end; i++ )
            {
              idx = FT_NEXT_USHORT( p );
              if ( idx != 0 )
              {
                idx = (ft_structs_h::FT_UInt)( idx + delta ) & 0xFFFFU;
								if ( idx >= TT_VALID_GLYPH_COUNT( valid ) )
									FT_INVALID_GLYPH_ID;
              }
            }
          }
        }
        else if ( offset == 0xFFFFU )
        {
          if ( valid->level >= ft_structs_h::FT_VALIDATE_PARANOID    ||
               n != num_segs - 1                       ||
               !( start == 0xFFFFU && end == 0xFFFFU ) )
            FT_INVALID_DATA;
        }
				last_start = start;
				last_end   = end;
			}
    }
		return error;
	};
	static ft_structs_h::FT_UInt
		tt_cmap4_char_map_linear( TT_CMap     cmap,
		ft_structs_h::FT_UInt32*  pcharcode,
		ft_structs_h::FT_Bool     next )
	{
    ft_structs_h::FT_UInt    num_segs2, start, end, offset;
    ft_structs_h::FT_Int     delta;
    ft_structs_h::FT_UInt    i, num_segs;
    ft_structs_h::FT_UInt32  charcode = *pcharcode;
    ft_structs_h::FT_UInt    gindex   = 0;
    ft_structs_h::FT_Byte*   p;
		p = cmap->data + 6;
    num_segs2 = FT_PAD_FLOOR( TT_PEEK_USHORT( p ), 2 );
    num_segs = num_segs2 >> 1;
    if ( !num_segs )
      return 0;
    if ( next )
      charcode++;
    /* linear search */
    for ( ; charcode <= 0xFFFFU; charcode++ )
    {
      ft_structs_h::FT_Byte*  q;
			p = cmap->data + 14;               /* ends table   */
      q = cmap->data + 16 + num_segs2;   /* starts table */
      for ( i = 0; i < num_segs; i++ )
      {
        end   = TT_NEXT_USHORT( p );
        start = TT_NEXT_USHORT( q );
				if ( charcode >= start && charcode <= end )
        {
          p       = q - 2 + num_segs2;
          delta   = TT_PEEK_SHORT( p );
          p      += num_segs2;
          offset  = TT_PEEK_USHORT( p );
					if ( i >= num_segs - 1                  &&
						start == 0xFFFFU && end == 0xFFFFU )
					{
						TT_Face   face  = (TT_Face)cmap->cmap.charmap.face;
            ft_structs_h::FT_Byte*  limit = face->cmap_table + face->cmap_size;
						if ( offset && p + offset + 2 > limit )
            {
              delta  = 1;
              offset = 0;
            }
          }
					if ( offset == 0xFFFFU )
						continue;
					if ( offset )
          {
            p += offset + ( charcode - start ) * 2;
            gindex = TT_PEEK_USHORT( p );
            if ( gindex != 0 )
              gindex = (ft_structs_h::FT_UInt)( gindex + delta ) & 0xFFFFU;
          }
          else
            gindex = (ft_structs_h::FT_UInt)( charcode + delta ) & 0xFFFFU;
					break;
        }
      }
			if ( !next || gindex )
				break;
		}
		if ( next && gindex )
			*pcharcode = charcode;
		return gindex;
	};
	static ft_structs_h::FT_UInt
		tt_cmap4_char_map_binary( TT_CMap     cmap,
		ft_structs_h::FT_UInt32*  pcharcode,
		ft_structs_h::FT_Bool     next )
	{
    ft_structs_h::FT_UInt   num_segs2, start, end, offset;
    ft_structs_h::FT_Int    delta;
    ft_structs_h::FT_UInt   max, min, mid, num_segs;
    ft_structs_h::FT_UInt   charcode = (ft_structs_h::FT_UInt)*pcharcode;
    ft_structs_h::FT_UInt   gindex   = 0;
    ft_structs_h::FT_Byte*  p;
		p = cmap->data + 6;
    num_segs2 = FT_PAD_FLOOR( TT_PEEK_USHORT( p ), 2 );
    if ( !num_segs2 )
      return 0;
    num_segs = num_segs2 >> 1;
    /* make compiler happy */
    mid = num_segs;
    end = 0xFFFFU;
    if ( next )
      charcode++;
    min = 0;
    max = num_segs;
    /* binary search */
    while ( min < max )
    {
      mid    = ( min + max ) >> 1;
      p      = cmap->data + 14 + mid * 2;
      end    = TT_PEEK_USHORT( p );
      p     += 2 + num_segs2;
      start  = TT_PEEK_USHORT( p );
			if ( charcode < start )
        max = mid;
      else if ( charcode > end )
        min = mid + 1;
      else
      {
        p     += num_segs2;
        delta  = TT_PEEK_SHORT( p );
        p     += num_segs2;
        offset = TT_PEEK_USHORT( p );
        if ( mid >= num_segs - 1                &&
					start == 0xFFFFU && end == 0xFFFFU )
				{
					TT_Face   face  = (TT_Face)cmap->cmap.charmap.face;
          ft_structs_h::FT_Byte*  limit = face->cmap_table + face->cmap_size;
					if ( offset && p + offset + 2 > limit )
          {
            delta  = 1;
            offset = 0;
          }
        }
        if ( cmap->flags & TT_CMAP_FLAG_OVERLAPPING )
        {
          ft_structs_h::FT_UInt  i;
          max = mid;
					if ( offset == 0xFFFFU )
						mid = max + 1;
					/* search in segments before the current segment */
					for ( i = max ; i > 0; i-- )
          {
            ft_structs_h::FT_UInt   prev_end;
            ft_structs_h::FT_Byte*  old_p;
						old_p    = p;
            p        = cmap->data + 14 + ( i - 1 ) * 2;
            prev_end = TT_PEEK_USHORT( p );
						if ( charcode > prev_end )
            {
              p = old_p;
              break;
            }
						end    = prev_end;
            p     += 2 + num_segs2;
            start  = TT_PEEK_USHORT( p );
            p     += num_segs2;
            delta  = TT_PEEK_SHORT( p );
            p     += num_segs2;
            offset = TT_PEEK_USHORT( p );
						if ( offset != 0xFFFFU )
							mid = i - 1;
          }
          if ( mid == max + 1 )
          {
            if ( i != max )
            {
              p      = cmap->data + 14 + max * 2;
              end    = TT_PEEK_USHORT( p );
              p     += 2 + num_segs2;
              start  = TT_PEEK_USHORT( p );
              p     += num_segs2;
              delta  = TT_PEEK_SHORT( p );
              p     += num_segs2;
              offset = TT_PEEK_USHORT( p );
            }
						mid = max;
            for ( i = max + 1; i < num_segs; i++ )
            {
              ft_structs_h::FT_UInt  next_end, next_start;
							p          = cmap->data + 14 + i * 2;
              next_end   = TT_PEEK_USHORT( p );
              p         += 2 + num_segs2;
              next_start = TT_PEEK_USHORT( p );
							if ( charcode < next_start )
                break;
							end    = next_end;
              start  = next_start;
              p     += num_segs2;
              delta  = TT_PEEK_SHORT( p );
              p     += num_segs2;
              offset = TT_PEEK_USHORT( p );
							if ( offset != 0xFFFFU )
								mid = i;
            }
            i--;
            if ( mid == max )
            {
              mid = i;
							break;
            }
          }
          if ( mid != i )
          {
            p      = cmap->data + 14 + mid * 2;
            end    = TT_PEEK_USHORT( p );
            p     += 2 + num_segs2;
            start  = TT_PEEK_USHORT( p );
            p     += num_segs2;
            delta  = TT_PEEK_SHORT( p );
            p     += num_segs2;
            offset = TT_PEEK_USHORT( p );
          }
        }
        else
        {
          if ( offset == 0xFFFFU )
            break;
        }
				if ( offset )
        {
          p += offset + ( charcode - start ) * 2;
          gindex = TT_PEEK_USHORT( p );
          if ( gindex != 0 )
						gindex = (ft_structs_h::FT_UInt)( gindex + delta ) & 0xFFFFU;
        }
        else
          gindex = (ft_structs_h::FT_UInt)( charcode + delta ) & 0xFFFFU;
				break;
      }
    }
    if ( next )
    {
      TT_CMap4  cmap4 = (TT_CMap4)cmap;
      if ( charcode > end )
      {
        mid++;
        if ( mid == num_segs )
          return 0;
      }
			if ( tt_cmap4_set_range( cmap4, mid ) )
      {
        if ( gindex )
          *pcharcode = charcode;
      }
      else
      {
        cmap4->cur_charcode = charcode;
				if ( gindex )
          cmap4->cur_gindex = gindex;
        else
        {
          cmap4->cur_charcode = charcode;
          tt_cmap4_next( cmap4 );
          gindex = cmap4->cur_gindex;
        }
				if ( gindex )
          *pcharcode = cmap4->cur_charcode;
      }
    }
    return gindex;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap4_char_index( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  char_code )
  {
    if ( char_code >= 0x10000UL )
      return 0;
    if ( cmap->flags & TT_CMAP_FLAG_UNSORTED )
      return tt_cmap4_char_map_linear( cmap, &char_code, 0 );
    else
      return tt_cmap4_char_map_binary( cmap, &char_code, 0 );
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 )
		tt_cmap4_char_next( TT_CMap     cmap,
		ft_structs_h::FT_UInt32  *pchar_code )
  {
    ft_structs_h::FT_UInt  gindex;
		if ( *pchar_code >= 0xFFFFU )
			return 0;
		if ( cmap->flags & TT_CMAP_FLAG_UNSORTED )
      gindex = tt_cmap4_char_map_linear( cmap, pchar_code, 1 );
    else
    {
      TT_CMap4  cmap4 = (TT_CMap4)cmap;
      if ( *pchar_code == cmap4->cur_charcode )
      {
        tt_cmap4_next( cmap4 );
        gindex = cmap4->cur_gindex;
        if ( gindex )
          *pchar_code = cmap4->cur_charcode;
      }
      else
        gindex = tt_cmap4_char_map_binary( cmap, pchar_code, 1 );
    }
    return gindex;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap4_get_info( TT_CMap       cmap,
		TT_CMapInfo  *cmap_info )
	{
    ft_structs_h::FT_Byte*  p = cmap->data + 4;
		cmap_info->format   = 4;
    cmap_info->language = (ft_structs_h::FT_ULong)TT_PEEK_USHORT( p );
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_DEFINE_TT_CMAP(tt_cmap4_class_rec,
		sizeof ( TT_CMap4Rec ),
		(FT_CMap_InitFunc)     tt_cmap4_init,
		(FT_CMap_DoneFunc)     NULL,
		(FT_CMap_CharIndexFunc)tt_cmap4_char_index,
		(FT_CMap_CharNextFunc) tt_cmap4_char_next,
		NULL, NULL, NULL, NULL, NULL
		,
		4,
    (TT_CMap_ValidateFunc)   tt_cmap4_validate,
    (TT_CMap_Info_GetFunc)   tt_cmap4_get_info
  )
#endif /* TT_CONFIG_CMAP_FORMAT_4 */
#ifdef TT_CONFIG_CMAP_FORMAT_6
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
	tt_cmap6_validate( ft_structs_h::FT_Byte*      table,
	ft_structs_h::FT_Validator  valid )
	{
    ft_structs_h::FT_Byte*  p;
    ft_structs_h::FT_UInt   length, count;
		if ( table + 10 > valid->limit )
      FT_INVALID_TOO_SHORT;
		p      = table + 2;
    length = TT_NEXT_USHORT( p );
		p      = table + 8;             /* skip language and start index */
    count  = TT_NEXT_USHORT( p );
		if ( table + length > valid->limit || length < 10 + count * 2 )
      FT_INVALID_TOO_SHORT;		
    if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
    {
      ft_structs_h::FT_UInt  gindex;
			for ( ; count > 0; count-- )
      {
        gindex = TT_NEXT_USHORT( p );
        if ( gindex >= TT_VALID_GLYPH_COUNT( valid ) )
          FT_INVALID_GLYPH_ID;
      }
    }
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap6_char_index( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  char_code )
	{
    ft_structs_h::FT_Byte*  table  = cmap->data;
    ft_structs_h::FT_UInt   result = 0;
    ft_structs_h::FT_Byte*  p      = table + 6;
    ft_structs_h::FT_UInt   start  = TT_NEXT_USHORT( p );
    ft_structs_h::FT_UInt   count  = TT_NEXT_USHORT( p );
    ft_structs_h::FT_UInt   idx    = (ft_structs_h::FT_UInt)( char_code - start );
		if ( idx < count )
    {
      p += 2 * idx;
      result = TT_PEEK_USHORT( p );
    }
    return result;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 )
		tt_cmap6_char_next( TT_CMap     cmap,
		ft_structs_h::FT_UInt32  *pchar_code )
	{
    ft_structs_h::FT_Byte*   table     = cmap->data;
    ft_structs_h::FT_UInt32  result    = 0;
    ft_structs_h::FT_UInt32  char_code = *pchar_code + 1;
    ft_structs_h::FT_UInt    gindex    = 0;
		ft_structs_h::FT_Byte*   p         = table + 6;
    ft_structs_h::FT_UInt    start     = TT_NEXT_USHORT( p );
    ft_structs_h::FT_UInt    count     = TT_NEXT_USHORT( p );
    ft_structs_h::FT_UInt    idx;
		if ( char_code >= 0x10000UL )
      goto Exit;
		if ( char_code < start )
      char_code = start;
		idx = (ft_structs_h::FT_UInt)( char_code - start );
    p  += 2 * idx;
		for ( ; idx < count; idx++ )
    {
      gindex = TT_NEXT_USHORT( p );
      if ( gindex != 0 )
      {
        result = char_code;
        break;
      }
      char_code++;
    }
Exit:
		*pchar_code = result;
		return gindex;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap6_get_info( TT_CMap       cmap,
		TT_CMapInfo  *cmap_info )
	{
    ft_structs_h::FT_Byte*  p = cmap->data + 4;
		cmap_info->format   = 6;
    cmap_info->language = (ft_structs_h::FT_ULong)TT_PEEK_USHORT( p );
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_DEFINE_TT_CMAP(tt_cmap6_class_rec,
		sizeof ( TT_CMapRec ),
		(FT_CMap_InitFunc)     tt_cmap_init,
		(FT_CMap_DoneFunc)     NULL,
		(FT_CMap_CharIndexFunc)tt_cmap6_char_index,
		(FT_CMap_CharNextFunc) tt_cmap6_char_next,
		NULL, NULL, NULL, NULL, NULL
		,
    6,
    (TT_CMap_ValidateFunc)   tt_cmap6_validate,
    (TT_CMap_Info_GetFunc)   tt_cmap6_get_info
  )
#endif /* TT_CONFIG_CMAP_FORMAT_6 */
#ifdef TT_CONFIG_CMAP_FORMAT_8
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
	tt_cmap8_validate( ft_structs_h::FT_Byte*      table,
	ft_structs_h::FT_Validator  valid )
	{
    ft_structs_h::FT_Byte*   p = table + 4;
    ft_structs_h::FT_Byte*   is32;
    ft_structs_h::FT_UInt32  length;
    ft_structs_h::FT_UInt32  num_groups;
		if ( table + 16 + 8192 > valid->limit )
      FT_INVALID_TOO_SHORT;
		length = TT_NEXT_ULONG( p );
		if ( length > (ft_structs_h::FT_UInt32)( valid->limit - table ) || length < 8192 + 16 )
			FT_INVALID_TOO_SHORT;
		is32       = table + 12;
    p          = is32  + 8192;          /* skip `is32' array */
    num_groups = TT_NEXT_ULONG( p );
		if ( p + num_groups * 12 > valid->limit )
			FT_INVALID_TOO_SHORT;
    {
      ft_structs_h::FT_UInt32  n, start, end, start_id, count, last = 0;
			for ( n = 0; n < num_groups; n++ )
      {
        ft_structs_h::FT_UInt   hi, lo;
				start    = TT_NEXT_ULONG( p );
        end      = TT_NEXT_ULONG( p );
        start_id = TT_NEXT_ULONG( p );
        if ( start > end )
          FT_INVALID_DATA;
				if ( n > 0 && start <= last )
          FT_INVALID_DATA;
				if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
        {
          if ( start_id + end - start >= TT_VALID_GLYPH_COUNT( valid ) )
            FT_INVALID_GLYPH_ID;
					count = (ft_structs_h::FT_UInt32)( end - start + 1 );
					if ( start & ~0xFFFFU )
          {
            for ( ; count > 0; count--, start++ )
            {
              hi = (ft_structs_h::FT_UInt)( start >> 16 );
              lo = (ft_structs_h::FT_UInt)( start & 0xFFFFU );
							if ( (is32[hi >> 3] & ( 0x80 >> ( hi & 7 ) ) ) == 0 )
                FT_INVALID_DATA;
							if ( (is32[lo >> 3] & ( 0x80 >> ( lo & 7 ) ) ) == 0 )
                FT_INVALID_DATA;
            }
          }
          else
          {
            if ( end & ~0xFFFFU )
              FT_INVALID_DATA;
            for ( ; count > 0; count--, start++ )
            {
              lo = (ft_structs_h::FT_UInt)( start & 0xFFFFU );
							if ( (is32[lo >> 3] & ( 0x80 >> ( lo & 7 ) ) ) != 0 )
                FT_INVALID_DATA;
            }
          }
        }
				last = end;
      }
    }
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap8_char_index( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  char_code )
	{
    ft_structs_h::FT_Byte*   table      = cmap->data;
    ft_structs_h::FT_UInt    result     = 0;
    ft_structs_h::FT_Byte*   p          = table + 8204;
    ft_structs_h::FT_UInt32  num_groups = TT_NEXT_ULONG( p );
    ft_structs_h::FT_UInt32  start, end, start_id;
		for ( ; num_groups > 0; num_groups-- )
    {
      start    = TT_NEXT_ULONG( p );
      end      = TT_NEXT_ULONG( p );
      start_id = TT_NEXT_ULONG( p );
			if ( char_code < start )
				break;
			if ( char_code <= end )
      {
        result = (ft_structs_h::FT_UInt)( start_id + char_code - start );
        break;
      }
    }
    return result;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 )
		tt_cmap8_char_next( TT_CMap     cmap,
		ft_structs_h::FT_UInt32  *pchar_code )
	{
    ft_structs_h::FT_UInt32  result     = 0;
    ft_structs_h::FT_UInt32  char_code  = *pchar_code + 1;
    ft_structs_h::FT_UInt    gindex     = 0;
    ft_structs_h::FT_Byte*   table      = cmap->data;
    ft_structs_h::FT_Byte*   p          = table + 8204;
    ft_structs_h::FT_UInt32  num_groups = TT_NEXT_ULONG( p );
    ft_structs_h::FT_UInt32  start, end, start_id;
		p = table + 8208;
		for ( ; num_groups > 0; num_groups-- )
    {
      start    = TT_NEXT_ULONG( p );
      end      = TT_NEXT_ULONG( p );
      start_id = TT_NEXT_ULONG( p );
			if ( char_code < start )
        char_code = start;
			if ( char_code <= end )
      {
        gindex = (ft_structs_h::FT_UInt)( char_code - start + start_id );
        if ( gindex != 0 )
        {
          result = char_code;
          goto Exit;
        }
      }
    }
Exit:
		*pchar_code = result;
		return gindex;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap8_get_info( TT_CMap       cmap,
		TT_CMapInfo  *cmap_info )
	{
    ft_structs_h::FT_Byte*  p = cmap->data + 8;
		cmap_info->format   = 8;
		cmap_info->language = (ft_structs_h::FT_ULong)TT_PEEK_ULONG( p );
		return ft_error_h::SFNT_Err_Ok;
	};
	FT_DEFINE_TT_CMAP(tt_cmap8_class_rec,
		sizeof ( TT_CMapRec ),
		(FT_CMap_InitFunc)     tt_cmap_init,
		(FT_CMap_DoneFunc)     NULL,
		(FT_CMap_CharIndexFunc)tt_cmap8_char_index,
		(FT_CMap_CharNextFunc) tt_cmap8_char_next,
		NULL, NULL, NULL, NULL, NULL
		,
		8,
		(TT_CMap_ValidateFunc)   tt_cmap8_validate,
		(TT_CMap_Info_GetFunc)   tt_cmap8_get_info
		)
#endif /* TT_CONFIG_CMAP_FORMAT_8 */
#ifdef TT_CONFIG_CMAP_FORMAT_10
		FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap10_validate( ft_structs_h::FT_Byte*      table,
		ft_structs_h::FT_Validator  valid )
	{
    ft_structs_h::FT_Byte*  p = table + 4;
    ft_structs_h::FT_ULong  length, count;
		if ( table + 20 > valid->limit )
			FT_INVALID_TOO_SHORT;
		length = TT_NEXT_ULONG( p );
    p      = table + 16;
    count  = TT_NEXT_ULONG( p );
		if ( length > (ft_structs_h::FT_ULong)( valid->limit - table ) ||
			length < 20 + count * 2                     )
			FT_INVALID_TOO_SHORT;
		/* check glyph indices */
    if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
    {
      ft_structs_h::FT_UInt  gindex;
			for ( ; count > 0; count-- )
      {
        gindex = TT_NEXT_USHORT( p );
        if ( gindex >= TT_VALID_GLYPH_COUNT( valid ) )
          FT_INVALID_GLYPH_ID;
      }
    }
		return ft_error_h::SFNT_Err_Ok;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap10_char_index( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  char_code )
	{
    ft_structs_h::FT_Byte*   table  = cmap->data;
    ft_structs_h::FT_UInt    result = 0;
    ft_structs_h::FT_Byte*   p      = table + 12;
    ft_structs_h::FT_UInt32  start  = TT_NEXT_ULONG( p );
    ft_structs_h::FT_UInt32  count  = TT_NEXT_ULONG( p );
    ft_structs_h::FT_UInt32  idx    = (ft_structs_h::FT_ULong)( char_code - start );
		if ( idx < count )
		{
      p     += 2 * idx;
      result = TT_PEEK_USHORT( p );
    }
    return result;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 )
		tt_cmap10_char_next( TT_CMap     cmap,
		ft_structs_h::FT_UInt32  *pchar_code )
	{
    ft_structs_h::FT_Byte*   table     = cmap->data;
    ft_structs_h::FT_UInt32  char_code = *pchar_code + 1;
    ft_structs_h::FT_UInt    gindex    = 0;
    ft_structs_h::FT_Byte*   p         = table + 12;
    ft_structs_h::FT_UInt32  start     = TT_NEXT_ULONG( p );
    ft_structs_h::FT_UInt32  count     = TT_NEXT_ULONG( p );
    ft_structs_h::FT_UInt32  idx;
		if ( char_code < start )
      char_code = start;
		idx = (ft_structs_h::FT_UInt32)( char_code - start );
    p  += 2 * idx;
		for ( ; idx < count; idx++ )
    {
      gindex = TT_NEXT_USHORT( p );
      if ( gindex != 0 )
        break;
      char_code++;
    }
		*pchar_code = char_code;
    return gindex;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap10_get_info( TT_CMap       cmap,
		TT_CMapInfo  *cmap_info )
  {
    ft_structs_h::FT_Byte*  p = cmap->data + 8;
		cmap_info->format   = 10;
		cmap_info->language = (ft_structs_h::FT_ULong)TT_PEEK_ULONG( p );
		return ft_error_h::SFNT_Err_Ok;
  }
	FT_DEFINE_TT_CMAP(tt_cmap10_class_rec,
		sizeof ( TT_CMapRec ),
		(FT_CMap_InitFunc)     tt_cmap_init,
		(FT_CMap_DoneFunc)     NULL,
		(FT_CMap_CharIndexFunc)tt_cmap10_char_index,
		(FT_CMap_CharNextFunc) tt_cmap10_char_next,
		NULL, NULL, NULL, NULL, NULL
		,
		10,
    (TT_CMap_ValidateFunc)   tt_cmap10_validate,
    (TT_CMap_Info_GetFunc)   tt_cmap10_get_info
  )
#endif /* TT_CONFIG_CMAP_FORMAT_10 */
#ifdef TT_CONFIG_CMAP_FORMAT_12
	typedef struct  TT_CMap12Rec_
  {
    TT_CMapRec  cmap;
    ft_structs_h::FT_Bool     valid;
    ft_structs_h::FT_ULong    cur_charcode;
    ft_structs_h::FT_UInt     cur_gindex;
    ft_structs_h::FT_ULong    cur_group;
    ft_structs_h::FT_ULong    num_groups;
	} TT_CMap12Rec, *TT_CMap12;
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap12_init( TT_CMap12  cmap,
		ft_structs_h::FT_Byte*   table )
	{
		cmap->cmap.data  = table;
		table           += 12;
    cmap->num_groups = FT_PEEK_ULONG( table );
		cmap->valid      = 0;
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap12_validate( ft_structs_h::FT_Byte*      table,
		ft_structs_h::FT_Validator  valid )
  {
    ft_structs_h::FT_Byte*   p;
    ft_structs_h::FT_ULong   length;
    ft_structs_h::FT_ULong   num_groups;
		if ( table + 16 > valid->limit )
      FT_INVALID_TOO_SHORT;
		p      = table + 4;
    length = TT_NEXT_ULONG( p );
		p          = table + 12;
    num_groups = TT_NEXT_ULONG( p );
		if ( length > (ft_structs_h::FT_ULong)( valid->limit - table ) ||
			length < 16 + 12 * num_groups               )
			FT_INVALID_TOO_SHORT;
		/* check groups, they must be in increasing order */
		{
			ft_structs_h::FT_ULong  n, start, end, start_id, last = 0;
			for ( n = 0; n < num_groups; n++ )
      {
        start    = TT_NEXT_ULONG( p );
        end      = TT_NEXT_ULONG( p );
        start_id = TT_NEXT_ULONG( p );
				if ( start > end )
					FT_INVALID_DATA;
				if ( n > 0 && start <= last )
					FT_INVALID_DATA;
				if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
				{
					if ( start_id + end - start >= TT_VALID_GLYPH_COUNT( valid ) )
						FT_INVALID_GLYPH_ID;
				}
				last = end;
      }
    }
		return ft_error_h::SFNT_Err_Ok;
	};
	static void
		tt_cmap12_next( TT_CMap12  cmap )
	{
		ft_structs_h::FT_Byte*  p;
    ft_structs_h::FT_ULong  start, end, start_id, char_code;
    ft_structs_h::FT_ULong  n;
    ft_structs_h::FT_UInt   gindex;
		if ( cmap->cur_charcode >= 0xFFFFFFFFUL )
			goto Fail;
		char_code = cmap->cur_charcode + 1;
		n = cmap->cur_group;
		for ( n = cmap->cur_group; n < cmap->num_groups; n++ )
    {
      p        = cmap->cmap.data + 16 + 12 * n;
      start    = TT_NEXT_ULONG( p );
      end      = TT_NEXT_ULONG( p );
      start_id = TT_PEEK_ULONG( p );
			if ( char_code < start )
				char_code = start;
			for ( ; char_code <= end; char_code++ )
      {
        gindex = (ft_structs_h::FT_UInt)( start_id + char_code - start );
				if ( gindex )
        {
          cmap->cur_charcode = char_code;;
          cmap->cur_gindex   = gindex;
          cmap->cur_group    = n;
					return;
        }
      }
    }
Fail:
		cmap->valid = 0;
	};
	static ft_structs_h::FT_UInt
		tt_cmap12_char_map_binary( TT_CMap     cmap,
		ft_structs_h::FT_UInt32*  pchar_code,
		ft_structs_h::FT_Bool     next )
  {
    ft_structs_h::FT_UInt    gindex     = 0;
    ft_structs_h::FT_Byte*   p          = cmap->data + 12;
    ft_structs_h::FT_UInt32  num_groups = TT_PEEK_ULONG( p );
    ft_structs_h::FT_UInt32  char_code  = *pchar_code;
    ft_structs_h::FT_UInt32  start, end, start_id;
    ft_structs_h::FT_UInt32  max, min, mid;
		if ( !num_groups )
			return 0;
		mid = num_groups;
    end = 0xFFFFFFFFUL;
		if ( next )
			char_code++;
		min = 0;
		max = num_groups;
    while ( min < max )
    {
      mid = ( min + max ) >> 1;
      p   = cmap->data + 16 + 12 * mid;
			start = TT_NEXT_ULONG( p );
      end   = TT_NEXT_ULONG( p );
			if ( char_code < start )
        max = mid;
      else if ( char_code > end )
        min = mid + 1;
      else
      {
        start_id = TT_PEEK_ULONG( p );
        gindex = (ft_structs_h::FT_UInt)( start_id + char_code - start );
				break;
      }
    }
		if ( next )
		{
			TT_CMap12  cmap12 = (TT_CMap12)cmap;
      if ( char_code > end )
      {
        mid++;
        if ( mid == num_groups )
          return 0;
      }
			cmap12->valid        = 1;
			cmap12->cur_charcode = char_code;
			cmap12->cur_group    = mid;
			if ( !gindex )
      {
        tt_cmap12_next( cmap12 );
				if ( cmap12->valid )
          gindex = cmap12->cur_gindex;
      }
      else
        cmap12->cur_gindex = gindex;
			if ( gindex )
				*pchar_code = cmap12->cur_charcode;
		}
		return gindex;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap12_char_index( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  char_code )
	{
		return tt_cmap12_char_map_binary( cmap, &char_code, 0 );
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 )
		tt_cmap12_char_next( TT_CMap     cmap,
		ft_structs_h::FT_UInt32  *pchar_code )
	{
		TT_CMap12  cmap12 = (TT_CMap12)cmap;
		ft_structs_h::FT_ULong   gindex;
		if ( cmap12->cur_charcode >= 0xFFFFFFFFUL )
			return 0;
    if ( cmap12->valid && cmap12->cur_charcode == *pchar_code )
    {
      tt_cmap12_next( cmap12 );
      if ( cmap12->valid )
      {
        gindex = cmap12->cur_gindex;
				if ( gindex )
					*pchar_code = (ft_structs_h::FT_UInt32)cmap12->cur_charcode;
			}
      else
        gindex = 0;
    }
    else
      gindex = tt_cmap12_char_map_binary( cmap, pchar_code, 1 );
    return (ft_structs_h::FT_UInt32)gindex;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap12_get_info( TT_CMap       cmap,
		TT_CMapInfo  *cmap_info )
	{
		ft_structs_h::FT_Byte*  p = cmap->data + 8;
		cmap_info->format   = 12;
    cmap_info->language = (ft_structs_h::FT_ULong)TT_PEEK_ULONG( p );
    return ft_error_h::SFNT_Err_Ok;
  };
	FT_DEFINE_TT_CMAP(tt_cmap12_class_rec,
		sizeof ( TT_CMap12Rec ),
		(FT_CMap_InitFunc)     tt_cmap12_init,
		(FT_CMap_DoneFunc)     NULL,
		(FT_CMap_CharIndexFunc)tt_cmap12_char_index,
		(FT_CMap_CharNextFunc) tt_cmap12_char_next,
		NULL, NULL, NULL, NULL, NULL
		,
    12,
    (TT_CMap_ValidateFunc)   tt_cmap12_validate,
    (TT_CMap_Info_GetFunc)   tt_cmap12_get_info
		)
#endif /* TT_CONFIG_CMAP_FORMAT_12 */
#ifdef TT_CONFIG_CMAP_FORMAT_13
	typedef struct  TT_CMap13Rec_
	{
		TT_CMapRec  cmap;
    ft_structs_h::FT_Bool     valid;
    ft_structs_h::FT_ULong    cur_charcode;
    ft_structs_h::FT_UInt     cur_gindex;
    ft_structs_h::FT_ULong    cur_group;
    ft_structs_h::FT_ULong    num_groups;
	} TT_CMap13Rec, *TT_CMap13;
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
  tt_cmap13_init( TT_CMap13  cmap,
                  ft_structs_h::FT_Byte*   table )
  {
    cmap->cmap.data  = table;
		table           += 12;
    cmap->num_groups = FT_PEEK_ULONG( table );
		cmap->valid      = 0;
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap13_validate( ft_structs_h::FT_Byte*      table,
		ft_structs_h::FT_Validator  valid )
  {
    ft_structs_h::FT_Byte*   p;
    ft_structs_h::FT_ULong   length;
    ft_structs_h::FT_ULong   num_groups;
		if ( table + 16 > valid->limit )
			FT_INVALID_TOO_SHORT;
		p      = table + 4;
		length = TT_NEXT_ULONG( p );
		p          = table + 12;
    num_groups = TT_NEXT_ULONG( p );
		if ( length > (ft_structs_h::FT_ULong)( valid->limit - table ) ||
			length < 16 + 12 * num_groups               )
			FT_INVALID_TOO_SHORT;
		{
			ft_structs_h::FT_ULong  n, start, end, glyph_id, last = 0;
			for ( n = 0; n < num_groups; n++ )
      {
        start    = TT_NEXT_ULONG( p );
        end      = TT_NEXT_ULONG( p );
        glyph_id = TT_NEXT_ULONG( p );
				if ( start > end )
          FT_INVALID_DATA;
				if ( n > 0 && start <= last )
          FT_INVALID_DATA;
				if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT )
        {
          if ( glyph_id >= TT_VALID_GLYPH_COUNT( valid ) )
            FT_INVALID_GLYPH_ID;
        }
				last = end;
      }
    }
		return ft_error_h::SFNT_Err_Ok;
  };
	static void
		tt_cmap13_next( TT_CMap13  cmap )
	{
    ft_structs_h::FT_Byte*  p;
    ft_structs_h::FT_ULong  start, end, glyph_id, char_code;
    ft_structs_h::FT_ULong  n;
    ft_structs_h::FT_UInt   gindex;
		if ( cmap->cur_charcode >= 0xFFFFFFFFUL )
			goto Fail;
		char_code = cmap->cur_charcode + 1;
		n = cmap->cur_group;
		for ( n = cmap->cur_group; n < cmap->num_groups; n++ )
    {
      p        = cmap->cmap.data + 16 + 12 * n;
      start    = TT_NEXT_ULONG( p );
      end      = TT_NEXT_ULONG( p );
      glyph_id = TT_PEEK_ULONG( p );
			if ( char_code < start )
				char_code = start;
			if ( char_code <= end )
			{
				gindex = (ft_structs_h::FT_UInt)glyph_id;
				if ( gindex )
				{
          cmap->cur_charcode = char_code;;
          cmap->cur_gindex   = gindex;
          cmap->cur_group    = n;
					return;
        }
      }
    }
Fail:
		cmap->valid = 0;
	};
	static ft_structs_h::FT_UInt
		tt_cmap13_char_map_binary( TT_CMap     cmap,
		ft_structs_h::FT_UInt32*  pchar_code,
		ft_structs_h::FT_Bool     next )
	{
    ft_structs_h::FT_UInt    gindex     = 0;
    ft_structs_h::FT_Byte*   p          = cmap->data + 12;
    ft_structs_h::FT_UInt32  num_groups = TT_PEEK_ULONG( p );
    ft_structs_h::FT_UInt32  char_code  = *pchar_code;
    ft_structs_h::FT_UInt32  start, end;
    ft_structs_h::FT_UInt32  max, min, mid;
		if ( !num_groups )
			return 0;
		mid = num_groups;
		end = 0xFFFFFFFFUL;
		if ( next )
			char_code++;
		min = 0;
		max = num_groups;
    while ( min < max )
    {
      mid = ( min + max ) >> 1;
      p   = cmap->data + 16 + 12 * mid;
			start = TT_NEXT_ULONG( p );
      end   = TT_NEXT_ULONG( p );
			if ( char_code < start )
        max = mid;
      else if ( char_code > end )
        min = mid + 1;
      else
      {
				gindex = (ft_structs_h::FT_UInt)TT_PEEK_ULONG( p );
				break;
      }
    }
		if ( next )
    {
      TT_CMap13  cmap13 = (TT_CMap13)cmap;
			if ( char_code > end )
      {
        mid++;
        if ( mid == num_groups )
          return 0;
      }
			cmap13->valid        = 1;
      cmap13->cur_charcode = char_code;
      cmap13->cur_group    = mid;
			if ( !gindex )
      {
        tt_cmap13_next( cmap13 );
				if ( cmap13->valid )
          gindex = cmap13->cur_gindex;
			}
      else
        cmap13->cur_gindex = gindex;
			if ( gindex )
        *pchar_code = cmap13->cur_charcode;
    }
		return gindex;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap13_char_index( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  char_code )
	{
    return tt_cmap13_char_map_binary( cmap, &char_code, 0 );
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 )
		tt_cmap13_char_next( TT_CMap     cmap,
		ft_structs_h::FT_UInt32  *pchar_code )
	{
    TT_CMap13  cmap13 = (TT_CMap13)cmap;
    ft_structs_h::FT_UInt    gindex;
		if ( cmap13->cur_charcode >= 0xFFFFFFFFUL )
			return 0;
    if ( cmap13->valid && cmap13->cur_charcode == *pchar_code )
    {
      tt_cmap13_next( cmap13 );
      if ( cmap13->valid )
      {
        gindex = cmap13->cur_gindex;
        if ( gindex )
          *pchar_code = cmap13->cur_charcode;
      }
      else
        gindex = 0;
    }
    else
      gindex = tt_cmap13_char_map_binary( cmap, pchar_code, 1 );
		return gindex;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap13_get_info( TT_CMap       cmap,
		TT_CMapInfo  *cmap_info )
	{
    ft_structs_h::FT_Byte*  p = cmap->data + 8;
		cmap_info->format   = 13;
		cmap_info->language = (ft_structs_h::FT_ULong)TT_PEEK_ULONG( p );
		return ft_error_h::SFNT_Err_Ok;
	};
	FT_DEFINE_TT_CMAP(tt_cmap13_class_rec,
		sizeof ( TT_CMap13Rec ),
		(FT_CMap_InitFunc)     tt_cmap13_init,
		(FT_CMap_DoneFunc)     NULL,
		(FT_CMap_CharIndexFunc)tt_cmap13_char_index,
		(FT_CMap_CharNextFunc) tt_cmap13_char_next,
		NULL, NULL, NULL, NULL, NULL
		,
		13,
		(TT_CMap_ValidateFunc)   tt_cmap13_validate,
		(TT_CMap_Info_GetFunc)   tt_cmap13_get_info
		)
#endif /* TT_CONFIG_CMAP_FORMAT_13 */
#ifdef TT_CONFIG_CMAP_FORMAT_14
	typedef struct  TT_CMap14Rec_
  {
    TT_CMapRec  cmap;
    ft_structs_h::FT_ULong    num_selectors;
		ft_structs_h::FT_UInt32   max_results;
    ft_structs_h::FT_UInt32*  results;
    ft_structs_h::FT_Memory   memory;
	} TT_CMap14Rec, *TT_CMap14;
	FT_CALLBACK_DEF( void )
		tt_cmap14_done( TT_CMap14  cmap )
	{
		ft_structs_h::FT_Memory  memory = cmap->memory;
		cmap->max_results = 0;
    if ( memory != NULL && cmap->results != NULL )
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
			FT_FREE( cmap->results );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
  };
	static ft_structs_h::FT_Error
		tt_cmap14_ensure( TT_CMap14  cmap,
		ft_structs_h::FT_UInt32  num_results,
		ft_structs_h::FT_Memory  memory )
	{
    ft_structs_h::FT_UInt32 old_max = cmap->max_results;
    ft_structs_h::FT_Error  error   = ft_error_h::SFNT_Err_Ok;
		if ( num_results > cmap->max_results )
    {
       cmap->memory = memory;
			 if ( FT_QRENEW_ARRAY( cmap->results, old_max, num_results ) )
         return error;
			 cmap->max_results = num_results;
		}
		return error;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap14_init( TT_CMap14  cmap,
		ft_structs_h::FT_Byte*   table )
	{
    cmap->cmap.data = table;
		table               += 6;
    cmap->num_selectors = FT_PEEK_ULONG( table );
    cmap->max_results   = 0;
    cmap->results       = NULL;
		return ft_error_h::SFNT_Err_Ok;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap14_validate( ft_structs_h::FT_Byte*      table,
		ft_structs_h::FT_Validator  valid )
	{
    ft_structs_h::FT_Byte*  p             = table + 2;
    ft_structs_h::FT_ULong  length        = TT_NEXT_ULONG( p );
    ft_structs_h::FT_ULong  num_selectors = TT_NEXT_ULONG( p );
		if ( length > (ft_structs_h::FT_ULong)( valid->limit - table ) ||
			length < 10 + 11 * num_selectors            )
			FT_INVALID_TOO_SHORT;
		{
			ft_structs_h::FT_ULong  n, lastVarSel = 1;
			for ( n = 0; n < num_selectors; n++ )
      {
        ft_structs_h::FT_ULong  varSel    = TT_NEXT_UINT24( p );
        ft_structs_h::FT_ULong  defOff    = TT_NEXT_ULONG( p );
        ft_structs_h::FT_ULong  nondefOff = TT_NEXT_ULONG( p );
				if ( defOff >= length || nondefOff >= length )
          FT_INVALID_TOO_SHORT;
				if ( varSel < lastVarSel )
          FT_INVALID_DATA;
				lastVarSel = varSel + 1;
        if ( defOff != 0 )
        {
          ft_structs_h::FT_Byte*  defp      = table + defOff;
          ft_structs_h::FT_ULong  numRanges = TT_NEXT_ULONG( defp );
          ft_structs_h::FT_ULong  i;
          ft_structs_h::FT_ULong  lastBase  = 0;
					if ( defp + numRanges * 4 > valid->limit )
						FT_INVALID_TOO_SHORT;
					for ( i = 0; i < numRanges; ++i )
          {
            ft_structs_h::FT_ULong  base = TT_NEXT_UINT24( defp );
            ft_structs_h::FT_ULong  cnt  = FT_NEXT_BYTE( defp );
						if ( base + cnt >= 0x110000UL )              /* end of Unicode */
              FT_INVALID_DATA;
						if ( base < lastBase )
              FT_INVALID_DATA;
						lastBase = base + cnt + 1U;
          }
        }
        if ( nondefOff != 0 )
        {
          ft_structs_h::FT_Byte*  ndp         = table + nondefOff;
          ft_structs_h::FT_ULong  numMappings = TT_NEXT_ULONG( ndp );
          ft_structs_h::FT_ULong  i, lastUni = 0;
					if ( numMappings * 4 > (ft_structs_h::FT_ULong)( valid->limit - ndp ) )
						FT_INVALID_TOO_SHORT;
					for ( i = 0; i < numMappings; ++i )
          {
            ft_structs_h::FT_ULong  uni = TT_NEXT_UINT24( ndp );
            ft_structs_h::FT_ULong  gid = TT_NEXT_USHORT( ndp );
						if ( uni >= 0x110000UL )                     /* end of Unicode */
							FT_INVALID_DATA;
						if ( uni < lastUni )
							FT_INVALID_DATA;
						lastUni = uni + 1U;
						if ( valid->level >= ft_structs_h::FT_VALIDATE_TIGHT    &&
							gid >= TT_VALID_GLYPH_COUNT( valid ) )
							FT_INVALID_GLYPH_ID;
					}
        }
      }
    }
		return ft_error_h::SFNT_Err_Ok;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap14_char_index( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  char_code )
	{
    FT_UNUSED( cmap );
    FT_UNUSED( char_code );
    return 0;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 )
		tt_cmap14_char_next( TT_CMap     cmap,
		ft_structs_h::FT_UInt32  *pchar_code )
	{
		FT_UNUSED( cmap );
		*pchar_code = 0;
    return 0;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_Error )
		tt_cmap14_get_info( TT_CMap       cmap,
		TT_CMapInfo  *cmap_info )
	{
    FT_UNUSED( cmap );
		cmap_info->format   = 14;
		cmap_info->language = 0xFFFFFFFFUL;
		return ft_error_h::SFNT_Err_Ok;
  };
	static ft_structs_h::FT_UInt
		tt_cmap14_char_map_def_binary( ft_structs_h::FT_Byte    *base,
		ft_structs_h::FT_UInt32   char_code )
	{
		ft_structs_h::FT_UInt32  numRanges = TT_PEEK_ULONG( base );
		ft_structs_h::FT_UInt32  max, min;
		min = 0;
		max = numRanges;
		base += 4;
    while ( min < max )
    {
			ft_structs_h::FT_UInt32  mid   = ( min + max ) >> 1;
      ft_structs_h::FT_Byte*   p     = base + 4 * mid;
      ft_structs_h::FT_ULong   start = TT_NEXT_UINT24( p );
      ft_structs_h::FT_UInt    cnt   = FT_NEXT_BYTE( p );
			if ( char_code < start )
				max = mid;
			else if ( char_code > start+cnt )
				min = mid + 1;
			else
				return TRUE;
		}
		return FALSE;
	};
	static ft_structs_h::FT_UInt
		tt_cmap14_char_map_nondef_binary( ft_structs_h::FT_Byte    *base,
		ft_structs_h::FT_UInt32   char_code )
	{
		ft_structs_h::FT_UInt32  numMappings = TT_PEEK_ULONG( base );
		ft_structs_h::FT_UInt32  max, min;
		min = 0;
		max = numMappings;
		base += 4;
		while ( min < max )
    {
      ft_structs_h::FT_UInt32  mid = ( min + max ) >> 1;
      ft_structs_h::FT_Byte*   p   = base + 5 * mid;
      ft_structs_h::FT_UInt32  uni = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( p );
			if ( char_code < uni )
        max = mid;
      else if ( char_code > uni )
        min = mid + 1;
      else
        return TT_PEEK_USHORT( p );
    }
		return 0;
  };
	static ft_structs_h::FT_Byte*
		tt_cmap14_find_variant( ft_structs_h::FT_Byte    *base,
		ft_structs_h::FT_UInt32   variantCode )
	{
		ft_structs_h::FT_UInt32  numVar = TT_PEEK_ULONG( base );
		ft_structs_h::FT_UInt32  max, min;
		min = 0;
		max = numVar;
		base += 4;
		while ( min < max )
    {
      ft_structs_h::FT_UInt32  mid    = ( min + max ) >> 1;
      ft_structs_h::FT_Byte*   p      = base + 11 * mid;
      ft_structs_h::FT_ULong   varSel = TT_NEXT_UINT24( p );
			if ( variantCode < varSel )
				max = mid;
			else if ( variantCode > varSel )
        min = mid + 1;
      else
        return p;
    }
		return NULL;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt )
		tt_cmap14_char_var_index( TT_CMap    cmap,
		TT_CMap    ucmap,
		ft_structs_h::FT_UInt32  charcode,
		ft_structs_h::FT_UInt32  variantSelector)
	{
    ft_structs_h::FT_Byte*  p = tt_cmap14_find_variant( cmap->data + 6, variantSelector );
    ft_structs_h::FT_ULong  defOff;
    ft_structs_h::FT_ULong  nondefOff;
		if ( !p )
			return 0;
		defOff    = TT_NEXT_ULONG( p );
    nondefOff = TT_PEEK_ULONG( p );
		if ( defOff != 0                                                    &&
			tt_cmap14_char_map_def_binary( cmap->data + defOff, charcode ) )
		{
      return ucmap->cmap.clazz->char_index( &ucmap->cmap, charcode );
    }
		if ( nondefOff != 0 )
			return tt_cmap14_char_map_nondef_binary( cmap->data + nondefOff,
			charcode );
		return 0;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_Int )
		tt_cmap14_char_var_isdefault( TT_CMap    cmap,
		ft_structs_h::FT_UInt32  charcode,
		ft_structs_h::FT_UInt32  variantSelector )
	{
    ft_structs_h::FT_Byte*  p = tt_cmap14_find_variant( cmap->data + 6, variantSelector );
    ft_structs_h::FT_ULong  defOff;
    ft_structs_h::FT_ULong  nondefOff;
		if ( !p )
			return -1;
		defOff    = TT_NEXT_ULONG( p );
		nondefOff = TT_NEXT_ULONG( p );
		if ( defOff != 0                                                    &&
			tt_cmap14_char_map_def_binary( cmap->data + defOff, charcode ) )
			return 1;
		if ( nondefOff != 0                                            &&
			tt_cmap14_char_map_nondef_binary( cmap->data + nondefOff,
			charcode ) != 0         )
			return 0;
		return -1;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32* )
		tt_cmap14_variants( TT_CMap    cmap,
		ft_structs_h::FT_Memory  memory )
	{
    TT_CMap14   cmap14 = (TT_CMap14)cmap;
    ft_structs_h::FT_UInt32   count  = cmap14->num_selectors;
    ft_structs_h::FT_Byte*    p      = cmap->data + 10;
    ft_structs_h::FT_UInt32*  result;
    ft_structs_h::FT_UInt32   i;
		if ( tt_cmap14_ensure( cmap14, ( count + 1 ), memory ) )
			return NULL;
		result = cmap14->results;
    for ( i = 0; i < count; ++i )
    {
      result[i] = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( p );
      p        += 8;
    }
    result[i] = 0;
		return result;
  };
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 * )
		tt_cmap14_char_variants( TT_CMap    cmap,
		ft_structs_h::FT_Memory  memory,
		ft_structs_h::FT_UInt32  charCode )
	{
    TT_CMap14   cmap14 = (TT_CMap14)  cmap;
    ft_structs_h::FT_UInt32   count  = cmap14->num_selectors;
    ft_structs_h::FT_Byte*    p      = cmap->data + 10;
    ft_structs_h::FT_UInt32*  q;
		if ( tt_cmap14_ensure( cmap14, ( count + 1 ), memory ) )
			return NULL;
		for ( q = cmap14->results; count > 0; --count )
		{
      ft_structs_h::FT_UInt32  varSel    = TT_NEXT_UINT24( p );
      ft_structs_h::FT_ULong   defOff    = TT_NEXT_ULONG( p );
      ft_structs_h::FT_ULong   nondefOff = TT_NEXT_ULONG( p );
			if ( ( defOff != 0                                               &&
				tt_cmap14_char_map_def_binary( cmap->data + defOff,
				charCode )                 ) ||
				( nondefOff != 0                                            &&
				tt_cmap14_char_map_nondef_binary( cmap->data + nondefOff,
				charCode ) != 0         ) )
			{
				q[0] = varSel;
        q++;
      }
    }
    q[0] = 0;
		return cmap14->results;
	};
	static ft_structs_h::FT_UInt
		tt_cmap14_def_char_count( ft_structs_h::FT_Byte  *p )
	{
		ft_structs_h::FT_UInt32  numRanges = (ft_structs_h::FT_UInt32)TT_NEXT_ULONG( p );
		ft_structs_h::FT_UInt    tot       = 0;
		p += 3;  /* point to the first `cnt' field */
    for ( ; numRanges > 0; numRanges-- )
    {
      tot += 1 + p[0];
      p   += 4;
    }
		return tot;
	};
	static ft_structs_h::FT_UInt32*
		tt_cmap14_get_def_chars( TT_CMap     cmap,
		ft_structs_h::FT_Byte*    p,
		ft_structs_h::FT_Memory   memory )
	{
    TT_CMap14   cmap14 = (TT_CMap14) cmap;
    ft_structs_h::FT_UInt32   numRanges;
    ft_structs_h::FT_UInt     cnt;
    ft_structs_h::FT_UInt32*  q;
		cnt       = tt_cmap14_def_char_count( p );
		numRanges = (ft_structs_h::FT_UInt32)TT_NEXT_ULONG( p );
		if ( tt_cmap14_ensure( cmap14, ( cnt + 1 ), memory ) )
			return NULL;
		for ( q = cmap14->results; numRanges > 0; --numRanges )
		{
			ft_structs_h::FT_UInt32 uni = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( p );
			cnt = FT_NEXT_BYTE( p ) + 1;
      do
      {
        q[0]  = uni;
        uni  += 1;
        q    += 1;
      } while ( --cnt != 0 );
    }
    q[0] = 0;
		return cmap14->results;
	};
	static ft_structs_h::FT_UInt32*
		tt_cmap14_get_nondef_chars( TT_CMap     cmap,
		ft_structs_h::FT_Byte    *p,
		ft_structs_h::FT_Memory   memory )
	{
    TT_CMap14   cmap14 = (TT_CMap14) cmap;
    ft_structs_h::FT_UInt32   numMappings;
    ft_structs_h::FT_UInt     i;
    ft_structs_h::FT_UInt32  *ret;
		numMappings = (ft_structs_h::FT_UInt32)TT_NEXT_ULONG( p );
		if ( tt_cmap14_ensure( cmap14, ( numMappings + 1 ), memory ) )
			return NULL;
		ret = cmap14->results;
    for ( i = 0; i < numMappings; ++i )
    {
      ret[i] = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( p );
      p += 2;
    }
    ret[i] = 0;
		return ret;
	};
	FT_CALLBACK_DEF( ft_structs_h::FT_UInt32 * )
		tt_cmap14_variant_chars( TT_CMap    cmap,
		ft_structs_h::FT_Memory  memory,
		ft_structs_h::FT_UInt32  variantSelector )
	{
    ft_structs_h::FT_Byte    *p  = tt_cmap14_find_variant( cmap->data + 6,
			variantSelector );
		ft_structs_h::FT_UInt32  *ret;
    ft_structs_h::FT_Int      i;
    ft_structs_h::FT_ULong    defOff;
    ft_structs_h::FT_ULong    nondefOff;
		if ( !p )
			return NULL;
		defOff    = TT_NEXT_ULONG( p );
		nondefOff = TT_NEXT_ULONG( p );
		if ( defOff == 0 && nondefOff == 0 )
			return NULL;
		if ( defOff == 0 )
			return tt_cmap14_get_nondef_chars( cmap, cmap->data + nondefOff,
			memory );
		else if ( nondefOff == 0 )
			return tt_cmap14_get_def_chars( cmap, cmap->data + defOff,
			memory );
		else
    {
      TT_CMap14  cmap14 = (TT_CMap14) cmap;
      ft_structs_h::FT_UInt32  numRanges;
      ft_structs_h::FT_UInt32  numMappings;
      ft_structs_h::FT_UInt32  duni;
      ft_structs_h::FT_UInt32  dcnt;
      ft_structs_h::FT_UInt32  nuni;
      ft_structs_h::FT_Byte*   dp;
      ft_structs_h::FT_UInt    di, ni, k;
			p  = cmap->data + nondefOff;
			dp = cmap->data + defOff;
			numMappings = (ft_structs_h::FT_UInt32)TT_NEXT_ULONG( p );
      dcnt        = tt_cmap14_def_char_count( dp );
      numRanges   = (ft_structs_h::FT_UInt32)TT_NEXT_ULONG( dp );
			if ( numMappings == 0 )
				return tt_cmap14_get_def_chars( cmap, cmap->data + defOff,
				memory );
			if ( dcnt == 0 )
				return tt_cmap14_get_nondef_chars( cmap, cmap->data + nondefOff,
				memory );
			if ( tt_cmap14_ensure( cmap14, ( dcnt + numMappings + 1 ), memory ) )
				return NULL;
			ret  = cmap14->results;
      duni = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( dp );
      dcnt = FT_NEXT_BYTE( dp );
      di   = 1;
      nuni = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( p );
      p   += 2;
      ni   = 1;
      i    = 0;
			for ( ;; )
      {
        if ( nuni > duni + dcnt )
        {
          for ( k = 0; k <= dcnt; ++k )
            ret[i++] = duni + k;
					++di;
					if ( di > numRanges )
						break;
					duni = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( dp );
          dcnt = FT_NEXT_BYTE( dp );
        }
        else
        {
          if ( nuni < duni )
            ret[i++] = nuni;
					++ni;
					if ( ni > numMappings )
						break;
					nuni = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( p );
					p += 2;
				}
			}
			if ( ni <= numMappings )
			{
				ret[i++] = nuni;
        while ( ni < numMappings )
        {
          ret[i++] = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( p );
          p += 2;
          ++ni;
        }
      }
      else if ( di <= numRanges )
      {
				for ( k = 0; k <= dcnt; ++k )
					ret[i++] = duni + k;
				while ( di < numRanges )
        {
          duni = (ft_structs_h::FT_UInt32)TT_NEXT_UINT24( dp );
          dcnt = FT_NEXT_BYTE( dp );
					for ( k = 0; k <= dcnt; ++k )
						ret[i++] = duni + k;
					++di;
				}
			}
			ret[i] = 0;
			return ret;
		}
	};
	FT_DEFINE_TT_CMAP(tt_cmap14_class_rec,
		sizeof ( TT_CMap14Rec ),
		(FT_CMap_InitFunc)     tt_cmap14_init,
		(FT_CMap_DoneFunc)     tt_cmap14_done,
		(FT_CMap_CharIndexFunc)tt_cmap14_char_index,
		(FT_CMap_CharNextFunc) tt_cmap14_char_next,
		(FT_CMap_CharVarIndexFunc)    tt_cmap14_char_var_index,
		(FT_CMap_CharVarIsDefaultFunc)tt_cmap14_char_var_isdefault,
		(FT_CMap_VariantListFunc)     tt_cmap14_variants,
		(FT_CMap_CharVariantListFunc) tt_cmap14_char_variants,
		(FT_CMap_VariantCharListFunc) tt_cmap14_variant_chars
		,
		14,
		(TT_CMap_ValidateFunc)tt_cmap14_validate,
		(TT_CMap_Info_GetFunc)tt_cmap14_get_info
		)
#endif /* TT_CONFIG_CMAP_FORMAT_14 */
#ifndef FT_CONFIG_OPTION_PIC
	static const TT_CMap_Class  tt_cmap_classes[] =
	{
#define TTCMAPCITEM(a) &a,
#ifdef TT_CONFIG_CMAP_FORMAT_0
		TTCMAPCITEM(tt_cmap0_class_rec)
#endif
#ifdef TT_CONFIG_CMAP_FORMAT_2
		TTCMAPCITEM(tt_cmap2_class_rec)
#endif
#ifdef TT_CONFIG_CMAP_FORMAT_4
    TTCMAPCITEM(tt_cmap4_class_rec)
#endif
#ifdef TT_CONFIG_CMAP_FORMAT_6
    TTCMAPCITEM(tt_cmap6_class_rec)
#endif
#ifdef TT_CONFIG_CMAP_FORMAT_8
    TTCMAPCITEM(tt_cmap8_class_rec)
#endif
#ifdef TT_CONFIG_CMAP_FORMAT_10
    TTCMAPCITEM(tt_cmap10_class_rec)
#endif
#ifdef TT_CONFIG_CMAP_FORMAT_12
    TTCMAPCITEM(tt_cmap12_class_rec)
#endif
#ifdef TT_CONFIG_CMAP_FORMAT_13
    TTCMAPCITEM(tt_cmap13_class_rec)
#endif
#ifdef TT_CONFIG_CMAP_FORMAT_14
    TTCMAPCITEM(tt_cmap14_class_rec)
#endif
		NULL,
	};
#else /*FT_CONFIG_OPTION_PIC*/
	void FT_Destroy_Class_tt_cmap_classes(FT_Library library, TT_CMap_Class* clazz)
	{
    FT_Memory memory = library->memory;
    if ( clazz )
      FT_FREE( clazz );
  }
	FT_Error FT_Create_Class_tt_cmap_classes(FT_Library library, TT_CMap_Class** output_class)
  {
    TT_CMap_Class*  clazz;
    TT_CMap_ClassRec* recs;
    FT_Error          error;
    FT_Memory memory = library->memory;
    int i = 0;
#define TTCMAPCITEM(a) i++;
#include "ttcmapc.h"
		/* allocate enough space for both the pointers +terminator and the class instances */
		if ( FT_ALLOC( clazz, sizeof(*clazz)*(i+1)+sizeof(TT_CMap_ClassRec)*i ) )
			return error;
		/* the location of the class instances follows the array of pointers */
		recs = (TT_CMap_ClassRec*) (((char*)clazz)+(sizeof(*clazz)*(i+1)));
		i=0;
#undef TTCMAPCITEM
#define TTCMAPCITEM(a)           \
	FT_Init_Class_##a(&recs[i]); \
	clazz[i] = &recs[i];         \
	i++;
#include "ttcmapc.h"
		clazz[i] = NULL;
		*output_class = clazz;
		return SFNT_Err_Ok;
	}
#endif /*FT_CONFIG_OPTION_PIC*/
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		tt_face_build_cmaps( TT_Face  face )
	{
		ft_structs_h::FT_Byte*           table = face->cmap_table;
    ft_structs_h::FT_Byte*           limit = table + face->cmap_size;
    ft_structs_h::FT_UInt volatile   num_cmaps;
    ft_structs_h::FT_Byte* volatile  p     = table;
    ft_structs_h::FT_Library         library = FT_FACE_LIBRARY( face );
		FT_UNUSED( library );
		if ( !p || p + 4 > limit )
			return ft_error_h::SFNT_Err_Invalid_Table;
		/* only recognize format 0 */
		if ( TT_NEXT_USHORT( p ) != 0 )
    {
      p -= 2;
      FT_ERROR(( "tt_face_build_cmaps:"
				" unsupported `cmap' table format = %d\n",
				TT_PEEK_USHORT( p ) ));
			return ft_error_h::SFNT_Err_Invalid_Table;
		}
		num_cmaps = TT_NEXT_USHORT( p );
#ifdef FT_MAX_CHARMAP_CACHEABLE
		if ( num_cmaps > FT_MAX_CHARMAP_CACHEABLE )
			FT_ERROR(( "tt_face_build_cmaps: too many cmap subtables(%d) "
			"subtable#%d and later are loaded but cannot be searched\n",
			num_cmaps, FT_MAX_CHARMAP_CACHEABLE + 1 ));
#endif
		for ( ; num_cmaps > 0 && p + 8 <= limit; num_cmaps-- )
    {
      ft_structs_h::FT_CharMapRec  charmap;
      ft_structs_h::FT_UInt32      offset;
			charmap.platform_id = TT_NEXT_USHORT( p );
      charmap.encoding_id = TT_NEXT_USHORT( p );
      charmap.face        = FT_FACE( face );
      charmap.encoding    = ft_structs_h::FT_ENCODING_NONE;  /* will be filled later */
      offset              = TT_NEXT_ULONG( p );
			if ( offset && offset <= face->cmap_size - 2 )
      {
        ft_structs_h::FT_Byte* volatile              cmap   = table + offset;
        volatile ft_structs_h::FT_UInt               format = TT_PEEK_USHORT( cmap );
        const TT_CMap_Class* volatile  pclazz = FT_TT_CMAP_CLASSES_GET;
        TT_CMap_Class volatile         clazz;
				for ( ; *pclazz; pclazz++ )
        {
          clazz = *pclazz;
          if ( clazz->format == format )
          {
            volatile TT_ValidatorRec  valid;
            volatile ft_structs_h::FT_Error         error = ft_error_h::SFNT_Err_Ok;
						ft_validator_init( FT_VALIDATOR( &valid ), cmap, limit,
							ft_structs_h::FT_VALIDATE_DEFAULT );
						valid.num_glyphs = (ft_structs_h::FT_UInt)face->max_profile.numGlyphs;
						if ( ft_setjmp(
							*((ft_jmp_buf*)&FT_VALIDATOR( &valid )->jump_buffer) ) == 0 )
						{
              /* validate this cmap sub-table */
              error = clazz->validate( cmap, FT_VALIDATOR( &valid ) );
            }
						if ( valid.validator.error == 0 )
            {
              ft_structs_h::FT_CMap  ttcmap;
							/* It might make sense to store the single variation selector */
              /* cmap somewhere special.  But it would have to be in the    */
              /* public FT_FaceRec, and we can't change that.               */
							if ( !FT_CMap_New( (ft_structs_h::FT_CMap_Class)clazz,
								cmap, &charmap, &ttcmap ) )
							{
                /* it is simpler to directly set `flags' than adding */
                /* a parameter to FT_CMap_New                        */
                ((TT_CMap)ttcmap)->flags = (ft_structs_h::FT_Int)error;
              }
            }
            else
            {
              FT_TRACE0(( "tt_face_build_cmaps:"
								" broken cmap sub-table ignored\n" ));
            }
            break;
          }
        }
				if ( *pclazz == NULL )
        {
          FT_TRACE0(( "tt_face_build_cmaps:"
						" unsupported cmap sub-table ignored\n" ));
        }
      }
    }
		return ft_error_h::SFNT_Err_Ok;
  };
FT_LOCAL_DEF( ft_structs_h::FT_Error )
	TT_Load_Glyph( TT_Size       size,
	TT_GlyphSlot  glyph,
	ft_structs_h::FT_UInt       glyph_index,
	ft_structs_h::FT_Int32      load_flags )
{
	ft_structs_h::FT_Error      error;
	TT_LoaderRec  loader;
	error = ft_error_h::TT_Err_Ok;
#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
	/* try to load embedded bitmap if any              */
	/*                                                 */
	/* XXX: The convention should be emphasized in     */
	/*      the documents because it can be confusing. */
	if ( size->strike_index != 0xFFFFFFFFUL      &&
		( load_flags & FT_LOAD_NO_BITMAP ) == 0 )
	{
		error = load_sbit_image( size, glyph, glyph_index, load_flags );
		if ( !error )
		{
			if ( FT_IS_SCALABLE( glyph->face ) )
			{
				/* for the bbox we need the header only */
				(void)tt_loader_init( &loader, size, glyph, load_flags, TRUE );
				(void)load_truetype_glyph( &loader, glyph_index, 0, TRUE );
				glyph->linearHoriAdvance = loader.linear;
				glyph->linearVertAdvance = loader.top_bearing + loader.bbox.yMax -
					loader.vadvance;
			}
			return ft_error_h::TT_Err_Ok;
		}
	}
#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */
	/* if FT_LOAD_NO_SCALE is not set, `ttmetrics' must be valid */
	if ( !( load_flags & FT_LOAD_NO_SCALE ) && !size->ttmetrics.valid )
		return ft_error_h::TT_Err_Invalid_Size_Handle;
	if ( load_flags & FT_LOAD_SBITS_ONLY )
		return ft_error_h::TT_Err_Invalid_Argument;
	error = tt_loader_init( &loader, size, glyph, load_flags, FALSE );
	if ( error )
		return error;
	glyph->format        = ft_structs_h::FT_GLYPH_FORMAT_OUTLINE;
	glyph->num_subglyphs = 0;
	glyph->outline.flags = 0;
	/* main loading loop */
	error = load_truetype_glyph( &loader, glyph_index, 0, FALSE );
	if ( !error )
	{
		if ( glyph->format == ft_structs_h::FT_GLYPH_FORMAT_COMPOSITE )
		{
			glyph->num_subglyphs = loader.gloader->base.num_subglyphs;
			glyph->subglyphs     = loader.gloader->base.subglyphs;
		}
		else
		{
			glyph->outline        = loader.gloader->base.outline;
			glyph->outline.flags &= ~FT_OUTLINE_SINGLE_PASS;
			/* Translate array so that (0,0) is the glyph's origin.  Note  */
			/* that this behaviour is independent on the value of bit 1 of */
			/* the `flags' field in the `head' table -- at least major     */
			/* applications like Acroread indicate that.                   */
			if ( loader.pp1.x )
				FT_Outline_Translate( &glyph->outline, -loader.pp1.x, 0 );
		}
#ifdef TT_USE_BYTECODE_INTERPRETER
		if ( IS_HINTED( load_flags ) )
		{
			if ( loader.exec->GS.scan_control )
			{
				/* convert scan conversion mode to FT_OUTLINE_XXX flags */
				switch ( loader.exec->GS.scan_type )
				{
				case 0: /* simple drop-outs including stubs */
					glyph->outline.flags |= FT_OUTLINE_INCLUDE_STUBS;
					break;
				case 1: /* simple drop-outs excluding stubs */
					/* nothing; it's the default rendering mode */
					break;
				case 4: /* smart drop-outs including stubs */
					glyph->outline.flags |= FT_OUTLINE_SMART_DROPOUTS |
						FT_OUTLINE_INCLUDE_STUBS;
					break;
				case 5: /* smart drop-outs excluding stubs  */
					glyph->outline.flags |= FT_OUTLINE_SMART_DROPOUTS;
					break;
				default: /* no drop-out control */
					glyph->outline.flags |= FT_OUTLINE_IGNORE_DROPOUTS;
					break;
				}
			}
			else
				glyph->outline.flags |= FT_OUTLINE_IGNORE_DROPOUTS;
		}
#endif /* TT_USE_BYTECODE_INTERPRETER */
		compute_glyph_metrics( &loader, glyph_index );
	}
	/* Set the `high precision' bit flag.                           */
	/* This is _critical_ to get correct output for monochrome      */
	/* TrueType glyphs at all sizes using the bytecode interpreter. */
	/*                                                              */
	if ( !( load_flags & FT_LOAD_NO_SCALE ) &&
		size->root.metrics.y_ppem < 24     )
		glyph->outline.flags |= FT_OUTLINE_HIGH_PRECISION;
		return error;
	}
FT_DEFINE_SERVICE_TTCMAPSREC(tt_service_get_cmap_info,
	(TT_CMap_Info_GetFunc)tt_get_cmap_info
	)
};
namespace sfnt_h
{
	static ft_structs_h::FT_Error
		sfnt_open_font( ft_structs_h::FT_Stream  stream,
		tt_h::TT_Face    face )
	{
    ft_structs_h::FT_Memory  memory = stream->memory;
    ft_structs_h::FT_Error   error;
    ft_structs_h::FT_ULong   tag, offset;
		static const ft_structs_h::FT_Frame_Field  ttc_header_fields[] =
		{
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TTC_HeaderRec
			FT_FRAME_START( 8 ),
			FT_FRAME_LONG( version ),
			FT_FRAME_LONG( count   ),  /* this is ULong in the specs */
			FT_FRAME_END
		};
		face->ttc_header.tag     = 0;
    face->ttc_header.version = 0;
    face->ttc_header.count   = 0;
		offset = FT_STREAM_POS();
		if ( FT_READ_ULONG( tag ) )
      return error;
		if ( tag != 0x00010000UL &&
			tag != TTAG_ttcf    &&
			tag != TTAG_OTTO    &&
			tag != TTAG_true    &&
			tag != TTAG_typ1    &&
			tag != 0x00020000UL )
		{
      FT_TRACE2(( "  not a font using the SFNT container format\n" ));
      return ft_error_h::SFNT_Err_Unknown_File_Format;
    }
		face->ttc_header.tag = TTAG_ttcf;
		if ( tag == TTAG_ttcf )
		{
			ft_structs_h::FT_Int  n;
			FT_TRACE3(( "sfnt_open_font: file is a collection\n" ));
			if ( FT_STREAM_READ_FIELDS( ttc_header_fields, &face->ttc_header ) )
				return error;
			if ( face->ttc_header.count == 0 )
				return ft_error_h::SFNT_Err_Invalid_Table;
			/* a rough size estimate: let's conservatively assume that there   */
      /* is just a single table info in each subfont header (12 + 16*1 = */
      /* 28 bytes), thus we have (at least) `12 + 4*count' bytes for the */
      /* size of the TTC header plus `28*count' bytes for all subfont    */
      /* headers                                                         */
      if ( (ft_structs_h::FT_ULong)face->ttc_header.count > stream->size / ( 28 + 4 ) )
        return ft_error_h::SFNT_Err_Array_Too_Large;
			/* now read the offsets of each font in the file */
			if ( FT_NEW_ARRAY( face->ttc_header.offsets, face->ttc_header.count ) )
				return error;
			if ( FT_FRAME_ENTER( face->ttc_header.count * 4L ) )
				return error;
			for ( n = 0; n < face->ttc_header.count; n++ )
				face->ttc_header.offsets[n] = FT_GET_ULONG();
			FT_FRAME_EXIT();
		}
    else
    {
      FT_TRACE3(( "sfnt_open_font: synthesize TTC\n" ));
			face->ttc_header.version = 1 << 16;
			face->ttc_header.count   = 1;
			if ( FT_NEW( face->ttc_header.offsets ) )
        return error;
			face->ttc_header.offsets[0] = offset;
		}
		return error;
  };
#define LOAD_( x )                                            \
	do {                                                        \
	FT_TRACE2(( "`" #x "' " ));                               \
	FT_TRACE3(( "-->\n" ));                                   \
	\
	error = sfnt->load_##x( face, stream );                   \
	\
	FT_TRACE2(( "%s\n", ( !error )                            \
	? "loaded"                            \
	: ( error == ft_error_h::SFNT_Err_Table_Missing ) \
	? "missing"                         \
	: "failed to load" ));              \
	FT_TRACE3(( "\n" ));                                      \
	} while ( 0 )
#define LOADM_( x, vertical )                                 \
	do {                                                        \
	FT_TRACE2(( "`%s" #x "' ",                                \
	vertical ? "vertical " : "" ));               \
	FT_TRACE3(( "-->\n" ));                                   \
	\
	error = sfnt->load_##x( face, stream, vertical );         \
	\
	FT_TRACE2(( "%s\n", ( !error )                            \
	? "loaded"                            \
	: ( error == ft_error_h::SFNT_Err_Table_Missing ) \
	? "missing"                         \
	: "failed to load" ));              \
	FT_TRACE3(( "\n" ));                                      \
	} while ( 0 )
#define GET_NAME( id, field )                                 \
	do {                                                        \
	error = tt_h::tt_face_get_name( face, TT_NAME_ID_##id, field ); \
	if ( error )                                              \
	goto Exit;                                              \
	} while ( 0 )
	static ft_structs_h::FT_Encoding
		sfnt_find_encoding( int  platform_id,
		int  encoding_id )
	{
		typedef struct  TEncoding_
		{
			int          platform_id;
			int          encoding_id;
			ft_structs_h::FT_Encoding  encoding;
		} TEncoding;
		static
			const TEncoding  tt_encodings[] =
		{
			{ TT_PLATFORM_ISO,           -1,                  ft_structs_h::FT_ENCODING_UNICODE },
			{ TT_PLATFORM_APPLE_UNICODE, -1,                  ft_structs_h::FT_ENCODING_UNICODE },
			{ TT_PLATFORM_MACINTOSH,     TT_MAC_ID_ROMAN,     ft_structs_h::FT_ENCODING_APPLE_ROMAN },
			{ TT_PLATFORM_MICROSOFT,     TT_MS_ID_SYMBOL_CS,  ft_structs_h::FT_ENCODING_MS_SYMBOL },
      { TT_PLATFORM_MICROSOFT,     TT_MS_ID_UCS_4,      ft_structs_h::FT_ENCODING_UNICODE },
      { TT_PLATFORM_MICROSOFT,     TT_MS_ID_UNICODE_CS, ft_structs_h::FT_ENCODING_UNICODE },
      { TT_PLATFORM_MICROSOFT,     TT_MS_ID_SJIS,       ft_structs_h::FT_ENCODING_SJIS },
      { TT_PLATFORM_MICROSOFT,     TT_MS_ID_GB2312,     ft_structs_h::FT_ENCODING_GB2312 },
      { TT_PLATFORM_MICROSOFT,     TT_MS_ID_BIG_5,      ft_structs_h::FT_ENCODING_BIG5 },
      { TT_PLATFORM_MICROSOFT,     TT_MS_ID_WANSUNG,    ft_structs_h::FT_ENCODING_WANSUNG },
      { TT_PLATFORM_MICROSOFT,     TT_MS_ID_JOHAB,      ft_structs_h::FT_ENCODING_JOHAB }
    };
		const TEncoding  *cur, *limit;
		cur   = tt_encodings;
		limit = cur + sizeof ( tt_encodings ) / sizeof ( tt_encodings[0] );
		for ( ; cur < limit; cur++ )
    {
      if ( cur->platform_id == platform_id )
      {
        if ( cur->encoding_id == encoding_id ||
					cur->encoding_id == -1          )
					return cur->encoding;
			}
    }
		return ft_structs_h::FT_ENCODING_NONE;
  };
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		sfnt_load_face( ft_structs_h::FT_Stream      stream,
		tt_h::TT_Face        face,
		ft_structs_h::FT_Int         face_index,
		ft_structs_h::FT_Int         num_params,
		ft_structs_h::FT_Parameter*  params )
	{
		ft_structs_h::FT_Error      error;
#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
		ft_structs_h::FT_Error      psnames_error;
#endif
		ft_structs_h::FT_Bool       has_outline;
    ft_structs_h::FT_Bool       is_apple_sbit;
    ft_structs_h::FT_Bool       ignore_preferred_family = FALSE;
    ft_structs_h::FT_Bool       ignore_preferred_subfamily = FALSE;
		tt_h::SFNT_Service  sfnt = (tt_h::SFNT_Service)face->sfnt;
		FT_UNUSED( face_index );
    {
			ft_structs_h::FT_Int  i;
			for ( i = 0; i < num_params; i++ )
      {
        if ( params[i].tag == FT_PARAM_TAG_IGNORE_PREFERRED_FAMILY )
          ignore_preferred_family = TRUE;
        else if ( params[i].tag == FT_PARAM_TAG_IGNORE_PREFERRED_SUBFAMILY )
          ignore_preferred_subfamily = TRUE;
      }
    }
    FT_TRACE2(( "sfnt_load_face: %08p\n\n", face ));
		/* do we have outlines in there? */
#ifdef FT_CONFIG_OPTION_INCREMENTAL
		has_outline = FT_BOOL( face->root.internal->incremental_interface != 0 ||
			tt_face_lookup_table( face, TTAG_glyf )    != 0 ||
			tt_face_lookup_table( face, TTAG_CFF )     != 0 );
#else
		has_outline = FT_BOOL( tt_face_lookup_table( face, TTAG_glyf ) != 0 ||
			tt_face_lookup_table( face, TTAG_CFF )  != 0 );
#endif
		is_apple_sbit = 0;
		/* if this font doesn't contain outlines, we try to load */
    /* a `bhed' table                                        */
		if ( !has_outline && sfnt->load_bhed )
    {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
      LOAD_( bhed );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
      is_apple_sbit = FT_BOOL( !error );
    }
		/* load the font header (`head' table) if this isn't an Apple */
    /* sbit font file                                             */
		if ( !is_apple_sbit )
    {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
      LOAD_( head );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
      if ( error )
        goto Exit;
    }
		if ( face->header.Units_Per_EM == 0 )
    {
      error = ft_error_h::SFNT_Err_Invalid_Table;
			goto Exit;
    }
		/* the following tables are often not present in embedded TrueType */
    /* fonts within PDF documents, so don't check for them.            */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		LOAD_( maxp );
    LOAD_( cmap );
		/* the following tables are optional in PCL fonts -- */
		/* don't check for errors                            */
    LOAD_( name );
    LOAD_( post );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
    psnames_error = error;
#endif
		/* do not load the metrics headers and tables if this is an Apple */
    /* sbit font file                                                 */
    if ( !is_apple_sbit )
    {
      /* load the `hhea' and `hmtx' tables */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
      LOADM_( hhea, 0 );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
      if ( !error )
      {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
        LOADM_( hmtx, 0 );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
        if ( error == ft_error_h::SFNT_Err_Table_Missing )
        {
          error = ft_error_h::SFNT_Err_Hmtx_Table_Missing;
#ifdef FT_CONFIG_OPTION_INCREMENTAL
					if ( face->root.internal->incremental_interface          &&
						face->root.internal->incremental_interface->funcs->
						get_glyph_metrics                                 )
					{
						face->horizontal.number_Of_HMetrics = 0;
            error = SFNT_Err_Ok;
          }
#endif
        }
      }
      else if ( error == ft_error_h::SFNT_Err_Table_Missing )
      {
				if ( face->format_tag == TTAG_true )
				{
					FT_TRACE2(( "This is an SFNT Mac font.\n" ));
					has_outline = 0;
					error       = ft_error_h::SFNT_Err_Ok;
				}
        else
        {
          error = ft_error_h::SFNT_Err_Horiz_Header_Missing;
#ifdef FT_CONFIG_OPTION_INCREMENTAL
					if ( face->root.internal->incremental_interface          &&
						face->root.internal->incremental_interface->funcs->
						get_glyph_metrics                                 )
					{
						face->horizontal.number_Of_HMetrics = 0;
            error = SFNT_Err_Ok;
          }
#endif
        }
      }
      if ( error )
        goto Exit;
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
			LOADM_( hhea, 1 );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
			if ( !error )
      {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
        LOADM_( hmtx, 1 );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
        if ( !error )
          face->vertical_info = 1;
      }
			if ( error && error != ft_error_h::SFNT_Err_Table_Missing )
				goto Exit;
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
			LOAD_( os2 );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
			if ( error )
      {
        face->os2.version = 0xFFFFU;
      }
    }
    if ( sfnt->load_eblc )
    {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
      LOAD_( eblc );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
      if ( error )
      {      
        if ( error == ft_error_h::SFNT_Err_Table_Missing )
          error = ft_error_h::SFNT_Err_Ok;
        else
          goto Exit;
      }
    }
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
    LOAD_( pclt );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
    if ( error )
    {
      if ( error != ft_error_h::SFNT_Err_Table_Missing )
        goto Exit;
      face->pclt.Version = 0;
    }
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
    LOAD_( gasp );
    LOAD_( kern );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
    face->root.num_glyphs = face->max_profile.numGlyphs;
    face->root.family_name = NULL;
    face->root.style_name  = NULL;
    if ( face->os2.version != 0xFFFFU && face->os2.fsSelection & 256 )
    {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
      if ( !ignore_preferred_family )
        GET_NAME( PREFERRED_FAMILY, &face->root.family_name );
      if ( !face->root.family_name )
        GET_NAME( FONT_FAMILY, &face->root.family_name );
			if ( !ignore_preferred_subfamily )
        GET_NAME( PREFERRED_SUBFAMILY, &face->root.style_name );
      if ( !face->root.style_name )
        GET_NAME( FONT_SUBFAMILY, &face->root.style_name );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
    }
    else
    {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
      GET_NAME( WWS_FAMILY, &face->root.family_name );
      if ( !face->root.family_name && !ignore_preferred_family )
        GET_NAME( PREFERRED_FAMILY, &face->root.family_name );
      if ( !face->root.family_name )
        GET_NAME( FONT_FAMILY, &face->root.family_name );
			GET_NAME( WWS_SUBFAMILY, &face->root.style_name );
      if ( !face->root.style_name && !ignore_preferred_subfamily )
        GET_NAME( PREFERRED_SUBFAMILY, &face->root.style_name );
      if ( !face->root.style_name )
        GET_NAME( FONT_SUBFAMILY, &face->root.style_name );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
    }
    {
      ft_structs_h::FT_Face  root  = &face->root;
      ft_structs_h::FT_Long  flags = root->face_flags;
      if ( has_outline == TRUE )
        flags |= FT_FACE_FLAG_SCALABLE;                              
      flags |= FT_FACE_FLAG_SFNT       |  /* SFNT file format  */
			FT_FACE_FLAG_HORIZONTAL;   /* horizontal data   */
#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
			if ( psnames_error == SFNT_Err_Ok               &&
				face->postscript.FormatType != 0x00030000L )
				flags |= FT_FACE_FLAG_GLYPH_NAMES;
#endif
			/* fixed width font? */
			if ( face->postscript.isFixedPitch )
				flags |= FT_FACE_FLAG_FIXED_WIDTH;
			/* vertical information? */
			if ( face->vertical_info )
				flags |= FT_FACE_FLAG_VERTICAL;
			/* kerning available ? */
      if ( TT_FACE_HAS_KERNING( face ) )
        flags |= FT_FACE_FLAG_KERNING;
#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
			/* Don't bother to load the tables unless somebody asks for them. */
      /* No need to do work which will (probably) not be used.          */
			if ( tt_face_lookup_table( face, TTAG_glyf ) != 0 &&
				tt_face_lookup_table( face, TTAG_fvar ) != 0 &&
				tt_face_lookup_table( face, TTAG_gvar ) != 0 )
				flags |= FT_FACE_FLAG_MULTIPLE_MASTERS;
#endif
			root->face_flags = flags;
      flags = 0;
      if ( has_outline == TRUE && face->os2.version != 0xFFFFU )
      {
        if ( face->os2.fsSelection & 512 )       /* bit 9 */
          flags |= FT_STYLE_FLAG_ITALIC;
        else if ( face->os2.fsSelection & 1 )    /* bit 0 */
          flags |= FT_STYLE_FLAG_ITALIC;
				if ( face->os2.fsSelection & 32 )        /* bit 5 */
					flags |= FT_STYLE_FLAG_BOLD;
      }
      else
      {
        if ( face->header.Mac_Style & 1 )
          flags |= FT_STYLE_FLAG_BOLD;
				if ( face->header.Mac_Style & 2 )
          flags |= FT_STYLE_FLAG_ITALIC;
      }
      root->style_flags = flags;
      tt_face_build_cmaps( face );
      {
        ft_structs_h::FT_Int  m;
        for ( m = 0; m < root->num_charmaps; m++ )
        {
          ft_structs_h::FT_CharMap  charmap = root->charmaps[m];
          charmap->encoding = sfnt_find_encoding( charmap->platform_id,
                                                  charmap->encoding_id );
#if 0
          if ( root->charmap     == NULL &&
               charmap->encoding == FT_ENCODING_UNICODE )
          {
            /* set 'root->charmap' to the first Unicode encoding we find */
            root->charmap = charmap;
          }
#endif
        }
      }
#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
      {
        FT_UInt  i, count;
#ifndef FT_CONFIG_OPTION_OLD_INTERNALS
				count = face->sbit_num_strikes;
#else
				count = (FT_UInt)face->num_sbit_strikes;
#endif
        if ( count > 0 )
        {
          FT_Memory        memory   = face->root.stream->memory;
          FT_UShort        em_size  = face->header.Units_Per_EM;
          FT_Short         avgwidth = face->os2.xAvgCharWidth;
          FT_Size_Metrics  metrics;
          if ( em_size == 0 || face->os2.version == 0xFFFFU )
          {
            avgwidth = 0;
            em_size = 1;
          }
          if ( FT_NEW_ARRAY( root->available_sizes, count ) )
            goto Exit;
          for ( i = 0; i < count; i++ )
          {
            FT_Bitmap_Size*  bsize = root->available_sizes + i;
            error = sfnt->load_strike_metrics( face, i, &metrics );
            if ( error )
              goto Exit;
            bsize->height = (FT_Short)( metrics.height >> 6 );
            bsize->width = (FT_Short)(
                ( avgwidth * metrics.x_ppem + em_size / 2 ) / em_size );
            bsize->x_ppem = metrics.x_ppem << 6;
            bsize->y_ppem = metrics.y_ppem << 6;
            bsize->size   = metrics.y_ppem << 6;
          }
          root->face_flags     |= FT_FACE_FLAG_FIXED_SIZES;
          root->num_fixed_sizes = (FT_Int)count;
        }
      }
#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */
			if ( !FT_HAS_FIXED_SIZES( root ) && !FT_IS_SCALABLE( root ) )
				root->face_flags |= FT_FACE_FLAG_SCALABLE;
      if ( FT_IS_SCALABLE( root ) )
      {
        root->bbox.xMin    = face->header.xMin;
        root->bbox.yMin    = face->header.yMin;
        root->bbox.xMax    = face->header.xMax;
        root->bbox.yMax    = face->header.yMax;
        root->units_per_EM = face->header.Units_Per_EM;
        root->ascender  = face->horizontal.Ascender;
        root->descender = face->horizontal.Descender;
        root->height = (ft_structs_h::FT_Short)( root->ascender - root->descender +
					face->horizontal.Line_Gap );
        if ( !( root->ascender || root->descender ) )
        {
          if ( face->os2.version != 0xFFFFU )
          {
            if ( face->os2.sTypoAscender || face->os2.sTypoDescender )
            {
              root->ascender  = face->os2.sTypoAscender;
              root->descender = face->os2.sTypoDescender;
							root->height = (ft_structs_h::FT_Short)( root->ascender - root->descender +
								face->os2.sTypoLineGap );
            }
            else
            {
							root->ascender  =  (ft_structs_h::FT_Short)face->os2.usWinAscent;
							root->descender = -(ft_structs_h::FT_Short)face->os2.usWinDescent;
							root->height = (ft_structs_h::FT_UShort)( root->ascender - root->descender );
            }
          }
        }
				root->max_advance_width  = face->horizontal.advance_Width_Max;
				root->max_advance_height = (ft_structs_h::FT_Short)( face->vertical_info
					? face->vertical.advance_Height_Max
					: root->height );        
				root->underline_position  = face->postscript.underlinePosition -
					face->postscript.underlineThickness / 2;
				root->underline_thickness = face->postscript.underlineThickness;
			}
    }
Exit:
		FT_TRACE2(( "sfnt_load_face: done\n" ));
		return error;
  };
#undef LOAD_
#undef LOADM_
#undef GET_NAME
	FT_LOCAL_DEF( ft_structs_h::FT_Error )
		sfnt_init_face( ft_structs_h::FT_Stream      stream,
		tt_h::TT_Face        face,
		ft_structs_h::FT_Int         face_index,
		ft_structs_h::FT_Int         num_params,
		ft_structs_h::FT_Parameter*  params )
	{
		ft_structs_h::FT_Error        error;
		ft_structs_h::FT_Library      library = face->root.driver->root.library;
		tt_h::SFNT_Service    sfnt;
		/* for now, parameters are unused */
		FT_UNUSED( num_params );
		FT_UNUSED( params );
		sfnt = (tt_h::SFNT_Service)face->sfnt;
		if ( !sfnt )
		{
      sfnt = (tt_h::SFNT_Service)FT_Get_Module_Interface( library, "sfnt" );
      if ( !sfnt )
      {
        FT_ERROR(( "sfnt_init_face: cannot access `sfnt' module\n" ));
        return ft_error_h::SFNT_Err_Missing_Module;
      }
			face->sfnt       = sfnt;
			face->goto_table = sfnt->goto_table;
		}
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FACE_FIND_GLOBAL_SERVICE( face, face->psnames, POSTSCRIPT_CMAPS );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		FT_TRACE2(( "SFNT driver\n" ));
		error = sfnt_open_font( stream, face );
    if ( error )
      return error;
		FT_TRACE2(( "sfnt_init_face: %08p, %ld\n", face, face_index ));
		if ( face_index < 0 )
			face_index = 0;
		if ( face_index >= face->ttc_header.count )
			return ft_error_h::SFNT_Err_Invalid_Argument;
		if ( FT_STREAM_SEEK( face->ttc_header.offsets[face_index] ) )
			return error;
		/* check that we have a valid TrueType file */
		error = sfnt->load_font_dir( face, stream );
		if ( error )
			return error;
		face->root.num_faces  = face->ttc_header.count;
		face->root.face_index = face_index;
		return error;
	};
	static void*
		get_sfnt_table( tt_h::TT_Face      face,
		FT_Sfnt_Tag  tag )
	{
		void*  table;
		switch ( tag )
		{
		case ft_sfnt_head:
			table = &face->header;
			break;
		case ft_sfnt_hhea:
      table = &face->horizontal;
      break;
		case ft_sfnt_vhea:
      table = face->vertical_info ? &face->vertical : 0;
      break;
		case ft_sfnt_os2:
      table = face->os2.version == 0xFFFFU ? 0 : &face->os2;
      break;
		case ft_sfnt_post:
			table = &face->postscript;
			break;
		case ft_sfnt_maxp:
      table = &face->max_profile;
      break;
		case ft_sfnt_pclt:
      table = face->pclt.Version ? &face->pclt : 0;
      break;
		default:
      table = 0;
    }
		return table;
  };
	static ft_structs_h::FT_Error
		sfnt_table_info( tt_h::TT_Face    face,
		ft_structs_h::FT_UInt    idx,
		ft_structs_h::FT_ULong  *tag,
		ft_structs_h::FT_ULong  *offset,
		ft_structs_h::FT_ULong  *length )
	{
		if ( !offset || !length )
			return ft_error_h::SFNT_Err_Invalid_Argument;
		if ( !tag )
      *length = face->num_tables;
    else
    {
      if ( idx >= face->num_tables )
        return ft_error_h::SFNT_Err_Table_Missing;
			*tag    = face->dir_tables[idx].Tag;
      *offset = face->dir_tables[idx].Offset;
      *length = face->dir_tables[idx].Length;
		}
		return ft_error_h::SFNT_Err_Ok;
  };
	static const char*
		sfnt_get_ps_name( tt_h::TT_Face  face )
	{
		ft_structs_h::FT_Int       n, found_win, found_apple;
    const char*  result = NULL;
		/* shouldn't happen, but just in case to avoid memory leaks */
    if ( face->postscript_name )
      return face->postscript_name;
		/* scan the name table to see whether we have a Postscript name here, */
    /* either in Macintosh or Windows platform encodings                  */
    found_win   = -1;
    found_apple = -1;
		for ( n = 0; n < face->num_names; n++ )
    {
      tt_h::TT_NameEntryRec*  name = face->name_table.names + n;
			if ( name->nameID == 6 && name->stringLength > 0 )
      {
        if ( name->platformID == 3     &&
					name->encodingID == 1     &&
					name->languageID == 0x409 )
					found_win = n;
				if ( name->platformID == 1 &&
					name->encodingID == 0 &&
					name->languageID == 0 )
					found_apple = n;
			}
		}
		if ( found_win != -1 )
    {
      ft_structs_h::FT_Memory         memory = face->root.memory;
      tt_h::TT_NameEntryRec*  name   = face->name_table.names + found_win;
      ft_structs_h::FT_UInt           len    = name->stringLength / 2;
      ft_structs_h::FT_Error          error  = ft_error_h::SFNT_Err_Ok;
			FT_UNUSED( error );
			if ( !FT_ALLOC( result, name->stringLength + 1 ) )
      {
        ft_structs_h::FT_Stream   stream = face->name_table.stream;
        ft_structs_h::FT_String*  r      = (ft_structs_h::FT_String*)result;
        ft_structs_h::FT_Byte*    p      = (ft_structs_h::FT_Byte*)name->string;
				if ( FT_STREAM_SEEK( name->stringOffset ) ||
					FT_FRAME_ENTER( name->stringLength ) )
				{
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
          FT_FREE( result );
          name->stringLength = 0;
          name->stringOffset = 0;
          FT_FREE( name->string );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
					goto Exit;
				}
				p = (ft_structs_h::FT_Byte*)stream->cursor;
				for ( ; len > 0; len--, p += 2 )
        {
          if ( p[0] == 0 && p[1] >= 32 && p[1] < 128 )
            *r++ = p[1];
        }
        *r = '\0';
				FT_FRAME_EXIT();
			}
			goto Exit;
		}
		if ( found_apple != -1 )
    {
      ft_structs_h::FT_Memory         memory = face->root.memory;
      tt_h::TT_NameEntryRec*  name   = face->name_table.names + found_apple;
      ft_structs_h::FT_UInt           len    = name->stringLength;
      ft_structs_h::FT_Error          error  = ft_error_h::SFNT_Err_Ok;
			FT_UNUSED( error );
			if ( !FT_ALLOC( result, len + 1 ) )
      {
        ft_structs_h::FT_Stream  stream = face->name_table.stream;
				if ( FT_STREAM_SEEK( name->stringOffset ) ||
					FT_STREAM_READ( result, len )        )
				{
          name->stringOffset = 0;
          name->stringLength = 0;
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
          FT_FREE( name->string );
          FT_FREE( result );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
          goto Exit;
        }
        ((char*)result)[len] = '\0';
      }
    }
Exit:
		face->postscript_name = result;
		return result;
	};	
	FT_LOCAL_DEF( void )
		sfnt_done_face( tt_h::TT_Face  face )
	{
		ft_structs_h::FT_Memory     memory;
    tt_h::SFNT_Service  sfnt;
		if ( !face )
			return;
		memory = face->root.memory;
    sfnt   = (tt_h::SFNT_Service)face->sfnt;
		if ( sfnt )
    {
      /* destroy the postscript names table if it is loaded */
      if ( sfnt->free_psnames )
        sfnt->free_psnames( face );
			/* destroy the embedded bitmaps table if it is loaded */
      if ( sfnt->free_eblc )
        sfnt->free_eblc( face );
    }
#ifdef TT_CONFIG_OPTION_BDF
    /* freeing the embedded BDF properties */
    tt_face_free_bdf_props( face );
#endif
		/* freeing the kerning table */
    tt_face_done_kern( face );
		/* freeing the collection table */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
    FT_FREE( face->ttc_header.offsets );
    face->ttc_header.count = 0;
		/* freeing table directory */
    FT_FREE( face->dir_tables );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
    face->num_tables = 0;
		{
      ft_structs_h::FT_Stream  stream = FT_FACE_STREAM( face );
			/* simply release the 'cmap' table frame */
      FT_FRAME_RELEASE( face->cmap_table );
      face->cmap_size = 0;
    }
		/* freeing the horizontal metrics */
#ifndef FT_CONFIG_OPTION_OLD_INTERNALS
    {
      ft_structs_h::FT_Stream  stream = FT_FACE_STREAM( face );
			FT_FRAME_RELEASE( face->horz_metrics );
      FT_FRAME_RELEASE( face->vert_metrics );
      face->horz_metrics_size = 0;
      face->vert_metrics_size = 0;
    }
#else
    FT_FREE( face->horizontal.long_metrics );
    FT_FREE( face->horizontal.short_metrics );
#endif
		/* freeing the vertical ones, if any */
    if ( face->vertical_info )
    {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
      FT_FREE( face->vertical.long_metrics  );
      FT_FREE( face->vertical.short_metrics );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
      face->vertical_info = 0;
    }
		/* freeing the gasp table */
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
    FT_FREE( face->gasp.gaspRanges );
    face->gasp.numRanges = 0;
		/* freeing the name table */
    if ( sfnt )
			sfnt->free_name( face );
		/* freeing family and style name */
    FT_FREE( face->root.family_name );
    FT_FREE( face->root.style_name );
		/* freeing sbit size table */
    FT_FREE( face->root.available_sizes );
    face->root.num_fixed_sizes = 0;
		FT_FREE( face->postscript_name );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
		face->sfnt = 0;
  };
#if defined TT_CONFIG_OPTION_POSTSCRIPT_NAMES && defined TT_CONFIG_OPTION_BDF
	FT_DEFINE_SERVICEDESCREC5(sfnt_services,
		FT_SERVICE_ID_SFNT_TABLE,           &FT_SFNT_SERVICE_SFNT_TABLE_GET,
		FT_SERVICE_ID_POSTSCRIPT_FONT_NAME, &FT_SFNT_SERVICE_PS_NAME_GET,
    FT_SERVICE_ID_GLYPH_DICT,           &FT_SFNT_SERVICE_GLYPH_DICT_GET,
    FT_SERVICE_ID_BDF,                  &FT_SFNT_SERVICE_BDF_GET,
    FT_SERVICE_ID_TT_CMAP,              &FT_TT_SERVICE_GET_CMAP_INFO_GET
  )
#elif defined TT_CONFIG_OPTION_POSTSCRIPT_NAMES
	FT_DEFINE_SERVICEDESCREC4(sfnt_services,
		FT_SERVICE_ID_SFNT_TABLE,           &FT_SFNT_SERVICE_SFNT_TABLE_GET,
		FT_SERVICE_ID_POSTSCRIPT_FONT_NAME, &FT_SFNT_SERVICE_PS_NAME_GET,
		FT_SERVICE_ID_GLYPH_DICT,           &FT_SFNT_SERVICE_GLYPH_DICT_GET,
		FT_SERVICE_ID_TT_CMAP,              &FT_TT_SERVICE_GET_CMAP_INFO_GET
  )
#elif defined TT_CONFIG_OPTION_BDF
  FT_DEFINE_SERVICEDESCREC4(sfnt_services,
    FT_SERVICE_ID_SFNT_TABLE,           &FT_SFNT_SERVICE_SFNT_TABLE_GET,
    FT_SERVICE_ID_POSTSCRIPT_FONT_NAME, &FT_SFNT_SERVICE_PS_NAME_GET,
    FT_SERVICE_ID_BDF,                  &FT_SFNT_SERVICE_BDF_GET,
    FT_SERVICE_ID_TT_CMAP,              &FT_TT_SERVICE_GET_CMAP_INFO_GET
  )
#else
	FT_DEFINE_SERVICE_SFNT_TABLEREC(sfnt_service_sfnt_table,
		(FT_SFNT_TableLoadFunc)tt_h::tt_face_load_any,
		(FT_SFNT_TableGetFunc) get_sfnt_table,
		(FT_SFNT_TableInfoFunc)sfnt_table_info
		)
	FT_DEFINE_SERVICE_PSFONTNAMEREC(sfnt_service_ps_name,
	(FT_PsName_GetFunc)sfnt_get_ps_name
	)	
  FT_DEFINE_SERVICEDESCREC3(sfnt_services,
    FT_SERVICE_ID_SFNT_TABLE,           &FT_SFNT_SERVICE_SFNT_TABLE_GET,
    FT_SERVICE_ID_POSTSCRIPT_FONT_NAME, &FT_SFNT_SERVICE_PS_NAME_GET,
    FT_SERVICE_ID_TT_CMAP,              &FT_TT_SERVICE_GET_CMAP_INFO_GET
  )
#endif
	FT_CALLBACK_DEF( ft_structs_h::FT_Module_Interface )
		sfnt_get_interface( ft_structs_h::FT_Module    module,
		const char*  module_interface )
	{
		/* FT_SFNT_SERVICES_GET derefers `library' in PIC mode */
#ifdef FT_CONFIG_OPTION_PIC
		FT_Library  library;
		if ( !module )
      return NULL;
    library = module->library;
    if ( !library )
      return NULL;
#else
    FT_UNUSED( module );
#endif
    return ft_service_list_lookup( FT_SFNT_SERVICES_GET, module_interface );
  };
#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
#define PUT_EMBEDDED_BITMAPS(a) a
#else
#define PUT_EMBEDDED_BITMAPS(a) 0
#endif
#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
#define PUT_PS_NAMES(a) a
#else
#define PUT_PS_NAMES(a) 0
#endif
};
namespace smooth_h
{
	static ft_structs_h::FT_Error
		ft_smooth_init( ft_structs_h::FT_Renderer  render )
	{
		ft_structs_h::FT_Library  library = FT_MODULE_LIBRARY( render );
		render->clazz->raster_class->raster_reset( render->raster,
			library->raster_pool,
			library->raster_pool_size );
		return 0;
	};
	static ft_structs_h::FT_Error
		ft_smooth_render_generic( ft_structs_h::FT_Renderer       render,
		ft_structs_h::FT_GlyphSlot      slot,
		ft_structs_h::FT_Render_Mode    mode,
		const ft_structs_h::FT_Vector*  origin,
		ft_structs_h::FT_Render_Mode    required_mode )
	{
		ft_structs_h::FT_Error     error;
    ft_structs_h::FT_Outline*  outline = NULL;
    ft_structs_h::FT_BBox      cbox;
    ft_structs_h::FT_Pos       width, height, pitch;
#ifndef FT_CONFIG_OPTION_SUBPIXEL_RENDERING
    ft_structs_h::FT_Pos       height_org, width_org;
#endif
    ft_structs_h::FT_Bitmap*   bitmap;
    ft_structs_h::FT_Memory    memory;
    ft_structs_h::FT_Int       hmul = mode == ft_structs_h::FT_RENDER_MODE_LCD;
    ft_structs_h::FT_Int       vmul = mode == ft_structs_h::FT_RENDER_MODE_LCD_V;
    ft_structs_h::FT_Pos       x_shift, y_shift, x_left, y_top;
		ft_structs_h::FT_Raster_Params  params;
		/* check glyph image format */
    if ( slot->format != render->glyph_format )
    {
      error = ft_error_h::Smooth_Err_Invalid_Argument;
      goto Exit;
    }
		/* check mode */
    if ( mode != required_mode )
      return ft_error_h::Smooth_Err_Cannot_Render_Glyph;
		outline = &slot->outline;
		/* translate the outline to the new origin if needed */
    if ( origin )
      FT_Outline_Translate( outline, origin->x, origin->y );
		/* compute the control box, and grid fit it */
    FT_Outline_Get_CBox( outline, &cbox );
		cbox.xMin = FT_PIX_FLOOR( cbox.xMin );
    cbox.yMin = FT_PIX_FLOOR( cbox.yMin );
    cbox.xMax = FT_PIX_CEIL( cbox.xMax );
    cbox.yMax = FT_PIX_CEIL( cbox.yMax );
		if ( cbox.xMin < 0 && cbox.xMax > FT_INT_MAX + cbox.xMin )
    {
      FT_ERROR(( "ft_smooth_render_generic: glyph too large:"
				" xMin = %d, xMax = %d\n",
				cbox.xMin >> 6, cbox.xMax >> 6 ));
			return ft_error_h::Smooth_Err_Raster_Overflow;
    }
    else
      width  = ( cbox.xMax - cbox.xMin ) >> 6;
		if ( cbox.yMin < 0 && cbox.yMax > FT_INT_MAX + cbox.yMin )
    {
      FT_ERROR(( "ft_smooth_render_generic: glyph too large:"
				" yMin = %d, yMax = %d\n",
				cbox.yMin >> 6, cbox.yMax >> 6 ));
			return ft_error_h::Smooth_Err_Raster_Overflow;
    }
    else
      height = ( cbox.yMax - cbox.yMin ) >> 6;
		bitmap = &slot->bitmap;
		memory = render->root.memory;
#ifndef FT_CONFIG_OPTION_SUBPIXEL_RENDERING
    width_org  = width;
    height_org = height;
#endif
		/* release old bitmap buffer */
    if ( slot->internal->flags & FT_GLYPH_OWN_BITMAP )
    {
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
      FT_FREE( bitmap->buffer );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
      slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
		/* allocate new one */
    pitch = width;
    if ( hmul )
    {
      width = width * 3;
      pitch = FT_PAD_CEIL( width, 4 );
    }
		if ( vmul )
			height *= 3;
		x_shift = (ft_structs_h::FT_Int) cbox.xMin;
    y_shift = (ft_structs_h::FT_Int) cbox.yMin;
    x_left  = (ft_structs_h::FT_Int)( cbox.xMin >> 6 );
    y_top   = (ft_structs_h::FT_Int)( cbox.yMax >> 6 );
#ifdef FT_CONFIG_OPTION_SUBPIXEL_RENDERING
		if ( slot->library->lcd_filter_func )
    {
      ft_structs_h::FT_Int  extra = slot->library->lcd_extra;
			if ( hmul )
      {
        x_shift -= 64 * ( extra >> 1 );
        width   += 3 * extra;
        pitch    = FT_PAD_CEIL( width, 4 );
        x_left  -= extra >> 1;
      }
      if ( vmul )
      {
        y_shift -= 64 * ( extra >> 1 );
        height  += 3 * extra;
        y_top   += extra >> 1;
      }
    }
#endif
#if FT_UINT_MAX > 0xFFFFU
		/* Required check is ( pitch * height < FT_ULONG_MAX ),     */
		/* but we care realistic cases only. Always pitch <= width. */
		if ( width > 0x7FFF || height > 0x7FFF )
		{
			FT_ERROR(( "ft_smooth_render_generic: glyph too large: %u x %u\n",
				width, height ));
			return ft_error_h::Smooth_Err_Raster_Overflow;
		}
#endif
		bitmap->pixel_mode = ft_structs_h::FT_PIXEL_MODE_GRAY;
    bitmap->num_grays  = 256;
    bitmap->width      = width;
    bitmap->rows       = height;
    bitmap->pitch      = pitch;
		/* translate outline to render it into the bitmap */
		FT_Outline_Translate( outline, -x_shift, -y_shift );
		if ( FT_ALLOC( bitmap->buffer, (ft_structs_h::FT_ULong)pitch * height ) )
			goto Exit;
		slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
		/* set up parameters */
		params.target = bitmap;
		params.source = outline;
		params.flags  = FT_RASTER_FLAG_AA;
#ifdef FT_CONFIG_OPTION_SUBPIXEL_RENDERING
		/* implode outline if needed */
		{
			ft_structs_h::FT_Vector*  points     = outline->points;
			ft_structs_h::FT_Vector*  points_end = points + outline->n_points;
			ft_structs_h::FT_Vector*  vec;
			if ( hmul )
				for ( vec = points; vec < points_end; vec++ )
					vec->x *= 3;
			if ( vmul )
				for ( vec = points; vec < points_end; vec++ )
					vec->y *= 3;
		}
		/* render outline into the bitmap */
		error = render->raster_render( render->raster, &params );
		/* deflate outline if needed */
		{
      ft_structs_h::FT_Vector*  points     = outline->points;
      ft_structs_h::FT_Vector*  points_end = points + outline->n_points;
      ft_structs_h::FT_Vector*  vec;
			if ( hmul )
				for ( vec = points; vec < points_end; vec++ )
					vec->x /= 3;
			if ( vmul )
				for ( vec = points; vec < points_end; vec++ )
					vec->y /= 3;
		}
		if ( slot->library->lcd_filter_func )
			slot->library->lcd_filter_func( bitmap, mode, slot->library );
#else /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */
		/* render outline into bitmap */
		error = render->raster_render( render->raster, &params );
		/* expand it horizontally */
		if ( hmul )
		{
			ft_structs_h::FT_Byte*  line = bitmap->buffer;
			ft_structs_h::FT_UInt   hh;
			for ( hh = height_org; hh > 0; hh--, line += pitch )
			{
				ft_structs_h::FT_UInt   xx;
				ft_structs_h::FT_Byte*  end = line + width;
				for ( xx = width_org; xx > 0; xx-- )
				{
					ft_structs_h::FT_UInt  pixel = line[xx-1];
					end[-3] = (ft_structs_h::FT_Byte)pixel;
          end[-2] = (ft_structs_h::FT_Byte)pixel;
          end[-1] = (ft_structs_h::FT_Byte)pixel;
          end    -= 3;
        }
      }
    }
		/* expand it vertically */
		if ( vmul )
    {
			ft_structs_h::FT_Byte*  read  = bitmap->buffer + ( height - height_org ) * pitch;
			ft_structs_h::FT_Byte*  write = bitmap->buffer;
			ft_structs_h::FT_UInt   hh;
			for ( hh = height_org; hh > 0; hh-- )
			{
				ft_memcpy( write, read, pitch );
				write += pitch;
				ft_memcpy( write, read, pitch );
				write += pitch;
				ft_memcpy( write, read, pitch );
        write += pitch;
        read  += pitch;
      }
    }
#endif /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */
		FT_Outline_Translate( outline, x_shift, y_shift );
		/*
     * XXX: on 16bit system, we return an error for huge bitmap
     * to prevent an overflow.
     */
		if ( x_left > FT_INT_MAX || y_top > FT_INT_MAX )
			return ft_error_h::Smooth_Err_Invalid_Pixel_Size;
		if ( error )
			goto Exit;
		slot->format      = ft_structs_h::FT_GLYPH_FORMAT_BITMAP;
		slot->bitmap_left = (ft_structs_h::FT_Int)x_left;
		slot->bitmap_top  = (ft_structs_h::FT_Int)y_top;
Exit:
		if ( outline && origin )
			FT_Outline_Translate( outline, -origin->x, -origin->y );
		return error;
	};
	static ft_structs_h::FT_Error
		ft_smooth_render( ft_structs_h::FT_Renderer       render,
		ft_structs_h::FT_GlyphSlot      slot,
		ft_structs_h::FT_Render_Mode    mode,
		const ft_structs_h::FT_Vector*  origin )
	{
		if ( mode == ft_structs_h::FT_RENDER_MODE_LIGHT )
			mode = ft_structs_h::FT_RENDER_MODE_NORMAL;
		return ft_smooth_render_generic( render, slot, mode, origin,
			ft_structs_h::FT_RENDER_MODE_NORMAL );
  };
	static ft_structs_h::FT_Error
		ft_smooth_set_mode( ft_structs_h::FT_Renderer  render,
		ft_structs_h::FT_ULong     mode_tag,
		ft_structs_h::FT_Pointer   data )
	{
		/* we simply pass it to the raster */
		return render->clazz->raster_class->raster_set_mode( render->raster,
			mode_tag,
			data );
	};
	static void
		ft_smooth_get_cbox( ft_structs_h::FT_Renderer   render,
		ft_structs_h::FT_GlyphSlot  slot,
		ft_structs_h::FT_BBox*      cbox )
	{
		FT_MEM_ZERO( cbox, sizeof ( *cbox ) );
		if ( slot->format == render->glyph_format )
			FT_Outline_Get_CBox( &slot->outline, cbox );
	};
	static ft_structs_h::FT_Error
		ft_smooth_transform( ft_structs_h::FT_Renderer       render,
		ft_structs_h::FT_GlyphSlot      slot,
		const ft_structs_h::FT_Matrix*  matrix,
		const ft_structs_h::FT_Vector*  delta )
	{
		ft_structs_h::FT_Error  error = ft_error_h::Smooth_Err_Ok;
		if ( slot->format != render->glyph_format )
		{
      error = ft_error_h::Smooth_Err_Invalid_Argument;
      goto Exit;
    }
		if ( matrix )
      FT_Outline_Transform( &slot->outline, matrix );
		if ( delta )
      FT_Outline_Translate( &slot->outline, delta->x, delta->y );
Exit:
		return error;
	}
#define ErrRaster_Invalid_Mode      ft_error_h::Smooth_Err_Cannot_Render_Glyph
#define ErrRaster_Invalid_Outline   ft_error_h::Smooth_Err_Invalid_Outline
#define ErrRaster_Memory_Overflow   ft_error_h::Smooth_Err_Out_Of_Memory
#define ErrRaster_Invalid_Argument  ft_error_h::Smooth_Err_Invalid_Argument
#define PIXEL_BITS  8
#undef FLOOR
#undef CEILING
#undef TRUNC
#undef SCALED
#define ONE_PIXEL       ( 1L << PIXEL_BITS )
#define PIXEL_MASK      ( -1L << PIXEL_BITS )
#define TRUNC( x )      ( (TCoord)( (x) >> PIXEL_BITS ) )
#define SUBPIXELS( x )  ( (TPos)(x) << PIXEL_BITS )
#define FLOOR( x )      ( (x) & -ONE_PIXEL )
#define CEILING( x )    ( ( (x) + ONE_PIXEL - 1 ) & -ONE_PIXEL )
#define ROUND( x )      ( ( (x) + ONE_PIXEL / 2 ) & -ONE_PIXEL )
#if PIXEL_BITS >= 6
#define UPSCALE( x )    ( (x) << ( PIXEL_BITS - 6 ) )
#define DOWNSCALE( x )  ( (x) >> ( PIXEL_BITS - 6 ) )
#else
#define UPSCALE( x )    ( (x) >> ( 6 - PIXEL_BITS ) )
#define DOWNSCALE( x )  ( (x) << ( 6 - PIXEL_BITS ) )
#endif
	typedef long  TCoord;   /* integer scanline/pixel coordinate */
  typedef long  TPos;     /* sub-pixel coordinate              */
#if PIXEL_BITS <= 7
	typedef int  TArea;
#else /* PIXEL_BITS >= 8 */
	/* approximately determine the size of integers using an ANSI-C header */
#if FT_UINT_MAX == 0xFFFFU
	typedef long  TArea;
#else
	typedef int   TArea;
#endif
#endif /* PIXEL_BITS >= 8 */
	typedef struct TCell_*  PCell;
	typedef struct  TCell_
	{
		TPos    x;     /* same with gray_TWorker.ex    */
		TCoord  cover; /* same with gray_TWorker.cover */
		TArea   area;
		PCell   next;
	} TCell;
#define FT_MAX_GRAY_SPANS  32
	typedef struct  gray_TWorker_
	{
		TCoord  ex, ey;
		TPos    min_ex, max_ex;
		TPos    min_ey, max_ey;
		TPos    count_ex, count_ey;
		TArea   area;
		TCoord  cover;
		int     invalid;
		PCell       cells;
		ft_structs_h::FT_PtrDist  max_cells;
		ft_structs_h::FT_PtrDist  num_cells;
		TCoord  cx, cy;
		TPos    x,  y;
		TPos    last_ey;
		ft_structs_h::FT_Vector   bez_stack[32 * 3 + 1];
		int         lev_stack[32];
		ft_structs_h::FT_Outline  outline;
		ft_structs_h::FT_Bitmap   target;
		ft_structs_h::FT_BBox     clip_box;
		ft_structs_h::FT_Span     gray_spans[FT_MAX_GRAY_SPANS];
		int         num_gray_spans;
		ft_structs_h::FT_Raster_Span_Func  render_span;
		void*                render_span_data;
		int                  span_y;
		int  band_size;
		int  band_shoot;
		ft_jmp_buf  jump_buffer;
		void*       buffer;
		long        buffer_size;
		PCell*     ycells;
		TPos       ycount;
	} gray_TWorker, *gray_PWorker;
	typedef struct gray_TRaster_
	{
		void*         buffer;
		long          buffer_size;
		int           band_size;
		void*         memory;
		gray_PWorker  worker;
	} gray_TRaster, *gray_PRaster;
#undef RAS_ARG
#undef RAS_ARG_
#undef RAS_VAR
#undef RAS_VAR_
#ifndef FT_STATIC_RASTER
#define RAS_ARG   gray_PWorker  worker
#define RAS_ARG_  gray_PWorker  worker,
#define RAS_VAR   worker
#define RAS_VAR_  worker,
#else /* FT_STATIC_RASTER */
#define RAS_ARG   /* empty */
#define RAS_ARG_  /* empty */
#define RAS_VAR   /* empty */
#define RAS_VAR_  /* empty */
#endif /* FT_STATIC_RASTER */
#ifndef FT_STATIC_RASTER
#define ras  (*worker)
#else
	static gray_TWorker  ras;
#endif
	typedef struct  gray_TBand_
	{
		TPos  min, max;
	} gray_TBand;
	static PCell
		gray_find_cell( RAS_ARG )
	{
		PCell  *pcell, cell;
		TPos    x = ras.ex;
		if ( x > ras.count_ex )
			x = ras.count_ex;
		pcell = &ras.ycells[ras.ey];
    for (;;)
    {
      cell = *pcell;
      if ( cell == NULL || cell->x > x )
        break;
			if ( cell->x == x )
        goto Exit;
			pcell = &cell->next;
    }
		if ( ras.num_cells >= ras.max_cells )
			ft_longjmp( ras.jump_buffer, 1 );
		cell        = ras.cells + ras.num_cells++;
    cell->x     = x;
    cell->area  = 0;
    cell->cover = 0;
		cell->next  = *pcell;
		*pcell      = cell;
Exit:
		return cell;
	};
	static void
		gray_record_cell( RAS_ARG )
	{
		if ( !ras.invalid && ( ras.area | ras.cover ) )
		{
			PCell  cell = gray_find_cell( RAS_VAR );
      cell->area  += ras.area;
      cell->cover += ras.cover;
    }
  };
	static void
		gray_set_cell( RAS_ARG_ TCoord  ex,
		TCoord  ey )
	{
		ey -= ras.min_ey;
		if ( ex > ras.max_ex )
			ex = ras.max_ex;
		ex -= ras.min_ex;
		if ( ex < 0 )
			ex = -1;
    if ( ex != ras.ex || ey != ras.ey )
    {
      if ( !ras.invalid )
        gray_record_cell( RAS_VAR );
      ras.area  = 0;
      ras.cover = 0;
    }
		ras.ex      = ex;
		ras.ey      = ey;
		ras.invalid = ( (unsigned)ey >= (unsigned)ras.count_ey ||
			ex >= ras.count_ex           );
	};
	static void
		gray_start_cell( RAS_ARG_ TCoord  ex,
		TCoord  ey )
	{
		if ( ex > ras.max_ex )
			ex = (TCoord)( ras.max_ex );
		if ( ex < ras.min_ex )
			ex = (TCoord)( ras.min_ex - 1 );
		ras.area    = 0;
    ras.cover   = 0;
    ras.ex      = ex - ras.min_ex;
    ras.ey      = ey - ras.min_ey;
    ras.last_ey = SUBPIXELS( ey );
    ras.invalid = 0;
		gray_set_cell( RAS_VAR_ ex, ey );
  };
	static int
		gray_move_to( const ft_structs_h::FT_Vector*  to,
		gray_PWorker      worker )
	{
		TPos  x, y;
		/* record current cell, if any */
		gray_record_cell( RAS_VAR );
		/* start to a new position */
		x = UPSCALE( to->x );
		y = UPSCALE( to->y );
		gray_start_cell( RAS_VAR_ TRUNC( x ), TRUNC( y ) );
		worker->x = x;
		worker->y = y;
		return 0;
	};
	static void
		gray_render_scanline( RAS_ARG_ TCoord  ey,
		TPos    x1,
		TCoord  y1,
		TPos    x2,
		TCoord  y2 )
	{
    TCoord  ex1, ex2, fx1, fx2, delta, mod, lift, rem;
    long    p, first, dx;
    int     incr;
		dx = x2 - x1;
		ex1 = TRUNC( x1 );
    ex2 = TRUNC( x2 );
    fx1 = (TCoord)( x1 - SUBPIXELS( ex1 ) );
    fx2 = (TCoord)( x2 - SUBPIXELS( ex2 ) );
    if ( y1 == y2 )
    {
      gray_set_cell( RAS_VAR_ ex2, ey );
      return;
    }
		if ( ex1 == ex2 )
    {
      delta      = y2 - y1;
      ras.area  += (TArea)(( fx1 + fx2 ) * delta);
      ras.cover += delta;
      return;
    }
    p     = ( ONE_PIXEL - fx1 ) * ( y2 - y1 );
    first = ONE_PIXEL;
    incr  = 1;
    if ( dx < 0 )
    {
      p     = fx1 * ( y2 - y1 );
      first = 0;
      incr  = -1;
      dx    = -dx;
    }
    delta = (TCoord)( p / dx );
    mod   = (TCoord)( p % dx );
    if ( mod < 0 )
    {
      delta--;
      mod += (TCoord)dx;
    }
    ras.area  += (TArea)(( fx1 + first ) * delta);
    ras.cover += delta;
    ex1 += incr;
    gray_set_cell( RAS_VAR_ ex1, ey );
    y1  += delta;
    if ( ex1 != ex2 )
    {
      p    = ONE_PIXEL * ( y2 - y1 + delta );
      lift = (TCoord)( p / dx );
      rem  = (TCoord)( p % dx );
      if ( rem < 0 )
      {
        lift--;
        rem += (TCoord)dx;
      }
      mod -= (int)dx;
      while ( ex1 != ex2 )
      {
        delta = lift;
        mod  += rem;
        if ( mod >= 0 )
        {
          mod -= (TCoord)dx;
          delta++;
        }
        ras.area  += (TArea)(ONE_PIXEL * delta);
        ras.cover += delta;
        y1        += delta;
        ex1       += incr;
        gray_set_cell( RAS_VAR_ ex1, ey );
      }
    }
    delta      = y2 - y1;
    ras.area  += (TArea)(( fx2 + ONE_PIXEL - first ) * delta);
    ras.cover += delta;
  };
	static void
		gray_render_line( RAS_ARG_ TPos  to_x,
		TPos  to_y )
	{
		TCoord  ey1, ey2, fy1, fy2, mod;
    TPos    dx, dy, x, x2;
    long    p, first;
    int     delta, rem, lift, incr;
		ey1 = TRUNC( ras.last_ey );
    ey2 = TRUNC( to_y );     /* if (ey2 >= ras.max_ey) ey2 = ras.max_ey-1; */
    fy1 = (TCoord)( ras.y - ras.last_ey );
    fy2 = (TCoord)( to_y - SUBPIXELS( ey2 ) );
		dx = to_x - ras.x;
    dy = to_y - ras.y;
		{
			TCoord  min, max;
			min = ey1;
      max = ey2;
      if ( ey1 > ey2 )
      {
        min = ey2;
        max = ey1;
      }
      if ( min >= ras.max_ey || max < ras.min_ey )
        goto End;
    }
		if ( ey1 == ey2 )
    {
      gray_render_scanline( RAS_VAR_ ey1, ras.x, fy1, to_x, fy2 );
      goto End;
    }
    incr = 1;
    if ( dx == 0 )
    {
      TCoord  ex     = TRUNC( ras.x );
      TCoord  two_fx = (TCoord)( ( ras.x - SUBPIXELS( ex ) ) << 1 );
      TArea   area;
			first = ONE_PIXEL;
      if ( dy < 0 )
      {
        first = 0;
        incr  = -1;
      }
      delta      = (int)( first - fy1 );
      ras.area  += (TArea)two_fx * delta;
      ras.cover += delta;
      ey1       += incr;
      gray_set_cell( RAS_VAR_ ex, ey1 );
      delta = (int)( first + first - ONE_PIXEL );
      area  = (TArea)two_fx * delta;
      while ( ey1 != ey2 )
      {
        ras.area  += area;
        ras.cover += delta;
        ey1       += incr;
        gray_set_cell( RAS_VAR_ ex, ey1 );
      }
      delta      = (int)( fy2 - ONE_PIXEL + first );
      ras.area  += (TArea)two_fx * delta;
      ras.cover += delta;
      goto End;
    }
		p     = ( ONE_PIXEL - fy1 ) * dx;
    first = ONE_PIXEL;
    incr  = 1;
		if ( dy < 0 )
    {
      p     = fy1 * dx;
      first = 0;
      incr  = -1;
      dy    = -dy;
    }
		delta = (int)( p / dy );
    mod   = (int)( p % dy );
    if ( mod < 0 )
    {
      delta--;
      mod += (TCoord)dy;
    }
    x = ras.x + delta;
    gray_render_scanline( RAS_VAR_ ey1, ras.x, fy1, x, (TCoord)first );
    ey1 += incr;
    gray_set_cell( RAS_VAR_ TRUNC( x ), ey1 );
    if ( ey1 != ey2 )
    {
      p     = ONE_PIXEL * dx;
      lift  = (int)( p / dy );
      rem   = (int)( p % dy );
      if ( rem < 0 )
      {
        lift--;
        rem += (int)dy;
      }
      mod -= (int)dy;
      while ( ey1 != ey2 )
      {
        delta = lift;
        mod  += rem;
        if ( mod >= 0 )
        {
          mod -= (int)dy;
          delta++;
        }
        x2 = x + delta;
        gray_render_scanline( RAS_VAR_ ey1, x,
					(TCoord)( ONE_PIXEL - first ), x2,
					(TCoord)first );
				x = x2;
				ey1 += incr;
				gray_set_cell( RAS_VAR_ TRUNC( x ), ey1 );
      }
    }
    gray_render_scanline( RAS_VAR_ ey1, x,
			(TCoord)( ONE_PIXEL - first ), to_x,
			fy2 );
End:
		ras.x       = to_x;
		ras.y       = to_y;
		ras.last_ey = SUBPIXELS( ey2 );
  };
	static int
		gray_line_to( const ft_structs_h::FT_Vector*  to,
		gray_PWorker      worker )
	{
		gray_render_line( RAS_VAR_ UPSCALE( to->x ), UPSCALE( to->y ) );
		return 0;
	};
	static void
		gray_split_conic( ft_structs_h::FT_Vector*  base )
	{
		TPos  a, b;
		base[4].x = base[2].x;
    b = base[1].x;
    a = base[3].x = ( base[2].x + b ) / 2;
    b = base[1].x = ( base[0].x + b ) / 2;
    base[2].x = ( a + b ) / 2;
		base[4].y = base[2].y;
    b = base[1].y;
    a = base[3].y = ( base[2].y + b ) / 2;
    b = base[1].y = ( base[0].y + b ) / 2;
    base[2].y = ( a + b ) / 2;
  };
	static void
		gray_render_conic( RAS_ARG_ const ft_structs_h::FT_Vector*  control,
		const ft_structs_h::FT_Vector*  to )
	{
    TPos        dx, dy;
    TPos        min, max, y;
    int         top, level;
    int*        levels;
    ft_structs_h::FT_Vector*  arc;
    levels = ras.lev_stack;
    arc      = ras.bez_stack;
    arc[0].x = UPSCALE( to->x );
    arc[0].y = UPSCALE( to->y );
    arc[1].x = UPSCALE( control->x );
    arc[1].y = UPSCALE( control->y );
    arc[2].x = ras.x;
    arc[2].y = ras.y;
    top      = 0;
    dx = FT_ABS( arc[2].x + arc[0].x - 2 * arc[1].x );
    dy = FT_ABS( arc[2].y + arc[0].y - 2 * arc[1].y );
    if ( dx < dy )
      dx = dy;
    if ( dx < ONE_PIXEL / 4 )
      goto Draw;
    min = max = arc[0].y;
    y = arc[1].y;
    if ( y < min ) min = y;
    if ( y > max ) max = y;
    y = arc[2].y;
    if ( y < min ) min = y;
    if ( y > max ) max = y;
    if ( TRUNC( min ) >= ras.max_ey || TRUNC( max ) < ras.min_ey )
      goto Draw;
    level = 0;
    do
    {
      dx >>= 2;
      level++;
    } while ( dx > ONE_PIXEL / 4 );
    levels[0] = level;
    do
    {
      level = levels[top];
      if ( level > 0 )
      {
        gray_split_conic( arc );
        arc += 2;
        top++;
        levels[top] = levels[top - 1] = level - 1;
        continue;
      }
Draw:
			gray_render_line( RAS_VAR_ arc[0].x, arc[0].y );
			top--;
			arc -= 2;
		} while ( top >= 0 );
  };
	static int
		gray_conic_to( const ft_structs_h::FT_Vector*  control,
		const ft_structs_h::FT_Vector*  to,
		gray_PWorker      worker )
	{
		gray_render_conic( RAS_VAR_ control, to );
		return 0;
	};
	static void
		gray_split_cubic( ft_structs_h::FT_Vector*  base )
	{
		TPos  a, b, c, d;
		base[6].x = base[3].x;
    c = base[1].x;
    d = base[2].x;
    base[1].x = a = ( base[0].x + c ) / 2;
    base[5].x = b = ( base[3].x + d ) / 2;
    c = ( c + d ) / 2;
    base[2].x = a = ( a + c ) / 2;
    base[4].x = b = ( b + c ) / 2;
    base[3].x = ( a + b ) / 2;
		base[6].y = base[3].y;
    c = base[1].y;
    d = base[2].y;
    base[1].y = a = ( base[0].y + c ) / 2;
    base[5].y = b = ( base[3].y + d ) / 2;
    c = ( c + d ) / 2;
    base[2].y = a = ( a + c ) / 2;
    base[4].y = b = ( b + c ) / 2;
    base[3].y = ( a + b ) / 2;
  };
	static void
		gray_render_cubic( RAS_ARG_ const ft_structs_h::FT_Vector*  control1,
		const ft_structs_h::FT_Vector*  control2,
		const ft_structs_h::FT_Vector*  to )
	{
		ft_structs_h::FT_Vector*  arc;
		TPos        min, max, y;
		arc      = ras.bez_stack;
    arc[0].x = UPSCALE( to->x );
    arc[0].y = UPSCALE( to->y );
    arc[1].x = UPSCALE( control2->x );
    arc[1].y = UPSCALE( control2->y );
    arc[2].x = UPSCALE( control1->x );
    arc[2].y = UPSCALE( control1->y );
    arc[3].x = ras.x;
    arc[3].y = ras.y;
    min = max = arc[0].y;
    y = arc[1].y;
    if ( y < min )
      min = y;
    if ( y > max )
      max = y;
    y = arc[2].y;
    if ( y < min )
      min = y;
    if ( y > max )
      max = y;
    y = arc[3].y;
    if ( y < min )
      min = y;
    if ( y > max )
      max = y;
    if ( TRUNC( min ) >= ras.max_ey || TRUNC( max ) < ras.min_ey )
      goto Draw;
    for (;;)
    {
      {
        TPos  dx, dy, dx_, dy_;
        TPos  dx1, dy1, dx2, dy2;
        TPos  L, s, s_limit;
        dx = arc[3].x - arc[0].x;
        dy = arc[3].y - arc[0].y;
        dx_ = FT_ABS( dx );
        dy_ = FT_ABS( dy );
				L = ( dx_ > dy_ ? 236 * dx_ +  97 * dy_
					:  97 * dx_ + 236 * dy_ ) >> 8;
				if ( L > 32767 )
					goto Split;
				s_limit = L * (TPos)( ONE_PIXEL / 6 );
        dx1 = arc[1].x - arc[0].x;
        dy1 = arc[1].y - arc[0].y;
        s = FT_ABS( dy * dx1 - dx * dy1 );
        if ( s > s_limit )
          goto Split;
        dx2 = arc[2].x - arc[0].x;
        dy2 = arc[2].y - arc[0].y;
        s = FT_ABS( dy * dx2 - dx * dy2 );
        if ( s > s_limit )
          goto Split;
        if ( dy * dy1 + dx * dx1 < 0                                     ||
					dy * dy2 + dx * dx2 < 0                                     ||
					dy * (arc[3].y - arc[1].y) + dx * (arc[3].x - arc[1].x) < 0 ||
					dy * (arc[3].y - arc[2].y) + dx * (arc[3].x - arc[2].x) < 0 )
					goto Split;
        goto Draw;
      }
Split:
			gray_split_cubic( arc );
			arc += 3;
			continue;
Draw:
			gray_render_line( RAS_VAR_ arc[0].x, arc[0].y );
			if ( arc == ras.bez_stack )
				return;
			arc -= 3;
		}
	};
	static int
		gray_cubic_to( const ft_structs_h::FT_Vector*  control1,
		const ft_structs_h::FT_Vector*  control2,
		const ft_structs_h::FT_Vector*  to,
		gray_PWorker      worker )
	{
		gray_render_cubic( RAS_VAR_ control1, control2, to );
    return 0;
  };
	FT_DEFINE_OUTLINE_FUNCS(func_interface,
		(ft_structs_h::FT_Outline_MoveTo_Func) gray_move_to,
		(ft_structs_h::FT_Outline_LineTo_Func) gray_line_to,
		(ft_structs_h::FT_Outline_ConicTo_Func)gray_conic_to,
		(ft_structs_h::FT_Outline_CubicTo_Func)gray_cubic_to,
		0,
		0
		)
#ifdef _STANDALONE_
	static int
		gray_raster_new( void*       memory,
		FT_Raster*  araster )
	{
		static gray_TRaster  the_raster;
		FT_UNUSED( memory );
		*araster = (FT_Raster)&the_raster;
		FT_MEM_ZERO( &the_raster, sizeof ( the_raster ) );
		return 0;
	};
	static void
		gray_raster_done( FT_Raster  raster )
	{
		/* nothing */
		FT_UNUSED( raster );
	};
#else /* !_STANDALONE_ */
	static int
		gray_raster_new( ft_structs_h::FT_Memory   memory,
		ft_structs_h::FT_Raster*  araster )
	{
		ft_structs_h::FT_Error      error;
		gray_PRaster  raster = NULL;
		*araster = 0;
		if ( !FT_ALLOC( raster, sizeof ( gray_TRaster ) ) )
		{
			raster->memory = memory;
			*araster       = (ft_structs_h::FT_Raster)raster;
		}
		return error;
	};
	static void
		gray_raster_done( ft_structs_h::FT_Raster  raster )
	{
		ft_structs_h::FT_Memory  memory = (ft_structs_h::FT_Memory)((gray_PRaster)raster)->memory;
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		FT_FREE( raster );
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
	};
	static void
		gray_raster_reset( ft_structs_h::FT_Raster  raster,
		char*      pool_base,
		long       pool_size )
	{
		gray_PRaster  rast = (gray_PRaster)raster;
		if ( raster )
    {
      if ( pool_base && pool_size >= (long)sizeof ( gray_TWorker ) + 2048 )
      {
        gray_PWorker  worker = (gray_PWorker)pool_base;
				rast->worker      = worker;
				rast->buffer      = pool_base +
					( ( sizeof ( gray_TWorker ) +
					sizeof ( TCell ) - 1 )  &
					~( sizeof ( TCell ) - 1 ) );
				rast->buffer_size = (long)( ( pool_base + pool_size ) -
					(char*)rast->buffer ) &
					~( sizeof ( TCell ) - 1 );
				rast->band_size   = (int)( rast->buffer_size /
					( sizeof ( TCell ) * 8 ) );
			}
      else
      {
				rast->buffer      = NULL;
				rast->buffer_size = 0;
				rast->worker      = NULL;
			}
		}
	};
	static void
		gray_render_span( int             y,
		int             count,
		const ft_structs_h::FT_Span*  spans,
		gray_PWorker    worker )
	{
		unsigned char*  p;
		ft_structs_h::FT_Bitmap*      map = &worker->target;
		/* first of all, compute the scanline offset */
		p = (unsigned char*)map->buffer - y * map->pitch;
		if ( map->pitch >= 0 )
			p += (unsigned)( ( map->rows - 1 ) * map->pitch );
		for ( ; count > 0; count--, spans++ )
    {
      unsigned char  coverage = spans->coverage;
			if ( coverage )
			{
        /* For small-spans it is faster to do it by ourselves than
         * calling `memset'.  This is mainly due to the cost of the
         * function call.
         */
        if ( spans->len >= 8 )
          FT_MEM_SET( p + spans->x, (unsigned char)coverage, spans->len );
        else
				{
					unsigned char*  q = p + spans->x;
					switch ( spans->len )
					{
          case 7: *q++ = (unsigned char)coverage;
          case 6: *q++ = (unsigned char)coverage;
          case 5: *q++ = (unsigned char)coverage;
          case 4: *q++ = (unsigned char)coverage;
          case 3: *q++ = (unsigned char)coverage;
          case 2: *q++ = (unsigned char)coverage;
          case 1: *q   = (unsigned char)coverage;
          default:
            ;
          }
        }
      }
    }
  };
	static void
		gray_init_cells( RAS_ARG_ void*  buffer,
		long            byte_size )
	{
		ras.buffer      = buffer;
		ras.buffer_size = byte_size;
		ras.ycells      = (PCell*) buffer;
    ras.cells       = NULL;
    ras.max_cells   = 0;
    ras.num_cells   = 0;
    ras.area        = 0;
    ras.cover       = 0;
    ras.invalid     = 1;
  };
	static void
		gray_compute_cbox( RAS_ARG )
	{
		ft_structs_h::FT_Outline*  outline = &ras.outline;
		ft_structs_h::FT_Vector*   vec     = outline->points;
		ft_structs_h::FT_Vector*   limit   = vec + outline->n_points;
		if ( outline->n_points <= 0 )
		{
			ras.min_ex = ras.max_ex = 0;
			ras.min_ey = ras.max_ey = 0;
			return;
		}
		ras.min_ex = ras.max_ex = vec->x;
		ras.min_ey = ras.max_ey = vec->y;
		vec++;
		for ( ; vec < limit; vec++ )
    {
      TPos  x = vec->x;
      TPos  y = vec->y;
			if ( x < ras.min_ex ) ras.min_ex = x;
      if ( x > ras.max_ex ) ras.max_ex = x;
      if ( y < ras.min_ey ) ras.min_ey = y;
      if ( y > ras.max_ey ) ras.max_ey = y;
    }
		/* truncate the bounding box to integer pixels */
    ras.min_ex = ras.min_ex >> 6;
    ras.min_ey = ras.min_ey >> 6;
    ras.max_ex = ( ras.max_ex + 63 ) >> 6;
    ras.max_ey = ( ras.max_ey + 63 ) >> 6;
  };
	static int
		gray_convert_glyph_inner( RAS_ARG )
	{
		volatile int  error = 0;
#ifdef FT_CONFIG_OPTION_PIC
		FT_Outline_Funcs func_interface;
		Init_Class_func_interface(&func_interface);
#endif		
		if ( ft_setjmp( ras.jump_buffer ) == 0 )
		{
			error = FT_Outline_Decompose( &ras.outline, &func_interface, &ras );
			gray_record_cell( RAS_VAR );
		}
		else
			error = ErrRaster_Memory_Overflow;
		return error;
  }
	static void
		gray_hline( RAS_ARG_ TCoord  x,
		TCoord  y,
		TPos    area,
		TCoord  acount )
	{
		ft_structs_h::FT_Span*  span;
		int       count;
		int       coverage;
		coverage = (int)( area >> ( PIXEL_BITS * 2 + 1 - 8 ) );
		/* use range 0..256 */
		if ( coverage < 0 )
			coverage = -coverage;
		if ( ras.outline.flags & FT_OUTLINE_EVEN_ODD_FILL )
		{
			coverage &= 511;
			if ( coverage > 256 )
        coverage = 512 - coverage;
      else if ( coverage == 256 )
        coverage = 255;
    }
    else
    {
      /* normal non-zero winding rule */
      if ( coverage >= 256 )
        coverage = 255;
    }
    y += (TCoord)ras.min_ey;
    x += (TCoord)ras.min_ex;
    if ( x >= 32767 )
      x = 32767;
    if ( y >= FT_INT_MAX )
      y = FT_INT_MAX;
    if ( coverage )
    {
      count = ras.num_gray_spans;
      span  = ras.gray_spans + count - 1;
      if ( count > 0                          &&
				ras.span_y == y                    &&
				(int)span->x + span->len == (int)x &&
				span->coverage == coverage         )
			{
        span->len = (unsigned short)( span->len + acount );
        return;
      }
			if ( ras.span_y != y || count >= FT_MAX_GRAY_SPANS )
			{
				if ( ras.render_span && count > 0 )
					ras.render_span( ras.span_y, count, ras.gray_spans,
					ras.render_span_data );
#ifdef FT_DEBUG_LEVEL_TRACE
				if ( count > 0 )
				{
					int  n;
					FT_TRACE7(( "y = %3d ", ras.span_y ));
					span = ras.gray_spans;
          for ( n = 0; n < count; n++, span++ )
						FT_TRACE7(( "[%d..%d]:%02x ",
						span->x, span->x + span->len - 1, span->coverage ));
					FT_TRACE7(( "\n" ));
        }
#endif /* FT_DEBUG_LEVEL_TRACE */
				ras.num_gray_spans = 0;
				ras.span_y         = (int)y;
				count = 0;
        span  = ras.gray_spans;
      }
      else
        span++;
      span->x        = (short)x;
      span->len      = (unsigned short)acount;
      span->coverage = (unsigned char)coverage;
			ras.num_gray_spans++;
		}
	};
	static void
		gray_sweep( RAS_ARG_ const ft_structs_h::FT_Bitmap*  target )
	{
		int  yindex;
		FT_UNUSED( target );
		if ( ras.num_cells == 0 )
			return;
		ras.num_gray_spans = 0;
		FT_TRACE7(( "gray_sweep: start\n" ));
		for ( yindex = 0; yindex < ras.ycount; yindex++ )
    {
      PCell   cell  = ras.ycells[yindex];
      TCoord  cover = 0;
      TCoord  x     = 0;
			for ( ; cell != NULL; cell = cell->next )
			{
				TPos  area;
				if ( cell->x > x && cover != 0 )
					gray_hline( RAS_VAR_ x, yindex, cover * ( ONE_PIXEL * 2 ),
					cell->x - x );
				cover += cell->cover;
				area   = cover * ( ONE_PIXEL * 2 ) - cell->area;
				if ( area != 0 && cell->x >= 0 )
					gray_hline( RAS_VAR_ cell->x, yindex, area, 1 );
				x = cell->x + 1;
			}
			if ( cover != 0 )
				gray_hline( RAS_VAR_ x, yindex, cover * ( ONE_PIXEL * 2 ),
				ras.count_ex - x );
		}
		if ( ras.render_span && ras.num_gray_spans > 0 )
			ras.render_span( ras.span_y, ras.num_gray_spans,
			ras.gray_spans, ras.render_span_data );
		FT_TRACE7(( "gray_sweep: end\n" ));
  };
	static int
		gray_convert_glyph( RAS_ARG )
	{
		gray_TBand            bands[40];    gray_TBand* volatile  band;
		int volatile          n, num_bands;
		TPos volatile         min, max, max_y;
		ft_structs_h::FT_BBox*              clip;
		/* Set up state in the raster object */
		gray_compute_cbox( RAS_VAR );
		/* clip to target bitmap, exit if nothing to do */
		clip = &ras.clip_box;
		if ( ras.max_ex <= clip->xMin || ras.min_ex >= clip->xMax ||
			ras.max_ey <= clip->yMin || ras.min_ey >= clip->yMax )
			return 0;
		if ( ras.min_ex < clip->xMin ) ras.min_ex = clip->xMin;
		if ( ras.min_ey < clip->yMin ) ras.min_ey = clip->yMin;
		if ( ras.max_ex > clip->xMax ) ras.max_ex = clip->xMax;
    if ( ras.max_ey > clip->yMax ) ras.max_ey = clip->yMax;
		ras.count_ex = ras.max_ex - ras.min_ex;
		ras.count_ey = ras.max_ey - ras.min_ey;
		/* set up vertical bands */
		num_bands = (int)( ( ras.max_ey - ras.min_ey ) / ras.band_size );
		if ( num_bands == 0 )
			num_bands = 1;
		if ( num_bands >= 39 )
			num_bands = 39;
		ras.band_shoot = 0;
		min   = ras.min_ey;
    max_y = ras.max_ey;
		for ( n = 0; n < num_bands; n++, min = max )
    {
      max = min + ras.band_size;
      if ( n == num_bands - 1 || max > max_y )
        max = max_y;
			bands[0].min = min;
      bands[0].max = max;
      band         = bands;
			while ( band >= bands )
      {
        TPos  bottom, top, middle;
        int   error;
				{
          PCell  cells_max;
          int    yindex;
          long   cell_start, cell_end, cell_mod;
          ras.ycells = (PCell*)ras.buffer;
          ras.ycount = band->max - band->min;
          cell_start = sizeof ( PCell ) * ras.ycount;
          cell_mod   = cell_start % sizeof ( TCell );
          if ( cell_mod > 0 )
						cell_start += sizeof ( TCell ) - cell_mod;
					cell_end  = ras.buffer_size;
          cell_end -= cell_end % sizeof ( TCell );
					cells_max = (PCell)( (char*)ras.buffer + cell_end );
          ras.cells = (PCell)( (char*)ras.buffer + cell_start );
          if ( ras.cells >= cells_max )
            goto ReduceBands;
					ras.max_cells = cells_max - ras.cells;
          if ( ras.max_cells < 2 )
            goto ReduceBands;
					for ( yindex = 0; yindex < ras.ycount; yindex++ )
						ras.ycells[yindex] = NULL;
				}
				ras.num_cells = 0;
        ras.invalid   = 1;
        ras.min_ey    = band->min;
        ras.max_ey    = band->max;
        ras.count_ey  = band->max - band->min;
				error = gray_convert_glyph_inner( RAS_VAR );
				if ( !error )
				{
					gray_sweep( RAS_VAR_ &ras.target );
					band--;
          continue;
        }
        else if ( error != ErrRaster_Memory_Overflow )
          return 1;
ReduceBands:
				/* render pool overflow; we will reduce the render band by half */
				bottom = band->min;
				top    = band->max;
				middle = bottom + ( ( top - bottom ) >> 1 );
				/* This is too complex for a single scanline; there must */
				/* be some problems.                                     */
				if ( middle == bottom )
        {
#ifdef FT_DEBUG_LEVEL_TRACE
          FT_TRACE7(( "gray_convert_glyph: rotten glyph\n" ));
#endif
          return 1;
        }
				if ( bottom-top >= ras.band_size )
					ras.band_shoot++;
				band[1].min = bottom;
        band[1].max = middle;
        band[0].min = middle;
        band[0].max = top;
        band++;
      }
    }
		if ( ras.band_shoot > 8 && ras.band_size > 16 )
			ras.band_size = ras.band_size / 2;
		return 0;
  }
	static int
		gray_raster_render( gray_PRaster             raster,
		const ft_structs_h::FT_Raster_Params*  params )
	{
		const ft_structs_h::FT_Outline*  outline    = (const ft_structs_h::FT_Outline*)params->source;
		const ft_structs_h::FT_Bitmap*   target_map = params->target;
		gray_PWorker       worker;
		if ( !raster || !raster->buffer || !raster->buffer_size )
			return ErrRaster_Invalid_Argument;
		if ( !outline )
			return ErrRaster_Invalid_Outline;
		/* return immediately if the outline is empty */
		if ( outline->n_points == 0 || outline->n_contours <= 0 )
			return 0;
		if ( !outline->contours || !outline->points )
      return ErrRaster_Invalid_Outline;
		if ( outline->n_points !=
			outline->contours[outline->n_contours - 1] + 1 )
			return ErrRaster_Invalid_Outline;
		worker = raster->worker;
		/* if direct mode is not set, we must have a target bitmap */
		if ( !( params->flags & FT_RASTER_FLAG_DIRECT ) )
		{
      if ( !target_map )
        return ErrRaster_Invalid_Argument;
			/* nothing to do */
			if ( !target_map->width || !target_map->rows )
				return 0;
			if ( !target_map->buffer )
				return ErrRaster_Invalid_Argument;
		}
		/* this version does not support monochrome rendering */
		if ( !( params->flags & FT_RASTER_FLAG_AA ) )
			return ErrRaster_Invalid_Mode;
		/* compute clipping box */
		if ( !( params->flags & FT_RASTER_FLAG_DIRECT ) )
		{
      /* compute clip box from target pixmap */
      ras.clip_box.xMin = 0;
      ras.clip_box.yMin = 0;
      ras.clip_box.xMax = target_map->width;
      ras.clip_box.yMax = target_map->rows;
    }
    else if ( params->flags & FT_RASTER_FLAG_CLIP )
      ras.clip_box = params->clip_box;
    else
    {
      ras.clip_box.xMin = -32768L;
      ras.clip_box.yMin = -32768L;
      ras.clip_box.xMax =  32767L;
      ras.clip_box.yMax =  32767L;
    }
		gray_init_cells( RAS_VAR_ raster->buffer, raster->buffer_size );
		ras.outline        = *outline;
    ras.num_cells      = 0;
    ras.invalid        = 1;
    ras.band_size      = raster->band_size;
    ras.num_gray_spans = 0;
		if ( params->flags & FT_RASTER_FLAG_DIRECT )
    {
      ras.render_span      = (FT_Raster_Span_Func)params->gray_spans;
      ras.render_span_data = params->user;
    }
    else
    {
      ras.target           = *target_map;
      ras.render_span      = (FT_Raster_Span_Func)gray_render_span;
      ras.render_span_data = &ras;
    }
		return gray_convert_glyph( RAS_VAR );
  };
#endif /* !_STANDALONE_ */
	FT_DEFINE_RASTER_FUNCS(ft_grays_raster,
		ft_structs_h::FT_GLYPH_FORMAT_OUTLINE,
		(ft_structs_h::FT_Raster_New_Func)     gray_raster_new,
		(ft_structs_h::FT_Raster_Reset_Func)   gray_raster_reset,
		(ft_structs_h::FT_Raster_Set_Mode_Func)0,
		(ft_structs_h::FT_Raster_Render_Func)  gray_raster_render,
		(ft_structs_h::FT_Raster_Done_Func)    gray_raster_done
		)
};
FT_CALLBACK_TABLE_DEF
	const ft_structs_h::FT_Driver_ClassRec  winfnt_driver_class =
{
	{
		FT_MODULE_FONT_DRIVER        |				FT_MODULE_DRIVER_NO_OUTLINES,
			sizeof ( ft_structs_h::FT_DriverRec ),
			"winfonts",
			0x10000L,
			0x20000L,
			0,
			0,                  /* FT_Module_Constructor */
			0,                  /* FT_Module_Destructor  */
			ft_winfnt_h::winfnt_get_service
	},
	sizeof ( ft_winfnt_h::FNT_FaceRec ),
	sizeof ( ft_structs_h::FT_SizeRec ),
	sizeof ( ft_structs_h::FT_GlyphSlotRec ),
	ft_winfnt_h::FNT_Face_Init,
	ft_winfnt_h::FNT_Face_Done,
	0,                    /* FT_Size_InitFunc */
	0,                    /* FT_Size_DoneFunc */
	0,                    /* FT_Slot_InitFunc */
	0,                    /* FT_Slot_DoneFunc */
#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
	ft_stub_set_char_sizes,
	ft_stub_set_pixel_sizes,
#endif
	ft_winfnt_h::FNT_Load_Glyph,
	0,                    /* FT_Face_GetKerningFunc  */
	0,                    /* FT_Face_AttachFunc      */
	0,                    /* FT_Face_GetAdvancesFunc */
	ft_winfnt_h::FNT_Size_Request,
	ft_winfnt_h::FNT_Size_Select
};
FT_DEFINE_DRIVER( tt_driver_class ,
	FT_MODULE_FONT_DRIVER     |
	FT_MODULE_DRIVER_SCALABLE |
	TT_HINTER_FLAG,
	sizeof ( tt_h::TT_DriverRec ),
	"truetype",      /* driver name                           */
	0x10000L,        /* driver version == 1.0                 */
	0x20000L,        /* driver requires FreeType 2.0 or above */
	(void*)0,        /* driver specific interface */
	tt_h::tt_driver_init,
	tt_h::tt_driver_done,
	tt_h::tt_get_interface,
	sizeof ( tt_h::TT_FaceRec ),
	sizeof ( tt_h::TT_SizeRec ),
	sizeof ( ft_structs_h::FT_GlyphSlotRec ),
	tt_h::tt_face_init,
	tt_h::tt_face_done,
	tt_h::tt_size_init,
	tt_h::tt_size_done,
	tt_h::tt_slot_init,
	0,                       /* FT_Slot_DoneFunc */
	ft_stub_set_char_sizes,  /* FT_CONFIG_OPTION_OLD_INTERNALS */
	ft_stub_set_pixel_sizes, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	tt_h::tt_glyph_load,
	tt_h::tt_get_kerning,
	0,                       /* FT_Face_AttachFunc */
	tt_h::tt_get_advances,
	tt_h::tt_size_request,
	TT_SIZE_SELECT
	)
	FT_DEFINE_SFNT_INTERFACE(sfnt_interface,
	tt_h::tt_face_goto_table,
	sfnt_h::sfnt_init_face,
	sfnt_h::sfnt_load_face,
	sfnt_h::sfnt_done_face,
	sfnt_h::sfnt_get_interface,
	tt_h::tt_face_load_any,
	tt_h::tt_face_load_sfnt_header_stub, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	tt_h::tt_face_load_directory_stub, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	tt_h::tt_face_load_head,
	tt_h::tt_face_load_hhea,
	tt_h::tt_face_load_cmap,
	tt_h::tt_face_load_maxp,
	tt_h::tt_face_load_os2,
	tt_h::tt_face_load_post,
	tt_h::tt_face_load_name,
	tt_h::tt_face_free_name,
	tt_h::tt_face_load_hdmx_stub, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	tt_h::tt_face_free_hdmx_stub, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	tt_h::tt_face_load_kern,
	tt_h::tt_face_load_gasp,
	tt_h::tt_face_load_pclt,
	/* see `ttload.h' */
	PUT_EMBEDDED_BITMAPS(tt_h::tt_face_load_bhed),
	tt_h::tt_face_set_sbit_strike_stub, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	tt_h::tt_face_load_sbit_stub, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	tt_h::tt_find_sbit_image, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	tt_h::tt_load_sbit_metrics, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	PUT_EMBEDDED_BITMAPS(tt_h::tt_face_load_sbit_image),
	tt_h::tt_face_free_sbit_stub, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	/* see `ttpost.h' */
	PUT_PS_NAMES(tt_h::tt_face_get_ps_name),
	PUT_PS_NAMES(tt_h::tt_face_free_ps_names),
	tt_h::tt_face_load_charmap_stub, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	tt_h::tt_face_free_charmap_stub, /* FT_CONFIG_OPTION_OLD_INTERNALS */
	/* since version 2.1.8 */
	tt_h::tt_face_get_kerning,
	/* since version 2.2 */
	tt_h::tt_face_load_font_dir,
	tt_h::tt_face_load_hmtx,
	/* see `ttsbit.h' and `sfnt.h' */
	PUT_EMBEDDED_BITMAPS(tt_h::tt_face_load_eblc),
	PUT_EMBEDDED_BITMAPS(tt_h::tt_face_free_eblc),
	PUT_EMBEDDED_BITMAPS(tt_h::tt_face_set_sbit_strike),
	PUT_EMBEDDED_BITMAPS(tt_h::tt_face_load_strike_metrics),
	tt_h::tt_face_get_metrics
	)
FT_DEFINE_MODULE(sfnt_module_class,
0,  /* not a font driver or renderer */
sizeof ( ft_structs_h::FT_ModuleRec ),
"sfnt",     /* driver name                            */
0x10000L,   /* driver version 1.0                     */
0x20000L,   /* driver requires FreeType 2.0 or higher */
(const void*)&FT_SFNT_INTERFACE_GET,  /* module specific interface */
(ft_structs_h::FT_Module_Constructor)0,
(ft_structs_h::FT_Module_Destructor) 0,
(ft_structs_h::FT_Module_Requester)  sfnt_h::sfnt_get_interface
)
FT_DEFINE_RENDERER( ft_smooth_renderer_class,
FT_MODULE_RENDERER,
sizeof ( ft_structs_h::FT_RendererRec ),
"smooth",
0x10000L,
0x20000L,
0,    /* module specific interface */
(ft_structs_h::FT_Module_Constructor)smooth_h::ft_smooth_init,
(ft_structs_h::FT_Module_Destructor) 0,
(ft_structs_h::FT_Module_Requester)  0
,
ft_structs_h::FT_GLYPH_FORMAT_OUTLINE,
(ft_structs_h::FT_Renderer_RenderFunc)   smooth_h::ft_smooth_render,
(ft_structs_h::FT_Renderer_TransformFunc)smooth_h::ft_smooth_transform,
(ft_structs_h::FT_Renderer_GetCBoxFunc)  smooth_h::ft_smooth_get_cbox,
(ft_structs_h::FT_Renderer_SetModeFunc)  smooth_h::ft_smooth_set_mode,
(ft_structs_h::FT_Raster_Funcs*)    &FT_GRAYS_RASTER_GET
)