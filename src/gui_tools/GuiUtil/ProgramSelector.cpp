#include "ProgramSelector.hpp"

#include <QLabel>
#include <QGridLayout>


MusicQuiz::GuiUtil::ProgramSelector::ProgramSelector(QWidget* parent) :
	QWidget(parent)
{
	/** Window */
	setWindowTitle("Select Program");
	setGeometry(300, 300, 500, 400);
	setMinimumHeight(100);
	setMinimumWidth(250);
	setMaximumHeight(200);
	setMaximumWidth(800);

	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	mainlayout->setVerticalSpacing(10);
	mainlayout->setHorizontalSpacing(10);

	/** Label */
	QLabel* label = new QLabel("Select Program");
	label->setAlignment(Qt::AlignCenter);
	mainlayout->addWidget(label, 0, 0, 1, 3);

	/** Buttons */
	//musicQuiz_btn = QuizEntry("Music Quiz", 0);
	//layout.addWidget(musicQuiz_btn, 1, 0, 1, 1);

	//quizCreator_btn = QuizEntry("Quiz Creator", 0);
	//layout.addWidget(quizCreator_btn, 1, 1, 1, 1);

	//exit_btn = QuizEntry("Exit", 0);
	//layout.addWidget(exit_btn, 1, 2, 1, 1);

	setLayout(mainlayout);
}
