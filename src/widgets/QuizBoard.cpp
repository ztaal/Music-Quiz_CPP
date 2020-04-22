#include "QuizBoard.hpp"

#include <stdexcept>

#include <QLabel>
#include <QSpacerItem>
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
	for ( size_t i = 0; i < _categories.size(); ++i ) {
		if ( rowCategories.size() != _categories[i]->size() ) {
			sameNumberOfEntries = false;
			break;
		}
	}

	if ( sameNumberOfEntries ) {
		_rowCategories = rowCategories;
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
	teamsLayout->setSpacing(10);
	categorylayout->setSpacing(0);

	/** Categories */
	for ( size_t i = 0; i < _categories.size(); ++i ) {
		categorylayout->addWidget(_categories[i]);
		categorylayout->setStretch(i, 1);
	}

	/** Row Categories */
	if ( !_rowCategories.empty() ) {
		QVBoxLayout* rowCategorylayout = new QVBoxLayout;
		rowCategorylayout->setSpacing(10);
		rowCategorylayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);

		/** Add Empty Box */
		QPushButton* rowCategoryBtn = new QPushButton("", this);
		rowCategoryBtn->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
		rowCategoryBtn->setObjectName("QuizEntry_rowCategoryLabel");
		rowCategorylayout->addWidget(rowCategoryBtn);

		/** Add Row Categories */
		for ( size_t i = 0; i < _rowCategories.size(); ++i ) {
			rowCategoryBtn = new QPushButton(_rowCategories[i], this);
			rowCategoryBtn->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
			rowCategoryBtn->setObjectName("QuizEntry_rowCategoryLabel");
			rowCategorylayout->addWidget(rowCategoryBtn);
		}

		/** Add layouts to main layout */
		QGridLayout* tmpLayout = new QGridLayout;
		tmpLayout->setSizeConstraint(QGridLayout::SetMinimumSize);
		tmpLayout->addItem(rowCategorylayout, 0, 0);
		tmpLayout->addItem(categorylayout, 0, 1);
		tmpLayout->setColumnStretch(1, 1);
		mainlayout->addItem(tmpLayout, 0, 0);
	} else {
		mainlayout->addItem(categorylayout, 0, 0);
	}

	/** Teams */
	for ( size_t i = 0; i < _teams.size(); ++i ) {
		teamsLayout->addWidget(_teams[i]);
	}
	mainlayout->addItem(teamsLayout, 1, 0);

	/** Set Layout */
	setLayout(mainlayout);
}