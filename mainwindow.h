#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QList>
#include <QGridLayout>

#include "coursecard.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void saveSettings();
    void loadSettings();

private slots:
    void on_clearButton_clicked();
    void on_addCourse_clicked();

private:
    Ui::MainWindow *ui;

    QList<CourseCard*> m_courses;
    QGridLayout* m_gridLayout;

    void calculateGPA();
    void reclusterGrid();
    void removeCourse(CourseCard* card);
};
#endif // MAINWINDOW_H
