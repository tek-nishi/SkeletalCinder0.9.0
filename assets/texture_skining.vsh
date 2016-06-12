//
// テクスチャ + スキニング
//
$version$


uniform mat4 ciModelViewProjection;
const int MAXBONES = 100;
uniform mat4 boneMatrices[MAXBONES];
in vec4  ciPosition;
in vec2  ciTexCoord0;
in ivec4 ciBoneIndex;
in vec4  ciBoneWeight;
out vec2 TexCoord0;
                                                        
void main(void) {
  mat4 m;
  m = boneMatrices[ciBoneIndex.x] * ciBoneWeight.x
    + boneMatrices[ciBoneIndex.y] * ciBoneWeight.y
    + boneMatrices[ciBoneIndex.z] * ciBoneWeight.z
    + boneMatrices[ciBoneIndex.w] * ciBoneWeight.w;
                                                          
  gl_Position	= ciModelViewProjection * m * ciPosition;
  TexCoord0   = ciTexCoord0;
}
