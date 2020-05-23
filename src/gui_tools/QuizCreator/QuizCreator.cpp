#include "QuizCreator.hpp"

#include <QList>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QTableWidgetItem>

#include "common/Log.hpp"
#include "gui_tools/QuizCreator/CategoryCreator.hpp"


MusicQuiz::QuizCreator::QuizCreator(QWidget* parent) :
	QDialog(parent)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Set Window Title */
	setWindowTitle("Quiz Creator");

	/** Set Object Name */
	setObjectName("QuizCreator");

	/** Create Layout */
	createLayout();
}

void MusicQuiz::QuizCreator::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;

	/** Tab Widget */
	_tabWidget = new QTabWidget;
	mainlayout->addWidget(_tabWidget, 0, 0, 1, 3);

	/** Setup Tab */
	QWidget* setupTab = new QWidget;
	QGridLayout* setupTabLayout = new QGridLayout;
	setupTab->setLayout(setupTabLayout);
	_tabWidget->addTab(setupTab, "Setup");
	size_t row = 0;

	/** Setup Tab - Quiz Name */
	QLabel* label = new QLabel("Quiz Name:");
	label->setObjectName("quizCreatorLabel");
	setupTabLayout->addWidget(label, ++row, 0, 1, 2, Qt::AlignLeft);

	_quizNameLineEdit = new QLineEdit;
	_quizNameLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_quizNameLineEdit->setObjectName("quizCreatorLineEdit");
	setupTabLayout->addWidget(_quizNameLineEdit, ++row, 0, 1, 2);

	/** Setup Tab - Quiz Description */
	label = new QLabel("Quiz Description:");
	label->setObjectName("quizCreatorLabel");
	setupTabLayout->addWidget(label, ++row, 0, 1, 2, Qt::AlignLeft);

	_quizDescriptionTextEdit = new QTextEdit;
	_quizDescriptionTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_quizDescriptionTextEdit->setObjectName("quizCreatorTextEdit");
	setupTabLayout->addWidget(_quizDescriptionTextEdit, ++row, 0, 1, 2);

	/** Setup Tab - Settings */
	label = new QLabel("Settings:");
	label->setObjectName("quizCreatorLabel");
	setupTabLayout->addWidget(label, ++row, 0, 1, 2, Qt::AlignLeft);

	_hiddenCategoriesCheckbox = new QCheckBox("Hidden Categories");
	_hiddenCategoriesCheckbox->setObjectName("quizCreatorLineEdit");
	setupTabLayout->addWidget(_hiddenCategoriesCheckbox, ++row, 0, 1, 2);

	/** Setup Tab - Categories */
	label = new QLabel("Categories:");
	label->setObjectName("quizCreatorLabel");
	setupTabLayout->addWidget(label, ++row, 0, 1, 1, Qt::AlignLeft);

	QPushButton* addCategoryBtn = new QPushButton("+");
	addCategoryBtn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	connect(addCategoryBtn, SIGNAL(released()), this, SLOT(addCategory()));
	setupTabLayout->addWidget(addCategoryBtn, row, 1, 1, 1, Qt::AlignRight);

	_categoriesTable = new QTableWidget(0, 2);
	_categoriesTable->setObjectName("quizCreatorTable");
	_categoriesTable->horizontalHeader()->setVisible(false);
	_categoriesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	_categoriesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	setupTabLayout->addWidget(_categoriesTable, ++row, 0, 1, 2);

	/** Setup Tab - Row Categories */
	label = new QLabel("Row Categories:");
	label->setObjectName("quizCreatorLabel");
	setupTabLayout->addWidget(label, ++row, 0, 1, 2, Qt::AlignLeft);

	QPushButton* addRowCategoryBtn = new QPushButton("+");
	addRowCategoryBtn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	connect(addRowCategoryBtn, SIGNAL(released()), this, SLOT(addRowCategory()));
	setupTabLayout->addWidget(addRowCategoryBtn, row, 1, 1, 1, Qt::AlignRight);

	_rowCategoriesTable = new QTableWidget(0, 2);
	_rowCategoriesTable->setObjectName("quizCreatorTable");
	_rowCategoriesTable->horizontalHeader()->setVisible(false);
	_rowCategoriesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	_rowCategoriesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	setupTabLayout->addWidget(_rowCategoriesTable, ++row, 0, 1, 2);

	/** Setup Tab - Spacer */
	setupTabLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Expanding), ++row, 0, 1, 2);

	/** Bottom Buttons */
	QPushButton* saveQuizBtn = new QPushButton("Save Quiz");
	mainlayout->addWidget(saveQuizBtn, 1, 0, 1, 1);

	QPushButton* previewQuizBtn = new QPushButton("Preview");
	mainlayout->addWidget(previewQuizBtn, 1, 1, 1, 1);

	QPushButton* quitCreatorBtn = new QPushButton("Quit");
	connect(quitCreatorBtn, SIGNAL(released()), this, SLOT(quitCreator()));
	mainlayout->addWidget(quitCreatorBtn, 1, 2, 1, 1);

	/** Set Layout */
	setLayout(mainlayout);
}

