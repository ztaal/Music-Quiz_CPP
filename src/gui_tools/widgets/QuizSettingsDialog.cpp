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
#include <QSpacerItem>

#include "common/Log.hpp"


MusicQuiz::QuizSettingsDialog::QuizSettingsDialog(const MusicQuiz::QuizSettings& settings, QWidget* parent) :
	QDialog(parent)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Set Title */
	setWindowTitle("Settings");

	/** Create Layout */
	createLayout(settings);
}

void MusicQuiz::QuizSettingsDialog::createLayout(const MusicQuiz::QuizSettings& settings)
{
	/** Layout */
	QVBoxLayout* mainlayout = new QVBoxLayout;
	mainlayout->setSpacing(10);

	/** Hidden Teams */
	QHBoxLayout* hiddenTeamLayout = new QHBoxLayout;
	hiddenTeamLayout->setSpacing(10);
	_hiddenTeam = new QCheckBox("Hidden Teams");
	_hiddenTeam->setChecked(settings.hiddenTeamScore);
	hiddenTeamLayout->addWidget(_hiddenTeam);

	QPushButton* infoBtn = new QPushButton;
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

	infoBtn = new QPushButton;
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showHiddenAnswersInfo()));
	hiddenAnswersLayout->addWidget(infoBtn);
	mainlayout->addItem(hiddenAnswersLayout);

	/** Line */
	QFrame* line = new QFrame;
	line->setFrameShape(QFrame::HLine);
	mainlayout->addWidget(line);

	/** Daily Double */
	QWidget* dailyDouble = getDailyDoubleLayout(settings);
	if ( dailyDouble != nullptr ) {
		mainlayout->addWidget(dailyDouble);
	}

	/** Line */
	line = new QFrame;
	line->setFrameShape(QFrame::HLine);
	mainlayout->addWidget(line);

	/** Daily Trible */
	QWidget* dailyTriple = getDailyTripleLayout(settings);
	if ( dailyTriple != nullptr ) {
		mainlayout->addWidget(dailyTriple);
	}

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

QWidget* MusicQuiz::QuizSettingsDialog::getDailyDoubleLayout(const MusicQuiz::QuizSettings& settings)
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	mainlayout->setColumnStretch(1, 1);
	mainlayout->setHorizontalSpacing(0);
	mainlayout->setContentsMargins(0, 10, 0, 10);
	int row = 0;

	/** Enable Checkbox */
	QHBoxLayout* checkBoxLayout = new QHBoxLayout;
	_dailyDouble = new QCheckBox("Daily Double");
	_dailyDouble->setChecked(settings.dailyDouble);
	connect(_dailyDouble, SIGNAL(toggled(bool)), this, SLOT(setDailyDoubleEnabled(bool)));
	checkBoxLayout->addWidget(_dailyDouble);

	QPushButton* infoBtn = new QPushButton;
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showDailyDoubleInfo()));
	checkBoxLayout->addWidget(infoBtn, Qt::AlignRight);
	mainlayout->addItem(checkBoxLayout, 0, 0, 1, 2, Qt::AlignLeft);

	/** Spacer */
	mainlayout->addItem(new QSpacerItem(20, 0, QSizePolicy::Fixed, QSizePolicy::Fixed), 1, 0);

	/** Options Layout */
	_dailyDoubleLayout = new QGridLayout;
	_dailyDoubleLayout->setHorizontalSpacing(10);
	_dailyDoubleLayout->setVerticalSpacing(5);
	mainlayout->addItem(_dailyDoubleLayout, 1, 1);

	/** Hidden Checkbox */
	checkBoxLayout = new QHBoxLayout;
	_dailyDoubleHidden = new QCheckBox("Hidden");
	_dailyDoubleHidden->setChecked(settings.dailyDoubleHidden);
	checkBoxLayout->addWidget(_dailyDoubleHidden);

	infoBtn = new QPushButton;
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showDailyDoubleHiddenInfo()));
	checkBoxLayout->addWidget(infoBtn, Qt::AlignRight);
	_dailyDoubleLayout->addItem(checkBoxLayout, 0, 0, 1, 2);

	/** Percentage */	
	QLabel* label = new QLabel("Percentage");
	_dailyDoublePercentageLineEdit = new QLineEdit(QString::number(settings.dailyDoublePercentage) + " %");
	_dailyDoublePercentageLineEdit->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); border: none;}");
	_dailyDoublePercentageLineEdit->setAlignment(Qt::AlignRight);
	_dailyDoublePercentageLineEdit->setReadOnly(true);
	_dailyDoubleLayout->addWidget(label, 1, 0, Qt::AlignLeft);
	_dailyDoubleLayout->addWidget(_dailyDoublePercentageLineEdit, 1, 1, Qt::AlignRight);
	
	_dailyDoublePercentage = new QSlider(Qt::Horizontal);
	_dailyDoublePercentage->setRange(_minDoublePercentage, _maxDoublePercentage);
	_dailyDoublePercentage->setPageStep(1);
	_dailyDoublePercentage->setSingleStep(1);
	_dailyDoublePercentage->setValue(settings.dailyDoublePercentage);
	connect(_dailyDoublePercentage, SIGNAL(valueChanged(int)), this, SLOT(setDailyDoublePercentage(int)));
	_dailyDoubleLayout->addWidget(_dailyDoublePercentage, 2, 0, 1, 2);

	/** Set Options Enabled / Disabled */
	setDailyDoubleEnabled(settings.dailyDouble);

	/** Return Layout Widget */
	QWidget* widget = new QWidget;
	widget->setLayout(mainlayout);
	return widget;
}

