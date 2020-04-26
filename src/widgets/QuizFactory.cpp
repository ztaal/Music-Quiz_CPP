#include "QuizFactory.hpp"

#include <vector>
#include <stdexcept>

#include <QString>

#include "common/Log.hpp"

#include "util/QuizLoader.hpp"
#include "widgets/QuizCategory.hpp"


MusicQuiz::QuizBoard* MusicQuiz::QuizFactory::createQuiz(const size_t idx, const QuizSettings& settings, const std::vector<MusicQuiz::QuizTeam*>& teams)
{
	/** Create Quiz Board */
	MusicQuiz::QuizBoard* quizBoard = nullptr;

	try {
		/** Load Categories */
		std::vector<MusicQuiz::QuizCategory*> categories = MusicQuiz::util::QuizLoader::loadQuizCategories(idx);

		/** Load Row Categories */
		std::vector< QString > rowCategories = MusicQuiz::util::QuizLoader::loadQuizRowCategories(idx);

		/** Create Quiz */
		quizBoard = new MusicQuiz::QuizBoard(categories, rowCategories, teams);
		
		/** Set Quiz Name */
		//quizBoard->setQuizName(QString::fromStdString(MusicQuiz::util::QuizLoader::getListOfQuizzes[idx]));

		return quizBoard;
	} catch ( const std::exception& err ) {
		LOG_ERROR("Failed to create quiz. " << err.what());
	} catch ( ... ) {
		LOG_ERROR("Failed to create quiz. Unknown Error.");
	}

	return nullptr;
}