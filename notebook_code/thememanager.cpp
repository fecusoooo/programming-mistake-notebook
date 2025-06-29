#include "thememanager.h"

ThemeManager::ThemeManager() {
}

void ThemeManager::applyTheme(QWidget* widget, int theme) {
    if (theme == 0) applyLight(widget);
    else applyDark(widget);
}

void ThemeManager::applyDark(QWidget* w){
    QString darkTheme = R"(
        QMainWindow, QDialog, QWidget {
            background-color: #333333;
            color: #ffffff;
        }
        QLabel, QCheckBox, QRadioButton {
            color: #ffffff;
        }
        QPushButton, QToolButton {
            background-color: #555555;
            color: #ffffff;
            border: 1px solid #777777;
            border-radius: 4px;
            padding: 5px;
        }
        QPushButton:hover, QToolButton:hover {
            background-color: #666666;
        }
        QPushButton:pressed, QToolButton:pressed {
            background-color: #444444;
        }
        QListWidget {
            background-color: #2a2a2a;
            color: #ffffff;
            border: 1px solid #444444;
        }
        QLineEdit, QTextEdit ,QPlainTextEdit{
            background-color: #2a2a2a;
            color: #ffffff;
            border: 1px solid #444444;
            selection-background-color: #555555;
        }
        QTabWidget::pane {
            border: 1px solid #444444;
            background: #333333;
        }
        QTabBar::tab {
            background: #444444;
            color: #ffffff;
            padding: 5px;
            border: 1px solid #444444;
            border-bottom: none;
        }
        QTabBar::tab:selected {
            background: #555555;
        }
    )";

    // 应用深色主题
    w->setStyleSheet(darkTheme);
}

void ThemeManager::applyLight(QWidget* w) {
    // 浅色主题样式 - 只设置控件样式，保留背景图片
    QString lightTheme = R"(
        QLabel, QCheckBox, QRadioButton {
            color: #000000;
            background-color: rgba(255, 255, 255, 150);
            //border-radius: 4px;
            //padding: 2px;
        }
        QPushButton, QToolButton {
            background-color: rgba(224, 224, 224, 200);
            color: #000000;
            border: 1px solid #cccccc;
            border-radius: 4px;
            padding: 5px;
        }
        QPushButton:hover, QToolButton:hover {
            background-color: rgba(208, 208, 208, 200);
        }
        QPushButton:pressed, QToolButton:pressed {
            background-color: rgba(192, 192, 192, 200);
        }
        QListWidget {
            background-color: rgba(255, 255, 255, 180);
            color: #000000;
            border: 1px solid #cccccc;
        }
        QLineEdit, QTextEdit {
            background-color: rgba(255, 255, 255, 180);
            color: #000000;
            border: 1px solid #cccccc;
            selection-background-color: #d0d0d0;
        }
        QTabWidget::pane {
            border: 1px solid #cccccc;
            background: rgba(240, 240, 240, 180);
        }
        QTabBar::tab {
            background: rgba(224, 224, 224, 180);
            color: #000000;
            padding: 5px;
            border: 1px solid #cccccc;
            border-bottom: none;
        }
        QTabBar::tab:selected {
            background: rgba(240, 240, 240, 180);
        }
    )";

    // 应用浅色主题
    w->setStyleSheet(lightTheme);
}
