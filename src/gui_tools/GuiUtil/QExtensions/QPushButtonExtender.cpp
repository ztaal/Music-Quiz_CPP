#include "QPushButtonExtender.hpp"


MusicQuiz::QExtensions::QPushButtonExtender::QPushButtonExtender(QWidget* parent) :
	QPushButton(parent)
{}

void MusicQuiz::QExtensions::QPushButtonExtender::mousePressEvent(QMouseEvent* event)
{
	if ( event->button() == Qt::LeftButton ) {
		emit leftClicked();
	} else if ( event->button() == Qt::RightButton ) {
		emit rightClicked();
	}
}