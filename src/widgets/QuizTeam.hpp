#pragma once

#include <QColor>
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
		 * @brief Default Destructor
		 */
		virtual ~QuizTeam() = default;

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

	protected:

		/** Variables */
		QString _name = "";
		size_t _score = 0;
		QColor _color = QColor(0, 0, 0);
	};
}
