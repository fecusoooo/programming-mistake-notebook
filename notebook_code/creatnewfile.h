#ifndef CREATNEWFILE_H
#define CREATNEWFILE_H

#include <QDialog>
#include<QPainter>
#include<QPaintEvent>
#include<QLabel>
#include<QPropertyAnimation>
#include"markdownhighlighter.h"

namespace Ui {
class CreatNewFile;
}

class CreatNewFile : public QDialog
{
    Q_OBJECT

public:
    explicit CreatNewFile(QWidget *parent = nullptr);
    ~CreatNewFile();
    void paintEvent(QPaintEvent*);
    bool is_saved = false;
    MarkdownHighlighter* highlighter;

private slots:
    void on_pushButton_clicked();

    void on_helpButton_clicked();

private:
    Ui::CreatNewFile *ui;
    QLabel *helpNotification;    // 帮助通知标签
    QPropertyAnimation *notificationAnim;  // 通知栏动画

signals:
    void themeChanged(int theme);  // 0=light, 1=dark
};

#endif // CREATNEWFILE_H
