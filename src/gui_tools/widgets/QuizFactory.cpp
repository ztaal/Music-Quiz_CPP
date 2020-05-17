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


MusicQuiz::QuizBoard* MusicQuiz::QuizFactory::createQuiz(const size_t idx, const QuizSettings& settings, const audio::AudioPlayer::Ptr& audioPlayer, const std::vector<MusicQuiz::QuizTeam*>& teams)
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
		quizBoard = new MusicQuiz::QuizBoard(categories, rowCategories, teams, settings);
		
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