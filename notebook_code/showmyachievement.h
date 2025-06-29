#ifndef SHOWMYACHIEVEMENT_H
#define SHOWMYACHIEVEMENT_H

#include <QDialog>
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>

namespace Ui {
class ShowMyAchievement;
}

class ShowMyAchievement : public QDialog
{
    Q_OBJECT

public:
    explicit ShowMyAchievement(QWidget *parent = nullptr);
    void setAchievements(bool ach_1, bool ach_2, bool ach_3,
                         bool ach_4, bool ach_5, bool ach_6);

private:
    QLabel *createAchievementLabel(const QString &text, bool achieved);
};

#endif // SHOWMYACHIEVEMENT_H
