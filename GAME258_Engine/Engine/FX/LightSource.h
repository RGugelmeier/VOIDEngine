#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <glm/glm.hpp>

using namespace glm;

struct LightSource
{
private:
	vec3 position;
	vec3 colour;
	float ambientVal, diffuseVal, specularVal;

public:
	LightSource();
	LightSource(vec3 position_, vec3 colour_, float ambient_, float diffuse_, float specular_);
	virtual ~LightSource();

	//Getters
	virtual inline vec3 GetPosition() const { return position; }
	virtual inline vec3 GetColour() const { return colour; }
	virtual inline float GetAmbient() const { return ambientVal; }
	virtual inline float GetDiffuse() const { return diffuseVal; }
	virtual inline float GetSpecular() const { return specularVal; }

	//Setters
	inline vec3 SetPosition(vec3 position_) { position = position_; }
	inline vec3 SetColour(vec3 colour_) { colour = colour_; }
	inline float SetAmbient(float ambientVal_) { ambientVal = ambientVal_; }
	inline float SetDiffuse(float diffuseVal_) { diffuseVal = diffuseVal_; }
	inline float SetSpecular(float specularVal_) { specularVal = specularVal_; }
};

//A light emitter that shines light on all objects at the same direction. (ex. Sun)
struct DirectionalLight : public LightSource
{
private:
	vec3 direction, colour;
	float ambient, diffuse, specular;

public:
	DirectionalLight(vec3 direction_, vec3 colour_, float ambient_, float diffuse_, float specular_);
	~DirectionalLight();

	inline vec3 GetDirection() const { return direction; }

	inline vec3 SetDirection(vec3 direction_) { direction = direction_; }
};

struct PointLight : public LightSource
{
public:
	PointLight(vec3 position_, float constant_, float linear_, float quadratic_, vec3 colour_, float ambient_, float specular_, float diffuse_);
	~PointLight();

	inline float GetConstant() const { return constant; }
	inline float GetLinear() const { return linear; }
	inline float GetQuadratic() const { return quadratic; }
private:
	vec3 position;
	float constant, linear, quadratic;
};

struct SpotLight : public LightSource
{
public:
	SpotLight(vec3 position_, vec3 direction_, float cutOff_, float outerCutOff_, float constant_, float linear_, float quadratic_, vec3 colour_, float ambientVal_, float diffuseVal_, float specularVal_);
	~SpotLight();

	inline float GetConstant() const { return constant; }
	inline float GetLinear() const { return linear; }
	inline float GetQuadratic() const { return quadratic; }
	inline float GetCutOff() const { return cutOff; }
	inline float GetOuterCutOff() const { return outerCutOff; }
	inline vec3 GetDirection() const { return direction; }
private:
	vec3 position, direction;
	float cutOff, outerCutOff, constant, linear, quadratic;
};

#endif