#include "TeamSelector.hpp"

#include <random>
#include <sstream>
#include <stdexcept>

#include <QColor>
#include <QLabel>
#include <QWidget>
#include <QRegExp>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringList>
#include <QListWidget>
#include <QPushButton>
#include <QSpacerItem>
#include <QMessageBox>
#include <QHeaderView>
#include <QListWidgetItem>

#include "common/Log.hpp"
#include "util/QuizSettings.hpp"
#include "gui_tools/widgets/QuizSettingsDialog.hpp"


MusicQuiz::TeamSelector::TeamSelector(QWidget* parent) :
	QDialog(parent)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Set Object Name */
	setObjectName("TeamSelector");

	/** Create Layout */
	createLayout();

	/** Set Fullscreen */
	showFullScreen();
}

void MusicQuiz::TeamSelector::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	QGridLayout* selectionLayout = new QGridLayout;
	QGridLayout* tableLayout = new QGridLayout;
	mainlayout->setHorizontalSpacing(15);
	mainlayout->setVerticalSpacing(15);
	selectionLayout->setHorizontalSpacing(15);
	selectionLayout->setVerticalSpacing(15);
	mainlayout->setColumnStretch(0, 2);
	mainlayout->setColumnStretch(1, 1);
	size_t row = 0;

	/** Team Table */
	_teamTable = new QTableWidget(0, 2); 
	_teamTable->setFocusPolicy(Qt::NoFocus);
	_teamTable->setSelectionMode(QAbstractItemView::NoSelection);
	_teamTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_teamTable->setHorizontalHeaderLabels(QStringList("Teams"));
	_teamTable->setColumnWidth(0, 200);
	_teamTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); 
	_teamTable->horizontalHeader()->setStretchLastSection(true);
	_teamTable->horizontalHeader()->setVisible(false);
	_teamTable->horizontalHeader()->setEnabled(false);
	_teamTable->verticalHeader()->setVisible(false);
	_teamTable->verticalHeader()->setDefaultSectionSize(100);

	/** Team Name */
	QLabel* teamNameLabel = new QLabel("Team Name:");
	teamNameLabel->setObjectName("teamInfoLabel");
	teamNameLabel->setAlignment(Qt::AlignLeft);
	selectionLayout->addWidget(teamNameLabel, 0, 0, Qt::AlignLeft);

	/** Team Line Edit */
	QRegExp re(".{1,14}");
	QRegExpValidator* validator = new QRegExpValidator(re);
	_teamNameLineEdit = new QLineEdit;
	_teamNameLineEdit->setValidator(validator);
	_teamNameLineEdit->setObjectName("_teamLineEdit");
	selectionLayout->addWidget(_teamNameLineEdit, 1, 0);

	/** Team Color */
	QLabel* teamColorLabel = new QLabel("Team Color:");
	teamColorLabel->setObjectName("teamInfoLabel");
	teamColorLabel->setAlignment(Qt::AlignLeft);
	selectionLayout->addWidget(teamColorLabel, 0, 1, Qt::AlignLeft);

	/** Gradient Slider */
	_hueSlider = new ColorPicker::QHueSlider(this);
	_hueSlider->setStyleSheet("	border: 2px solid rgb(128, 128, 128); min-height: 100px;");
	connect(_hueSlider, SIGNAL(colorChanged(QColor)), this, SLOT(teamColorChanged(QColor)));
	selectionLayout->addWidget(_hueSlider, 1, 1);

	/** Add Button */
	QPushButton* addBtn = new QPushButton("+");
	addBtn->setObjectName("addButton");
	connect(addBtn, SIGNAL(released()), this, SLOT(addTeam()));
	selectionLayout->addWidget(addBtn, 1, 2);

	/** Remove Button */
	//QPushButton* removeBtn = new QPushButton("Remove Team");
	//connect(removeBtn, SIGNAL(released()), this, SLOT(removeTeam()));
	//selectionLayout->addWidget(removeBtn, 2, 1, 1, 1);


	/** Start Quiz Button */
	QPushButton* startBtn = new QPushButton("Start Quiz");
	startBtn->setObjectName("menuButton");
	connect(startBtn, SIGNAL(released()), this, SLOT(teamSelected()));

	/** Quit Button */
	QPushButton* quitBtn = new QPushButton("Quit");
	quitBtn->setObjectName("menuButton");
	connect(quitBtn, SIGNAL(released()), this, SLOT(quit()));

	/** Add Widgets */
	mainlayout->addItem(selectionLayout, ++row, 0, 1, 3);
	mainlayout->addWidget(_teamTable, ++row, 0, 1, 3);
	mainlayout->addWidget(startBtn, ++row, 0, 1, 2);
	mainlayout->addWidget(quitBtn, row, 2, 1, 1);

	/** Set Layout */
	setLayout(mainlayout);
}

