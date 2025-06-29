#ifndef SHOWREVIEWSTATISTICS_H
#define SHOWREVIEWSTATISTICS_H

#include <sstream>
#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QWidget>
#include <QtCharts>

namespace Ui {
class showReviewStatistics;
}

class ShowReviewStatistics : public QWidget
{
    Q_OBJECT

public:
    explicit ShowReviewStatistics(QWidget *parent = nullptr);
    ~ShowReviewStatistics();
    QVector<QPair<QString, int>> each_reviewData;
    QVector<int> total_reviewData={0,0,0,0,0};
    QVector<int> each_reasonData={0,0,0,0,0,0};
private slots:

    void on_reviewBarButton_clicked();

    void on_reviewPieButton_clicked();

    void on_reasonBarButton_clicked();

    void on_reasonPieButton_clicked();

private:
    Ui::showReviewStatistics *ui;
};

#endif // SHOWREVIEWSTATISTICS_H
