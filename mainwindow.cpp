#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_gridLayout = qobject_cast<QGridLayout*>(ui->containerWidget->layout());
    for(int i = 0; i < 4; i++) {
        m_gridLayout->setColumnMinimumWidth(i, 320);
    }
    reclusterGrid();
    m_gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
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
        ui->outputLabel->setText("GPA: 0.00");
        return;
    }

    double gpa = totalPoints / totalCredits;
    ui->outputLabel->setText("GPA: " + QString::number(gpa, 'f', 2));
}

void MainWindow::on_addCourse_clicked()
{
    CourseCard* newCard = new CourseCard(this);
    connect(newCard, &CourseCard::removeRequested, this, &MainWindow::removeCourse);

    int index = m_courses.count();
    int row = index / 4;
    int col = index % 4;

    m_gridLayout->addWidget(newCard, row, col);
    m_courses.append(newCard);
    reclusterGrid();
}

void MainWindow::reclusterGrid()
{
    for (int i = 0; i < m_courses.size(); ++i) {
        m_gridLayout->addWidget(m_courses[i], i / 4, i % 4);
        m_courses[i]->setVisible(true);
    }
    int btnIndex = m_courses.size();
    int btnRow = btnIndex / 4;
    int btnCol = btnIndex % 4;

    if (btnCol == 0)
    {
        m_gridLayout->setRowMinimumHeight(btnRow, 285);
    }
    m_gridLayout->removeWidget(ui->addCourse);
    m_gridLayout->addWidget(ui->addCourse, btnIndex / 4, btnIndex % 4);
}

void MainWindow::removeCourse(CourseCard* card)
{
    m_gridLayout->removeWidget(card);
    m_courses.removeOne(card);
    card->deleteLater();

    reclusterGrid();
    on_calculateButton_clicked();
}
