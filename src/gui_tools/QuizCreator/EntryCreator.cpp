#include "EntryCreator.hpp"

#include <QTime>
#include <QLabel>
#include <QString>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>

#include <QAudioBuffer>
#include <QAudioDecoder>
#include <QMediaPlayer>
#include <QAudioDeviceInfo >

#include <boost/filesystem.hpp>

#include "common/Log.hpp"


MusicQuiz::EntryCreator::EntryCreator(const QString& name, const size_t points, const audio::AudioPlayer::Ptr& audioPlayer, QWidget* parent) :
	QWidget(parent), _entryName(name), _points(points), _audioPlayer(audioPlayer)
{
	/** Create Layout */
	createLayout();
}

void MusicQuiz::EntryCreator::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	mainlayout->setHorizontalSpacing(10);
	mainlayout->setVerticalSpacing(10);
	mainlayout->setColumnStretch(0, 1);
	mainlayout->setColumnStretch(1, 3);
	size_t row = 0;

	/** Entry Name */
	_entryNameLabel = new QLabel(_entryName);
	_entryNameLabel->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(_entryNameLabel, ++row, 0, 1, 2, Qt::AlignCenter);

	/** Answer */
	QLabel* label = new QLabel("Answer:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	_answerLineEdit = new QLineEdit;
	_answerLineEdit->setAlignment(Qt::AlignCenter);
	_answerLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_answerLineEdit->setObjectName("quizCreatorLineEdit");
	mainlayout->addWidget(_answerLineEdit, row, 1);

	/** Points */
	label = new QLabel("Points:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	_pointsSpinbox = new QSpinBox;
	_pointsSpinbox->setAlignment(Qt::AlignCenter);
	_pointsSpinbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_pointsSpinbox->setObjectName("quizCreatorSpinbox");
	_pointsSpinbox->setRange(0, 10000);
	_pointsSpinbox->setSingleStep(100);
	_pointsSpinbox->setValue(_points);
	connect(_pointsSpinbox, SIGNAL(valueChanged(int)), this, SLOT(pointsChanged(int)));
	mainlayout->addWidget(_pointsSpinbox, row, 1);

	/** Type */
	QHBoxLayout* typeLayout = new QHBoxLayout;
	typeLayout->setStretch(0, 1);
	typeLayout->setStretch(1, 1);
	typeLayout->setStretch(2, 1);
	label = new QLabel("Type:");
	label->setObjectName("quizCreatorLabel");
	typeLayout->addWidget(label);

	QButtonGroup* buttonGroup = new QButtonGroup;
	QRadioButton* songBtn = new QRadioButton("Song");
	QRadioButton* videoBtn = new QRadioButton("Video");
	songBtn->setObjectName("quizCreatorRadioButton");
	videoBtn->setObjectName("quizCreatorRadioButton");
	buttonGroup->addButton(songBtn, 0);
	buttonGroup->addButton(videoBtn, 1);
	connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(setEntryType(int)));
	songBtn->setChecked(true);

	typeLayout->addWidget(songBtn);
	typeLayout->addWidget(videoBtn);
	mainlayout->addItem(typeLayout, ++row, 0, 1, 2);

	/** Song File Layout */
	mainlayout->addItem(createSongFileLayout(), ++row, 0, 1, 2);

	/** Video - File */
	mainlayout->addItem(createVideoFileLayout(), ++row, 0, 1, 2);

	/** Set Type to song */
	setEntryType(0);

	/** Set Layout */
	mainlayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Expanding), ++row, 0, 1, 2);

	/** Set Layout */
	setLayout(mainlayout);
}

