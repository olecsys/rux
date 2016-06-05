#version 130
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
precision highp float;
in vec4 in_tex_pos;
out vec4 out_frag_color;
uniform float frame_width;
uniform float frame_height;
uniform sampler2DRect yv12_tex;
uniform float opacity;	
uniform float brightness;
uniform float contrast;
void main() {
  float nx , ny;
  float x_offset;
  float r , g , b , y , u , v;
  nx = gl_TexCoord[0].x / 2.0;
  ny = gl_TexCoord[0].y / 2.0;
  x_offset = frame_width / 2.0;
  if( ny == 0 || int( ny ) % 2 == 0 )
  {
   x_offset = 0;
  }
  ny = ny / 2;
  v = texture( yv12_tex , vec2( x_offset + nx , frame_height + ny ) ).r;
  u = texture( yv12_tex , vec2( x_offset + nx , frame_height + ny + ( frame_height / 4 ) ) ).r;
  y = texture( yv12_tex , gl_TexCoord[0].st ).r;
  y=1.1643*(y-0.0625);
  u=u-0.5;
  v=v-0.5;
  r=y+1.5958*v;
  g=y-0.39173*u-0.81290*v;
  b=y+2.017*u;
  out_frag_color = vec4( ( r + brightness  - 0.5)*contrast+0.5 ,  ( g + brightness  - 0.5)*contrast+0.5 , ( b + brightness  - 0.5)*contrast+0.5 , opacity );
}