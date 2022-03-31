#include "Mesh.h"

//Set default values and generate buffers.
Mesh::Mesh(SubMesh& subMesh_, GLuint shaderProgram_) : VAO(0), VBO(0), shaderProgram(0), modelLocation(0), viewLocation(0), projectionLocation(0), diffuseMapLoc(0)
{
	subMesh = subMesh_;
	shaderProgram = shaderProgram_;
	GenerateBuffers();
}

//Delete vertex buffer and array (VAO + VBO), then clear the vertex list and mesh indicies lists of all sub meshes.
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	if (subMesh.vertexList.size() > 0)
	{
		subMesh.vertexList.clear();
	}

	if (subMesh.meshIndicies.size() > 0)
	{
		subMesh.meshIndicies.clear();
	}
}

//This function renders the mesh. First we bind the VAO to be drawn, set the model transformation matrix, then call the GL function to draw it by giving the function the draw type, starting position, and the end of the array.
// Then unbind the vertex array because we are no longer using it.
void Mesh::Render(Camera* camera_, vector<mat4>& instances_)
{
	directionalLights = camera_->GetDirectionalLights();
	pointLights = camera_->GetPointLights();
	spotLights = camera_->GetSpotLights();

	//Set the parameters for the texture going onto this mesh.
	glUniform1i(diffuseMapLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseMap);

	//Set values of the uniforms.
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(camera_->GetPerspective()));
	glUniform3fv(viewPos, 1, value_ptr(camera_->GetPosition()));

	glUniform3fv(ambientLoc, 1, value_ptr(subMesh.material.ambient));
	glUniform3fv(specularLoc, 1, value_ptr(subMesh.material.specular));
	glUniform3fv(diffuseLoc, 1, value_ptr(subMesh.material.diffuse));
	glUniform1f(shininessLoc, subMesh.material.shininess);
	glUniform1f(transparencyLoc, subMesh.material.transparency);
	glUniform1i(diffuseMapLoc, 0);

	for (auto light : camera_->GetLights())
	{
		if (dynamic_cast<DirectionalLight*>(light))
		{
			directionalLights.push_back(static_cast<DirectionalLight*>(light));
		}
		else if (dynamic_cast<PointLight*>(light))
		{
			pointLights.push_back(static_cast<PointLight*>(light));
		}
		else if (dynamic_cast<SpotLight*>(light))
		{
			spotLights.push_back(static_cast<SpotLight*>(light));
		}
	}

	//Bind directional light uniforms to values.
	for (int i = 0; i < directionalLights.size(); i++)
	{
		glUniform3fv(dirLightDir[i], 1, value_ptr(camera_->GetDirectionalLights()[i]->GetDirection()));
		glUniform1f(dirLightAmbientVal[i], camera_->GetDirectionalLights()[i]->GetAmbient());
		glUniform1f(dirLightSpecularVal[i], camera_->GetDirectionalLights()[i]->GetSpecular());
		glUniform1f(dirLightDiffuseVal[i], camera_->GetDirectionalLights()[i]->GetDiffuse());
		glUniform3fv(dirLightColour[i], 1, value_ptr(camera_->GetDirectionalLights()[i]->GetColour()));
	}
	//Bind point light uniforms to values.
	for (int i = 0; i < pointLights.size(); i++)
	{
		glUniform3fv(pointLightPos[i], 1, value_ptr(camera_->GetPointLights()[i]->GetPosition()));
		glUniform1f(pointLightAmbientVal[i], camera_->GetPointLights()[i]->GetAmbient());
		glUniform1f(pointLightSpecularVal[i], camera_->GetPointLights()[i]->GetSpecular());
		glUniform1f(pointLightDiffuseVal[i], camera_->GetPointLights()[i]->GetDiffuse());
		glUniform3fv(pointLightColour[i], 1, value_ptr(camera_->GetPointLights()[i]->GetColour()));
		glUniform1f(pointLightConstant[i], camera_->GetPointLights()[i]->GetConstant());
		glUniform1f(pointLightLinear[i], camera_->GetPointLights()[i]->GetLinear());
		glUniform1f(pointLightQuadratic[i], camera_->GetPointLights()[i]->GetQuadratic());
	}
	//Bind spot light uniforms to values.
	for (int i = 0; i < spotLights.size(); i++)
	{
		glUniform3fv(spotLightPos[i], 1, value_ptr(camera_->GetSpotLights()[i]->GetPosition()));
		glUniform3fv(spotLightDir[i], 1, value_ptr(camera_->GetSpotLights()[i]->GetDirection()));
		glUniform1f(spotLightAmbient[i], camera_->GetSpotLights()[i]->GetAmbient());
		glUniform1f(spotLightSpecular[i], camera_->GetSpotLights()[i]->GetSpecular());
		glUniform1f(spotLightDiffuse[i], camera_->GetSpotLights()[i]->GetDiffuse());
		glUniform3fv(spotLightColour[i], 1, value_ptr(camera_->GetSpotLights()[i]->GetColour()));
		glUniform1f(spotLightConstant[i], camera_->GetSpotLights()[i]->GetConstant());
		glUniform1f(spotLightLinear[i], camera_->GetSpotLights()[i]->GetLinear());
		glUniform1f(spotLightQuadratic[i], camera_->GetSpotLights()[i]->GetQuadratic());
		glUniform1f(spotLightCutOff[i], camera_->GetSpotLights()[i]->GetCutOff());
		glUniform1f(spotLightOuterCutOff[i], camera_->GetSpotLights()[i]->GetOuterCutOff());
	}

	//for (int i = 0; i < camera_->GetLights().size(); i++)
	//{
	//	glUniform3fv(lightPos[i], 1, value_ptr(camera_->GetLights()[i]->GetPosition()));
	//	glUniform1f(lightAmbientVal[i], camera_->GetLights()[i]->GetAmbient());
	//	glUniform1f(lightDiffuseVal[i], camera_->GetLights()[i]->GetDiffuse());
	//	glUniform1f(lightSpecularVal[i], camera_->GetLights()[i]->GetSpecular());
	//	glUniform3fv(lightColour[i], 1, value_ptr(camera_->GetLights()[i]->GetColour()));
	//}

	glBindVertexArray(VAO);

	//Enable GL depth
	glEnable(GL_DEPTH_TEST);

	//Set and draw each model for each mesh instance.
	for (int i = 0; i < instances_.size(); i++)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(instances_[i]));
		glDrawArrays(GL_TRIANGLES, 0, subMesh.vertexList.size());
	}

	//Unbind things.
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::SkyboxRender(Camera* camera_)
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, subMesh.vertexList.size());
	glBindVertexArray(0);
}

