#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showfileedit.h"
#include "creatnewfile.h"
#include "showmyachievement.h"
#include "showfile.h"
#include"thememanager.h"
#include"showreviewstatistics.h"
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include<iostream>
#include <QUrl>
#include<QPainter>
#include<QIcon>
#include<QRandomGenerator>
#include <QtCharts>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("MyCompany", "CheckInApp", this);
    //std::cout<<"llll"<<std::endl;
    ui->searchModeButton->setText("当前模式: 按标题");
    ui->reviewStatsButton->setText("复习情况");
    searchByFilename = true;
    // 从设置中获取当前主题
    //settings2 = new QSettings("MyCompany", "CheckInApp", this);
    //QString currentTheme = settings2->value("theme", "light").toString();
    // 切换主题

    // 加载打卡数据
    loadCheckInData();

    // 更新UI显示
    updateCheckInUI();

    setWindowTitle("PKU智能错题本");
    setWindowIcon(QIcon(":/icons/mainwindow.png"));
    this->hide();
    l = new login();
    connect(l,&login::right_login,[=](){
        l->setAttribute(Qt::WA_DeleteOnClose);                  // 关闭时释放窗口
        ui->name_label->setText(QString::fromStdString(l->crt_name));
        userTxt = l->crt_name+".txt";
        loadFromFile(userTxt);
        updateCheckInUI();
        std::cout << "kick"<<std::endl;
        if(l->is_login)ach_1=1;

        l->close();                                             // 关闭单机模式界面
        this->show();                                           // 重新显示菜单界面

        l = nullptr;
        std::cout<<"yes"<<std::endl;

    });

    l->show();

}


MainWindow::~MainWindow()
{
    saveToFile(userTxt);
    delete ui;
}

/*
    void MyWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // 绘制纯色背景
    painter.fillRect(rect(), QColor(255, 0, 0));

    // 绘制图片背景（保持比例缩放）
    // QPixmap bgImage(":/images/background.jpg");
    // painter.drawPixmap(rect(), bgImage.scaled(size(), Qt::KeepAspectRatioByExpanding));

    // 绘制渐变背景
    // QLinearGradient gradient(0, 0, width(), height());
    // gradient.setColorAt(0, Qt::white);
    // gradient.setColorAt(1, Qt::black);
    // painter.fillRect(rect(), gradient);
}
    */
void MainWindow::paintEvent(QPaintEvent* event){
    //绘制图片背景（保持比例缩放）
    if(login::theme_now==0){
    QPainter painter(this);
    QPixmap bgImage(":/background/background.png");
    painter.drawPixmap(rect(), bgImage.scaled(size(), Qt::KeepAspectRatioByExpanding));
    ThemeManager::applyTheme(this,0);
    }else{
        ThemeManager::applyTheme(this,1);
    }
}

// 用户数据存储
void MainWindow::saveToFile(const std::string& filename){
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
    if(ach_1)out<<"1";
    else out<<"0";
    if(ach_2)out<<"1";
    else out<<"0";
    if(ach_3)out<<"1";
    else out<<"0";
    if(ach_4)out<<"1";
    else out<<"0";
    if(ach_5)out<<"1";
    else out<<"0";
    if(ach_6)out<<"1";
    else out<<"0";

    out<<" ";
    out<<cnt_save;
    out<<" ";
    out<<cnt_reveal;

    // 保存打卡数据
    out << " " << totalCheckInDays;
    out << " " << consecutiveCheckInDays;
    out << " " << lastCheckInDate.toString("yyyy.MM.dd");

    // 保存打卡历史
    out << " " << checkInHistory.size();
    for (const QDate &date : checkInHistory) {
        out << " " << date.toString("yyyy.MM.dd");
    }
    file.close();
}


// 用户数据读取
void MainWindow::loadFromFile(const std::string& filename){
    std::cout<<"loadfromfile"<<std::endl;
    FileName=filename;
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
    std::cout<<"open file"<<std::endl;
    QTextStream in(&file);
    QString line = in.readLine();
    QStringList parts = line.split(" ");

    if (parts.size() >= 1) {
        // 读取成就状态
        ach_1 = parts[0][0] == '1';
        ach_2 = parts[0][1] == '1';
        ach_3 = parts[0][2] == '1';
        ach_4 = parts[0][3] == '1';
        ach_5 = parts[0][4] == '1';
        ach_6 = parts[0][5] == '1';
    }

        // 读取其他数据
        if (parts.size() > 1) cnt_save = parts[1].toInt();
        if (parts.size() > 2) cnt_reveal = parts[2].toInt();

        // 读取打卡数据
        if (parts.size() > 3) totalCheckInDays = parts[3].toInt();
        else totalCheckInDays=0;
        if (parts.size() > 4) consecutiveCheckInDays = parts[4].toInt();
        else consecutiveCheckInDays=0;
        if (parts.size() > 5) lastCheckInDate = QDate::fromString(parts[5], "yyyy.MM.dd");
        else lastCheckInDate=QDate::fromString("0000.00.00", "yyyy.MM.dd");

        // 读取打卡历史
        checkInHistory.clear();
        if (parts.size() > 6) {
            int historyCount = parts[6].toInt();
            for (int i = 0; i < historyCount && (7 + i) < parts.size(); i++) {
                QDate date = QDate::fromString(parts[7 + i], "yyyy.MM.dd");
                if (date.isValid()) {
                    checkInHistory.append(date);
                }
            }
        }

    std::cout<<"goout"<<std::endl;
    file.close();
}

