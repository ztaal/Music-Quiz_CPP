#pragma once

#include <QObject>
#include <QWidget>
#include <QTabWidget>


namespace MusicQuiz {
	namespace GuiUtil {
		class ProgramSelector : public QWidget
		{
			Q_OBJECT
		public:
			explicit ProgramSelector(QWidget *parent = nullptr);

			virtual ~ProgramSelector() = default;

		public slots:
		protected:
		};
	}
}