void MusicQuiz::TeamSelector::teamSelected()
{
	/** Sanity Check */
	if ( _teamTable == nullptr ) {
		LOG_ERROR("Failed to start quiz. Team Table not created.");
		return;
	}

	/** Get Teams */
	std::vector<MusicQuiz::QuizTeam*> teams;
	for ( size_t i = 0; i < _teamTable->rowCount(); ++i ) {
		const QString teamName = _teamTable->item(i, 1)->text();
		const QColor teamColor = _teamTable->item(i, 1)->backgroundColor();
		MusicQuiz::QuizTeam* teamEntry = new MusicQuiz::QuizTeam(teamName, teamColor);

		teams.push_back(teamEntry);
	}

	QString msg = QString::fromStdString("Are you sure you want to start the quiz?");
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Start Quiz?", msg,
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

	if ( resBtn == QMessageBox::Yes ) {
		emit teamSelectedSignal(teams);
	}
}

void MusicQuiz::TeamSelector::addTeam()
{
	/** Sanity Check */
	if ( _teamTable == nullptr || _teamNameLineEdit == nullptr || _hueSlider == nullptr ) {
		LOG_ERROR("Failed to add team. Team Selector not created correctly.");
		return;
	}

	QString teamName = _teamNameLineEdit->text();
	if ( teamName == "" ) {
		LOG_INFO("Can not add a team with a team name.");
		return;
	}

	/** Limit Number of teams */
	const int row = _teamTable->rowCount();
	if ( row >= 8 ) {
		LOG_INFO("Maximum number of teams reached.");
		return;
	}

	/** Check if team exisis */
	for ( size_t i = 0; i < row; ++i ) {
		if ( teamName == _teamTable->item(i, 1)->text() ) {
			LOG_INFO("Team name alredy exists.");
			return;
		}
	}

	/** Get Text color */
	QColor textColor = QColor(255 - _currentColor.red(), 255 - _currentColor.green(), 255 - _currentColor.blue());

	/** Insert Row */
	_teamTable->insertRow(row);

	/** Add Team Number */
	QTableWidgetItem* entry = new QTableWidgetItem;
	const QString teamNumber = " Team " + QString::number(row);
	entry->setData(Qt::DisplayRole, teamNumber);
	entry->setTextAlignment(Qt::AlignCenter);
	entry->setTextColor(QColor(255, 255, 0));
	_teamTable->setItem(row, 0, entry);

	/** Add Entry */
	entry = new QTableWidgetItem;
	entry->setData(Qt::DisplayRole, teamName);
	entry->setBackgroundColor(_currentColor);
	entry->setTextAlignment(Qt::AlignCenter);
	entry->setTextColor(textColor);
	_teamTable->setItem(row, 1, entry);

	/** Choose a random color for the team */
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(_hueSlider->minimum(), _hueSlider->maximum());
	_hueSlider->setValue(dist(rng));

	/** Clear Line Edit */
	_teamNameLineEdit->setText("");
}

void MusicQuiz::TeamSelector::removeTeam()
{
	/** Sanity Check */
	if ( _teamTable == nullptr ) {
		LOG_ERROR("Failed to add team. Team Table not created.");
		return;
	}

	if ( _teamTable->selectionModel()->selectedRows().empty() ) {
		return;
	}

	/** Get Selected Row */
	const int currentRow = _teamTable->selectionModel()->selectedRows().front().row();

	/** Block Signals */
	_teamTable->blockSignals(true);

	/** Remove Team */
	_teamTable->removeRow(currentRow);

	/** Unblock Signals */
	_teamTable->blockSignals(false);
}

void MusicQuiz::TeamSelector::teamColorChanged(QColor color)
{
	/** Set Color */
	_currentColor = color;

	/** Update Line Edit color */
	std::stringstream ss;
	ss << "background-color	: rgb(" << color.red() << ", " << color.green() << ", " << color.blue() << ");";
	_teamNameLineEdit->setStyleSheet(QString::fromStdString(ss.str()));
}

void MusicQuiz::TeamSelector::quit()
{
	closeWindow();
}

void MusicQuiz::TeamSelector::closeEvent(QCloseEvent* event)
{
	if ( _quizClosed || closeWindow() ) {
		event->accept();
	} else {
		event->ignore();
	}
}

bool MusicQuiz::TeamSelector::closeWindow()
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Close Music Quiz?", "Are you sure you want to close the quiz?",
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

	if ( resBtn == QMessageBox::Yes ) {
		_quizClosed = true;
		emit quitSignal();
		return true;
	}

	return false;
}

void MusicQuiz::TeamSelector::keyPressEvent(QKeyEvent* event)
{
	switch ( event->key() )
	{
	case Qt::Key_Escape:
		closeWindow();
		break;
	default:
		QWidget::keyPressEvent(event);
		break;
	}
}