#include "showmyachievement.h"
#include "ui_showmyachievement.h"
#include <QIcon>

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

ShowMyAchievement::ShowMyAchievement(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("成就");
    setWindowIcon(QIcon(":/icons/achievement.png")); // 如果有图标资源
    resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);
}

void ShowMyAchievement::setAchievements(bool ach_1, bool ach_2, bool ach_3,
                                        bool ach_4, bool ach_5, bool ach_6)
{
    // 清除现有内容
    QLayoutItem *child;
    while ((child = layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // 添加标题
    QLabel *titleLabel = new QLabel("已获得的成就", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout()->addWidget(titleLabel);

    // 添加分隔线
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout()->addWidget(line);

    // 添加各个成就
    layout()->addWidget(createAchievementLabel("成就1: 初出茅庐", ach_1));
    layout()->addWidget(createAchievementLabel("成就2: 小试牛刀", ach_2));
    layout()->addWidget(createAchievementLabel("成就3: 渐入佳境", ach_3));
    layout()->addWidget(createAchievementLabel("成就4: 炉火纯青", ach_4));
    layout()->addWidget(createAchievementLabel("成就5: 登峰造极", ach_5));
    layout()->addWidget(createAchievementLabel("成就6: 天下无双", ach_6));

    // 添加拉伸项使内容靠上
    qobject_cast<QVBoxLayout*>(layout())->addStretch();
}

QLabel *ShowMyAchievement::createAchievementLabel(const QString &text, bool achieved)
{
    QLabel *label = new QLabel(this);

    if (achieved) {
        label->setText(QString("✓ %1").arg(text));
        label->setStyleSheet("color: green; font-size: 14px;");
    } else {
        label->setText(QString("✗ %1 (未获得)").arg(text));
        label->setStyleSheet("color: gray; font-size: 14px;");
    }

    return label;
}
