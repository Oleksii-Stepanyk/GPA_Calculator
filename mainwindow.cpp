#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "GlobalConfig.h"

#include <QSettings>
#include <QCloseEvent>

using namespace GlobalConfig;

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::saveSettings()
{
    QSettings settings("UkhylSystems", "GPACalculator");

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

    int size = settings.beginReadArray("courses");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        CourseCard* newCard = new CourseCard(this);
        connect(newCard, &CourseCard::removeRequested, this, &MainWindow::removeCourse);

        newCard->setTitle(settings.value("name").toString());
        newCard->setCredits(settings.value("credits").toInt());
        newCard->setGrade(settings.value("grade").toDouble());

        m_courses.append(newCard);
    }
    settings.endArray();

    reclusterGrid();
    ui->containerWidget->updateGeometry();
    ui->scrollArea->update();
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

void MainWindow::on_calculateButton_clicked()
{
    double totalPoints = 0.0;
    double totalCredits = 0.0;

    for (int i = 0; i < m_courses.size(); ++i) {
        double grade = m_courses[i]->getGrade();
        double credits = m_courses[i]->getCredits();

        if (credits <= 0.0) {
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

void MainWindow::on_addCourse_clicked()
{
    CourseCard* newCard = new CourseCard(this);
    connect(newCard, &CourseCard::removeRequested, this, &MainWindow::removeCourse);

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
    on_calculateButton_clicked();
}
