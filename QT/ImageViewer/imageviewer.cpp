#include "imageviewer.h"
#include "ui_imageviewer.h"

#include <QtWidgets>
#include <QFileDialog>
#include <QMessageBox>

#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    openAct = ui->openAct;
    printAct = ui->printAct;
    exitAct = ui->exitAct;
    zoomInAct = ui->zoomInAct;
    zoomOutAct = ui->zoomOutAct;
    normalSizeAct = ui->normalSizeAct;
    fitToWindowAct = ui->fitToWindowAct;
    aboutAct = ui->aboutAct;
    aboutQtAct = ui->aboutQtAct;

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    setWindowTitle(tr("Image Viewer"));
    resize(500, 400);

    connect(openAct,SIGNAL(triggered()),this,SLOT(open()));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageViewer::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
         QImage image(fileName);
         if (image.isNull()) {
             QMessageBox::information(this, tr("Image Viewer"),
                                      tr("Cannot load %1.").arg(fileName));
             return;
         }
         imageLabel->setPixmap(QPixmap::fromImage(image));
         scaleFactor = 1.0;

         printAct->setEnabled(true);
         fitToWindowAct->setEnabled(true);
         updateActions();

         if (!fitToWindowAct->isChecked())
             imageLabel->adjustSize();
    }
}

void ImageViewer::print()
{
    Q_ASSERT(imageLabel->pixmap());
#ifndef QT_NO_PRINTER
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}
void ImageViewer::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<b>Image Viewer</b> example."));
}
