#include "TeamSelector.hpp"

#include <sstream>
#include <stdexcept>

#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringList>
#include <QListWidget>
#include <QPushButton>
#include <QSpacerItem>
#include <QMessageBox>
#include <QListWidgetItem>

#include "common/Log.hpp"
#include "util/QuizSettings.hpp"
#include "gui_tools/widgets/QuizSettingsDialog.hpp"
#include "gui_tools/GuiUtil/ColorPicker/QHueSlider.hpp"


MusicQuiz::TeamSelector::TeamSelector(QWidget* parent) :
	QDialog(parent)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Set Size */
	const size_t width = 1200;
	const size_t height = 600;
	if ( parent == nullptr ) {
		resize(width, height);
	} else {
		setGeometry(parent->x() + parent->width() / 2 - width / 2, parent->y() + parent->height() / 2 - height / 2, width, height);
	}

	/** Create Layout */
	createLayout();
}

void MusicQuiz::TeamSelector::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	mainlayout->setHorizontalSpacing(10);
	mainlayout->setVerticalSpacing(5);
	mainlayout->setColumnStretch(0, 1);
	mainlayout->setColumnStretch(1, 3);

	/** Label */
	QLabel* topLabel = new QLabel("Select Teams");
	topLabel->setObjectName("selectTeamsLabel");

	/** Horizontal Line */
	QWidget* horizontalLine = new QWidget(this);
	horizontalLine->setFixedHeight(3);
	horizontalLine->setStyleSheet("background-color: rgb(0, 0, 0);");
	horizontalLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


	/** Gradient Slider */
	ColorPicker::QHueSlider* slider = new ColorPicker::QHueSlider(this);

	/** Add Widgets */
	mainlayout->addWidget(topLabel, 0, 0, 1, 3, Qt::AlignCenter);
	mainlayout->addWidget(horizontalLine, 1, 0, 1, 3);
	mainlayout->addWidget(slider, 3, 0, 1, 3);

	/** Set Layout */
	setLayout(mainlayout);
}

void MusicQuiz::TeamSelector::teamSelected()
{
	/** Sanity Check */


	/** Current Index */


/*	QString msg = QString::fromStdString("Are you sure you want to select quiz '" + _quizPreviews[currentIndex].quizName + "'.");
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Select Quiz?", msg,
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

	if ( resBtn == QMessageBox::Yes ) {
		emit quizSelectedSignal(currentIndex, _settings);
	}*/
}

void MusicQuiz::TeamSelector::quit()
{
	/** Send Quit Signal */
	emit quitSignal();

	/** Call Destructor */
	close();
}

void MusicQuiz::TeamSelector::keyPressEvent(QKeyEvent* event)
{
	switch ( event->key() )
	{
	case Qt::Key_Escape:
		emit quitSignal();
		break;
	default:
		QWidget::keyPressEvent(event);
		break;
	}
}