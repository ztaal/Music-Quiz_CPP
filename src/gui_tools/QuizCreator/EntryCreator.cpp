#include "EntryCreator.hpp"

#include <QLabel>
#include <QString>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QButtonGroup>


MusicQuiz::EntryCreator::EntryCreator(const QString& name, const size_t points, QWidget* parent) :
	QWidget(parent), _entryName(name), _points(points)
{
	/** Create Layout */
	createLayout();
}

void MusicQuiz::EntryCreator::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
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
	_answerLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_answerLineEdit->setObjectName("quizCreatorLineEdit");
	mainlayout->addWidget(_answerLineEdit, row, 1);

	/** Points */
	label = new QLabel("Points:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	_pointsSpinbox = new QSpinBox;
	_pointsSpinbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_pointsSpinbox->setObjectName("quizCreatorSpinbox");
	_pointsSpinbox->setRange(0, 10000);
	_pointsSpinbox->setSingleStep(100);
	_pointsSpinbox->setValue(_points);
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
	buttonGroup->addButton(songBtn);
	buttonGroup->addButton(videoBtn);
	songBtn->setChecked(true);

	typeLayout->addWidget(songBtn);
	typeLayout->addWidget(videoBtn);
	mainlayout->addItem(typeLayout, ++row, 0, 1, 2);

	/** Song File */
	label = new QLabel("Song File:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);
	
	QHBoxLayout* songFileLayout = new QHBoxLayout;
	_songFileLineEdit = new QLineEdit;
	_songFileLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_songFileLineEdit->setObjectName("quizCreatorLineEdit");
	songFileLayout->addWidget(_songFileLineEdit);

	QPushButton* browseBtn = new QPushButton("[]");
	browseBtn->setObjectName("quizCreatorBrowseBtn");
	connect(browseBtn, SIGNAL(released()), this, SLOT(browseSong()));
	songFileLayout->addWidget(browseBtn);
	mainlayout->addItem(songFileLayout, ++row, 0, 1, 2);

	/** Video File */
	label = new QLabel("Video File:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	QHBoxLayout* videoFileLayout = new QHBoxLayout;
	_videoFileLineEdit = new QLineEdit;
	_videoFileLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_videoFileLineEdit->setObjectName("quizCreatorLineEdit");
	videoFileLayout->addWidget(_videoFileLineEdit);

	browseBtn = new QPushButton("[]");
	browseBtn->setObjectName("quizCreatorBrowseBtn");
	connect(browseBtn, SIGNAL(released()), this, SLOT(browseVideo()));
	videoFileLayout->addWidget(browseBtn);
	mainlayout->addItem(videoFileLayout, ++row, 0, 1, 2);

	/** Set Layout */
	mainlayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Expanding), ++row, 0, 1, 2);
	
	/** Set Layout */
	setLayout(mainlayout);
}

void MusicQuiz::EntryCreator::browseSong()
{
	/** Sanity Check */
	if ( _songFileLineEdit == nullptr ) {
		return;
	}

	/** Open File Dialog */
	const QString filePath = QFileDialog::getOpenFileName(this, "Select Song File", "~/", "Song File (*.mp3; *.wav);");
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
	const QString filePath = QFileDialog::getOpenFileName(this, "Select Video File", "~/", "Video File (*.mp4);");
	if ( filePath.isEmpty() ) {
		return;
	}

	/** Update Line Edit */
	_videoFileLineEdit->setText(filePath);
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