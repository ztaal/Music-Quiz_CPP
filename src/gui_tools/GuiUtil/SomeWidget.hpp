#pragma once

#include <QObject>
#include <QWidget>
#include <QTabWidget>


namespace jeopardy {
	namespace GuiUtil {
		class SomeWidget : public QTabWidget
		{
			Q_OBJECT
		public:
			explicit SomeWidget(QWidget *parent = Q_NULLPTR);

			virtual ~SomeWidget() = default;

		public slots:
		protected:
		};
	}
}
