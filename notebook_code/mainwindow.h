#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include <iostream>
#include<QListWidgetItem>
#include<QPaintEvent>
#include <QSettings>
#include <QDate>

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
    bool is_check_in=false;
    //登录一次 每日打卡 存入错题  存入多道错题 复习错题 复习多道错题
    string userTxt = "";
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    void paintEvent(QPaintEvent*);
    bool ach_1 = false;
    bool ach_2 = false;
    bool ach_3 = false;
    bool ach_4 = false;
    bool ach_5 = false;
    bool ach_6 = false;

    int cnt_save=0;
    int cnt_reveal=0;

    QSettings* settings2;

    //void handleThemeChange(int);

    void applyDarkTheme();
    void applyLightTheme();

private slots:

    void on_log_out_Button_clicked();

    void on_newfile_botton_clicked();

    void on_search_button_clicked();

    void on_openButton_clicked();

    void on_resultList_itemDoubleClicked(QListWidgetItem *item);

    void on_checkInButton_clicked();

    void on_achieveButton_clicked();


    void on_showMy_clicked();

    void on_randomQes_clicked();


    void on_themeButton_clicked();

    void on_searchModeButton_clicked();

    void on_reviewStatsButton_clicked();

private:

    Ui::MainWindow *ui;
    login * l = nullptr;

    QSettings *settings;  // 用于存储打卡数据

    // 打卡相关变量
    int totalCheckInDays=0;    // 累计打卡天数
    int consecutiveCheckInDays=0; // 连续打卡天数
    QDate lastCheckInDate;   // 上次打卡日期

    QList<QDate> checkInHistory;
    QString currentAccount;

    void loadCheckInData();  // 加载打卡数据
    void saveCheckInData();  // 保存打卡数据
    void updateCheckInUI();  // 更新UI显示

    std::string FileName;
    bool searchByFilename = true; // 默认按文件名查找
    void searchByTag(const QString& tag); // 新增的按标签查找函数

    void showReviewStatistics(); // 显示复习统计图表



public:
    int sign_num = 0;
};
#endif // MAINWINDOW_H