// 登出按钮
void MainWindow::on_log_out_Button_clicked()
{
    std::cout<<":save:"<<FileName<<std::endl;
    saveToFile(FileName);
    l = new login();
    // if(login::theme_now==0){
    //     ThemeManager::applyTheme(l,)
    // }
    connect(l,&login::right_login,[=](){
        l->setAttribute(Qt::WA_DeleteOnClose);                  // 关闭时释放窗口
        ui->name_label->setText(QString::fromStdString(l->crt_name));

        userTxt = l->crt_name+".txt";
        FileName=userTxt;
        std::cout<<"::"<<FileName<<std::endl;
        loadFromFile(userTxt);
        updateCheckInUI();
        std::cout << "kick"<<std::endl;
        if(l->is_login)ach_1=1;
        l->close();                                             // 关闭单机模式界面
        this->show();                                           // 重新显示菜单界面
        l = nullptr;
        std::cout<<"yes"<<std::endl;
    });
    this->hide();
    l->show();
    ui->resultList->clear();

}

// 创建新题按钮
void MainWindow::on_newfile_botton_clicked()
{
    CreatNewFile* cnf = new CreatNewFile(this);
    cnf->setWindowTitle("新建错题");
    // make it window‑modal (blocks only the main window, not the entire app)
    std::cout<<"new file called"<<std::endl;
    cnf->exec();
    std::cout<<cnf->is_saved<<std::endl;
    if(cnf->is_saved)
    {
        cnt_save++;
        if(!ach_3)ach_3=1;
        if(cnt_save>=5)ach_4=1;
    }
    delete cnf;
}

// 新增的按标签查找函数
void MainWindow::searchByTag(const QString& tag)
{
    ui->resultList->clear();
    QDir dir;
    if (!dir.cd("save")) {
        QMessageBox::warning(this, "警告", "save文件夹不存在或不可访问");
        return;
    }

    QStringList allFiles = dir.entryList(QStringList() << "*.txt", QDir::Files);
    QStringList matchedFiles;

    foreach (const QString &filename, allFiles) {
        QFile file(dir.filePath(filename));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.readLine(); // 跳过第一行
            QString fileTag = in.readLine().trimmed(); // 读取第二行作为标签
            file.close();

            if (fileTag.contains(tag, Qt::CaseInsensitive)) {
                matchedFiles << filename;
            }
        }
    }

    if (matchedFiles.isEmpty()) {
        QMessageBox::information(this, "提示", "未找到匹配标签的文件");
    } else {
        ui->resultList->addItems(matchedFiles);
    }
}

// 搜索按钮
void MainWindow::on_search_button_clicked()
{
    QString searchText = ui->searchEdit->text().trimmed();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入查找内容");
        return;
    }
    if(searchByFilename)
    {

        ui->resultList->clear();

        QDir dir;
        if (!dir.cd("save")) {
            QMessageBox::warning(this, "警告", "save文件夹不存在或不可访问");
            return;
        }

        // 设置过滤器：只匹配包含搜索词的.txt文件
        QStringList filters;
        filters << "*" + searchText + "*.txt";
        dir.setNameFilters(filters);
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        QStringList fileList = dir.entryList();
        if (fileList.isEmpty()) {
            QMessageBox::information(this, "提示", "未找到匹配的.txt文件");
        } else {
            ui->resultList->addItems(fileList);
        }
    }
    else
    {
        searchByTag(searchText);
    }
}


void MainWindow::on_openButton_clicked()
{
    QListWidgetItem *item = ui->resultList->currentItem();
    if (!item) {
        QMessageBox::warning(this, "警告", "请先选择一个文件");
        return;
    }

    // 获取文件路径（在save子目录中）
    QDir dir;
    dir.cd("save");
    QString filePath = dir.absoluteFilePath(item->text());

    // 使用ShowFileEdit打开文件
    ShowFileEdit *editor = new ShowFileEdit(this);
    editor->loadFile(filePath);
    editor->exec();  // 模态显示窗口

    if(editor->is_opened)
    {
        cnt_reveal++;
        if(!ach_5)ach_5=1;
        if(cnt_reveal>=5)ach_6=1;
    }
    delete editor;   // 关闭后释放内存
}


void MainWindow::on_resultList_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    on_openButton_clicked();  // 双击项目时执行打开操作
}


void MainWindow::loadCheckInData()
{
    totalCheckInDays = 0;
    consecutiveCheckInDays = 0;
    lastCheckInDate = QDate();
    checkInHistory.clear();
}

