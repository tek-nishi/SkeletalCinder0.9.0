#pragma once

//
// メッシュ
//

#include <cinder/TriMesh.h>
#include <cinder/Matrix44.h>
#include <cinder/gl/VboMesh.h>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>
#include <string>
#include "common.hpp"
#include "misc.hpp"
#include "triMesh.hpp"


struct Weight {
  u_int vertex_id;
  float value;
};

struct Bone {
  std::string name;
  ci::mat4 offset;

  std::vector<Weight> weights;
};

struct Mesh {
  Mesh()
    : has_vertex_color(false),
      has_bone(false)
  {}

  TriMesh body;
  ci::gl::VboMeshRef vbo_mesh;

  u_int material_index;

  bool has_vertex_color;
  bool has_bone;

  std::vector<Bone> bones;
  std::vector<ci::mat4> bone_matrices;

  u_int shader_index;
};


// ボーンの情報を作成
Bone createBone(const aiBone* b) {
  Bone bone;
  
  bone.name = b->mName.C_Str();

  // Assimpの行列はcolmn-major
  // OpenGLはrow-major 転置が必要
  ci::mat4 m = glm::make_mat4(b->mOffsetMatrix[0]);
  bone.offset = glm::transpose(m);

  ci::app::console() << "bone:" << bone.name << " weights:" << b->mNumWeights << std::endl;

  {
    const aiVertexWeight* w = b->mWeights;
    u_int i;
    for (i = 0; i < b->mNumWeights; ++i) {
      Weight weight{ w[i].mVertexId, w[i].mWeight };
      bone.weights.push_back(weight);
    }
    // 残りはウェイト０
    for (; i < 4; ++i) {
      bone.weights.push_back({ 0, 0.0f });
    }
  }

  return bone;
}


// メッシュを生成
Mesh createMesh(const aiMesh* const m) {
  Mesh mesh;

  // VBOのレイアウトを決める
  std::vector<ci::gl::VboMesh::Layout> layout;
  
  // 頂点データを取り出す
  u_int num_vtx = m->mNumVertices;
  ci::app::console() << "Vertices:" << num_vtx << std::endl;
  layout.push_back({ ci::gl::VboMesh::Layout().interleave(true).usage(GL_STATIC_DRAW).attrib(ci::geom::Attrib::POSITION, 3) });

  const aiVector3D* vtx = m->mVertices;
  for (u_int h = 0; h < num_vtx; ++h) {
    mesh.body.appendPosition(fromAssimp(vtx[h]));
  }

  // 法線
  if (m->HasNormals()) {
    ci::app::console() << "Has Normals." << std::endl;
    layout.push_back({ ci::gl::VboMesh::Layout().interleave(true).usage(GL_STATIC_DRAW).attrib(ci::geom::Attrib::NORMAL, 3) });

    const aiVector3D* normal = m->mNormals;
    for (u_int h = 0; h < num_vtx; ++h) {
      mesh.body.appendNormal(fromAssimp(normal[h]));
    }
  }

  // テクスチャ座標(マルチテクスチャには非対応)
  if (m->HasTextureCoords(0)) {
    ci::app::console() << "Has TextureCoords." << std::endl;
    layout.push_back({ ci::gl::VboMesh::Layout().interleave(true).usage(GL_STATIC_DRAW).attrib(ci::geom::Attrib::TEX_COORD_0, 2) });

    const aiVector3D* uv = m->mTextureCoords[0];
    for (u_int h = 0; h < num_vtx; ++h) {
      mesh.body.appendTexCoord(ci::vec2(uv[h].x, uv[h].y));
    }
  }

  // 頂点カラー(マルチカラーには非対応)
  mesh.has_vertex_color = m->HasVertexColors(0);
  if (mesh.has_vertex_color) {
    ci::app::console() << "Has VertexColors." << std::endl;
    layout.push_back({ ci::gl::VboMesh::Layout().interleave(true).usage(GL_STATIC_DRAW).attrib(ci::geom::Attrib::COLOR, 4) });

    const aiColor4D* color = m->mColors[0];
    for (u_int h = 0; h < num_vtx; ++h) {
      mesh.body.appendColorRgba(fromAssimp(color[h]));
    }
  }

  // 面情報
  if (m->HasFaces()) {
    ci::app::console() << "Has Faces." << std::endl;
    const aiFace* face = m->mFaces;
    for (u_int h = 0; h < m->mNumFaces; ++h) {
      assert(face[h].mNumIndices == 3);

      const u_int* indices = face[h].mIndices;
      mesh.body.appendTriangle(indices[0], indices[1], indices[2]);
    }
  }

  // 骨情報
  mesh.has_bone = m->HasBones();
  if (mesh.has_bone) {
    ci::app::console() << "Has Bones." << std::endl;
    layout.push_back({ ci::gl::VboMesh::Layout().interleave(true).usage(GL_STATIC_DRAW).attrib(ci::geom::Attrib::BONE_INDEX, 4) });
    layout.push_back({ ci::gl::VboMesh::Layout().interleave(true).usage(GL_STATIC_DRAW).attrib(ci::geom::Attrib::BONE_WEIGHT, 4) });

    aiBone** b = m->mBones;
    for (u_int i = 0; i < m->mNumBones; ++i) {
      mesh.bones.push_back(createBone(b[i]));
    }
    mesh.bone_matrices.resize(m->mNumBones);

    std::vector<ci::ivec4> bone_indices(num_vtx);
    std::vector<ci::vec4>  bone_weights(num_vtx);
    
    for (size_t i = 0; i < mesh.bones.size(); ++i) {
      for (const auto& weight : mesh.bones[i].weights) {
        auto& bw = bone_weights[weight.vertex_id];
        for (size_t h = 0; h < 4; ++h) {
          if (bw[h] == 0.0f) {
            bw[h] = weight.value;
            bone_indices[weight.vertex_id][h] = i;
            break;
          }
        }
      }
    }
    
    // データをコピー
    mesh.body.appendBoneIndices(bone_indices);
    mesh.body.appendBoneWeights(bone_weights);
  }

  mesh.vbo_mesh = ci::gl::VboMesh::create(mesh.body);

  mesh.material_index = m->mMaterialIndex;

  return mesh;
}
