#pragma once

#include <string>
#include <memory>

#include "util/QuizSettings.hpp"
#include "audio/AudioPlayer.hpp"
#include "gui_tools/widgets/QuizTeam.hpp"
#include "gui_tools/widgets/QuizBoard.hpp"



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
		static MusicQuiz::QuizBoard* createQuiz(const size_t idx, const MusicQuiz::QuizSettings& settings, const std::shared_ptr< audio::AudioPlayer >& audioPlayer, const std::vector<MusicQuiz::QuizTeam*>& teams = {});

	protected:
	};
}
