#version 130
uniform sampler1D common_texture1d;
uniform float texture_width;
out vec4 out_frag_color;
vec4 get_common( int ind )
{
	return texture1D( common_texture1d , ( float( ind ) + 0.5 ) / texture_width );
}
vec4 compute_color( float x , float color_offset , vec4 begin_color , vec4 end_color )
{
	x = x / color_offset;
	float red = begin_color.r + x * ( end_color.r - begin_color.r );
	float green = begin_color.g + x * ( end_color.g - begin_color.g );
	float blue = begin_color.b + x * ( end_color.b - begin_color.b );
	float alpha = begin_color.a + x * ( end_color.a - begin_color.a );
	return vec4( red , green , blue , alpha * get_common( 12 ).a );
}
float percent;
float get_colors_offset( int ind )
{
	if( ind == 0 )
		return get_common( 8 ).r;
	else if( ind == 1 )
		return get_common( 8 ).g;
	else if( ind == 2 )
		return get_common( 8 ).b;
	else if( ind == 3 )
		return get_common( 8 ).a;
	else if( ind == 4 )
		return get_common( 9 ).r;
	else if( ind == 5 )
		return get_common( 9 ).g;
	else if( ind == 6 )
		return get_common( 9 ).b;
	return 0.0;
}
bool check_colors_offset( int colors_offset_index )
{
	percent = percent - get_colors_offset( colors_offset_index - 1 );
	if( percent <= get_colors_offset( colors_offset_index ) )
		out_frag_color = compute_color( percent , get_colors_offset( colors_offset_index ) 
			, get_common( colors_offset_index - 1 ) , get_common( colors_offset_index ) );
	else if( int( get_common( 7 ).r ) == colors_offset_index + 1 )
		out_frag_color = vec4( get_common( colors_offset_index ).r , get_common( colors_offset_index ).g 
			, get_common( colors_offset_index ).b , get_common( colors_offset_index ).a * get_common( 12 ).a );
	else
		return false;
	return true;
}
void main()
{
	float frag_coord_x = gl_FragCoord.x - get_common( 10 ).b;
	float frag_coord_y = ( get_common( 12 ).b - gl_FragCoord.y ) - get_common( 10 ).a;
	if( get_common( 12 ).r > frag_coord_x || frag_coord_x > get_common( 10 ).r - get_common( 12 ).r 
		|| get_common( 12 ).r > frag_coord_y || frag_coord_y > get_common( 10 ).g - get_common( 12 ).r )
	{
		if( get_common( 11 ).a > 0.0 )
			out_frag_color = vec4( get_common( 11 ).r , get_common( 11 ).g , get_common( 11 ).b
				, get_common( 11 ).a * get_common( 12 ).a );
		else
			discard;
	}
	else
	{
		if( int( get_common( 7 ).r ) == 0 )
			discard;
		else if( int( get_common( 7 ).r ) == 1 )
		{
			if( get_common( 0 ).a != 0.0 )
				out_frag_color = vec4( get_common( 0 ).r , get_common( 0 ).g , get_common( 0 ).b , 
					get_common( 0 ).a * get_common( 12 ).a );
			else
				discard;
		}
		else
		{
			percent =  frag_coord_x / get_common( 10 ).r;
			if( int( get_common( 12 ).g ) == 1 )
				percent = frag_coord_y / get_common( 10 ).g;
			if( check_colors_offset( 1 ) == false )
			{
				if( check_colors_offset( 2 ) == false )
				{
					if( check_colors_offset( 3 ) == false )
					{
						if( check_colors_offset( 4 ) == false )
						{
							if( check_colors_offset( 5 ) == false )
							{
								if( check_colors_offset( 6 ) == false )
								{
								}
							}
						}
					}
				}
			}
		}
	}
}
