uniform float frame_width;
uniform float frame_height;
uniform sampler2D yv12_tex;
uniform float opacity;	
uniform float brightness;
uniform float contrast;
uniform float window_width;
uniform float window_height;
uniform float texture_width;
uniform float texture_height;
uniform float window_x;
uniform float window_y;
void main() {
  vec2 invScreenSize = vec2( 2048.0 , 2048.0 );
  float nx , ny;
  float v , u , y , r , g , b;
  float x_offset;
  nx = ( gl_FragCoord.x - window_x ) / 2.0;
  ny = ( window_height - gl_FragCoord.y - window_y ) / 2.0;
  x_offset = ( frame_width / 2.0 );
  if( int( ny ) - int( 2 * int( ny / 2.0 ) ) == 0 )
  {
    x_offset = 0.0;
  }
  ny = ny / 2.0;
  v = texture2D( yv12_tex , vec2( x_offset + nx ,frame_height + ny ) ).r;
  u = texture2D( yv12_tex , vec2( x_offset + nx , frame_height + frame_height / 4.0 + ny ) ).r;
  y = texture2D( yv12_tex , gl_TexCoord[0].st ).r;
  y=1.1643*(y-0.0625);
  u=u-0.5;
  v=v-0.5;
  r=y+1.5958*v;
  g=y-0.39173*u-0.81290*v;
  b=y+2.017*u;
  vec4 color = vec4( r , g , b , opacity );
  vec3 bright_color = color.rgb + vec3( brightness , brightness , brightness );
  vec3 contrast_color = ( bright_color.rgb - 0.5 ) * contrast + 0.5;
  gl_FragColor = vec4( contrast_color[ 0 ] , contrast_color[ 1 ] , contrast_color[ 2 ] , color.a );
}