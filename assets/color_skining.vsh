//
// マテリアルカラー + スキニング
// 

$version$

uniform mat4 ciModelViewProjection;
const int MAXBONES = 100;
uniform mat4 boneMatrices[MAXBONES];
in vec4 ciPosition;
in vec4 ciColor;
in ivec4 ciBoneIndex;
in vec4 ciBoneWeight;
out vec4 Color;
                                                      
void main(void) {
  mat4 m;
  m = boneMatrices[ciBoneIndex.x] * ciBoneWeight.x
    + boneMatrices[ciBoneIndex.y] * ciBoneWeight.y
    + boneMatrices[ciBoneIndex.z] * ciBoneWeight.z
    + boneMatrices[ciBoneIndex.w] * ciBoneWeight.w;

  gl_Position	= ciModelViewProjection * m * ciPosition;
  Color       = ciColor;
}
