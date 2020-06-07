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


MusicQuiz::QuizSettingsDialog::QuizSettingsDialog(const MusicQuiz::QuizSettings& settings, QWidget* parent) :
	QDialog(parent)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Create Layout */
	createLayout(settings);
}

void MusicQuiz::QuizSettingsDialog::createLayout(const MusicQuiz::QuizSettings& settings)
{
	/** Layout */
	QVBoxLayout* mainlayout = new QVBoxLayout;
	mainlayout->setSpacing(10);

	/** Top Label */
	QLabel* label = new QLabel("Settings");
	label->setObjectName("settingsLabel");
	label->setAlignment(Qt::AlignHCenter);
	mainlayout->addWidget(label);

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
	_hiddenTeam->setChecked(settings.hiddenTeamScore);
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
	_hiddenAnswers->setChecked(settings.hiddenTeamScore);
	hiddenAnswersLayout->addWidget(_hiddenAnswers);

	infoBtn = new QPushButton("?");
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showHiddenAnswersInfo()));
	hiddenAnswersLayout->addWidget(infoBtn);
	mainlayout->addItem(hiddenAnswersLayout);

	/** Daily Double */
	QHBoxLayout* dailyDoubleLayout = new QHBoxLayout;
	dailyDoubleLayout->setSpacing(10);
	_dailyDouble = new QCheckBox("Daily Double");
	_dailyDouble->setChecked(settings.dailyDouble);
	dailyDoubleLayout->addWidget(_dailyDouble);

	infoBtn = new QPushButton("?");
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showDailyDoubleInfo()));
	dailyDoubleLayout->addWidget(infoBtn);
	mainlayout->addItem(dailyDoubleLayout);

	/** Daily Double Hidden */
	QHBoxLayout* dailyDoubleHiddenLayout = new QHBoxLayout;
	dailyDoubleHiddenLayout->setSpacing(10);
	_dailyDoubleHidden = new QCheckBox("Daily Double Hidden");
	_dailyDoubleHidden->setChecked(settings.dailyDoubleHidden);
	dailyDoubleHiddenLayout->addWidget(_dailyDoubleHidden);

	infoBtn = new QPushButton("?");
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showDailyDoubleHiddenInfo()));
	dailyDoubleHiddenLayout->addWidget(infoBtn);
	mainlayout->addItem(dailyDoubleHiddenLayout);

	/** Daily Trible */
	QHBoxLayout* dailyTripleLayout = new QHBoxLayout;
	dailyTripleLayout->setSpacing(10);
	_dailyTriple = new QCheckBox("Daily Triple");
	_dailyTriple->setChecked(settings.dailyTriple);
	dailyTripleLayout->addWidget(_dailyTriple);

	infoBtn = new QPushButton("?");
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showDailyTripleInfo()));
	dailyTripleLayout->addWidget(infoBtn);
	mainlayout->addItem(dailyTripleLayout);

	/** Daily Trible Hidden */
	QHBoxLayout* dailyTripleHiddenLayout = new QHBoxLayout;
	dailyTripleHiddenLayout->setSpacing(10);
	_dailyTripleHidden = new QCheckBox("Daily Triple Hidden");
	_dailyTripleHidden->setChecked(settings.dailyTripleHidden);
	dailyTripleHiddenLayout->addWidget(_dailyTripleHidden);

	infoBtn = new QPushButton("?");
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showDailyTripleHiddenInfo()));
	dailyTripleHiddenLayout->addWidget(infoBtn);
	mainlayout->addItem(dailyTripleHiddenLayout);

	/** Guess the category */
	//bool guessTheCategory = false;
	//size_t pointsPerCategory = 500;

	/** Save Button */
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing(10);
	QPushButton* saveBtn = new QPushButton("Save");
	connect(saveBtn, SIGNAL(released()), this, SLOT(saveSettings()));
	buttonLayout->addWidget(saveBtn);

	/** Close Button */
	QPushButton* closeBtn = new QPushButton("Close");
	connect(closeBtn, SIGNAL(released()), this, SLOT(close()));
	buttonLayout->addWidget(closeBtn);
	mainlayout->addItem(buttonLayout);

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

void MusicQuiz::QuizSettingsDialog::saveSettings()
{
	MusicQuiz::QuizSettings settings;

	/** Hidden Anwsers */
	settings.hiddenAnswers = _hiddenAnswers->isChecked();

	/** Hidden Teams */
	settings.hiddenTeamScore = _hiddenTeam->isChecked();

	/** Daily Double */
	settings.dailyDouble = _dailyDouble->isChecked();
	settings.dailyDoubleHidden = _dailyDoubleHidden->isChecked();

	/** Daily Triple */
	settings.dailyTriple = _dailyTriple->isChecked();
	settings.dailyTripleHidden = _dailyTripleHidden->isChecked();

	/** Emit Signal with settings */
	emit settingsUpdated(settings);

	/** Close Dialog */
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
	QMessageBox::information(this, "Info", info);
}

void MusicQuiz::QuizSettingsDialog::showHiddenTeamsInfo()
{
	informationMessageBox("If enabled the team score will be hidden from the players.");
}

void MusicQuiz::QuizSettingsDialog::showHiddenAnswersInfo()
{
	informationMessageBox("If enabled the quiz answers will be hidden after the song have been guessed.");
}

void MusicQuiz::QuizSettingsDialog::showDailyDoubleInfo()
{
	informationMessageBox("If enabled the set percentage of entries will give double points. The entries are choosen randomly.");
}

void MusicQuiz::QuizSettingsDialog::showDailyDoubleHiddenInfo()
{
	informationMessageBox("If enabled the entries that gives double points will be hidden until it starts playing.");
}

void MusicQuiz::QuizSettingsDialog::showDailyTripleInfo()
{
	informationMessageBox("If enabled the set percentage of entries will give triple points. The entries are choosen randomly.");
}

void MusicQuiz::QuizSettingsDialog::showDailyTripleHiddenInfo()
{
	informationMessageBox("If enabled the entries that gives triple points will be hidden until it starts playing.");
}