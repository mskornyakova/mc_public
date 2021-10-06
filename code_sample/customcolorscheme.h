#ifndef CUSTOMCOLORSCHEME_H
#define CUSTOMCOLORSCHEME_H

#include <QWidget>
#include <QCloseEvent>
#include <QShowEvent>
#include <QHideEvent>
#include "QColorDialog"

namespace Ui {
class customColorScheme;
}

class customColorScheme : public QWidget
{
    Q_OBJECT

public:
    customColorScheme(QWidget *parent = nullptr, Qt::WindowFlags flags=Qt::Widget);

    static customColorScheme*& widget(QWidget *parent = nullptr, Qt::WindowFlags flags=Qt::Widget)
    {
        static customColorScheme* wd_=new customColorScheme(parent,flags);
        return wd_;
    }

    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

    void init();
    void setupColorScheme(quint8 id);

signals:
    void sgCustomColorScheme(const QPalette&);
    void sgSaveScheme(quint8, const QPalette&);
    void sgFontSizeChanged(int);
    void sgFontChanged(const QFont &font);

public slots:
    void currentColorChanged(const QColor&);

private slots:
    // настройка ролей палитры
    void on_window_clicked(bool checked);
    void on_windowText_clicked(bool checked);
    void on_base_clicked(bool checked);
    void on_button_clicked(bool checked);
    void on_highlightText_clicked(bool checked);
    void on_dark_clicked(bool checked);
    void on_text_clicked(bool checked);
    void on_toolTipText_clicked(bool checked);
    void on_buttonText_clicked(bool checked);

    // выбор цветовой схемы
    void on_blue_clicked(bool checked);
    void on_green_clicked(bool checked);
    void on_ocean_clicked(bool checked);
    void on_black_clicked(bool checked);
    void on_white_clicked(bool checked);

    void on_strack_clicked(bool checked);

    void on_cancel_clicked();

    void on_save_clicked();

private:
    Ui::customColorScheme *ui;

    customColorScheme(){};
    ~customColorScheme();
    customColorScheme(customColorScheme const&)=delete;
    customColorScheme& operator= (customColorScheme const &)=delete;

    void roleButtonClicked(QPushButton* button, bool checked);
    void schemeButtonClicked(QPushButton* button, bool checked);
    void setupButtonsStyleSheet(const QPalette& palette);

    QColorDialog    colorWidget;

    QPushButton*    m_currentRole;
    QPushButton*    m_currentScheme;

    QPalette        m_palette; //
    QPalette        m_oldPalette; // палитра на момент открытия окна
};

#endif // CUSTOMCOLORSCHEME_H
