#include "LightSource.h"

LightSource::LightSource(vec3 position_, vec3 colour_, float ambient_, float diffuse_, float specular_) : position(position_), colour(colour_), ambientVal(ambient_), diffuseVal(diffuse_), specularVal(specular_)
{
}

LightSource::~LightSource()
{
}
