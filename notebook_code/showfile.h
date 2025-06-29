#ifndef SHOWFILE_H
#define SHOWFILE_H

//#include"showfileedit.h"
#include"ai_linking.h"
#include<QDialog>
#include<QPainter>
#include<QPaintEvent>
#include<markdownhighlighter.h>

namespace Ui {
class ShowFile;
}

class ShowFile : public QDialog
{
    Q_OBJECT

public:
    explicit ShowFile(QWidget *parent = nullptr);
    ~ShowFile();
    void loadFile(const QString &filePath);  // 加载文件内容
    void paintEvent(QPaintEvent*);
    bool is_opened = 0;
    int reviewCount = 0;
    QStringList parts;
    QString flpth;
    MarkdownHighlighter* highlighter;

private slots:
    void on_closeButton_clicked();

    void on_checkButton_clicked();

    void on_ai_chating_Button_clicked();

    void on_ai_solving_Button_clicked();

private:
    Ui::ShowFile *ui;
};

#endif // SHOWFILE_H
