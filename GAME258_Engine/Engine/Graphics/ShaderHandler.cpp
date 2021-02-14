#include "ShaderHandler.h"

//Set default values of, and redeclare static variables.
unique_ptr<ShaderHandler> ShaderHandler::shaderInstance = nullptr;
unordered_map<string, GLuint> ShaderHandler::programs = unordered_map<string, GLuint>();

ShaderHandler::ShaderHandler()
{

}

ShaderHandler::~ShaderHandler()
{
	OnDestroy();
}

//Reads the passed in file at the file path and outputs it as a string.
string ShaderHandler::ReadShader(const string& filePath_)
{
	string shaderCode = "";
	ifstream file;

	//Check if something is wrong with the file.
	file.exceptions(ifstream::badbit);

	//Try to open the file and read it into a string, then close the file and set the string to return to be what we just read.
	try
	{
		file.open(filePath_);
		stringstream tmpStream;
		tmpStream << file.rdbuf();
		file.close();
		shaderCode = tmpStream.str();
	}
	//If something goes wrong, print an error and return empty string.
	catch (ifstream::failure error_)
	{
		Debug::Error("Could not read the shader: " + filePath_, "ShaderHandler.cpp", __LINE__);
		return "";
	}

	return shaderCode;
}

//Creates a shader object of the given type.
GLuint ShaderHandler::CreateShader(GLenum shaderType_, const string& source_, const string& shaderName_)
{
	//Create the shader of given type to the shader variable.
	GLint compileResult = 0;
	GLuint shader = glCreateShader(shaderType_);

	//Convert the source string to a char star and store it's size because OpenGl needs to know it in other functions.
	const char* shaderCodePtr = source_.c_str();
	const int shaderCodeSize = source_.size();

	//Attempt to compile the shader.
	glShaderSource(shader, 1, &shaderCodePtr, &shaderCodeSize);
	glCompileShader(shader);

	//Gets the compile status.
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	//If the shader did not compile properly, get the length of the information that we will be given on what went wrong. Then create a vector with that size and input the error into that vector.
	//Then print the error to debug and return 0. This is good for debugging shaders.
	if (!compileResult)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		vector<char> shaderLog(infoLogLength);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]);
		string shaderString(shaderLog.begin(), shaderLog.end());
		Debug::Error("Error compiling shader: " + shaderName_ + ". Error: \n" + shaderString, "ShaderHandler.cpp", __LINE__);

		return 0;
	}

	//Return the created shader.
	return shader;
}

//Gets the instance of the shader handler if there is one. If there is not, create and set one.
ShaderHandler* ShaderHandler::GetInstance()
{
	if (shaderInstance.get() == nullptr)
	{
		shaderInstance.reset(new ShaderHandler);
	}
	return shaderInstance.get();
}

//Creates the program for the vertex and fragment shader.
void ShaderHandler::CreateProgram(const string& shaderName_, const string& vertexShaderFileName_, const string& fragmentShaderFileName_)
{
	//Get the shaders read into variables.
	string vertexShaderCode = ReadShader(vertexShaderFileName_);
	string fragmentShaderCode = ReadShader(fragmentShaderFileName_);

	//Return if either string is empty.
	if (vertexShaderCode == "" || fragmentShaderCode == "")
	{
		return;
	}

	//Create the shaders into variables.
	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, shaderName_);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, shaderName_);

	//Return if either shader failed to create.
	if (vertexShader == 0 || fragmentShader == 0)
	{
		return;
	}

	//Create the program and attempt to attach the vertex and fragment shader to the program.
	GLint linkResult = 0;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//Attempt to link the program.
	glLinkProgram(program);

	//Get the link status
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

	//If the shaders did not link properly, get the length of the information that we will be given on what went wrong. Then create a vector with that size and input the error into that vector.
	//Then print the error to debug, delete the previously created shaders and program, and return. This is good for debugging shaders.
	if (!linkResult)
	{
		GLint infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		vector<char> programLog(infoLogLength);
		glGetProgramInfoLog(program, infoLogLength, NULL, &programLog[0]);
		string programString(programLog.begin(), programLog.end());
		Debug::Error("Failed to link shader: " + shaderName_ + ". Error: " + programString, "ShaderHandler.cpp", __LINE__);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);

		return;
	}

	//Link the newly linked program in the programs map to the shader that was passed in when calling this function.
	programs[shaderName_] = program;

	//Delete the shaders as they have already been linked and all set up to the program.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

//Gets the provided shader's address, if it exists in the map. Otherwise return 0.
GLuint ShaderHandler::GetShader(const string& shaderName_)
{
	if (programs.find(shaderName_) != programs.end())
	{
		return programs[shaderName_];
	}
	return 0;
}

//Go through and delete all programs in the programs map. First delete all values, then clear the keys.
void ShaderHandler::OnDestroy()
{
	if (programs.size() > 0)
	{
		for (auto p : programs)
		{
			glDeleteProgram(p.second);
		}
		programs.clear();
	}
}
