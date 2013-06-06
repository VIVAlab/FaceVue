/********************************************************************************
** Form generated from reading UI file 'facevue.ui'
**
** Created: Tue Jun 4 15:27:49 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACEVUE_H
#define UI_FACEVUE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FaceVueClass
{
public:
    QWidget *centralWidget;
    QTabWidget *widgetTAB;
    QWidget *registrationTAB;
    QLabel *registrationDisplayLBL;
    QWidget *recognitionTAB;
    QLabel *recognitionDisplayLBL;
    QCheckBox *overlayCKB;
    QLabel *FaceD;
    QLabel *FaceR;
    QLabel *label;
    QPushButton *deleteBTN;
    QLabel *Txtlabel_1;
    QLabel *Lbl_faceR;
    QLabel *Lbl_nameR;
    QTableWidget *tableWidget;
    QLabel *label_name;
    QLineEdit *lineEdit;
    QPushButton *add_BTN;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FaceVueClass)
    {
        if (FaceVueClass->objectName().isEmpty())
            FaceVueClass->setObjectName(QString::fromUtf8("FaceVueClass"));
        FaceVueClass->setEnabled(true);
        FaceVueClass->resize(1169, 743);
        centralWidget = new QWidget(FaceVueClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        widgetTAB = new QTabWidget(centralWidget);
        widgetTAB->setObjectName(QString::fromUtf8("widgetTAB"));
        widgetTAB->setGeometry(QRect(10, 10, 831, 711));
        registrationTAB = new QWidget();
        registrationTAB->setObjectName(QString::fromUtf8("registrationTAB"));
        registrationDisplayLBL = new QLabel(registrationTAB);
        registrationDisplayLBL->setObjectName(QString::fromUtf8("registrationDisplayLBL"));
        registrationDisplayLBL->setGeometry(QRect(10, 10, 800, 600));
        widgetTAB->addTab(registrationTAB, QString());
        recognitionTAB = new QWidget();
        recognitionTAB->setObjectName(QString::fromUtf8("recognitionTAB"));
        recognitionDisplayLBL = new QLabel(recognitionTAB);
        recognitionDisplayLBL->setObjectName(QString::fromUtf8("recognitionDisplayLBL"));
        recognitionDisplayLBL->setGeometry(QRect(10, 10, 800, 600));
        overlayCKB = new QCheckBox(recognitionTAB);
        overlayCKB->setObjectName(QString::fromUtf8("overlayCKB"));
        overlayCKB->setGeometry(QRect(490, 630, 121, 17));
        overlayCKB->setChecked(false);
        FaceD = new QLabel(recognitionTAB);
        FaceD->setObjectName(QString::fromUtf8("FaceD"));
        FaceD->setGeometry(QRect(10, 625, 30, 30));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        font.setKerning(false);
        FaceD->setFont(font);
        FaceR = new QLabel(recognitionTAB);
        FaceR->setObjectName(QString::fromUtf8("FaceR"));
        FaceR->setGeometry(QRect(60, 625, 30, 30));
        QFont font1;
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        FaceR->setFont(font1);
        widgetTAB->addTab(recognitionTAB, QString());
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(864, 30, 291, 71));
        label->setPixmap(QPixmap(QString::fromUtf8(":/FaceVue/Resources/logo.png")));
        deleteBTN = new QPushButton(centralWidget);
        deleteBTN->setObjectName(QString::fromUtf8("deleteBTN"));
        deleteBTN->setGeometry(QRect(947, 670, 111, 31));
        Txtlabel_1 = new QLabel(centralWidget);
        Txtlabel_1->setObjectName(QString::fromUtf8("Txtlabel_1"));
        Txtlabel_1->setGeometry(QRect(870, 410, 61, 20));
        Txtlabel_1->setAlignment(Qt::AlignCenter);
        Lbl_faceR = new QLabel(centralWidget);
        Lbl_faceR->setObjectName(QString::fromUtf8("Lbl_faceR"));
        Lbl_faceR->setGeometry(QRect(934, 110, 140, 140));
        Lbl_faceR->setFrameShape(QFrame::StyledPanel);
        Lbl_faceR->setFrameShadow(QFrame::Plain);
        Lbl_faceR->setLineWidth(3);
        Lbl_faceR->setScaledContents(true);
        Lbl_faceR->setAlignment(Qt::AlignCenter);
        Lbl_faceR->setMargin(1);
        Lbl_nameR = new QLabel(centralWidget);
        Lbl_nameR->setObjectName(QString::fromUtf8("Lbl_nameR"));
        Lbl_nameR->setGeometry(QRect(955, 260, 100, 13));
        Lbl_nameR->setAlignment(Qt::AlignCenter);
        tableWidget = new QTableWidget(centralWidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setEnabled(true);
        tableWidget->setGeometry(QRect(910, 310, 191, 341));
        tableWidget->setMouseTracking(true);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        tableWidget->setIconSize(QSize(80, 80));
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(0);
        tableWidget->horizontalHeader()->setDefaultSectionSize(160);
        tableWidget->horizontalHeader()->setMinimumSectionSize(160);
        tableWidget->verticalHeader()->setDefaultSectionSize(80);
        tableWidget->verticalHeader()->setMinimumSectionSize(80);
        label_name = new QLabel(centralWidget);
        label_name->setObjectName(QString::fromUtf8("label_name"));
        label_name->setGeometry(QRect(910, 290, 46, 13));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(950, 286, 91, 20));
        add_BTN = new QPushButton(centralWidget);
        add_BTN->setObjectName(QString::fromUtf8("add_BTN"));
        add_BTN->setGeometry(QRect(1050, 283, 51, 23));
        FaceVueClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FaceVueClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FaceVueClass->setStatusBar(statusBar);

        retranslateUi(FaceVueClass);

        widgetTAB->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FaceVueClass);
    } // setupUi

    void retranslateUi(QMainWindow *FaceVueClass)
    {
        FaceVueClass->setWindowTitle(QApplication::translate("FaceVueClass", "FaceVue", 0, QApplication::UnicodeUTF8));
        registrationDisplayLBL->setText(QString());
        widgetTAB->setTabText(widgetTAB->indexOf(registrationTAB), QApplication::translate("FaceVueClass", "Registration", 0, QApplication::UnicodeUTF8));
        recognitionDisplayLBL->setText(QString());
        overlayCKB->setText(QApplication::translate("FaceVueClass", "Display Overlays", 0, QApplication::UnicodeUTF8));
        FaceD->setText(QApplication::translate("FaceVueClass", " D", 0, QApplication::UnicodeUTF8));
        FaceR->setText(QApplication::translate("FaceVueClass", " R", 0, QApplication::UnicodeUTF8));
        widgetTAB->setTabText(widgetTAB->indexOf(recognitionTAB), QApplication::translate("FaceVueClass", "Recognition", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        deleteBTN->setText(QApplication::translate("FaceVueClass", "Delete Image", 0, QApplication::UnicodeUTF8));
        Txtlabel_1->setText(QString());
        Lbl_faceR->setText(QString());
        Lbl_nameR->setText(QString());
        label_name->setText(QApplication::translate("FaceVueClass", "Name:", 0, QApplication::UnicodeUTF8));
        add_BTN->setText(QApplication::translate("FaceVueClass", "Add", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FaceVueClass: public Ui_FaceVueClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACEVUE_H
