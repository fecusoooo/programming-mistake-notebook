#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H
#include<QWidget>

class ThemeManager
{
public:
    ThemeManager();
public:
    static void applyTheme(QWidget* widget, int theme);
    bool is_review_barchart_shown = false;
    bool is_review_piechart_shown = false;
    bool is_reason_barchart_shown = false;
    bool is_reason_piechart_shown = false;

private:
    static void applyLight(QWidget* w);
    static void applyDark(QWidget* w);
};

#endif // THEMEMANAGER_H
