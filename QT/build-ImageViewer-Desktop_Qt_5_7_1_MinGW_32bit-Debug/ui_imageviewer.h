/********************************************************************************
** Form generated from reading UI file 'imageviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEWER_H
#define UI_IMAGEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageViewer
{
public:
    QAction *openAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageViewer)
    {
        if (ImageViewer->objectName().isEmpty())
            ImageViewer->setObjectName(QStringLiteral("ImageViewer"));
        ImageViewer->resize(393, 325);
        openAct = new QAction(ImageViewer);
        openAct->setObjectName(QStringLiteral("openAct"));
        printAct = new QAction(ImageViewer);
        printAct->setObjectName(QStringLiteral("printAct"));
        printAct->setEnabled(false);
        exitAct = new QAction(ImageViewer);
        exitAct->setObjectName(QStringLiteral("exitAct"));
        zoomInAct = new QAction(ImageViewer);
        zoomInAct->setObjectName(QStringLiteral("zoomInAct"));
        zoomInAct->setEnabled(false);
        zoomOutAct = new QAction(ImageViewer);
        zoomOutAct->setObjectName(QStringLiteral("zoomOutAct"));
        zoomOutAct->setEnabled(false);
        normalSizeAct = new QAction(ImageViewer);
        normalSizeAct->setObjectName(QStringLiteral("normalSizeAct"));
        normalSizeAct->setEnabled(false);
        fitToWindowAct = new QAction(ImageViewer);
        fitToWindowAct->setObjectName(QStringLiteral("fitToWindowAct"));
        fitToWindowAct->setCheckable(true);
        fitToWindowAct->setEnabled(false);
        aboutAct = new QAction(ImageViewer);
        aboutAct->setObjectName(QStringLiteral("aboutAct"));
        aboutQtAct = new QAction(ImageViewer);
        aboutQtAct->setObjectName(QStringLiteral("aboutQtAct"));
        centralWidget = new QWidget(ImageViewer);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        ImageViewer->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ImageViewer);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 393, 21));
        fileMenu = new QMenu(menuBar);
        fileMenu->setObjectName(QStringLiteral("fileMenu"));
        viewMenu = new QMenu(menuBar);
        viewMenu->setObjectName(QStringLiteral("viewMenu"));
        helpMenu = new QMenu(menuBar);
        helpMenu->setObjectName(QStringLiteral("helpMenu"));
        ImageViewer->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImageViewer);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ImageViewer->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ImageViewer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ImageViewer->setStatusBar(statusBar);

        menuBar->addAction(fileMenu->menuAction());
        menuBar->addAction(viewMenu->menuAction());
        menuBar->addAction(helpMenu->menuAction());
        fileMenu->addAction(openAct);
        fileMenu->addAction(printAct);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAct);
        viewMenu->addAction(zoomInAct);
        viewMenu->addAction(zoomOutAct);
        viewMenu->addAction(normalSizeAct);
        viewMenu->addSeparator();
        viewMenu->addAction(fitToWindowAct);
        helpMenu->addAction(aboutAct);
        helpMenu->addAction(aboutQtAct);

        retranslateUi(ImageViewer);

        QMetaObject::connectSlotsByName(ImageViewer);
    } // setupUi

    void retranslateUi(QMainWindow *ImageViewer)
    {
        ImageViewer->setWindowTitle(QApplication::translate("ImageViewer", "ImageViewer", Q_NULLPTR));
        openAct->setText(QApplication::translate("ImageViewer", "&Open...", Q_NULLPTR));
        openAct->setShortcut(QApplication::translate("ImageViewer", "Ctrl+O", Q_NULLPTR));
        printAct->setText(QApplication::translate("ImageViewer", "&Print...", Q_NULLPTR));
        printAct->setShortcut(QApplication::translate("ImageViewer", "Ctrl+P", Q_NULLPTR));
        exitAct->setText(QApplication::translate("ImageViewer", "E&xit", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        exitAct->setToolTip(QApplication::translate("ImageViewer", "Exit", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        exitAct->setShortcut(QApplication::translate("ImageViewer", "Ctrl+Q", Q_NULLPTR));
        zoomInAct->setText(QApplication::translate("ImageViewer", "Zoom &In (25%)", Q_NULLPTR));
        zoomInAct->setShortcut(QApplication::translate("ImageViewer", "Ctrl+=", Q_NULLPTR));
        zoomOutAct->setText(QApplication::translate("ImageViewer", "Zoom &Out (25%)", Q_NULLPTR));
        zoomOutAct->setShortcut(QApplication::translate("ImageViewer", "Ctrl+-", Q_NULLPTR));
        normalSizeAct->setText(QApplication::translate("ImageViewer", "&Normal Size", Q_NULLPTR));
        normalSizeAct->setShortcut(QApplication::translate("ImageViewer", "Ctrl+S", Q_NULLPTR));
        fitToWindowAct->setText(QApplication::translate("ImageViewer", "&Fit to Window", Q_NULLPTR));
        fitToWindowAct->setShortcut(QApplication::translate("ImageViewer", "Ctrl+F", Q_NULLPTR));
        aboutAct->setText(QApplication::translate("ImageViewer", "&About", Q_NULLPTR));
        aboutQtAct->setText(QApplication::translate("ImageViewer", "About &Qt", Q_NULLPTR));
        fileMenu->setTitle(QApplication::translate("ImageViewer", "&File", Q_NULLPTR));
        viewMenu->setTitle(QApplication::translate("ImageViewer", "&View", Q_NULLPTR));
        helpMenu->setTitle(QApplication::translate("ImageViewer", "&Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ImageViewer: public Ui_ImageViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEWER_H
