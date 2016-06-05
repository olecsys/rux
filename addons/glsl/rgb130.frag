#version 130
#extension GL_ARB_texture_rectangle : enable
uniform float frame_width;
uniform float frame_height;
uniform sampler2DRect rgb_tex;
uniform float opacity;
uniform float brightness;
uniform float contrast;
out vec4 out_frag_color;
void main() {
  vec4 color = texture2DRect( rgb_tex , vec2( gl_TexCoord[0].x , gl_TexCoord[0].y ) );
  vec3 bright_color = color.rgb + vec3( brightness , brightness , brightness );
  vec3 contrast_color = ( bright_color.rgb - 0.5 ) * contrast + 0.5;
  out_frag_color = vec4( contrast_color[ 0 ] , contrast_color[ 1 ] , contrast_color[ 2 ] , opacity );
}