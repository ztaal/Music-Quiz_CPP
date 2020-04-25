#include "QuizLoader.hpp"

#include <stdexcept>

#include <boost/filesystem/fstream.hpp>


std::vector<std::string> MusicQuiz::util::QuizLoader::getListOfQuizzes()
{
	/** Check if data folder exists */
	boost::filesystem::path dataFolder = "./data/";
	if ( !boost::filesystem::is_directory(dataFolder) ) {
		throw std::runtime_error("Data folder does not exists.");
	}

	/** Find Quizzes */
	std::vector<std::string> quizList;
	boost::filesystem::recursive_directory_iterator end;
	
	for ( boost::filesystem::recursive_directory_iterator file(dataFolder); file != end; ++file ) {
		const std::string fileStr = boost::filesystem::path(*file).string();

		/** Skip non quiz files */
		if ( fileStr.find(".quiz.xml") == std::string::npos ) {
			continue;
		}

		/** Push back quiz */
		quizList.push_back(fileStr);
	}

	return quizList;
}