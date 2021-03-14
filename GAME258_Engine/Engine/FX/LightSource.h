#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <glm/glm.hpp>

using namespace glm;

class LightSource
{
private:
	vec3 position;
	vec3 colour;
	float ambientVal, diffuseVal, specularVal;

public:
	LightSource(vec3 position_, vec3 colour_, float ambient_, float diffuse_, float specular_);
	~LightSource();

	//Getters
	inline vec3 GetPosition() const { return position; }
	inline vec3 GetColour() const { return colour; }
	inline float GetAmbient() const { return ambientVal; }
	inline float GetDiffuse() const { return diffuseVal; }
	inline float GetSpecular() const { return specularVal; }

	//Setters
	inline vec3 SetPosition(vec3 position_) { position = position_; }
	inline vec3 SetColour(vec3 colour_) { colour = colour_; }
	inline float SetAmbient(float ambientVal_) { ambientVal = ambientVal_; }
	inline float SetDiffuse(float diffuseVal_) { diffuseVal = diffuseVal_; }
	inline float SetSpecular(float specularVal_) { specularVal = specularVal_; }
};

#endif