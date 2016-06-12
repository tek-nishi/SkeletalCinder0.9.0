#pragma once

//
// 光源
//

struct Light {
  ci::ColorAf ambient;
  ci::ColorAf diffuse;
  ci::ColorAf specular;

  ci::vec4 position;
  
  
  void setAmbient(const ci::Color& aAmbient) {
    ambient = aAmbient;
  }
 
  void setDiffuse(const ci::Color& aDiffuse) {
    diffuse = aDiffuse;
  }
 
  void 	setSpecular (const ci::Color& aSpecular) {
    specular = ci::ColorAf(aSpecular, 0.0f);
  }

  void setDirection(const ci::vec3& aDirection) {
    position = ci::vec4(aDirection, 0.0f);
  }
  
};
