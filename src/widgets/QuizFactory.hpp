#pragma once

#include <string>

#include "widgets/QuizTeam.hpp"
#include "widgets/QuizBoard.hpp"


namespace MusicQuiz {
	class QuizFactory
{
	public:
		struct QuizSettings
		{
			/** Daily Double */
			bool dailyDouble = false;
			bool dailyDoubleHidden = false;
			bool dailyDoublePercentage = 0;

			/** Daily Trible */
			bool dailyTriple = false;
			bool dailyTripleHidden = false;
			bool dailyTriplePercentage = 0;

			/** Teams */
			bool hiddenTeamScore = false;

			/** Answers */
			bool hiddenAnswers = false;

			/** Guess the category */
			bool guessTheCategory = false;
			size_t pointsPerCategory = 500;
		};

		/**
		 * @brief Deleted constructor.
		 */
		QuizFactory() = delete;

		/**
		* @brief Deleted Destructor.
		*/
		~QuizFactory() = delete;

		/**
		 * @brief Creates the music quiz.
		 *
		 * @param[in] idx The quiz index to load.
		 * @param[in] settings The quiz settings.
		 * @param[in] teams The teams list.
		 */
		static MusicQuiz::QuizBoard* createQuiz(const size_t idx, const QuizSettings& settings, const std::vector<MusicQuiz::QuizTeam*>& teams = {});

	protected:
	};
}
