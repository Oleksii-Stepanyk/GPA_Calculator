#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculateButton_clicked()
{
    double totalPoints = 0.0;
    double totalCredits = 0.0;

    for (int i = 0; i < m_gradeInputs.size(); ++i) {
        double grade = m_gradeInputs[i]->value();
        double credits = m_creditInputs[i]->value();

        if (credits > 0.0) {
            totalPoints += (grade * credits);
            totalCredits += credits;
        }
    }

    if (totalCredits == 0.0) {
        ui->outputLabel->setText("GPA: 0.00");
        return;
    }

    double gpa = totalPoints / totalCredits;
    ui->outputLabel->setText("GPA: " + QString::number(gpa, 'f', 2));
}
