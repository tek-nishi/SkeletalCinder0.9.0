#pragma once

//
// トライアングルメッシュ
//   スキニングの情報を含むのを独自定義
//

#include <cinder/GeomIo.h>
#include <vector>


#if defined (CINDER_COCOA_TOUCH)
// FIXME:ivec4をシェーダーに送れない(原因調査中)
using index_t   = ci::vec4;
using element_t = uint16_t;
#else
using index_t   = ci::ivec4;
using element_t = uint32_t;
#endif


class TriMesh : public ci::geom::Source {
  std::vector<ci::vec3> positions;
  std::vector<ci::vec3> normals;
  std::vector<ci::vec2> uvs;
  std::vector<ci::ColorA> colors;

  std::vector<uint32_t> indices;
  
  std::vector<index_t>  bone_indices;
  std::vector<ci::vec4> bone_weights;
  

public:
  void appendPosition(const ci::vec3& value) {
    positions.push_back(value);
  }

  void appendNormal(const ci::vec3& value) {
    normals.push_back(value);
  }

  void appendTexCoord(const ci::vec2& value) {
    uvs.push_back(value);
  }

  void appendColorRgba(const ci::ColorA& value) {
    colors.push_back(value);
  }

  void appendTriangle(const uint32_t t1, const uint32_t t2, const uint32_t t3) {
    indices.push_back(t1);
    indices.push_back(t2);
    indices.push_back(t3);
  }

  void appendBoneIndices(const std::vector<ci::ivec4>& values) {
#if defined (CINDER_COCOA_TOUCH)
    for (const auto& v : values) {
      bone_indices.push_back(ci::vec4(v));
    }
#else
    bone_indices = values;
#endif
  }

  void appendBoneWeights(const std::vector<ci::vec4>& values) {
    bone_weights = values;
  }
  

  size_t getNumVertices() const {
    return positions.size();
  }
  
	size_t getNumIndices() const {
    return indices.size();
  }

  
  const std::vector<ci::vec3>& getPositions() const {
    return positions;
  }

  const std::vector<ci::vec3>& getNormals() const {
    return normals;
  }

  
  ci::geom::Primitive	getPrimitive() const {
    return ci::geom::Primitive::TRIANGLES;
  }
  
	uint8_t	getAttribDims(ci::geom::Attrib attr) const {
    switch (attr) {
    case ci::geom::Attrib::POSITION:
      return 3;

    case ci::geom::Attrib::COLOR:
      return 4;

    case ci::geom::Attrib::TEX_COORD_0:
      return 2;

    case ci::geom::Attrib::NORMAL:
      return 3;

    case ci::geom::Attrib::BONE_INDEX:
      return 4;
      
    case ci::geom::Attrib::BONE_WEIGHT:
      return 4;

    default:
      return 0;
    }      
  }
  
  ci::geom::AttribSet	getAvailableAttribs() const {
    ci::geom::AttribSet attribs;

    if (!positions.empty()) attribs.insert(ci::geom::Attrib::POSITION);
    if (!normals.empty())   attribs.insert(ci::geom::Attrib::NORMAL);
    if (!uvs.empty())       attribs.insert(ci::geom::Attrib::TEX_COORD_0);
    if (!colors.empty())    attribs.insert(ci::geom::Attrib::COLOR);

    if (!bone_indices.empty()) attribs.insert(ci::geom::Attrib::BONE_INDEX);
    if (!bone_weights.empty()) attribs.insert(ci::geom::Attrib::BONE_WEIGHT);
    
    return attribs;
  }

  const void* getAttribPointer(const ci::geom::Attrib attr) const {
    switch (attr) {
    case ci::geom::Attrib::POSITION:
      return static_cast<const void*>(&positions[0]);

    case ci::geom::Attrib::COLOR:
      return static_cast<const void*>(&colors[0]);

    case ci::geom::Attrib::TEX_COORD_0:
      return static_cast<const void*>(&uvs[0]);

    case ci::geom::Attrib::NORMAL:
      return static_cast<const void*>(&normals[0]);

    case ci::geom::Attrib::BONE_INDEX:
      return static_cast<const void*>(&bone_indices[0]);
      
    case ci::geom::Attrib::BONE_WEIGHT:
      return static_cast<const void*>(&bone_weights[0]);

    default:
      return nullptr;
    }      
  }
  
	void loadInto(ci::geom::Target* target, const ci::geom::AttribSet& requestedAttribs) const {
    for (auto &attrib : requestedAttribs) {
      size_t dims = getAttribDims(attrib);
      if (dims) {
        const void* pointer = getAttribPointer(attrib);
        if(pointer) target->copyAttrib(attrib, dims, 0, static_cast<const float*>(pointer), getNumVertices());
      }
    }

    if (getNumIndices()) {
      target->copyIndices(getPrimitive(), &indices[0], getNumIndices(), sizeof(element_t));
    }
  }

  ci::geom::Source*	clone() const {
    return new TriMesh(*this);
  }
};
