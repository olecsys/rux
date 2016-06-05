#extension GL_ARB_texture_rectangle : enable
uniform float frame_width;
uniform float frame_height;
uniform sampler2DRect yuyv_tex;
uniform float opacity;
uniform float brightness;
uniform float contrast;
out vec4 out_frag_color;
void main() {
  float nx , ny;
  float r , g , b , y , u , v;
  nx = gl_TexCoord[0].x * frame_width;
  ny = gl_TexCoord[0].y * frame_height;
  y = texture2DRect( yuyv_tex , vec2( nx , ny ) ).r;
  if( int( nx ) - 2 * ( int( nx ) / 2 ) == 1 )
  {
  v = texture2DRect( yuyv_tex , vec2( nx , ny ) ).a;
  u = texture2DRect( yuyv_tex , vec2( nx , ny ) + vec2( 1.0 , 0.0 ) ).a;
  }
  else
  {
  v = texture2DRect( yuyv_tex , vec2( nx , ny ) - vec2( 1.0 , 0.0 ) ).a;
  u = texture2DRect( yuyv_tex , vec2( nx , ny ) ).a;
  }
  y=1.1643*(y-0.0625);
  u=u-0.5;
  v=v-0.5;
  r=y+1.5958*v;
  g=y-0.39173*u-0.81290*v;
  b=y+2.017*u;
  vec4 color = vec4( r , g , b , opacity );
  vec3 bright_color = color.rgb + vec3( brightness , brightness , brightness );
  vec3 contrast_color = ( bright_color.rgb - 0.5 ) * contrast + 0.5;
  out_frag_color = vec4( contrast_color[ 0 ] , contrast_color[ 1 ] , contrast_color[ 2 ] , color.a );
}