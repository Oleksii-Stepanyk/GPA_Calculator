#ifndef COURSECARD_H
#define COURSECARD_H

#include <QWidget>

namespace Ui {
class CourseCard;
}

class CourseCard : public QWidget
{
    Q_OBJECT

public:
    explicit CourseCard(QWidget *parent = nullptr);
    ~CourseCard();
    double getGrade();
    double getCredits();

signals:
    void removeRequested(CourseCard* card);

private slots:
    void on_removeCourse_clicked();

private:
    Ui::CourseCard *ui;
};

#endif // COURSECARD_H
