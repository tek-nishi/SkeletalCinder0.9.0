//
// カラー + ライティング
//
$version$

uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

// ライト
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 light_position;

// マテリアル
uniform vec4  mat_ambient;
uniform vec4  mat_diffuse;
uniform vec4  mat_specular;
uniform float mat_shininess;
uniform vec4  mat_emission;

in vec4 ciPosition;
in vec3 ciNormal;

out vec4 Color;


void main(void) {
  vec4 position = ciModelViewProjection * ciPosition;
  vec3 normal   = normalize(ciNormalMatrix * ciNormal);
  vec3 light    = normalize((light_position * position.w - position * light_position.w).xyz);

  float diffuse = max(dot(light, normal), 0.0);

  // スペキュラは反射ベクトルを求める方式
  vec3 reflect   = reflect(-light, normal);
  float specular = pow(max(dot(normal, reflect), 0.0), mat_shininess);
  
  gl_Position = position;

  Color = mat_diffuse * light_diffuse * diffuse
        + mat_specular * light_specular * specular
        + mat_ambient * light_ambient
        + mat_emission;
}
