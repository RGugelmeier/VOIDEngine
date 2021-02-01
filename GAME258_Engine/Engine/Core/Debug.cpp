#include "Debug.h"

//Redeclare static member variables.
//Set default message type to none.
//Set default output name to an empty string.
Debug::MessageType Debug::currentSev = MessageType::TYPE_NONE;
std::string Debug::outputName = "";

/*Create or replace the output file with the given name (or the default name in the header file if no other name is given.
  Then open and close a file to createw the file or replace it if it already exists.
  Then set the currentSev variable to use the message type enum class. */ 
void Debug::OnCreate(const std::string& name_)
{
	outputName = name_ + ".txt";
	std::ofstream outputFile;
	outputFile.open(outputName.c_str(), std::ios::out);
	outputFile.close();
	currentSev = MessageType::TYPE_INFO;
}

//Sets the severity of the message that will be outputed.
void Debug::SetSeverity(MessageType type_)
{
	currentSev = type_;
}

//Log for an info type message.
void Debug::Info(const std::string& message_, const std::string& filename_, const int line_)
{
	Log(MessageType::TYPE_INFO, "[INFO]: " + message_, filename_, line_);
}

//Log for a trace type message.
void Debug::Trace(const std::string& message_, const std::string& filename_, const int line_)
{
	Log(MessageType::TYPE_TRACE, "[TRACE]: " + message_, filename_, line_);
}

//Log for a warning type message.
void Debug::Warning(const std::string& message_, const std::string& filename_, const int line_)
{
	Log(MessageType::TYPE_WARNING, "[WARNING]: " + message_, filename_, line_);
}

//Log for an error type message.
void Debug::Error(const std::string& message_, const std::string& filename_, const int line_)
{
	Log(MessageType::TYPE_ERROR, "[ERROR]: " + message_, filename_, line_);
}

//Log for a fatal error type message.
void Debug::FatalError(const std::string& message_, const std::string& filename_, const int line_)
{
	Log(MessageType::TYPE_FATAL_ERROR, "[FATAL ERROR]: " + message_, filename_, line_);
}

/*This function logs messages to the output file. It checks the error type against whatever type you specify so it can filter out some logs if you would like.
  It then sends the error message, what file the error is in, and what line it is on to the end of the output file.
  Then, it empties the output buffer and closes the file */
void Debug::Log(const MessageType type_, const std::string& message_, const std::string& fileName_, const int line_)
{
	if (type_ <= currentSev && currentSev > MessageType::TYPE_NONE)
	{
		std::ofstream outputFile;
		outputFile.open(outputName.c_str(), std::ios::app | std::ios::out);
		outputFile << message_ << " in : " << fileName_ << " on line: " << line_ << std::endl;
		outputFile.flush();
		outputFile.close();
	}
}
