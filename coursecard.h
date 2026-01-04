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

private:
    Ui::CourseCard *ui;
};

#endif // COURSECARD_H