QWidget* MusicQuiz::QuizSettingsDialog::getDailyTripleLayout(const MusicQuiz::QuizSettings& settings)
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	mainlayout->setColumnStretch(1, 1);
	mainlayout->setHorizontalSpacing(0);
	mainlayout->setContentsMargins(0, 10, 0, 10);
	int row = 0;

	/** Enable Checkbox */
	QHBoxLayout* checkBoxLayout = new QHBoxLayout;
	_dailyTriple = new QCheckBox("Daily Triple");
	_dailyTriple->setChecked(settings.dailyDouble);
	connect(_dailyTriple, SIGNAL(toggled(bool)), this, SLOT(setDailyTripleEnabled(bool)));
	checkBoxLayout->addWidget(_dailyTriple);

	QPushButton* infoBtn = new QPushButton;
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showDailyTripleInfo()));
	checkBoxLayout->addWidget(infoBtn, Qt::AlignRight);
	mainlayout->addItem(checkBoxLayout, 0, 0, 1, 2, Qt::AlignLeft);

	/** Spacer */
	mainlayout->addItem(new QSpacerItem(20, 0, QSizePolicy::Fixed, QSizePolicy::Fixed), 1, 0);

	/** Options Layout */
	_dailyTripleLayout = new QGridLayout;
	_dailyTripleLayout->setHorizontalSpacing(10);
	_dailyTripleLayout->setVerticalSpacing(5);
	mainlayout->addItem(_dailyTripleLayout, 1, 1);

	/** Hidden Checkbox */
	checkBoxLayout = new QHBoxLayout;
	_dailyTripleHidden = new QCheckBox("Hidden");
	_dailyTripleHidden->setChecked(settings.dailyTripleHidden);
	checkBoxLayout->addWidget(_dailyTripleHidden);

	infoBtn = new QPushButton;
	infoBtn->setObjectName("settingsInfo");
	connect(infoBtn, SIGNAL(released()), this, SLOT(showDailyTripleHiddenInfo()));
	checkBoxLayout->addWidget(infoBtn, Qt::AlignRight);
	_dailyTripleLayout->addItem(checkBoxLayout, 0, 0, 1, 2);

	/** Percentage */
	QLabel* label = new QLabel("Percentage");
	_dailyTriplePercentageLineEdit = new QLineEdit(QString::number(settings.dailyTriplePercentage) + " %");
	_dailyTriplePercentageLineEdit->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); border: none;}");
	_dailyTriplePercentageLineEdit->setAlignment(Qt::AlignRight);
	_dailyTriplePercentageLineEdit->setReadOnly(true);
	_dailyTripleLayout->addWidget(label, 1, 0, Qt::AlignLeft);
	_dailyTripleLayout->addWidget(_dailyTriplePercentageLineEdit, 1, 1, Qt::AlignRight);

	_dailyTriplePercentage = new QSlider(Qt::Horizontal);
	_dailyTriplePercentage->setRange(_minTriplePercentage, _maxTriplePercentage);
	_dailyTriplePercentage->setPageStep(1);
	_dailyTriplePercentage->setSingleStep(1);
	_dailyTriplePercentage->setValue(settings.dailyTriplePercentage);
	connect(_dailyTriplePercentage, SIGNAL(valueChanged(int)), this, SLOT(setDailyTriplePercentage(int)));
	_dailyTripleLayout->addWidget(_dailyTriplePercentage, 2, 0, 1, 2);

	/** Set Options Enabled / Disabled */
	setDailyTripleEnabled(settings.dailyTriple);

	/** Return Layout Widget */
	QWidget* widget = new QWidget;
	widget->setLayout(mainlayout);
	return widget;
}