void MusicQuiz::QuizCreator::addCategory()
{
	/** Sanity Check */
	if ( _categoriesTable == nullptr ) {
		return;
	}

	/** Get Number of Categories */
	const size_t categoryCount = _categoriesTable->rowCount();

	/** Insert New Category */
	_categoriesTable->insertRow(categoryCount);

	/** Add Line Edit */
	QLineEdit* categoryName = new QLineEdit("Category " + QString::number(categoryCount + 1));
	categoryName->setObjectName("quizCreatorCategoryLineEdit"); 
	categoryName->setProperty("index", categoryCount);
	connect(categoryName, SIGNAL(textChanged(const QString&)), this, SLOT(updateCategoryTabName(const QString&)));
	_categoriesTable->setCellWidget(categoryCount, 0, categoryName);

	/** Add Remove Category Button */
	QPushButton* removeBtn = new QPushButton("X");
	removeBtn->setObjectName("quizCreatorRemoveBtn");
	removeBtn->setProperty("index", categoryCount);
	connect(removeBtn, SIGNAL(released()), this, SLOT(removeCategory()));

	QHBoxLayout* removeBtnLayout = new QHBoxLayout;
	removeBtnLayout->addWidget(removeBtn, Qt::AlignCenter);

	QWidget* layoutWidget = new QWidget;
	layoutWidget->setLayout(removeBtnLayout);
	_categoriesTable->setCellWidget(categoryCount, 1, layoutWidget);

	/** Add Tab */
	MusicQuiz::CategoryCreator* category = new MusicQuiz::CategoryCreator();
	_tabWidget->addTab(category, categoryName->text());
}

void MusicQuiz::QuizCreator::addRowCategory()
{
	/** Sanity Check */
	if ( _rowCategoriesTable == nullptr ) {
		return;
	}

	/** Get Number of Categories */
	const size_t rowCategoryCount = _rowCategoriesTable->rowCount();

	/** Insert New Category */
	_rowCategoriesTable->insertRow(rowCategoryCount);

	/** Add Line Edit */
	QLineEdit* rowCategoryName = new QLineEdit("Row Category " + QString::number(rowCategoryCount + 1));
	rowCategoryName->setObjectName("quizCreatorCategoryLineEdit");
	_rowCategoriesTable->setCellWidget(rowCategoryCount, 0, rowCategoryName);

	/** Add Remove Category Button */
	QPushButton* removeBtn = new QPushButton("X");
	removeBtn->setObjectName("quizCreatorRemoveBtn");
	removeBtn->setProperty("index", rowCategoryCount);
	connect(removeBtn, SIGNAL(released()), this, SLOT(removeRowCategory()));

	QHBoxLayout* removeBtnLayout = new QHBoxLayout;
	removeBtnLayout->addWidget(removeBtn, Qt::AlignCenter);

	QWidget* layoutWidget = new QWidget;
	layoutWidget->setLayout(removeBtnLayout);
	_rowCategoriesTable->setCellWidget(rowCategoryCount, 1, layoutWidget);
}

