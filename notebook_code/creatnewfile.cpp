#include "creatnewfile.h"
#include "ui_creatnewfile.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include<QDir>
#include<QFile>
#include<iostream>
#include<QPainter>
#include<QPixmap>
#include<QTimer>
#include<QPropertyAnimation>
#include"login.h"
#include"thememanager.h"
#include"markdownhighlighter.h"

CreatNewFile::CreatNewFile(QWidget *parent)
    : QDialog(parent),
    highlighter(nullptr)
    , ui(new Ui::CreatNewFile)
{
    ui->setupUi(this);
    std::cout<<"1"<<std::endl;
    //connect(ui->pushButton, &QPushButton::clicked, this, &NewDialog::handleButtonClick);
    //connect(ui->pushButton,&QPushButton::clicked,this,&CreatNewFile::on_pushButton_clicked);
    QPixmap image_title(":/imag/title.png");
    QPixmap image_tag(":/imag/label.png");
    QPixmap image_des(":/imag/description.png");
    QPixmap image_date(":/imag/date.png");

    ui->title->setPixmap(image_title);
    ui->tag->setPixmap(image_tag);
    ui->reason->setPixmap(image_tag);
    ui->description->setPixmap(image_des);
    ui->date->setPixmap(image_date);

    ui->title->setScaledContents(true);
    ui->tag->setScaledContents(true);
    ui->reason->setScaledContents(true);
    ui->description->setScaledContents(true);
    ui->date->setScaledContents(true);

    ui->dateEdit->setDate(QDate::currentDate()); // 默认当前日期
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd"); // 设置日期格式

    helpNotification = new QLabel(this);
    helpNotification->setStyleSheet(
        "background-color: rgba(240, 247, 255, 220);"  // 半透明背景
        "border: 1px solid #4da6ff;"
        "border-radius: 8px;"
        "padding: 15px;"
        "font: bold 14px 'Microsoft YaHei';"
        "color: #0066cc;"
        );
    helpNotification->setAlignment(Qt::AlignCenter);
    helpNotification->setFixedSize(350, 200);  // 设置合适的大小
    helpNotification->setText(
        "帮助信息：\n"
        "1. 填写标题（必填）\n"
        "2. 添加标签（必填）\n"
        "3. 选择日期（默认今天）\n"
        "4. 选择错因分类（必选）\n"
        "5. 输入描述（支持markdown格式）\n"
        "6. 点击保存"
        );
    helpNotification->hide();  // 初始隐藏

    // 创建动画
    notificationAnim = new QPropertyAnimation(helpNotification, "pos", this);
    notificationAnim->setDuration(500);  // 动画时长500ms
    notificationAnim->setEasingCurve(QEasingCurve::OutBack);  // 平滑的动画曲线

    if(!highlighter){
        highlighter = new MarkdownHighlighter(ui->plainTextEdit->document());
    }else{
        highlighter->setDocument(ui->plainTextEdit->document());
    }

    // 可选：设置字体为等宽字体，更适合代码显示
    QFont font("Consolas", 10);
    ui->plainTextEdit->setFont(font);
}

CreatNewFile::~CreatNewFile()
{
    delete ui;
    delete highlighter;
    highlighter = nullptr;

    delete helpNotification;
    delete notificationAnim;
}

void CreatNewFile::paintEvent(QPaintEvent* event){
    //绘制图片背景（保持比例缩放）
    if(login::theme_now==0){
    QPainter painter(this);
    QPixmap bgImage(":/background/seefile.png");
    painter.drawPixmap(rect(), bgImage.scaled(size(), Qt::KeepAspectRatioByExpanding));
    ThemeManager::applyTheme(this,0);
    }else{
        ThemeManager::applyTheme(this,1);
    }
}

void CreatNewFile::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    if(name.isEmpty()){
        QMessageBox::warning(this,"警告","标题为空！");
        return;
    }
    QString date = ui->dateEdit->date().toString("yyyy-MM-dd"); // 获取日期
    QString lable = ui->lineEdit_2->text();
    QString content = ui->plainTextEdit->toPlainText();
    // QString filename = QFileDialog::getSaveFileName(
    //     this,
    //     "保存文件",
    //     QDir::homePath(),
    //     "文本文件 (*.txt);;所有文件 (*)"
    //     );
    if(name.isEmpty()) return;

    QString saveDir = "save";
    QString fileName = name+".txt";
    QDir dir;
    if (!dir.exists(saveDir)) {
        if (!dir.mkdir(saveDir)) {
            // 目录创建失败，处理错误（如输出日志或返回）
            qDebug() << "无法创建目录：" << saveDir;
            return;
        }
    }
    std::cout<<dir.absolutePath().toStdString()<<std::endl;
    // 构造完整文件路径（跨平台兼容）
    QString filePath = dir.filePath(saveDir + "/" + fileName);

    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        is_saved=1;
        QTextStream out(&file);
        bool flg1 = ui->checkBox_1->isChecked();
        bool flg2 = ui->checkBox_2->isChecked();
        bool flg3 = ui->checkBox_3->isChecked();
        bool flg4 = ui->checkBox_4->isChecked();
        bool flg5 = ui->checkBox_5->isChecked();
        bool flg6 = ui->checkBox_6->isChecked();

        out << name << Qt::endl << lable << Qt::endl<< "0" << Qt::endl;
        out << flg1 << " " << flg2 << " "<< flg3 <<" " << flg4 <<" " << flg5 <<" " << flg6 <<Qt::endl;
        out << date << Qt::endl;
        out << content << Qt::endl;
        file.close();
        QMessageBox::information(this, "成功", "成功保存");

        this->close();
    } else {
        QMessageBox::critical(this, "illegal title", "only letters and numbers and underscore can be included");
    }

    // std::ifstream file(name);
    // if(!file){
    //     std::cerr<<"fail to open:"<<name<<std::endl;
    //     return;
    // }
    // else file>>name>>lable>>content;
    // file.close();
}

void CreatNewFile::on_helpButton_clicked()
{
    if (notificationAnim->state() == QAbstractAnimation::Running) {
        notificationAnim->stop();
    }

    // 设置通知栏位置（居中显示）
    int startY = -helpNotification->height();  // 从屏幕上方开始
    int endY = 50;  // 最终位置（距顶部50像素）
    int x = (this->width() - helpNotification->width()) / 2;

    // 设置动画参数
    notificationAnim->setStartValue(QPoint(x, startY));
    notificationAnim->setEndValue(QPoint(x, endY));

    // 显示通知栏并启动动画
    helpNotification->show();
    helpNotification->raise();  // 确保在最上层
    notificationAnim->start();

    // 3秒后自动隐藏通知
    QTimer::singleShot(5000, this, [this]() {
        // 设置隐藏动画
        notificationAnim->setStartValue(helpNotification->pos());
        notificationAnim->setEndValue(QPoint(helpNotification->x(), -helpNotification->height()));

        // 动画结束后隐藏
        connect(notificationAnim, &QPropertyAnimation::finished, this, [this]() {
            helpNotification->hide();
            disconnect(notificationAnim, &QPropertyAnimation::finished, this, nullptr);
        });

        notificationAnim->start();
    });
}

