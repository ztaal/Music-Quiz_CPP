#pragma once

#include <string>
#include <memory>

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
		/**
		 * @brief Constructor
		 *
		 * @param[in] audioFile The audio file to play.
		 * @param[in] answer The entry anwser.
		 * @param[in] points The number of points obtained by guessing the entry.
		 * @param[in] startTime The start time of the media in [ms].
		 * @param[in] answerStartTime The answer media start time in [ms].
		 * @param[in] parent The parent widget.
		 */
		explicit QuizEntry(const QString &audioFile, const QString &answer, size_t points, size_t startTime, size_t answerStartTime,
			const std::shared_ptr< audio::AudioPlayer >& audioPlayer, QWidget* parent = nullptr);

		/**
		 * @brief Default Destructor
		 */
		virtual ~QuizEntry() = default;

		enum class EntryState
		{
			IDLE = 1, // Default
			PLAYING = 2,
			PAUSED = 3,
			PLAYING_ANSWER = 4,
			PLAYED = 5,
		};

		/**
		 * @brief Get the entry state.
		 *
		 * @return The entry state.
		 */
		EntryState getEntryState();

	public slots:
		/**
		 * @brief Sets the color of the button (used after the entry is answered).
		 *
		 * @param[in] color The color.
		 */
		void setColor(const QColor &color);

		/**
		 * @brief Enables / disables the hidden answers setting.
		 *
		 * @param[in] hidden If true the answers will be hidden.
		 */
		void setHiddenAnswer(bool hidden);

	signals:
		void answered(size_t points);
		void played();

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

		/**
		 * @brief Applys the color to the button.
		 *
		 * @param[in] color The color.
		 */
		void applyColor(const QColor& color);

		/** Variables */
		size_t _points = 0;

		size_t _startTime = 0;
		size_t _answerStartTime = 0;

		QString _answer = "";
		bool _entryAnswered = false;
		QColor _answeredColor = QColor(0, 0, 120);

		QString _audioFile = "";

		EntryState _state = EntryState::IDLE;

		std::shared_ptr< audio::AudioPlayer > _audioPlayer = nullptr;

		bool _hiddenAnswer = false; // setting
	};
}
