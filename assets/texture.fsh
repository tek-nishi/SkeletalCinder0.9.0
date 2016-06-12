//
// テクスチャ
//

$version$
$precision$

uniform sampler2D	uTex0;
                                                          
in vec2  TexCoord0;
in vec4  Color;

out vec4 oColor;
		
void main(void) {
  oColor = texture(uTex0, TexCoord0) * Color;
}