QGridLayout* MusicQuiz::EntryCreator::createSongFileLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	QHBoxLayout* songFileLayout = new QHBoxLayout;
	QGridLayout* songSettingsLayout = new QGridLayout;
	songSettingsLayout->setHorizontalSpacing(5);
	songSettingsLayout->setVerticalSpacing(10);
	mainlayout->setVerticalSpacing(10);
	size_t row = 0;

	/** Song - File */
	QLabel* label = new QLabel("Song File:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	songFileLayout->setSpacing(10);
	_songFileLineEdit = new QLineEdit;
	_songFileLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_songFileLineEdit->setObjectName("quizCreatorLineEdit");
	connect(_songFileLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkSongFileName(const QString&)));
	songFileLayout->addWidget(_songFileLineEdit);

	QPushButton* browseBtn = new QPushButton;
	browseBtn->setObjectName("quizCreatorBrowseBtn");
	connect(browseBtn, SIGNAL(released()), this, SLOT(browseSong()));
	songFileLayout->addWidget(browseBtn);
	mainlayout->addItem(songFileLayout, ++row, 0, 1, 2);


	/** Song - Set Song Start and End Times */
	label = new QLabel("Song:");
	label->setObjectName("quizCreatorLabel");
	songSettingsLayout->addWidget(label, 0, 0, 1, 1);

	/** Song Start Time */
	_songStartTimeEdit = new QTimeEdit;
	_songStartTimeEdit->setAlignment(Qt::AlignCenter);
	_songStartTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_songStartTimeEdit->setObjectName("quizCreatorTimeEdit");
	songSettingsLayout->addWidget(_songStartTimeEdit, 0, 1, 1, 1);

	/** Song End Time */
	_songEndTimeEdit = new QTimeEdit;
	_songEndTimeEdit->setTime(QTime(5, 0, 0));
	_songEndTimeEdit->setAlignment(Qt::AlignCenter);
	_songEndTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_songEndTimeEdit->setObjectName("quizCreatorTimeEdit");
	songSettingsLayout->addWidget(_songEndTimeEdit, 0, 2, 1, 1);

	/** Song Audio Buttons - Play */
	QPushButton* btn = new QPushButton;
	btn->setProperty("type", "song");
	btn->setObjectName("quizCreatorPlayBtn");
	connect(btn, SIGNAL(released()), this, SLOT(playSong()));
	songSettingsLayout->addWidget(btn, 0, 3, 1, 1);

	/** Song Audio Buttons - Pause */
	btn = new QPushButton;
	btn->setObjectName("quizCreatorPauseBtn");
	connect(btn, SIGNAL(released()), this, SLOT(pauseSong()));
	songSettingsLayout->addWidget(btn, 0, 4, 1, 1);

	/** Song Audio Buttons - Stop */
	btn = new QPushButton;
	btn->setObjectName("quizCreatorStopBtn");
	connect(btn, SIGNAL(released()), this, SLOT(stopSong()));
	songSettingsLayout->addWidget(btn, 0, 5, 1, 1);

	/** Song - Set Answer Start and End Times */
	label = new QLabel("Answer:");
	label->setObjectName("quizCreatorLabel");
	songSettingsLayout->addWidget(label, 1, 0, 1, 1);


	/** Answer Start Time */
	_answerStartTimeEdit = new QTimeEdit;
	_answerStartTimeEdit->setAlignment(Qt::AlignCenter);
	_answerStartTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_answerStartTimeEdit->setObjectName("quizCreatorTimeEdit");
	songSettingsLayout->addWidget(_answerStartTimeEdit, 1, 1, 1, 1);

	/** Answer End Time */
	_answerEndTimeEdit = new QTimeEdit;
	_answerEndTimeEdit->setTime(QTime(5, 0, 0));
	_answerEndTimeEdit->setAlignment(Qt::AlignCenter);
	_answerEndTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_answerEndTimeEdit->setObjectName("quizCreatorTimeEdit");
	songSettingsLayout->addWidget(_answerEndTimeEdit, 1, 2, 1, 1);

	/** Answer Audio Buttons - Play */
	btn = new QPushButton;
	btn->setProperty("type", "songAnswer");
	btn->setObjectName("quizCreatorPlayBtn");
	connect(btn, SIGNAL(released()), this, SLOT(playSong()));
	songSettingsLayout->addWidget(btn, 1, 3, 1, 1);

	/** Answer Audio Buttons - Pause */
	btn = new QPushButton;
	btn->setObjectName("quizCreatorPauseBtn");
	connect(btn, SIGNAL(released()), this, SLOT(pauseSong()));
	songSettingsLayout->addWidget(btn, 1, 4, 1, 1);

	/** Answer Audio Buttons - Stop */
	btn = new QPushButton;
	btn->setObjectName("quizCreatorStopBtn");
	connect(btn, SIGNAL(released()), this, SLOT(stopSong()));
	songSettingsLayout->addWidget(btn, 1, 5, 1, 1);

	/** Add layout to settings widget */
	_songSettings = new QWidget;
	_songSettings->setEnabled(false);
	_songSettings->setLayout(songSettingsLayout);
	mainlayout->addWidget(_songSettings, ++row, 0, 1, 2);

	/** Set Layout */
	return mainlayout;
}

