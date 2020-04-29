#pragma once

#include <string>

#include "widgets/QuizTeam.hpp"
#include "widgets/QuizBoard.hpp"
#include "util/QuizSettings.hpp"


namespace MusicQuiz {
	class QuizFactory
{
	public:
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
		static MusicQuiz::QuizBoard* createQuiz(const size_t idx, const MusicQuiz::QuizSettings& settings, const std::vector<MusicQuiz::QuizTeam*>& teams = {});

	protected:
	};
}