void MusicQuiz::QuizCreator::removeCategory()
{
	/** Sanity Check */
	if ( _categoriesTable == nullptr ) {
		return;
	}

	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if ( button == nullptr ) {
		return;
	}

	/** Get Number of Categories */
	const size_t categoryCount = _categoriesTable->rowCount();

	/** Get Index */
	const size_t index = button->property("index").toInt();
	if ( index >= _tabWidget->count() || index >= categoryCount ) {
		return;
	}

	/** Get Category Name */
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_categoriesTable->cellWidget(index, 0));
	if ( lineEdit == nullptr ) {
		return;
	}
	const QString categoryName = lineEdit->text();

	/** Popup to ensure the user wants to delete the category */
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Delete Category?", "Are you sure you want to delete category '" + categoryName + "'?",
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
	if ( resBtn != QMessageBox::Yes ) {
		return;
	}

	/** Delete Category in table */
	_categoriesTable->removeRow(index);

	/** Delete Tab */
	_tabWidget->removeTab(index + 1);

	/** Update Indices */
	for ( size_t i = 0; i < _categoriesTable->rowCount(); ++i ) {
		/** Line Edit */
		QLineEdit* tmpLineEdit = qobject_cast<QLineEdit*>(_categoriesTable->cellWidget(i, 0));
		if ( tmpLineEdit != nullptr ) {
			tmpLineEdit->setProperty("index", i);
		}

		/** Button */
		QList<QPushButton*> buttons = _categoriesTable->cellWidget(i, 1)->findChildren<QPushButton*>();
		for ( QPushButton* tmpButton : buttons ) {
			if ( tmpButton != nullptr ) {
				tmpButton->setProperty("index", i);
			}
		}
	}
}

void MusicQuiz::QuizCreator::removeRowCategory()
{
	/** Sanity Check */
	if ( _rowCategoriesTable == nullptr ) {
		return;
	}

	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if ( button == nullptr ) {
		return;
	}

	/** Get Number of Row Categories */
	const size_t rowCategoryCount = _rowCategoriesTable->rowCount();

	/** Get Index */
	const size_t index = button->property("index").toInt();
	if ( index >= rowCategoryCount ) {
		return;
	}

	/** Get Row Category Name */
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_rowCategoriesTable->cellWidget(index, 0));
	if ( lineEdit == nullptr ) {
		return;
	}
	const QString rowCategoryName = lineEdit->text();

	/** Popup to ensure the user wants to delete the row category */
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Delete Row Category?", "Are you sure you want to delete row category '" + rowCategoryName + "'?",
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
	if ( resBtn != QMessageBox::Yes ) {
		return;
	}

	/** Delete Row Category in table */
	_rowCategoriesTable->removeRow(index);

	/** Update Button Indices */
	for ( size_t i = 0; i < _rowCategoriesTable->rowCount(); ++i ) {
		QList<QPushButton*> buttons = _rowCategoriesTable->cellWidget(i, 1)->findChildren<QPushButton*>();
		for ( QPushButton* tmpButton : buttons ) {
			if ( tmpButton != nullptr ) {
				tmpButton->setProperty("index", i);
			}
		}
	}
}

void MusicQuiz::QuizCreator::updateCategoryTabName(const QString& str)
{
	/** Sanity Check */
	if ( _tabWidget == nullptr ) {
		return;
	}

	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
	if ( lineEdit == nullptr ) {
		return;
	}

	/** Get Index */
	const size_t index = lineEdit->property("index").toInt() + 1;
	if ( index >= _tabWidget->count() ) {
		return;
	}

	/** Set Tab Text */
	_tabWidget->setTabText(index, lineEdit->text());
}

void MusicQuiz::QuizCreator::quitCreator()
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Close Quiz Creator?", "Are you sure you want to close the Quiz Creator?",
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

	if ( resBtn == QMessageBox::Yes ) {
		/** Call Destructor */
		QApplication::quit();
	}
}