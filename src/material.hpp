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
    : has_texture(false)
  { }

  ci::ColorA diffuse;
  ci::ColorA ambient;
  ci::ColorA specular;
  float shininess;
  ci::ColorA emission;
  
  
  bool has_texture;
  std::string texture_name;
};


// マテリアルを生成
Material createMaterial(const aiMaterial* const mat) {
  Material material;

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

  {
    aiColor3D color;
    mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
    material.emission = fromAssimp(color);
  }

  aiString tex_name;
  if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), tex_name) == AI_SUCCESS) {
    material.has_texture  = true;
    material.texture_name = getFilename(std::string(tex_name.C_Str()));
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
