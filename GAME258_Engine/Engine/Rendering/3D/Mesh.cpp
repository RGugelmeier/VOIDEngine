#include "Mesh.h"

//Set default values and generate buffers.
Mesh::Mesh(SubMesh& subMesh_, GLuint shaderProgram_) : VAO(0), VBO(0), shaderProgram(0), modelLocation(0), viewLocation(0), projectionLocation(0), textureLocation(0)
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
	//Set the parameters for the texture going onto this mesh.
	glUniform1i(textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, subMesh.textureID);

	//Set values of the uniforms.
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(camera_->GetPerspective()));
	glUniform3fv(viewPos, 1, value_ptr(camera_->GetPosition()));

	glUniform3fv(lightPos, 1, value_ptr(camera_->GetLights()[0]->GetPosition()));
	glUniform1f(lightAmbientVal, camera_->GetLights()[0]->GetAmbient());
	glUniform1f(lightDiffuseVal, camera_->GetLights()[0]->GetDiffuse());
	glUniform1f(lightSpecularVal, camera_->GetLights()[0]->GetSpecular());
	glUniform3fv(lightColour, 1, value_ptr(camera_->GetLights()[0]->GetColour()));

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

	//COLOUR
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, colour));

	//Last, unbind the vertex array and buffer because we have already set all of it's attributes and no longer need to work on it.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Get the uniform locations. This is so we can change the values later on, allowing us to change the camera's view, and any model's size, rotation, or position, as well as edit the texture.
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	viewLocation = glGetUniformLocation(shaderProgram, "view");
	projectionLocation = glGetUniformLocation(shaderProgram, "projection");
	textureLocation = glGetUniformLocation(shaderProgram, "inputTexture");

	viewPos = glGetUniformLocation(shaderProgram, "cameraPosition");
	lightPos = glGetUniformLocation(shaderProgram, "light.position");
	lightAmbientVal = glGetUniformLocation(shaderProgram, "light.ambientVal");
	lightSpecularVal = glGetUniformLocation(shaderProgram, "light.specularVal");
	lightDiffuseVal = glGetUniformLocation(shaderProgram, "light.diffuseVal");
	lightColour = glGetUniformLocation(shaderProgram, "light.lightColour");
}
