#include "CategoryCreator.hpp"

#include <QLabel>
#include <QString>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QHeaderView>
#include <QApplication>
#include <QTableWidgetItem>


MusicQuiz::CategoryCreator::CategoryCreator(QWidget* parent) :
	QWidget(parent)
{
	/** Create Layout */
	createLayout();
}

void MusicQuiz::CategoryCreator::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;

	QLabel* test = new QLabel("Fisk");
	mainlayout->addWidget(test);

	/** Set Layout */
	setLayout(mainlayout);
}