QGridLayout* MusicQuiz::EntryCreator::createVideoFileLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	QHBoxLayout* videoFileLayout = new QHBoxLayout;
	QGridLayout* videoSettingsLayout = new QGridLayout;
	videoSettingsLayout->setHorizontalSpacing(5);
	videoSettingsLayout->setVerticalSpacing(10);
	mainlayout->setVerticalSpacing(10);
	size_t row = 0;

	/** Video - File */
	QLabel* label = new QLabel("Video File:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	videoFileLayout->setSpacing(10);
	_videoFileLineEdit = new QLineEdit;
	_videoFileLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_videoFileLineEdit->setObjectName("quizCreatorLineEdit");
	connect(_videoFileLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkVideoFileName(const QString&)));
	videoFileLayout->addWidget(_videoFileLineEdit);

	_browseVideoBtn = new QPushButton;
	_browseVideoBtn->setObjectName("quizCreatorBrowseBtn");
	connect(_browseVideoBtn, SIGNAL(released()), this, SLOT(browseVideo()));
	videoFileLayout->addWidget(_browseVideoBtn);
	mainlayout->addItem(videoFileLayout, ++row, 0, 1, 2);


	/** Video - Set Video Start and End Times */
	label = new QLabel("Video:");
	label->setObjectName("quizCreatorLabel");
	videoSettingsLayout->addWidget(label, 0, 0, 1, 1);

	/** Video Start Time */
	_videoStartTimeEdit = new QTimeEdit;
	_videoStartTimeEdit->setAlignment(Qt::AlignCenter);
	_videoStartTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_videoStartTimeEdit->setObjectName("quizCreatorTimeEdit");
	videoSettingsLayout->addWidget(_videoStartTimeEdit, 0, 1, 1, 1);

	/** Video End Time */
	_videoEndTimeEdit = new QTimeEdit;
	_videoEndTimeEdit->setTime(QTime(5, 0, 0));
	_videoEndTimeEdit->setAlignment(Qt::AlignCenter);
	_videoEndTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_videoEndTimeEdit->setObjectName("quizCreatorTimeEdit");
	videoSettingsLayout->addWidget(_videoEndTimeEdit, 0, 2, 1, 1);

	/** Video Buttons - Play */
	QPushButton* btn = new QPushButton;
	//btn->setProperty("type", "videoSong");
	btn->setObjectName("quizCreatorPlayBtn");
	//connect(btn, SIGNAL(released()), this, SLOT(playSong()));
	videoSettingsLayout->addWidget(btn, 0, 3, 1, 1);

	/** Video Buttons - Pause */
	btn = new QPushButton;
	btn->setObjectName("quizCreatorPauseBtn");
	//connect(btn, SIGNAL(released()), this, SLOT(pauseSong()));
	videoSettingsLayout->addWidget(btn, 0, 4, 1, 1);

	/** Video Buttons - Stop */
	btn = new QPushButton;
	btn->setObjectName("quizCreatorStopBtn");
	//connect(btn, SIGNAL(released()), this, SLOT(stopSong()));
	videoSettingsLayout->addWidget(btn, 0, 5, 1, 1);


	/** Video - Set Answer Start and End Times */
	label = new QLabel("Answer:");
	label->setObjectName("quizCreatorLabel");
	videoSettingsLayout->addWidget(label, 1, 0, 1, 1);

	/** Answer Start Time */
	_videoAnswerStartTimeEdit = new QTimeEdit;
	_videoAnswerStartTimeEdit->setAlignment(Qt::AlignCenter);
	_videoAnswerStartTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_videoAnswerStartTimeEdit->setObjectName("quizCreatorTimeEdit");
	videoSettingsLayout->addWidget(_videoAnswerStartTimeEdit, 1, 1, 1, 1);

	/** Answer End Time */
	_videoAnswerEndTimeEdit = new QTimeEdit;
	_videoAnswerEndTimeEdit->setTime(QTime(5, 0, 0));
	_videoAnswerEndTimeEdit->setAlignment(Qt::AlignCenter);
	_videoAnswerEndTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_videoAnswerEndTimeEdit->setObjectName("quizCreatorTimeEdit");
	videoSettingsLayout->addWidget(_videoAnswerEndTimeEdit, 1, 2, 1, 1);

	/** Answer Audio Buttons - Play */
	btn = new QPushButton;
	//btn->setProperty("type", "videoSongAnswer");
	btn->setObjectName("quizCreatorPlayBtn");
	//connect(btn, SIGNAL(released()), this, SLOT(playSong()));
	videoSettingsLayout->addWidget(btn, 1, 3, 1, 1);

	/** Answer Audio Buttons - Pause */
	btn = new QPushButton;
	btn->setObjectName("quizCreatorPauseBtn");
	//connect(btn, SIGNAL(released()), this, SLOT(pauseSong()));
	videoSettingsLayout->addWidget(btn, 1, 4, 1, 1);

	/** Answer Audio Buttons - Stop */
	btn = new QPushButton;
	btn->setObjectName("quizCreatorStopBtn");
	//connect(btn, SIGNAL(released()), this, SLOT(stopSong()));
	videoSettingsLayout->addWidget(btn, 1, 5, 1, 1);

	/** Add layout to settings widget */
	_videoSettings = new QWidget;
	_videoSettings->setEnabled(false);
	_videoSettings->setLayout(videoSettingsLayout);
	mainlayout->addWidget(_videoSettings, ++row, 0, 1, 2);

	/** Set Layout */
	return mainlayout;
}

