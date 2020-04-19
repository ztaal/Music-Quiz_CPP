#pragma once

#include <string>

#include <QString>
#include <QObject>
#include <QWidget>
#include <QPushButton>

#include "audio/AudioPlayer.hpp"

class QMouseEvent;

namespace MusicQuiz {
	class QuizEntry : public QPushButton
	{
		Q_OBJECT
	public:
		explicit QuizEntry(std::string _audioFile, QString answer, size_t points, size_t startTime, size_t endTime, size_t answerStartTime, size_t answerEndTime, QWidget* parent = nullptr);

		virtual ~QuizEntry() = default;

		enum class EntryState
		{
			IDLE = 1,				// Default
			PLAYING = 2,
			PAUSED = 3,
			PLAYING_ANSWER = 4,
			PLAYED = 5,
		};

	public slots:

	protected:
		/**
		 * @brief Handles the mouse release event.
		 *
		 * @param[in] event The event.
		 */
		void mouseReleaseEvent(QMouseEvent* event);

		/**
		 * @brief Handles the mouse left click event.
		 */
		void leftClickEvent();

		/**
		 * @brief Handles the mouse right click event.
		 */
		void rightClickEvent();

		/** Variables */
		size_t _points = 0;

		size_t _startTime = 0;
		size_t _endTime = 0;

		size_t _answerStartTime = 0;
		size_t _answerEndTime = 0;

		QString _answer = "";

		std::string _audioFile = "";

		EntryState _state = EntryState::IDLE;

		audio::AudioPlayer _audioPlayer;
	};
}
