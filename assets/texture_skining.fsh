//
// テクスチャ + スキニング
//
$version$
$precision$

uniform vec4 uColor;
uniform sampler2D	uTex0;
                                                          
in vec2  TexCoord0;
out vec4 oColor;
		
void main(void) {
  oColor = texture( uTex0, TexCoord0 ) * uColor;
}
