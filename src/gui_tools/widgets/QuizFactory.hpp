#pragma once

#include <string>
#include <memory>

#include <QWidget>

#include "util/QuizSettings.hpp"
#include "audio/AudioPlayer.hpp"
#include "media/VideoPlayer.hpp"
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
		 * @param[in] quizName The quiz name to load.
		 * @param[in] settings The quiz settings.
		 * @param[in] audioPlayer The audio player.
		 * @param[in] videoPlayer The video player
		 * @param[in] teams The teams list.
		 * @param[in] preview If the quiz should be displayed in preview mode..
		 * @param[in] parent The quiz board parent.
		 *
		 * @return The quiz board.
		 */
		static MusicQuiz::QuizBoard* createQuiz(const std::string& quizName, const MusicQuiz::QuizSettings& settings, const std::shared_ptr< audio::AudioPlayer >& audioPlayer, 
			const std::shared_ptr< media::VideoPlayer >& videoPlayer, const std::vector<MusicQuiz::QuizTeam*>& teams = {}, bool preview = false, QWidget * parent = nullptr);

		/**
		 * @brief Creates the music quiz.
		 *
		 * @param[in] idx The quiz index to load.
		 * @param[in] settings The quiz settings.
		 * @param[in] audioPlayer The audio player.
		 * @param[in] videoPlayer The video player
		 * @param[in] teams The teams list.
		 * @param[in] preview If the quiz should be displayed in preview mode.
		 * @param[in] parent The quiz board parent.
		 *
		 * @return The quiz board.
		 */
		static MusicQuiz::QuizBoard* createQuiz(const size_t idx, const MusicQuiz::QuizSettings& settings, const std::shared_ptr< audio::AudioPlayer >& audioPlayer,
			const std::shared_ptr< media::VideoPlayer >& videoPlayer, const std::vector<MusicQuiz::QuizTeam*>& teams = {}, bool preview = false, QWidget* parent = nullptr);

	protected:
	};
}
