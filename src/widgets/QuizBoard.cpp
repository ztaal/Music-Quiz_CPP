#include "QuizBoard.hpp"

#include <stdexcept>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "common/Log.hpp"

#include "widgets/QuizTeam.hpp"
#include "widgets/QuizCategory.hpp"


MusicQuiz::QuizBoard::QuizBoard(const std::vector<MusicQuiz::QuizCategory*>& categories, const std::vector<QString>& rowCategories, const std::vector<MusicQuiz::QuizTeam*>& teams, QWidget* parent) :
	QWidget(parent), _categories(categories), _teams(teams)
{
	/** Sanity Check */
	if ( _categories.empty() ) {
		throw std::runtime_error("Cannot create quiz board without any categories.");
	}

	/** Set Row Categories if they match the number of entries in the categories */
	bool sameNumberOfEntries = true;
	for ( size_t i = 0; i < _categories.size() - 1; ++i ) {
		if ( _categories[i]->size() != _categories[i + 1]->size() ) {
			sameNumberOfEntries = false;
			break;
		}
	}

	if ( sameNumberOfEntries ) {
		//_rowCategories = rowCategories;
	}

	/** Create Widget Layout */
	createLayout();
}

void MusicQuiz::QuizBoard::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	QHBoxLayout* categorylayout = new QHBoxLayout;
	QHBoxLayout* teamsLayout = new QHBoxLayout;
	mainlayout->setHorizontalSpacing(0);
	mainlayout->setVerticalSpacing(5);
	mainlayout->setRowStretch(0, 1);
	teamsLayout->setSpacing(10);
	categorylayout->setSpacing(0);

	/** Categories */
	for ( size_t i = 0; i < _categories.size(); ++i ) {
		categorylayout->addWidget(_categories[i]);
		categorylayout->setStretch(i, 1);
	}
	mainlayout->addItem(categorylayout, 0, 0);

	/** Row Categories */
	if ( !_rowCategories.empty() ) {
		QVBoxLayout* rowCategorylayout = new QVBoxLayout;
		rowCategorylayout->addWidget(new QLabel(""));
		for ( size_t i = 0; i < _rowCategories.size(); ++i ) {
			QLabel* rowLabel = new QLabel(_rowCategories[i]);
			rowCategorylayout->addWidget(rowLabel);
		}
		mainlayout->addItem(rowCategorylayout, 0, 0);
		mainlayout->addItem(categorylayout, 0, 1);
		mainlayout->setColumnStretch(1, 1);
	} else {
		mainlayout->addItem(categorylayout, 0, 0, 1, 2);
	}

	/** Teams */
	for ( size_t i = 0; i < _teams.size(); ++i ) {
		teamsLayout->addWidget(_teams[i]);
	}
	mainlayout->addItem(teamsLayout, 1, 0, 1, 2);

	/** Set Layout */
	setLayout(mainlayout);
}