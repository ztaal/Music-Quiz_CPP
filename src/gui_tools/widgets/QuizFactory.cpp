#include "QuizFactory.hpp"

#include <vector>
#include <string>
#include <stdexcept>

#include <QString>
#include <QMessageBox>

#include "common/Log.hpp"

#include "util/QuizLoader.hpp"
#include "gui_tools/widgets/QuizEntry.hpp"
#include "gui_tools/widgets/QuizCategory.hpp"



MusicQuiz::QuizBoard* MusicQuiz::QuizFactory::createQuiz(const std::string& quizName, const QuizSettings& settings, const audio::AudioPlayer::Ptr& audioPlayer,
	const std::vector<MusicQuiz::QuizTeam*>& teams, bool preview, QWidget* parent)
{
	/** Get List of Quizzes */
	std::vector<std::string> quizList = MusicQuiz::util::QuizLoader::getListOfQuizzes();
	if ( quizList.empty() ) {
		throw std::runtime_error("No quizzes found in the data folder.");
	}

	/** Check if Quiz Exists */
	size_t idx = 0;
	bool quizExists = false;
	for ( size_t i = 0; i < quizList.size(); ++i ) {
		std::replace(quizList[i].begin(), quizList[i].end(), '\\', '/');
		if ( quizName == quizList[i] ) {
			idx = i;
			quizExists = true;
			break;
		}
	}

	/** Sanity Check */
	if ( !quizExists ) {
		throw std::runtime_error("Quiz does not exists.");
	}

	LOG_DEBUG("QuizName = " << quizName);
	LOG_DEBUG("QuizListName = " << quizList[idx]);

	/** Create Quiz */
	createQuiz(idx, settings, audioPlayer, teams, preview, parent);
}

MusicQuiz::QuizBoard* MusicQuiz::QuizFactory::createQuiz(const size_t idx, const QuizSettings& settings, const audio::AudioPlayer::Ptr& audioPlayer,
	const std::vector<MusicQuiz::QuizTeam*>& teams, bool preview, QWidget* parent)
{
	/** Create Quiz Board */
	MusicQuiz::QuizBoard* quizBoard = nullptr;

	try {
		/** Load Categories */
		std::string loadError;
		std::vector<MusicQuiz::QuizCategory*> categories = MusicQuiz::util::QuizLoader::loadQuizCategories(idx, audioPlayer, loadError);
		if ( !loadError.empty() ) {
			QMessageBox::information(nullptr, "Info", "Incomplete Quiz:\n\n" + QString::fromStdString(loadError));
		}

		/** Load Row Categories */
		std::vector< QString > rowCategories = MusicQuiz::util::QuizLoader::loadQuizRowCategories(idx);

		/** Apply Settings */

		/** Hidden Team Score */
		if ( settings.hiddenTeamScore ) {
			for ( size_t i = 0; i < teams.size(); ++i ) {
				teams[i]->setHideScore(true);
			}
		}

		/** Hidden Answers */
		if ( settings.hiddenAnswers ) {
			for ( size_t i = 0; i < categories.size(); ++i ) {
				for ( size_t j = 0; j < categories[i]->size(); ++j ) {
					MusicQuiz::QuizEntry* quizEntry = (*categories[i])[j];
					if ( quizEntry != nullptr ) {
						quizEntry->setHiddenAnswer(true);
					}
				}
			}
		}

		/** Create Quiz */
		quizBoard = new MusicQuiz::QuizBoard(categories, rowCategories, teams, settings, preview, parent);
		
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