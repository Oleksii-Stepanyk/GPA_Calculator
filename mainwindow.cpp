#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_gridLayout = qobject_cast<QGridLayout*>(ui->centralwidget->layout());
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
