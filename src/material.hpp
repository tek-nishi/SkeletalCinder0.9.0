#pragma once

//
// マテリアル
//

#include <assimp/scene.h>
#include <string>
#include "common.hpp"
#include "misc.hpp"


struct Material {
  Material()
    : diffuse(0.6f, 0.6f, 0.6f, 1.0f),
      ambient(0.0f, 0.0f, 0.0f, 1.0f),
      specular(0.0f, 0.0f, 0.0f, 1.0f),
      shininess(80.0f),
      emission(0.0f, 0.0f, 0.0f, 1.0f),
      has_texture(false)
  { }

  ci::ColorA diffuse;
  ci::ColorA ambient;
  ci::ColorA specular;
  float shininess;
  ci::ColorA emission;
  
  bool has_texture;
  std::string texture_name;
  GLenum wrap_s, wrap_t;
};


GLenum getTextureWrap(const int wrap) {
  switch (wrap) {
  // case aiTextureMapMode_Wrap:   return GL_REPEAT;
  case aiTextureMapMode_Clamp:  return GL_CLAMP_TO_EDGE;
  // case aiTextureMapMode_Decal:  return ;
  // case aiTextureMapMode_Mirror: return ;
  default:                      return GL_REPEAT;
  }
}


// マテリアルを生成
Material createMaterial(const aiMaterial* const mat) {
  Material material;

  // TODO:assimpのエラーをチェックする
  {
    aiColor3D color;
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.diffuse = fromAssimp(color);
  }

  {
    aiColor3D color;
    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ambient = fromAssimp(color);
  }

  {
    aiColor3D color;
    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.specular = fromAssimp(color);
  }

  mat->Get(AI_MATKEY_SHININESS, material.shininess);
  // TIPS:シェーダー側で「0の0乗」を計算しないようにしている
  material.shininess = std::max(std::numeric_limits<float>::epsilon(),
                                material.shininess);

  {
    aiColor3D color;
    mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
    material.emission = fromAssimp(color);
  }

  aiString tex_name;
  if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), tex_name) == AI_SUCCESS) {
    material.has_texture  = true;
    material.texture_name = getFilename(std::string(tex_name.C_Str()));

    int map_u = aiTextureMapMode_Wrap;
    mat->Get(AI_MATKEY_MAPPINGMODE_U(aiTextureType_DIFFUSE, 0), map_u);
    material.wrap_s = getTextureWrap(map_u);
    
    int map_v = aiTextureMapMode_Wrap;
    mat->Get(AI_MATKEY_MAPPINGMODE_V(aiTextureType_DIFFUSE, 0), map_v);
    material.wrap_t = getTextureWrap(map_v);
  }

  ci::app::console() << "Diffuse:"   << material.diffuse   << std::endl;
  ci::app::console() << "Ambient:"   << material.ambient   << std::endl;
  ci::app::console() << "Specular:"  << material.specular  << std::endl;
  ci::app::console() << "Shininess:" << material.shininess << std::endl;
  ci::app::console() << "Emission:"  << material.emission  << std::endl;
  if (material.has_texture) {
    ci::app::console() << "Texture:" << material.texture_name << std::endl;
  }

  return material;
}
