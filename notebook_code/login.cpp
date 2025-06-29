#include "login.h"
#include "ui_login.h"
#include <iostream>
#include"thememanager.h"
#include <fstream>
#include<QDir>
#include<QFile>
#include <QTextStream>
#include<QMessageBox>
#include<QPixmap>
#include<QIcon>
#include<QPainter>
#include<QSettings>
#include"mainwindow.h"

using namespace std;
login::login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::login)
{
    /*
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("MyCompany", "CheckInApp", this);
*/
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/mainwindow.png"));
    setWindowTitle("登录");
    loadMapFromTextFile(map,"account.txt");
    ui->input_password->setEchoMode(QLineEdit::Password);
    ui->PassWordButton->setIcon(QIcon(":/imag/eye.png"));
    //ui->PassWordButton->setIcon(QIcon(":/imag/eye1.jpg"));
    QPixmap image_name(":/imag/person.png");
    QPixmap image_key(":/imag/password.png");

    ui->name_pic->setPixmap(image_name);
    ui->name_pic->setScaledContents(true);

    ui->password_pic->setPixmap(image_key);
    ui->password_pic->setScaledContents(true);

    settings = new QSettings("MyCompany", "CheckInApp", this);

    // 加载主题设置
    //QString theme = settings->value("theme", "light").toString();
    // 切换主题
    // if (theme == "light") {
    //     theme_now = 0;
    // } else {
    //     theme_now = 1;
    // }

    if(theme_now==0){
        ThemeManager::applyTheme(this,0);
    }else{
        ThemeManager::applyTheme(this,1);
    }
}

bool login::theme_now = 0;

login::~login()
{
    delete ui;
}

void login::paintEvent(QPaintEvent* event){
    //绘制图片背景（保持比例缩放）
    QPainter painter(this);
    QPixmap bgImage(":/background/login.png");
    painter.drawPixmap(rect(), bgImage.scaled(size(), Qt::KeepAspectRatioByExpanding));
}

void login::saveMapToTextFile(const std::unordered_map<std::string, std::string>& map, const std::string& filename) {
    QString saveDir = "account";
    QString fileName = QString::fromStdString(filename);
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
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << file.errorString();
        return;
    }
    QTextStream out(&file);
    for (const auto& pair : map) {
        out << QString::fromStdString(pair.first) << ":"
            << QString::fromStdString(pair.second) << "\n";
    }
    file.close();
}

// 存储用户数据
void login::loadMapFromTextFile(std::unordered_map<std::string, std::string>& map,const std::string& filename) {
    QString saveDir = "account";
    QString fileName = QString::fromStdString(filename);
    QDir dir;
    if (!dir.exists(saveDir)) {
        qDebug() << "没有存储目录：" << saveDir;
        return;
    }
    std::cout<<dir.absolutePath().toStdString()<<std::endl;
    QString filePath = dir.filePath(saveDir + "/" + fileName);
    QFile file(filePath);

    // 尝试打开文件
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << file.errorString();
        return ; // 返回空map
    }

    QTextStream in(&file);

    // 逐行读取
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed(); // 去除首尾空白
        if (line.isEmpty()) continue; // 跳过空行

        // 按分隔符":"拆分键值
        QStringList parts = line.split(":");
        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();
            map[key.toStdString()] = value.toStdString();
        } else {
            qDebug() << "忽略无效行：" << line;
        }
    }
    file.close();
}

// 检查用户密码
bool login::checkaccount(std::string name,std::string password){
    if(map[name]==password)return true;
    else return false;
}

// 检查用户是否存在
bool login::checkname(std::string name){
    if(map.find(name)!=map.end())return true;
    return false;
}

// 创建用户按钮
void login::on_create_clicked()
{
    crt_name = ui->input_name->text().toStdString();
    crt_password = ui->input_password->text().toStdString();
    if(checkname(crt_name)){
        //cerr<<"name has been used"<<endl;
        QMessageBox::critical(this, "error", "name has been used");
        return;
    }
    map.insert(make_pair(crt_name,crt_password));
    QMessageBox::information(this, "succeeded", "succeed to create new account");
    return;
}

// 登录按钮
void login::on_login_accout_clicked()
{
    crt_name = ui->input_name->text().toStdString();
    crt_password = ui->input_password->text().toStdString();
    if(!checkname(crt_name)){
       QMessageBox::critical(this, "error", "no such account");
        return;
    }
    if(!checkaccount(crt_name,crt_password)){
        QMessageBox::critical(this, "error", "false password");
        return;
    }
    cout<<"true password"<<endl;

    is_login=1;
    //QMessageBox::information(this, "succeeded", "succeed to log in");
    saveMapToTextFile(map,"account.txt");
    emit right_login();
}

// 忘记密码按钮
void login::on_forget_clicked()
{
    crt_name = ui->input_name->text().toStdString();
    crt_password = ui->input_password->text().toStdString();
    if(!checkname(crt_name)){
        cerr<<"no such account"<<endl;
    }
    return;
    map[crt_name]=crt_password;
    return;
}

// 关闭窗口
void login::on_pushButton_clicked()
{
    this->close();
}

// 是否显示密码函数
void login::on_PassWordButton_clicked()
{
    if(isPassWordShowed){
        isPassWordShowed=false;
        ui->input_password->setEchoMode(QLineEdit::Password);
    }
    else{
        isPassWordShowed=true;
        ui->input_password->setEchoMode(QLineEdit::Normal);
    }
}

// themeButton 的点击事件处理
void login::on_themeButton_clicked()
{
    login::theme_now = !login::theme_now;
    ThemeManager::applyTheme(this, login::theme_now);

}

