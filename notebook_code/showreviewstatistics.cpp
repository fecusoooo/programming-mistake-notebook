#include "showreviewstatistics.h"
#include "ui_showreviewstatistics.h"

ShowReviewStatistics::ShowReviewStatistics(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::showReviewStatistics)
{
    ui->setupUi(this);
    QDir dir("save");
    if (!dir.exists()) {
        QMessageBox::warning(this, "警告", "save文件夹不存在");
        return;
    }

    QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);
    if (files.isEmpty()) {
        QMessageBox::information(this, "提示", "没有可分析的错题文件");
        return;
    }

    // 准备数据：题目名称和复习次数


    foreach (const QString &filename, files) {
        QFile file(dir.filePath(filename));
        QVector<bool> crt_vector;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            // 第一行：标题
            QString title = in.readLine();
            // 第二行：标签
            in.readLine();
            // 第三行：复习次数
            int count = in.readLine().toInt();
            if(count<=3)total_reviewData[0]++;
            else if(count<=5)total_reviewData[1]++;
            else if(count<=7)total_reviewData[2]++;
            else if(count<=9)total_reviewData[3]++;
            else total_reviewData[4]++;
            crt_vector.clear();
            // 第四行：错题原因
            std::string strForReason = in.readLine().toStdString();
            std::istringstream iss(strForReason); // 使用字符串流解析
            int num;
            while (iss >> num) { // 逐个读取数字
                crt_vector.push_back(num != 0); // 0 → false，非0 → true
            }
            for(int l=0;l<6;l++){
                if(crt_vector[l])each_reasonData[l]++;
            }
            file.close();
            each_reviewData.append(qMakePair(title, count));
        }
    }

    // 按复习次数排序
    std::sort(each_reviewData.begin(), each_reviewData.end(),
              [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
                  return a.second > b.second;
              });
}

ShowReviewStatistics::~ShowReviewStatistics()
{
    delete ui;
}

void ShowReviewStatistics::on_reviewBarButton_clicked() {
    // 创建图表
    // QDialog *chartDialog = new QDialog(this);
    // chartDialog->setWindowTitle("错题复习情况统计");
    // chartDialog->resize(800, 600);

    QChart *chart = new QChart();
    chart->setTitle("错题复习次数柱状统计");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("复习次数");

    QStringList categories;
    for (const auto &data : each_reviewData) {
        *set << data.second;
        categories << data.first;
    }

    series->append(set);
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    axisY->setTitleText("复习次数");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setWindowTitle("错题复习情况统计");
    chartView->resize(800, 600);
    chartView->show();

    // QVBoxLayout *layout = new QVBoxLayout();
    // layout->addWidget(chartView);
    // chartDialog->setLayout(layout);

    // chartDialog->exec();
    // delete chartDialog;
}

void ShowReviewStatistics::on_reviewPieButton_clicked() {
    // QDialog *chartDialog = new QDialog(this);
    // chartDialog->setWindowTitle("错题复习情况统计");
    // chartDialog->resize(800, 600);

    // 创建饼图系列
    QVector<QString> labels = {"复习1-3次", "复习4-5次", "复习6-7次","复习8-9次","复习10次以上"};
    QPieSeries *series = new QPieSeries();
    for (int i = 0; i < 5; ++i) {
        QPieSlice *slice = series->append(labels[i], total_reviewData[i]);
        slice->setLabelVisible(true); // 显示标签
        //////////////////////////////
    }

    // 创建图表并设置标题
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("错题复习次数饼状统计");
    chart->legend()->setVisible(true);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 显示图表
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setWindowTitle("错题复习情况统计");
    chartView->resize(800, 600);
    chartView->show();
    // QVBoxLayout *layout = new QVBoxLayout();
    // layout->addWidget(chartView);
    // chartDialog->setLayout(layout);

    // chartDialog->exec();
    // delete chartDialog;
}

void ShowReviewStatistics::on_reasonBarButton_clicked() {
    QChart *chart = new QChart();
    chart->setTitle("出错原因次数柱状统计");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("出错次数");

    QStringList categories;
    for(int l=0;l<6;l++){
        *set << each_reasonData[l];
    }
    categories << "审题出错" << "计算失误" << "理解偏差" << "思路错误" << "创新不足" << "其他";
    series->append(set);
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    axisY->setTitleText("出错次数");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setWindowTitle("出错原因情况统计");
    chartView->resize(800, 600);
    chartView->show();
}

void ShowReviewStatistics::on_reasonPieButton_clicked() {
    // QDialog *chartDialog = new QDialog(this);
    // chartDialog->setWindowTitle("错题复习情况统计");
    // chartDialog->resize(800, 600);

    // 创建饼图系列
    QVector<QString> labels = {"审题出错", "计算失误", "理解偏差","思路错误","创新不足","其他"};
    QPieSeries *series = new QPieSeries();
    for (int i = 0; i < 6; ++i) {
        QPieSlice *slice = series->append(labels[i], each_reasonData[i]);
        slice->setLabelVisible(true); // 显示标签
    }

    // 创建图表并设置标题
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("出错原因次数饼状统计");
    chart->legend()->setVisible(true);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 显示图表
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setWindowTitle("出错原因情况统计");
    chartView->resize(800, 600);
    chartView->show();
    // QVBoxLayout *layout = new QVBoxLayout();
    // layout->addWidget(chartView);
    // chartDialog->setLayout(layout);

    // chartDialog->exec();
    // delete chartDialog;
}