void MainWindow::saveCheckInData()
{

}

void MainWindow::updateCheckInUI()
{
    // 更新UI显示
    ui->totalDaysLabel->setText(QString("累计打卡: %1天").arg(totalCheckInDays));
    ui->consecutiveDaysLabel->setText(QString("连续打卡: %1天").arg(consecutiveCheckInDays));

    // 根据是否今天已打卡来设置按钮状态
    if (lastCheckInDate == QDate::currentDate()) {
        ui->checkInButton->setText("今日已打卡");
        ui->checkInButton->setEnabled(false);
    } else {
        ui->checkInButton->setText("打卡");
        ui->checkInButton->setEnabled(true);
    }
}

void MainWindow::on_checkInButton_clicked()
{
    QDate currentDate = QDate::currentDate();

    // 如果是第一次打卡
    if (lastCheckInDate.isNull()) {
        totalCheckInDays = 1;
        consecutiveCheckInDays = 1;
    } else {
        // 增加累计打卡天数
        totalCheckInDays++;

        // 检查是否是连续打卡
        if (lastCheckInDate.daysTo(currentDate) == 1) {
            consecutiveCheckInDays++;
        } else if (lastCheckInDate != currentDate) {
            // 如果不是连续打卡，重置连续打卡天数
            consecutiveCheckInDays = 1;
        }
    }

    // 更新最后打卡日期和历史记录
    lastCheckInDate = currentDate;
    checkInHistory.append(currentDate);

    // 保存数据到当前用户文件
    saveToFile(userTxt);

    // 更新UI
    updateCheckInUI();

    // 显示打卡成功消息
    QMessageBox::information(this, "打卡成功",
                             QString("打卡成功！\n累计打卡: %1天\n连续打卡: %2天")
                                 .arg(totalCheckInDays).arg(consecutiveCheckInDays));

    // 根据连续打卡天数给予奖励
    if (consecutiveCheckInDays % 7 == 0) {
        QMessageBox::information(this, "每周奖励", "恭喜你获得每周连续打卡奖励！");
        ach_2 = 1;
        saveToFile(userTxt); // 更新成就状态
    }
}




void MainWindow::on_achieveButton_clicked()
{
    ShowMyAchievement* sma=new ShowMyAchievement(this);
    std::cout<<ach_1<<ach_2<<ach_3<<ach_4<<ach_5<<ach_6<<std::endl;
    sma->setAchievements(ach_1, ach_2, ach_3, ach_4, ach_5, ach_6);

    sma->show();
    sma->raise();
    sma->activateWindow();
}


void MainWindow::on_showMy_clicked()
{
    QDir dir_listread;
    if (!dir_listread.cd("save")) {
        QMessageBox::warning(this, "警告", "save文件夹不存在或不可访问");
        return;
    }
    QStringList fileList = dir_listread.entryList(QDir::Files | QDir::NoDotAndDotDot);
    if (fileList.isEmpty()) {
        QMessageBox::information(this, "提示", "您还没有添加任何错题！");
    } else {
        ui->resultList->clear();
        ui->resultList->addItems(fileList);
    }
}


void MainWindow::on_randomQes_clicked()
{
    QDir dir;
    if (!dir.cd("save")){
        QMessageBox::warning(this, "警告", "save文件夹不存在或不可访问");
        return;
    }

    QStringList files = dir.entryList(QDir::Files);
    if (files.isEmpty()){
        QMessageBox::information(this, "提示", "您还没有添加任何错题！");
    }

    int randomIndex = QRandomGenerator::global()->bounded(files.size());
    QString filePath = dir.filePath(files.at(randomIndex)); // 返回完整文件路径
    dir.cd("save");
    //QString filePath = dir.absoluteFilePath(selectedFile);
    //qDebug() << "随机选中的文件：" << selectedFile;
    if(ui->resultList->count()!=0){
        ui->resultList->setCurrentRow(randomIndex); // 高亮显示随机选中的项
    }
    ShowFile *file = new ShowFile(this);
    file->show();
    file->loadFile(filePath);
}


// themeButton 的点击事件处理
void MainWindow::on_themeButton_clicked()
{
    // 从设置中获取当前主题
    //QString currentTheme = settings2->value("theme", "light").toString();

    // 切换主题
    login::theme_now = !login::theme_now;
    ThemeManager::applyTheme(this, login::theme_now);
}

void MainWindow::on_searchModeButton_clicked()
{
    searchByFilename = !searchByFilename;
    if (searchByFilename) {
        ui->searchModeButton->setText("当前模式: 按标题");
    } else {
        ui->searchModeButton->setText("当前模式: 按标签");
    }
}

void MainWindow::showReviewStatistics()
{
    ShowReviewStatistics* srs = new ShowReviewStatistics();
    srs->setWindowTitle("数据分析");
    srs->show();
}

void MainWindow::on_reviewStatsButton_clicked()
{
    showReviewStatistics();
}

