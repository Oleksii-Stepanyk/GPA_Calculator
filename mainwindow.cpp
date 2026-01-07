#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "GlobalConfig.h"

using namespace GlobalConfig;

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
    reclusterGrid();
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
        m_gridLayout->addWidget(m_courses[i], i / GRID_COLUMNS, i % GRID_COLUMNS);
        m_courses[i]->setVisible(true);
    }
    int btnIndex = m_courses.size();
    int btnRow = btnIndex / GRID_COLUMNS;
    int btnCol = btnIndex % GRID_COLUMNS;

    if (btnCol == 0)
    {
        m_gridLayout->setRowMinimumHeight(btnRow, 240);
    }
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
