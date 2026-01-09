#include "coursecard.h"
#include "ui_coursecard.h"

CourseCard::CourseCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CourseCard)
{
    ui->setupUi(this);

    connect(ui->creditEdit, &QSpinBox::valueChanged, this, &CourseCard::cardChanged);
    connect(ui->gradeEdit, &QDoubleSpinBox::valueChanged, this, &CourseCard::cardChanged);
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

QString CourseCard::getTitle(){
    return ui->nameEdit->text();
}

void CourseCard::setGrade(double grade){
    ui->gradeEdit->setValue(grade);
}

void CourseCard::setCredits(double grade){
    ui->creditEdit->setValue(grade);
}

void CourseCard::setTitle(const QString& title){
    ui->nameEdit->setText(title);
}

void CourseCard::on_removeCourse_clicked()
{
    emit removeRequested(this);
}
