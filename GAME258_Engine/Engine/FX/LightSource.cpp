#include "LightSource.h"

LightSource::LightSource()
{
}

LightSource::LightSource(vec3 position_, vec3 colour_, float ambient_, float diffuse_, float specular_) : position(position_), colour(colour_), ambientVal(ambient_), diffuseVal(diffuse_), specularVal(specular_)
{
}

LightSource::~LightSource()
{
}

DirectionalLight::DirectionalLight(vec3 direction_, vec3 colour_, float ambient_, float diffuse_, float specular_) : LightSource(vec3(0.0f), colour_, ambient_, diffuse_, specular_), direction(direction_), colour(colour_), ambient(ambient_), diffuse(diffuse_), specular(specular_)
{
}

DirectionalLight::~DirectionalLight()
{
}

PointLight::PointLight(vec3 position_, float constant_, float linear_, float quadratic_, vec3 colour_, float ambient_, float specular_, float diffuse_) :
	LightSource(position_, colour_, ambient_, diffuse_, specular_), position(position_), constant(constant_), linear(linear_), quadratic(quadratic_)
{
}

PointLight::~PointLight()
{
}

SpotLight::SpotLight(vec3 position_, vec3 direction_, float cutOff_, float outerCutOff_, float constant_, float linear_, float quadratic_, vec3 colour_, float ambientVal_, float diffuseVal_, float specularVal_) :
	LightSource(position_, colour_, ambientVal_, diffuseVal_, specularVal_), position(position_), direction(direction_), cutOff(cutOff_), outerCutOff(outerCutOff_), constant(constant_),
	linear(linear_), quadratic(quadratic_)
{
}

SpotLight::~SpotLight()
{
}
