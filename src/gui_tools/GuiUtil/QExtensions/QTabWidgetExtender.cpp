#include "QTabWidgetExtender.hpp"

#include "gui_tools/GuiUtil/QExtensions/QTabBarExtender.hpp"


MusicQuiz::QExtensions::QTabWidgetExtender::QTabWidgetExtender(QWidget *parent) :
	QTabWidget(parent)
{
	setTabBar(new QTabBarExtender);
	setTabPosition(QTabWidget::West);
}