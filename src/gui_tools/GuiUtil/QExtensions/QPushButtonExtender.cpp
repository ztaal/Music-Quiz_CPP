#include "QPushButtonExtender.hpp"

#include <sstream>


MusicQuiz::QExtensions::QPushButtonExtender::QPushButtonExtender(QWidget* parent) :
	QPushButton(parent)
{}

void MusicQuiz::QExtensions::QPushButtonExtender::mouseReleaseEvent(QMouseEvent* event)
{
	if ( event->button() == Qt::LeftButton ) {
		emit leftClicked();
	} else if ( event->button() == Qt::RightButton ) {
		emit rightClicked();
	}
}

void MusicQuiz::QExtensions::QPushButtonExtender::setColor(const QColor& color)
{
	std::stringstream ss;
	ss << "background-color	: rgb(" << color.red() << ", " << color.green() << ", " << color.blue() << ");";
	setStyleSheet(QString::fromStdString(ss.str()));
}