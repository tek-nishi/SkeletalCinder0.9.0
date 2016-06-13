//
// 頂点カラー + テクスチャ + スキニング + ライティング
//
$version$


uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

// ライト
// TIPS:UBOを利用
layout (std140) uniform Light {
  vec4 light_ambient;
  vec4 light_diffuse;
  vec4 light_specular;
  vec4 light_position;
};

// マテリアル
uniform vec4  mat_ambient;
uniform vec4  mat_diffuse;
uniform vec4  mat_specular;
uniform float mat_shininess;
uniform vec4  mat_emission;

const int MAXBONES = 100;
uniform mat4 boneMatrices[MAXBONES];

in vec4  ciPosition;
in vec3  ciNormal;
in vec4  ciColor;
in vec2  ciTexCoord0;
in ivec4 ciBoneIndex;
in vec4  ciBoneWeight;

out vec2 TexCoord0;
out vec4 Color;
out vec4 Specular;


void main(void) {
  mat4 m;
  m = boneMatrices[ciBoneIndex.x] * ciBoneWeight.x
    + boneMatrices[ciBoneIndex.y] * ciBoneWeight.y
    + boneMatrices[ciBoneIndex.z] * ciBoneWeight.z
    + boneMatrices[ciBoneIndex.w] * ciBoneWeight.w;
                                                          
  vec4 position	= ciModelViewProjection * m * ciPosition;
  vec3 normal   = normalize(ciNormalMatrix * mat3(m) * ciNormal);
  vec3 light    = normalize((light_position * position.w - position * light_position.w).xyz);

  float diffuse = max(dot(light, normal), 0.0);

  // スペキュラは反射ベクトルを求める方式
  vec3 reflect   = reflect(-light, normal);
  float specular = pow(max(dot(normal, reflect), 0.0), mat_shininess);
  
  gl_Position = position;

  // FIXME:頂点カラーとマテリアル色をどう計算するか悩む
  Color = ciColor * clamp(mat_diffuse * light_diffuse * diffuse
                         + mat_ambient * light_ambient
                         + mat_emission,
                          vec4(0.0f, 0.0f, 0.0f, 0.0f),
                          vec4(1.0f, 1.0f, 1.0f, 1.0f));
  Specular  = mat_specular * light_specular * specular;
  TexCoord0 = ciTexCoord0;
}
