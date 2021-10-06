#include "customcolorscheme.h"
#include "ui_custom_color_scheme.h"
#include "QApplication"

customColorScheme::customColorScheme(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent,flags)
    , ui(new Ui::customColorScheme)
    , colorWidget(this)
    , m_currentRole(nullptr)
    , m_currentScheme(nullptr)
{
    colorWidget.setOptions(QColorDialog::DontUseNativeDialog|QColorDialog::ShowAlphaChannel|QColorDialog::NoButtons);
    setWindowFlags(flags);

    ui->setupUi(this);
    ui->gridLayout->addWidget(&colorWidget,1,1,8,4,Qt::AlignTop|Qt::AlignLeft);

    ui->gridLayout->addWidget(ui->save,9,3,1,1);
    ui->gridLayout->addWidget(ui->cancel,9,4,1,1);

    connect(&colorWidget,SIGNAL(currentColorChanged(const QColor&)),this,SLOT(currentColorChanged(const QColor&)));

    ui->highlightText->setProperty("PaletteRole", QPalette::HighlightedText);
    ui->window->setProperty("PaletteRole", QVariant(QPalette::Window));
    ui->button->setProperty("PaletteRole", QPalette::Button);
    ui->windowText->setProperty("PaletteRole", QPalette::WindowText); // Text,WindowText,ButtonText
    ui->text->setProperty("PaletteRole", QPalette::Text);
    ui->toolTipText->setProperty("PaletteRole",QPalette::ToolTipText);
    ui->buttonText->setProperty("PaletteRole",QPalette::ButtonText);
    ui->base->setProperty("PaletteRole", QPalette::Base);
    ui->dark->setProperty("PaletteRole", QPalette::Dark);

    ui->blue->setProperty("SchemeId", 1);
    ui->green->setProperty("SchemeId",2);
    ui->black->setProperty("SchemeId",3);
    ui->ocean->setProperty("SchemeId",4);
    ui->white->setProperty("SchemeId",0);

    ui->strack->setProperty("PaletteRole", QPalette::BrightText);

    colorWidget.setCurrentColor(QColor(255,255,255));

    connect(ui->fontSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(sgFontSizeChanged(int)));
    connect(ui->fontComboBox, SIGNAL(currentFontChanged(const QFont&)), this, SIGNAL(sgFontChanged(const QFont&)));
    // изменение шрифта приложения (всем окнам приложения)
    connect(ui->fontSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int fz){
        QFont font=qApp->font();
        font.setPointSize(fz);
        qApp->setFont(font);
            });

    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this,
            [](const QFont &font){
        QFont font_=qApp->font();
        font_.setFamily(font.family());
        qApp->setFont(font_);
    });
}

//------------------------------------------------------------------------
void customColorScheme::schemeButtonClicked(QPushButton* button, bool checked)
{
    if (checked)
    {
        if (m_currentScheme !=nullptr)
            m_currentScheme->setChecked(false);
        m_currentScheme=button;
        setupColorScheme(button->property("SchemeId").value<quint8>());
    }
    else
    {
        if (m_currentScheme!=nullptr)
            m_currentScheme->setChecked(false);
        m_currentScheme=nullptr;
    }
}


//------------------------------------------------------------------------
void customColorScheme::setupColorScheme(quint8 id)
{
 //   qDebug("%s:%i",__func__,id);
    QPalette pal;
    // init pal
    emit sgCustomColorScheme(pal);
}



//------------------------------------------------------------------------
customColorScheme::~customColorScheme()
{
    qDebug("%s:",__func__);
    if (ui!=nullptr) delete ui;
}
