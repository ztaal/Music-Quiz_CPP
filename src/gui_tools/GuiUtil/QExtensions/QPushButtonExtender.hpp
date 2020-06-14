#pragma once

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QMouseEvent>


namespace MusicQuiz {
	namespace QExtensions {
		class QPushButtonExtender : public QPushButton
		{
			Q_OBJECT

		public:
			explicit QPushButtonExtender(QWidget* parent = nullptr);

		private slots:
			void mousePressEvent(QMouseEvent* e);

		signals:
			void leftClicked();
			void rightClicked();

		public slots:

		};
	}
}