//This function will generate the buffers for the mesh.
void Mesh::GenerateBuffers()
{
	//First, create the vertex array and vertex buffer by passing in a reference to the VAO and VBO to the generation functions.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Second, bind the previously created vertex array and vertex buffer to be used by passing them into the bind functions.
	//Binding sets the bound objects to be used with OpenGL.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Third, set the data to buffer. This will take in the type of buffer, the size of the object that is being added to the buffer, the first object of the array, and the draw type.
	//If this gives an error, check to make sure the vertexList is actually filled in, as this will break if the list is empty.
	glBufferData(GL_ARRAY_BUFFER, subMesh.vertexList.size() * sizeof(Vertex), &subMesh.vertexList[0], GL_STATIC_DRAW);

	/* Fourth, set the attributes of the vertices by selecting what attribute to set, then setting how many values of which type this attribute
	has (ex. the position has 3 for x, y, and z), then set if the value should be normalized, then set the size of the jump needed to get from
	one beginning of a value to the next, and set the starting point of the attribute in the list. */

	//POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//NORMAL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	//TEXTURE COORDINATES
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoordinates));

	//Last, unbind the vertex array and buffer because we have already set all of it's attributes and no longer need to work on it.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Get the uniform locations. This is so we can change the values later on, allowing us to change the camera's view, and any model's size, rotation, or position, as well as edit the material.
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	viewLocation = glGetUniformLocation(shaderProgram, "view");
	projectionLocation = glGetUniformLocation(shaderProgram, "projection");

	diffuseMapLoc = glGetUniformLocation(shaderProgram, "material.diffuseMap");
	shininessLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	transparencyLoc = glGetUniformLocation(shaderProgram, "material.transparency");
	ambientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
	diffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
	specularLoc = glGetUniformLocation(shaderProgram, "material.specular");

	viewPos = glGetUniformLocation(shaderProgram, "cameraPosition");

	//Set up directional light buffers
	for (GLuint i = 0; i < 10; i++)
	{
		string index = to_string(i);
		dirLightDir[i] = glGetUniformLocation(shaderProgram, ("dirLights[" + index + "].direction").c_str());
		dirLightAmbientVal[i] = glGetUniformLocation(shaderProgram, ("dirLights[" + index + "].ambientVal").c_str());
		dirLightSpecularVal[i] = glGetUniformLocation(shaderProgram, ("dirLights[" + index + "].specularVal").c_str());
		dirLightDiffuseVal[i] = glGetUniformLocation(shaderProgram, ("dirLights[" + index + "].diffuseVal").c_str());
		dirLightColour[i] = glGetUniformLocation(shaderProgram, ("dirLights[" + index + "].lightColour").c_str());
	}

	//Set up point light buffers
	for (GLuint i = 0; i < 10; i++)
	{
		string index = to_string(i);
		pointLightPos[i] = glGetUniformLocation(shaderProgram, ("pointLights[" + index + "].position").c_str());
		pointLightAmbientVal[i] = glGetUniformLocation(shaderProgram, ("pointLights[" + index + "].ambientVal").c_str());
		pointLightSpecularVal[i] = glGetUniformLocation(shaderProgram, ("pointLights[" + index + "].specularVal").c_str());
		pointLightDiffuseVal[i] = glGetUniformLocation(shaderProgram, ("pointLights[" + index + "].diffuseVal").c_str());
		pointLightColour[i] = glGetUniformLocation(shaderProgram, ("pointLights[" + index + "].lightColour").c_str());
		pointLightConstant[i] = glGetUniformLocation(shaderProgram, ("pointLights[" + index + "].constant").c_str());
		pointLightLinear[i] = glGetUniformLocation(shaderProgram, ("pointLights[" + index + "].linear").c_str());
		pointLightQuadratic[i] = glGetUniformLocation(shaderProgram, ("pointLights[" + index + "].quadratic").c_str());
	}

	//Set up spot light buffers
	for (GLuint i = 0; i < 10; i++)
	{
		string index = to_string(i);
		spotLightPos[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].position").c_str());
		spotLightDir[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].direction").c_str());
		spotLightAmbient[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].ambientVal").c_str());
		spotLightSpecular[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].specularVal").c_str());
		spotLightDiffuse[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].diffuseVal").c_str());
		spotLightColour[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].lightColour").c_str());
		spotLightConstant[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].constant").c_str());
		spotLightLinear[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].linear").c_str());
		spotLightQuadratic[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].quadratic").c_str());
		spotLightCutOff[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].cutOff").c_str());
		spotLightOuterCutOff[i] = glGetUniformLocation(shaderProgram, ("spotLights[" + index + "].outerCutOff").c_str());
	}
}
