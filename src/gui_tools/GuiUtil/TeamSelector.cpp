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
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Set Size */
	const size_t width = 800;
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
	QGridLayout* selectionLayout = new QGridLayout;
	QGridLayout* tableLayout = new QGridLayout;
	mainlayout->setHorizontalSpacing(10);
	mainlayout->setVerticalSpacing(5);
	mainlayout->setColumnStretch(0, 2);
	mainlayout->setColumnStretch(1, 1);
	size_t row = 0;

	/** Label */
	QLabel* topLabel = new QLabel("Select Teams");
	topLabel->setObjectName("selectTeamsLabel");

	/** Horizontal Line */
	QWidget* horizontalLine = new QWidget(this);
	horizontalLine->setFixedHeight(3);
	horizontalLine->setStyleSheet("background-color: rgb(0, 0, 0);");
	horizontalLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	/** Team Line Edit */
	QRegExp re(".{1,16}");
	QRegExpValidator* validator = new QRegExpValidator(re);
	_teamNameLineEdit = new QLineEdit;
	_teamNameLineEdit->setValidator(validator);
	_teamNameLineEdit->setObjectName("_teamLineEdit");
	selectionLayout->addWidget(_teamNameLineEdit, 0, 0, 1, 2);

	/** Gradient Slider */
	_hueSlider = new ColorPicker::QHueSlider(this);
	connect(_hueSlider, SIGNAL(colorChanged(QColor)), this, SLOT(teamColorChanged(QColor)));
	selectionLayout->addWidget(_hueSlider, 1, 0, 1, 2);

	/** Add Button */
	QPushButton* addBtn = new QPushButton("Add Team");
	connect(addBtn, SIGNAL(released()), this, SLOT(addTeam()));
	selectionLayout->addWidget(addBtn, 2, 0, 1, 1);

	/** Remove Button */
	QPushButton* removeBtn = new QPushButton("Remove Team");
	connect(removeBtn, SIGNAL(released()), this, SLOT(removeTeam()));
	selectionLayout->addWidget(removeBtn, 2, 1, 1, 1);

	/** Team Table */
	_teamTable = new QTableWidget(0, 1);
	_teamTable->setHorizontalHeaderLabels(QStringList("Teams"));
	_teamTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	_teamTable->horizontalHeader()->setEnabled(false);
	_teamTable->verticalHeader()->setVisible(false);
	_teamTable->verticalHeader()->setDefaultSectionSize(75);

	/** Start Quiz Button */
	QPushButton* startBtn = new QPushButton("Start Quiz");
	connect(startBtn, SIGNAL(released()), this, SLOT(teamSelected()));

	/** Add Widgets */
	mainlayout->addWidget(topLabel, ++row, 0, 1, 3, Qt::AlignCenter);
	mainlayout->addWidget(horizontalLine, ++row, 0, 1, 3);
	mainlayout->addItem(selectionLayout, ++row, 0, 1, 2);
	mainlayout->addWidget(_teamTable, row, 2, 1, 1);
	mainlayout->addWidget(startBtn, ++row, 0, 1, 3);

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
		const QString teamName = _teamTable->item(i, 0)->text();
		const QColor teamColor = _teamTable->item(i, 0)->backgroundColor();
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
		if ( teamName == _teamTable->item(i, 0)->text() ) {
			LOG_INFO("Team name alredy exists.");
			return;
		}
	}

	/** Get Text color */
	QColor textColor = QColor(255 - _currentColor.red(), 255 - _currentColor.green(), 255 - _currentColor.blue());

	/** Insert Row */
	_teamTable->insertRow(row);

	/** Add Entry */
	QTableWidgetItem* entry = new QTableWidgetItem;
	entry->setData(Qt::DisplayRole, teamName);
	entry->setBackgroundColor(_currentColor);
	entry->setTextAlignment(Qt::AlignCenter);
	entry->setTextColor(textColor);
	_teamTable->setItem(row, 0, entry);

	/** Choose a random color for the team */
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(_hueSlider->minimum(), _hueSlider->maximum());
	_hueSlider->setValue(dist(rng));
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