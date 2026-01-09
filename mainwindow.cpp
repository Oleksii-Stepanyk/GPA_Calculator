#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "GlobalConfig.h"

#include <QSettings>
#include <QCloseEvent>
#include <QStyle>
#include <QStyleFactory>
#include <QPalette>
#include <QApplication>

using namespace GlobalConfig;

/* ==============================
TODO:
1. Reorder class methods
2. Separate different types
   of methods into separate files
============================== */

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::saveSettings()
{
    QSettings settings("UkhylSystems", "GPACalculator");

    settings.setValue("isDarkMode", m_isDarkMode);

    settings.remove("courses");
    settings.beginWriteArray("courses");

    for (int i = 0; i < m_courses.size(); ++i) {
        settings.setArrayIndex(i);

        settings.setValue("name", m_courses[i]->getTitle());
        settings.setValue("credits", m_courses[i]->getCredits());
        settings.setValue("grade", m_courses[i]->getGrade());
    }
    settings.endArray();
    settings.sync();
}

void MainWindow::loadSettings()
{
    QSettings settings("UkhylSystems", "GPACalculator");

    bool savedTheme = settings.value("isDarkMode", false).toBool();
    ui->themeToggle->setChecked(savedTheme);
    applyTheme(savedTheme);

    int size = settings.beginReadArray("courses");

    // TODO: Separate card creation
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        CourseCard* newCard = new CourseCard(this);
        connect(newCard, &CourseCard::removeRequested, this, &MainWindow::removeCourse);
        connect(newCard, &CourseCard::cardChanged, this, &MainWindow::calculateGPA);

        newCard->setTitle(settings.value("name").toString());
        newCard->setCredits(settings.value("credits").toInt());
        newCard->setGrade(settings.value("grade").toDouble());

        m_courses.append(newCard);
    }
    settings.endArray();

    reclusterGrid();
    calculateGPA();
}

// TODO: Move to a separate file (Make a factory for example)
void MainWindow::applyTheme(bool dark)
{
    m_isDarkMode = dark;

    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette p;

    if (dark) {
        p.setColor(QPalette::Window, QColor(53, 53, 53));
        p.setColor(QPalette::WindowText, Qt::white);
        p.setColor(QPalette::Base, QColor(25, 25, 25));
        p.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        p.setColor(QPalette::ToolTipBase, Qt::white);
        p.setColor(QPalette::ToolTipText, Qt::white);
        p.setColor(QPalette::Text, Qt::white);
        p.setColor(QPalette::Button, QColor(53, 53, 53));
        p.setColor(QPalette::ButtonText, Qt::white);
        p.setColor(QPalette::BrightText, Qt::red);
        p.setColor(QPalette::Link, QColor(42, 130, 218));
        p.setColor(QPalette::Highlight, QColor(42, 130, 218));
        p.setColor(QPalette::HighlightedText, Qt::black);

        p.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
        p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
    }
    else {
        p.setColor(QPalette::Window, QColor(240, 240, 240));
        p.setColor(QPalette::WindowText, Qt::black);
        p.setColor(QPalette::Base, Qt::white);
        p.setColor(QPalette::AlternateBase, QColor(245, 245, 245));
        p.setColor(QPalette::ToolTipBase, Qt::white);
        p.setColor(QPalette::ToolTipText, Qt::black);
        p.setColor(QPalette::Text, Qt::black);
        p.setColor(QPalette::Button, QColor(240, 240, 240));
        p.setColor(QPalette::ButtonText, Qt::black);
        p.setColor(QPalette::BrightText, Qt::red);
        p.setColor(QPalette::Link, QColor(0, 0, 255));
        p.setColor(QPalette::Highlight, QColor(42, 130, 218));
        p.setColor(QPalette::HighlightedText, Qt::white);

        p.setColor(QPalette::Disabled, QPalette::Text, Qt::gray);
        p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::gray);
    }

    qApp->setPalette(p);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_gridLayout = qobject_cast<QGridLayout*>(ui->containerWidget->layout());
    for(int i = 0; i < GRID_COLUMNS; i++) {
        m_gridLayout->setColumnMinimumWidth(i, 240);
    }
    int horSpacing = (WINDOW_WIDTH - GRID_COLUMNS * 240) / GRID_COLUMNS;

    m_gridLayout->setHorizontalSpacing(horSpacing);
    m_gridLayout->setVerticalSpacing(22);
    m_gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft | Qt::AlignCenter);

    ui->outputLabel->setText(QString::fromStdString(GPA_STRING) + QString::number(GPA_DEFAULT_VALUE, 'f', 2));
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculateGPA()
{
    double totalPoints = 0.0;
    double totalCredits = 0.0;

    for (int i = 0; i < m_courses.size(); ++i) {
        double grade = m_courses[i]->getGrade();
        double credits = m_courses[i]->getCredits();

        if (!(credits > 0.0)) {
            continue;
        }

        totalPoints += (grade * credits);
        totalCredits += credits;
    }

    if (totalCredits == 0.0) {
        ui->outputLabel->setText(QString::fromStdString(GPA_STRING) + QString::number(GPA_DEFAULT_VALUE, 'f', 2));
        return;
    }

    double gpa = totalPoints / totalCredits;
    ui->outputLabel->setText(QString::fromStdString(GPA_STRING) + QString::number(gpa, 'f', 2));
}

void MainWindow::on_clearButton_clicked(){
    for (int i = 0; i < m_courses.size(); ++i) {
        CourseCard* card = m_courses[i];
        m_gridLayout->removeWidget(card);
        card->deleteLater();
    }
    m_courses.clear();
    reclusterGrid();
}

void MainWindow::on_themeToggle_clicked()
{
    applyTheme(!m_isDarkMode);
}

void MainWindow::on_addCourse_clicked()
{
    CourseCard* newCard = new CourseCard(this);
    connect(newCard, &CourseCard::removeRequested, this, &MainWindow::removeCourse);
    connect(newCard, &CourseCard::cardChanged, this, &MainWindow::calculateGPA);

    int index = m_courses.count();
    int row = index / GRID_COLUMNS;
    int col = index % GRID_COLUMNS;

    m_gridLayout->addWidget(newCard, row, col);
    m_courses.append(newCard);
    reclusterGrid();
}

void MainWindow::reclusterGrid()
{
    for (int i = 0; i < m_courses.size(); ++i) {
        int row = i / GRID_COLUMNS;
        int col = i % GRID_COLUMNS;
        m_gridLayout->addWidget(m_courses[i], row, col);
        m_gridLayout->setRowMinimumHeight(row, 240);
        m_courses[i]->setVisible(true);
        m_courses[i]->show();
    }
    int btnIndex = m_courses.size();
    int btnRow = btnIndex / GRID_COLUMNS;
    int btnCol = btnIndex % GRID_COLUMNS;

    m_gridLayout->setRowMinimumHeight(btnRow, 240);
    m_gridLayout->removeWidget(ui->addCourse);
    m_gridLayout->addWidget(ui->addCourse, btnRow, btnCol, Qt::AlignCenter);
}

void MainWindow::removeCourse(CourseCard* card)
{
    m_gridLayout->removeWidget(card);
    m_courses.removeOne(card);
    card->deleteLater();

    reclusterGrid();
    calculateGPA();
}
