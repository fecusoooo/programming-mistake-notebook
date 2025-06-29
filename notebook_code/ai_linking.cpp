#include "ai_linking.h"
#include "ui_ai_linking.h"
#include"login.h"
#include<QPainter>
#include"thememanager.h"
#include <QTextCursor>  // 添加光标支持
#include<QCloseEvent>
#include<QPointer>

AI_Linking::AI_Linking(int m,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AI_Linking)
{
    ui->setupUi(this);
    mode=m;
    if(mode==1)ui->label->setText("错题分析模式");
    else ui->label->setText("交谈模式");
    manager = new QNetworkAccessManager(this);

    QPixmap model_image(":/imag/model.png");

    ui->model->setPixmap(model_image);
    ui->model->setScaledContents(true);
}
AI_Linking::~AI_Linking()
{
    // 确保在析构时取消所有请求
    abortCurrentRequest();
    manager->deleteLater();
    delete ui;
}

void AI_Linking::abortCurrentRequest()
{
    if (currentReply) {
        // 断开所有信号连接
        QObject::disconnect(currentReply, 0, 0, 0);
        currentReply->abort();
        currentReply->deleteLater();
        currentReply = nullptr;
    }
}

// 重写关闭事件
void AI_Linking::closeEvent(QCloseEvent *event)
{
    abortCurrentRequest();  // 窗口关闭时终止请求
    event->accept();        // 接受关闭事件
}

void AI_Linking::paintEvent(QPaintEvent*){
    //绘制图片背景（保持比例缩放）
    if(login::theme_now==0){
        QPainter painter(this);
        QPixmap bgImage(":/background/login.png");
        painter.drawPixmap(rect(), bgImage.scaled(size(), Qt::KeepAspectRatioByExpanding));
        ThemeManager::applyTheme(this,0);
    }else{
        ThemeManager::applyTheme(this,1);
    }
}

void AI_Linking::link(const QString &message)
{
    abortCurrentRequest();  // 开始新请求前终止旧请求
    // // 如果已有请求在进行，先取消
    // if (currentReply) {
    //     currentReply->abort();
    //     currentReply->deleteLater();
    //     currentReply = nullptr;
    // }

    accumulatedResponse.clear();  // 清空累积响应
    ui->o_textEdit->clear();      // 清空显示区域

    const QString API_KEY = "sk-de825dccc1a24cbf8fee35dfc01790ac";  // 替换为你的API密钥
    const QString API_URL = "https://api.deepseek.com/v1/chat/completions";

    // 创建请求
    QNetworkRequest request;
    request.setUrl(QUrl(API_URL));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", ("Bearer " + API_KEY).toUtf8());

    // 构建JSON请求体
    QJsonArray messages;
    QJsonObject systm_message;
    systm_message["role"] = "system";
    if(mode==1)systm_message["content"] = "你是一个有丰富经验的教育工作者，请根据以下信息给出下面题目的答案并根据错因进行题目分析";
    else systm_message["content"] = "";

    QJsonObject user_message;
    user_message["role"] = "user";
    QString text;
    if(mode==1){
        text = "错题名称" + title + "\n" + "错题标签" +  label + "\n" + "错题原因" + reason +"\n" + "题目内容" + content + "\n" +message;
    }
    else text = message;
    user_message["content"] = text;

    messages.append(systm_message);
    messages.append(user_message);

    QJsonObject requestBody;
    requestBody["model"] = "deepseek-chat";
    requestBody["messages"] = messages;
    requestBody["max_tokens"] = 400;
    requestBody["stream"] = true;  // 关键：启用流式传输

    QByteArray jsonData = QJsonDocument(requestBody).toJson();

    // 发送POST请求
    currentReply = manager->post(request, jsonData);

    // 修改lambda捕获方式，使用weak pointer确保安全
    QPointer<AI_Linking> weakThis(this);

    // 处理流式数据
    QObject::connect(currentReply, &QNetworkReply::readyRead, [weakThis,this]() {
        if (!weakThis) return;  // 检查窗口是否已销毁
        while (currentReply->canReadLine()) {
            QByteArray line = currentReply->readLine().trimmed();

            if (line.startsWith("data: ")) {
                QByteArray jsonData = line.mid(6);  // 去掉"data: "

                if (jsonData == "[DONE]") {
                    // 流结束
                    return;
                }

                // 解析JSON
                QJsonParseError error;
                QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

                if (error.error == QJsonParseError::NoError) {
                    QJsonObject root = doc.object();
                    if (root.contains("choices")) {
                        QJsonArray choices = root["choices"].toArray();
                        if (!choices.isEmpty()) {
                            QJsonObject choice = choices[0].toObject();
                            if (choice.contains("delta")) {
                                QJsonObject delta = choice["delta"].toObject();
                                if (delta.contains("content")) {
                                    QString content = delta["content"].toString();

                                    // 累积响应
                                    accumulatedResponse.append(content);

                                    // 更新UI（使用文本光标提高性能）
                                    QTextCursor cursor(ui->o_textEdit->textCursor());
                                    cursor.movePosition(QTextCursor::End);
                                    cursor.insertText(content);

                                    // 自动滚动到底部
                                    ui->o_textEdit->ensureCursorVisible();
                                }
                            }
                        }
                    }
                }
            }
        }
    });

    // 处理请求完成
    QObject::connect(currentReply, &QNetworkReply::finished, [weakThis,this]() {
        if (!weakThis) return;  // 检查窗口是否已销毁
        if (currentReply->error() != QNetworkReply::NoError) {
            if (currentReply->error() != QNetworkReply::OperationCanceledError) {
                qDebug() << "请求错误：" << currentReply->errorString();
                ui->o_textEdit->append("\n[错误: " + currentReply->errorString() + "]");
            }
        } else {
            qDebug() << "流式传输完成";
        }
        currentReply->deleteLater();
        currentReply = nullptr;
    });
}

void AI_Linking::on_pushButton_clicked()
{
    link(ui->i_textEdit->toPlainText().trimmed());
}

