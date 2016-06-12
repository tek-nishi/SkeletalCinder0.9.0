//
// テクスチャ + ライティング
// 
$version$

uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 light_position;

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

out vec2 TexCoord0;

out vec4 Color;
out vec4 Specular;


void main(void) {
  vec4 position = ciModelViewProjection * ciPosition;
  vec3 normal   = normalize(ciNormalMatrix * ciNormal);
  vec3 light    = normalize((light_position * position.w - position * light_position.w).xyz);

  float diffuse = max(dot(light, normal), 0.0);

  // スペキュラは反射ベクトルを求める方式
  vec3 reflect   = reflect(-light, normal);
  float specular = pow(max(dot(normal, reflect), 0.0), 80.0f);
  
  gl_Position = position;

  Color    = light_diffuse  * diffuse
           + light_ambient;
  Specular = light_specular * specular;
  
  TexCoord0   = ciTexCoord0;
}