void MusicQuiz::EntryCreator::playSong()
{
	/** Sanity Check */
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if ( button == nullptr || _audioPlayer == nullptr ) {
		return;
	}

	/** Get Type */
	QString fileName = "";
	size_t startTime = 0, endTime = 0;
	const QString type = button->property("type").toString();
	if ( type == "song" ) {
		/** Sanity Check */
		if ( _songFileLineEdit == nullptr || _songStartTimeEdit == nullptr || _songEndTimeEdit == nullptr ) {
			return;
		}

		/** Get File Name */
		fileName = _songFileLineEdit->text();

		/** Get Start and End Time */
		startTime = getMSec(_songStartTimeEdit->time());
		endTime = getMSec(_songEndTimeEdit->time());
	} else if ( type == "songAnswer" ) {
		/** Sanity Check */
		if ( _songFileLineEdit == nullptr || _answerStartTimeEdit == nullptr || _answerEndTimeEdit == nullptr ) {
			return;
		}

		/** Get File Name */
		fileName = _songFileLineEdit->text();

		/** Get Start and End Time */
		startTime = getMSec(_answerStartTimeEdit->time());
		endTime = getMSec(_answerEndTimeEdit->time());
	} else if ( type == "videoSong" ) {

	} else if ( type == "videoSongAnswer" ) {

	}

	/** Check if file is valid */
	if ( fileName.isEmpty() || !isSongFileValid(fileName) ) {
		return;
	}

	try {
		/** Stop Song */
		stopSong();
		
		/** Play Song */
		if ( endTime > 240000 ) { // \hack if end time is larger than 4 min play without an end time
			_audioPlayer->play(fileName.toStdString(), startTime);
		} else {
			_audioPlayer->play(fileName.toStdString(), startTime, endTime);
		}
	} catch ( ... ) {
	}
}

void MusicQuiz::EntryCreator::pauseSong()
{
	/** Sanity Check */
	if ( _audioPlayer == nullptr ) {
		return;
	}

	/** Play Song */
	try {
		_audioPlayer->pause();
	} catch ( ... ) {
	}
}

void MusicQuiz::EntryCreator::stopSong()
{
	/** Sanity Check */
	if ( _audioPlayer == nullptr ) {
		return;
	}

	/** Play Song */
	try {
		_audioPlayer->stop();
	} catch ( ... ) {
	}
}

void MusicQuiz::EntryCreator::browseSong()
{
	/** Sanity Check */
	if ( _songFileLineEdit == nullptr ) {
		return;
	}

	/** Open File Dialog */
	const QString filePath = QFileDialog::getOpenFileName(this, "Select Song File", "./data/", "Song File (*.mp3; *.wav);");
	if ( filePath.isEmpty() ) {
		return;
	}

	/** Update Line Edit */
	_songFileLineEdit->setText(filePath);
}

