#include "QuizSettingsDialog.hpp"

#include <stdexcept>

#include <QString>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include "common/Log.hpp"


MusicQuiz::QuizSettingsDialog::QuizSettingsDialog(MusicQuiz::QuizFactory::QuizSettings settings, QWidget* parent) :
	QDialog(parent), _settings(settings)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Create Layout */
	createLayout();
}

void MusicQuiz::QuizSettingsDialog::createLayout()
{
	/** Layout */
	QVBoxLayout* mainlayout = new QVBoxLayout;
	mainlayout->setSpacing(10);

	/** Top Label */
	QLabel* label = new QLabel("Settings");
	label->setObjectName("settingsLabel");
	label->setAlignment(Qt::AlignHCenter);
	mainlayout->addWidget(label);
	//mainlayout->addWidget(label, Qt::AlignHCenter);

	/** Horizontal Line */
	QWidget* horizontalLine = new QWidget(this);
	horizontalLine->setFixedHeight(2);
	horizontalLine->setStyleSheet("background-color: rgb(0, 0, 0);");
	horizontalLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	mainlayout->addWidget(horizontalLine);

	/** Hidden Teams */
	QHBoxLayout* hiddenTeamLayout = new QHBoxLayout;
	hiddenTeamLayout->setSpacing(10);
	_hiddenTeam = new QCheckBox("Hidden Teams");
	_hiddenTeam->setChecked(_settings.hiddenTeamScore);
	hiddenTeamLayout->addWidget(_hiddenTeam);

	QPushButton* infoBtn = new QPushButton("?");
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showHiddenTeamsInfo()));
	hiddenTeamLayout->addWidget(infoBtn);
	mainlayout->addItem(hiddenTeamLayout);

	/** Hidden Anwsers */
	QHBoxLayout* hiddenAnswersLayout = new QHBoxLayout;
	hiddenAnswersLayout->setSpacing(10);
	_hiddenAnswers = new QCheckBox("Hidden Answers");
	_hiddenAnswers->setChecked(_settings.hiddenTeamScore);
	hiddenAnswersLayout->addWidget(_hiddenAnswers);

	infoBtn = new QPushButton("?");
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showHiddenAnswersInfo()));
	hiddenAnswersLayout->addWidget(infoBtn);
	mainlayout->addItem(hiddenAnswersLayout);


	/** Daily Double */
	//bool dailyDouble = false;
	//bool dailyDoubleHidden = false;
	//bool dailyDoublePercentage = 0;

	/** Daily Trible */
	//bool dailyTriple = false;
	//bool dailyTripleHidden = false;
	//bool dailyTriplePercentage = 0;

	/** Guess the category */
	//bool guessTheCategory = false;
	//size_t pointsPerCategory = 500;


	/** Set Layout */
	setLayout(mainlayout);
}

void MusicQuiz::QuizSettingsDialog::quit()
{
	/** Send Quit Signal */
	emit quitSignal();

	/** Call Destructor */
	close();
}

void MusicQuiz::QuizSettingsDialog::keyPressEvent(QKeyEvent* event)
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

void MusicQuiz::QuizSettingsDialog::informationMessageBox(const QString& info)
{
	QMessageBox::information(this, "info", info);
}

void MusicQuiz::QuizSettingsDialog::showHiddenTeamsInfo()
{
	informationMessageBox("If enabled the team score will be hidden from the players.");
}

void MusicQuiz::QuizSettingsDialog::showHiddenAnswersInfo()
{
	informationMessageBox("If enabled the quiz answers will be hidden after the song have been quessed.");
}