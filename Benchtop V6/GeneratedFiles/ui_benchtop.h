/********************************************************************************
** Form generated from reading UI file 'benchtop.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BENCHTOP_H
#define UI_BENCHTOP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BenchtopClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *BenchtopClass)
    {
        if (BenchtopClass->objectName().isEmpty())
            BenchtopClass->setObjectName(QString::fromUtf8("BenchtopClass"));
        BenchtopClass->resize(600, 400);
        menuBar = new QMenuBar(BenchtopClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        BenchtopClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(BenchtopClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        BenchtopClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(BenchtopClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        BenchtopClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(BenchtopClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        BenchtopClass->setStatusBar(statusBar);

        retranslateUi(BenchtopClass);

        QMetaObject::connectSlotsByName(BenchtopClass);
    } // setupUi

    void retranslateUi(QMainWindow *BenchtopClass)
    {
        BenchtopClass->setWindowTitle(QApplication::translate("BenchtopClass", "Benchtop", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BenchtopClass: public Ui_BenchtopClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BENCHTOP_H