void MusicQuiz::QuizSettingsDialog::setDailyDoubleEnabled(bool enabled)
{
	/** Sanity Check */
	if ( _dailyDoubleLayout == nullptr ) {
		return;
	}

	/** Enable / Disable */
	setLayoutEnabled(_dailyDoubleLayout, enabled);
}

void MusicQuiz::QuizSettingsDialog::setDailyTripleEnabled(bool enabled)
{
	/** Sanity Check */
	if ( _dailyTripleLayout == nullptr ) {
		return;
	}

	/** Enable / Disable */
	setLayoutEnabled(_dailyTripleLayout, enabled);
}

void MusicQuiz::QuizSettingsDialog::setDailyDoublePercentage(int value)
{
	/** Sanity Check */
	if ( _dailyDoublePercentageLineEdit == nullptr ) {
		return;
	}

	/** Set Value */
	_dailyDoublePercentageLineEdit->setText(QString::number(value) + " %");
}

void MusicQuiz::QuizSettingsDialog::setDailyTriplePercentage(int value)
{
	/** Sanity Check */
	if ( _dailyTriplePercentageLineEdit == nullptr ) {
		return;
	}

	/** Set Value */
	_dailyTriplePercentageLineEdit->setText(QString::number(value) + " %");
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
	settings.dailyDoublePercentage = _dailyDoublePercentage->value();

	/** Daily Triple */
	settings.dailyTriple = _dailyTriple->isChecked();
	settings.dailyTripleHidden = _dailyTripleHidden->isChecked();
	settings.dailyTriplePercentage = _dailyTriplePercentage->value();

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

void MusicQuiz::QuizSettingsDialog::quit()
{
	/** Send Quit Signal */
	emit quitSignal();

	/** Call Destructor */
	close();
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

void MusicQuiz::QuizSettingsDialog::setLayoutEnabled(QLayout* layout, bool enabled)
{
	/** Sanity Check */
	if ( layout == nullptr ) {
		return;
	}

	/** Enable / Disable layout and its children */
	for ( size_t i = 0; i < layout->count(); ++i ) {
		QWidget* subWidget = layout->itemAt(i)->widget();
		if ( subWidget != nullptr ) {
			subWidget->setEnabled(enabled);
		} else {
			QLayout* subLayout = layout->itemAt(i)->layout();
			if ( subLayout != nullptr ) {
				for ( size_t i = 0; i < subLayout->count(); ++i ) {
					QWidget* subSubWidget = subLayout->itemAt(i)->widget();
					if ( subSubWidget != nullptr ) {
						subSubWidget->setEnabled(enabled);
					}
				}
			}
		}
	}
}