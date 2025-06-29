#ifndef AI_LINKING_H
#define AI_LINKING_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

namespace Ui {
class AI_Linking;
}

class AI_Linking : public QMainWindow
{
    Q_OBJECT

public:
    explicit AI_Linking(int m,QWidget *parent = nullptr);
    ~AI_Linking();
    void link(const QString &message);
    int mode;
    QString title;
    QString label;
    QString reason;
    QString content;
    QString str_output;
    void paintEvent(QPaintEvent*);

private slots:
    void on_pushButton_clicked();

private:
    Ui::AI_Linking *ui;
    QNetworkAccessManager *manager;
    QNetworkReply* currentReply = nullptr;  // 跟踪当前请求
    QString accumulatedResponse;            // 累积响应内容
    void abortCurrentRequest();  // 添加请求终止函数

protected:
    void closeEvent(QCloseEvent *event) override;  // 添加关闭事件重写
};

#endif // AI_LINKING_H
