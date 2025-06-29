#ifndef SHOWFILEEDIT_H
#define SHOWFILEEDIT_H

#include <QDialog>
#include<QPainter>
#include<QPaintEvent>
#include"markdownhighlighter.h"
#include"ai_linking.h"

namespace Ui {
class ShowFileEdit;
}

class ShowFileEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ShowFileEdit(QWidget *parent = nullptr);
    ~ShowFileEdit();

    void loadFile(const QString &filePath);  // 加载文件内容
    void paintEvent(QPaintEvent*);

    bool is_opened=0;
    int reviewCount=0;
    QStringList parts;

    MarkdownHighlighter* highlighter;

private slots:
    void on_saveButton_clicked();

    void on_checkButton_clicked();

    void on_ai_chating_Button_clicked();

    void on_ai_solving_Button_clicked();

private:
    Ui::ShowFileEdit *ui;

    QString currentFilePath;       // 当前编辑的文件路径
};

#endif // SHOWFILEEDIT_H
