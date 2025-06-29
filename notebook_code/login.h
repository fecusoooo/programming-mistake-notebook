#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <unordered_map>
#include<QPaintEvent>
#include<QSettings>
using namespace std;

namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    std::string crt_name;
    std::string crt_password;
    void saveMapToTextFile(const std::unordered_map<std::string, std::string>& map, const std::string& filename);
    void loadMapFromTextFile(std::unordered_map<std::string, std::string>& map,const std::string& filename);
    void paintEvent(QPaintEvent*);
    bool checkname(std::string nam);
    bool checkaccount(std::string name,std::string account);
    bool isPassWordShowed = false;

    bool is_login=false;
    static bool theme_now;
    QSettings* settings;

private slots:
    void on_create_clicked();

    void on_login_accout_clicked();

    void on_forget_clicked();

    void on_pushButton_clicked();

    void on_PassWordButton_clicked();

    void on_themeButton_clicked();

private:
    void applyDarkTheme();
    void applyLightTheme();
    Ui::login *ui;
    std::unordered_map<std::string, std::string> map;

signals:
    void right_login();
};

#endif // LOGIN_H