void MusicQuiz::EntryCreator::browseVideo()
{
	/** Sanity Check */
	if ( _videoFileLineEdit == nullptr ) {
		return;
	}

	/** Open File Dialog */
	const QString filePath = QFileDialog::getOpenFileName(this, "Select Video File", "./data/", "Video File (*.mp4);");
	if ( filePath.isEmpty() ) {
		return;
	}

	/** Update Line Edit */
	_videoFileLineEdit->setText(filePath);
}

void MusicQuiz::EntryCreator::checkSongFileName(const QString& fileName)
{
	/** Sanity Check */
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
	if ( lineEdit == nullptr ) {
		return;
	}

	/** Check if name is valid */
	bool isValid = isSongFileValid(fileName);

	if ( isValid ) {
		/** Set Line Edit Color */
		lineEdit->setStyleSheet("color: black;");

		/** Enable Song Controls */
		if ( !_songSettings->isEnabled() ) {
			_songSettings->setEnabled(true);
		}
	} else {
		/** Set Line Edit Color */
		lineEdit->setStyleSheet("color: red;");

		/** Disable Song Controls */
		if ( _songSettings->isEnabled() ) {
			_songSettings->setEnabled(false);
		}
	}
}

void MusicQuiz::EntryCreator::checkVideoFileName(const QString& fileName)
{
	/** Sanity Check */
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
	if ( lineEdit == nullptr ) {
		return;
	}

	/** Check if name is valid */
	bool isValid = isVideoFileValid(fileName);

	if ( isValid ) {
		/** Set Line Edit Color */
		lineEdit->setStyleSheet("color: black;");

		/** Enable Video Controls */
		if ( !_videoSettings->isEnabled() ) {
			_videoSettings->setEnabled(true);
		}
	} else {
		/** Set Line Edit Color */
		lineEdit->setStyleSheet("color: red;");

		/** Disable Video Controls */
		if ( _videoSettings->isEnabled() ) {
			_videoSettings->setEnabled(false);
		}
	}
}

bool MusicQuiz::EntryCreator::isSongFileValid(const QString& fileName)
{
	/** Check if file has a valid format */
	if ( !fileName.contains(".mp3") && !fileName.contains(".wav") ) {
		return false;
	}

	/** Check if file exists */
	if ( !boost::filesystem::exists(fileName.toStdString()) ) {
		return false;
	}

	return true;
}

bool MusicQuiz::EntryCreator::isVideoFileValid(const QString& fileName)
{
	/** Check if file has a valid format */
	if ( !fileName.contains(".mp4") ) {
		return false;
	}

	/** Check if file exists */
	if ( !boost::filesystem::exists(fileName.toStdString()) ) {
		return false;
	}

	return true;
}

size_t MusicQuiz::EntryCreator::getMSec(const QTime& time)
{
	return time.hour() * 60000 + time.minute() * 1000;
}

void MusicQuiz::EntryCreator::setEntryType(int index)
{
	/** Sanity Check */
	if ( _videoSettings == nullptr || _browseVideoBtn == nullptr || _videoFileLineEdit == nullptr ) {
		return;
	}

	/** Get Type */
	if ( index == 0 ) { // song
		/** Disable Video Widgets */
		_videoSettings->setEnabled(false);
		_browseVideoBtn->setEnabled(false);
		_videoFileLineEdit->setEnabled(false);
	} else if ( index == 1 ) { // video
		/** Enable Video Widgets */
		_videoSettings->setEnabled(true);
		_browseVideoBtn->setEnabled(true);
		_videoFileLineEdit->setEnabled(true);
	}
}

void MusicQuiz::EntryCreator::setName(const QString& name)
{
	/** Sanity Check */
	if ( _entryNameLabel == nullptr ) {
		return;
	}

	/** Set Name */
	_entryName = name;

	/** Update Label */
	_entryNameLabel->setText(_entryName);
}

const QString MusicQuiz::EntryCreator::getName() const
{
	return _entryName;
}

const QString MusicQuiz::EntryCreator::getAnswer() const
{
	/** Sanity Check */
	if ( _answerLineEdit == nullptr ) {
		return "";
	}

	return _answerLineEdit->text();
}

void MusicQuiz::EntryCreator::pointsChanged(int points)
{
	_points = points;
}

const size_t MusicQuiz::EntryCreator::getPoints() const
{
	return _points;
}
