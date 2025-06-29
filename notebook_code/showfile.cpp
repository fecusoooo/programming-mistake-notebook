#include "showfile.h"
#include "ui_showfile.h"
#include"mainwindow.h"
#include"thememanager.h"
#include<vector>
#include<sstream>
#include<QFile>
#include<QMessageBox>
#include<QFileInfo>
#include<QPixmap>
#include"markdownhighlighter.h"

ShowFile::ShowFile(QWidget *parent)
    : QDialog(parent),
    highlighter(nullptr)
    , ui(new Ui::ShowFile)
{
    ui->setupUi(this);
    QPixmap image_title(":/imag/title.png");
    QPixmap image_tag(":/imag/label.png");
    QPixmap image_des(":/imag/description.png");
    QPixmap image_date(":/imag/date.png");

    ui->title->setPixmap(image_title);
    ui->tag->setPixmap(image_tag);
    ui->description_2->setPixmap(image_des);
    ui->date->setPixmap(image_date);

    ui->title->setScaledContents(true);
    ui->tag->setScaledContents(true);
    ui->description_2->setScaledContents(true);
    ui->date->setScaledContents(true);

    if (!highlighter) {
        highlighter = new MarkdownHighlighter(ui->description->document());
    }else{
        highlighter->setDocument(ui->description->document());
    }

    // 可选：设置字体为等宽字体，更适合代码显示
    QFont font("Consolas", 10);
    ui->description->setFont(font);
}

ShowFile::~ShowFile()
{
    delete ui;
    delete highlighter;
    highlighter = nullptr;
}

void ShowFile::paintEvent(QPaintEvent* event){
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

void ShowFile::loadFile(const QString &filePath){
    flpth = filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
        return;
    }
    is_opened=1;
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // 假设文件格式为：第一行标题，第二行标签，其余为内容
    parts = content.split("\n");
    if (parts.size() >= 4) {
        ui->titleshow->setText(parts[0]);      // 标题（假设有lineEdit）
        ui->labelshow->setText(parts[1]);      // 标签（假设有lineEdit_2）

        reviewCount = parts[2].toInt();

        string strForReason = parts[3].toStdString();
        std::istringstream iss(strForReason); // 使用字符串流解析
        std::vector<bool> boolArray;
        int num;
        while (iss >> num) { // 逐个读取数字
            boolArray.push_back(num != 0); // 0 → false，非0 → true
        }
        ui->checkBox_1->setChecked(boolArray[0]);
        ui->checkBox_2->setChecked(boolArray[1]);
        ui->checkBox_3->setChecked(boolArray[2]);
        ui->checkBox_4->setChecked(boolArray[3]);
        ui->checkBox_5->setChecked(boolArray[4]);
        ui->checkBox_6->setChecked(boolArray[5]);

        ui->dateEdit->setDate(QDate::fromString(parts[4], "yyyy-MM-dd")); // 日期
        // 保存回文件
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << parts.join("\n");
            file.close();
        }

        ui->description->setPlainText(parts.mid(5).join("\n"));  // 内容
    }

    setWindowTitle("复习: " + QFileInfo(filePath).fileName());

}

void ShowFile::on_closeButton_clicked()
{
    this->close();
}

// 完成复习的功能
void ShowFile::on_checkButton_clicked()
{
    reviewCount++;
    ui->checkButton->setText("复习已完成");
    ui->checkButton->setEnabled(false);
    QFile file(flpth);
    parts[2] = QString::number(reviewCount);
    // 保存回文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << parts.join("\n");
        file.close();
    }
}


void ShowFile::on_ai_chating_Button_clicked()
{
    AI_Linking* ai_linking = new AI_Linking(2,this);
    ai_linking->show();
}


void ShowFile::on_ai_solving_Button_clicked()
{
    AI_Linking* ai_linking = new AI_Linking(1,this);
    QString crt_reason;
    if(ui->checkBox_1->isChecked())crt_reason+="审题出错";
    if(ui->checkBox_2->isChecked())crt_reason+="计算失误";
    if(ui->checkBox_3->isChecked())crt_reason+="理解偏差";
    if(ui->checkBox_4->isChecked())crt_reason+="思路错误";
    if(ui->checkBox_5->isChecked())crt_reason+="创新不足";
    if(ui->checkBox_6->isChecked())crt_reason+="其他";
    ai_linking->title=ui->title->text();
    ai_linking->label=ui->tag->text();
    ai_linking->reason=crt_reason;
    ai_linking->content=ui->description->toPlainText().trimmed();
    ai_linking->show();
}

