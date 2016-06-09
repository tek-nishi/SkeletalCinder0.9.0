#pragma once

//
// ノード
//

#include "mesh.hpp"
#include <glm/gtc/type_ptr.hpp>


struct Node {
  std::string name;

  std::vector<Mesh> mesh;

  ci::mat4 matrix;
  ci::mat4 matrix_orig;
  ci::mat4 global_matrix;
  ci::mat4 invert_matrix;

  std::vector<std::shared_ptr<Node> > children;
};


// 再帰で子供のノードも生成
std::shared_ptr<Node> createNode(const aiNode* const n, aiMesh** mesh) {
  auto node = std::make_shared<Node>();

  node->name = n->mName.C_Str();

  ci::app::console() << "Node:" << node->name << std::endl;

  for (u_int i = 0; i < n->mNumMeshes; ++i) {
    node->mesh.push_back(createMesh(mesh[n->mMeshes[i]]));
  }

  // Assimpの行列はcolmn-major
  // OpenGLはrow-major 転置が必要
  ci::mat4 m = glm::make_mat4(n->mTransformation[0]);
  node->matrix = glm::transpose(m);

  // 初期値を保存しておく
  node->matrix_orig = node->matrix;

  for (u_int i = 0; i < n->mNumChildren; ++i) {
    node->children.push_back(createNode(n->mChildren[i], mesh));
  }

  return node;
}

// 再帰を使って全ノード情報を生成
void createNodeInfo(const std::shared_ptr<Node>& node,
                    std::map<std::string, std::shared_ptr<Node> >& node_index,
                    std::vector<std::shared_ptr<Node> >& node_list) {

  node_index.insert(std::make_pair(node->name, node));
  node_list.push_back(node);

  for (auto child : node->children) {
    createNodeInfo(child, node_index, node_list);
  }
}


// 全ノードの親行列適用済み行列と、その逆行列を計算
//   メッシュアニメーションで利用
void updateNodeDerivedMatrix(const std::shared_ptr<Node>& node,
                             const ci::mat4& parent_matrix) {
  node->global_matrix = parent_matrix * node->matrix;
  node->invert_matrix = ci::inverse(node->global_matrix);

  for (auto child : node->children) {
    updateNodeDerivedMatrix(child, node->global_matrix);
  }
}
