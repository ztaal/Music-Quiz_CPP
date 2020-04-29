#pragma once

#include <atomic>
#include <thread>

#include <QColor>
#include <QTimer>
#include <QString>
#include <QObject>
#include <QWidget>
#include <QPushButton>


namespace MusicQuiz {
	class QuizTeam : public QPushButton
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] name The name of the team.
		 * @param[in] color The color of the team anwser.
		 * @param[in] parent The parent widget.
		 */
		explicit QuizTeam(const QString& name, const QColor& color, QWidget* parent = nullptr);

		/**
		 * @brief Destructor
		 */
		virtual ~QuizTeam();

		/**
		 * @brief Sets if the team score is visiable during the quiz.
		 *
		 * @param[in] hide Set to true if the score should be hidden.
		 */
		void setHideScore(bool hide);

	public slots:
		/**
		 * @brief Adds points to the team score.
		 *
		 * @param[in] points The points to be added to the score.
		 */
		void addPoints(size_t points);

		/**
		 * @brief Gets the team name.
		 *
		 * @return The name.
		 */
		QString getName() const;

		/**
		 * @brief Gets the team score.
		 *
		 * @return The score.
		 */
		size_t getScore() const;

		/**
		 * @brief Gets the team color.
		 *
		 * @return The color.
		 */
		QColor getColor() const;

	private slots:
		/**
		 * @brief Accumulates the score given by the addPoints function.
		 */
		void accumulateScore();

	protected:

		/** Variables */
		QString _name = "";
		std::atomic<size_t> _score = 0;
		QColor _color = QColor(0, 0, 0);

		QTimer _scoreCntTimer;
		std::atomic<size_t> _newPoints = 0;
		std::atomic <size_t> _scoreCntRate = 1;
		const size_t _scoreTimerDelay = 25; // ms

		bool _hideScore = false;
	};
}
