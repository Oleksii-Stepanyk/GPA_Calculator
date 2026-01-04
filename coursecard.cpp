#include "coursecard.h"
#include "ui_coursecard.h"

CourseCard::CourseCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CourseCard)
{
    ui->setupUi(this);
}

CourseCard::~CourseCard()
{
    delete ui;
}

double CourseCard::getGrade(){
    return ui->gradeEdit->value();
}

double CourseCard::getCredits(){
    return ui->creditEdit->value();
}
