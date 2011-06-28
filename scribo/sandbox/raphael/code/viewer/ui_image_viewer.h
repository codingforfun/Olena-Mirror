/********************************************************************************
** Form generated from reading UI file 'image_viewer.ui'
**
** Created: Thu Jun 23 16:02:52 2011
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGE_VIEWER_H
#define UI_IMAGE_VIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageViewer
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *zoomLabel;
    QPushButton *zoomIn;
    QPushButton *zoomOut;
    QPushButton *zoomFixed;
    QPushButton *zoomOriginal;
    QSpacerItem *horizontalSpacer_2;
    QGraphicsView *image_;
    QHBoxLayout *sliderLayout;
    QSlider *slider;
    QLabel *label_2;
    QLabel *frame_nb;

    void setupUi(QWidget *ImageViewer)
    {
        if (ImageViewer->objectName().isEmpty())
            ImageViewer->setObjectName(QString::fromUtf8("ImageViewer"));
        ImageViewer->resize(626, 783);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ImageViewer->sizePolicy().hasHeightForWidth());
        ImageViewer->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(ImageViewer);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        zoomLabel = new QLabel(ImageViewer);
        zoomLabel->setObjectName(QString::fromUtf8("zoomLabel"));
        zoomLabel->setEnabled(false);

        horizontalLayout_2->addWidget(zoomLabel);

        zoomIn = new QPushButton(ImageViewer);
        zoomIn->setObjectName(QString::fromUtf8("zoomIn"));
        zoomIn->setEnabled(false);
        zoomIn->setMaximumSize(QSize(20, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/search-add-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomIn->setIcon(icon);

        horizontalLayout_2->addWidget(zoomIn);

        zoomOut = new QPushButton(ImageViewer);
        zoomOut->setObjectName(QString::fromUtf8("zoomOut"));
        zoomOut->setEnabled(false);
        zoomOut->setMaximumSize(QSize(20, 20));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/search-remove-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomOut->setIcon(icon1);
        zoomOut->setIconSize(QSize(16, 16));

        horizontalLayout_2->addWidget(zoomOut);

        zoomFixed = new QPushButton(ImageViewer);
        zoomFixed->setObjectName(QString::fromUtf8("zoomFixed"));
        zoomFixed->setEnabled(false);
        zoomFixed->setMaximumSize(QSize(16777215, 20));

        horizontalLayout_2->addWidget(zoomFixed);

        zoomOriginal = new QPushButton(ImageViewer);
        zoomOriginal->setObjectName(QString::fromUtf8("zoomOriginal"));
        zoomOriginal->setEnabled(false);
        zoomOriginal->setMaximumSize(QSize(16777215, 20));

        horizontalLayout_2->addWidget(zoomOriginal);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        image_ = new QGraphicsView(ImageViewer);
        image_->setObjectName(QString::fromUtf8("image_"));
        image_->setInteractive(true);
        image_->setRenderHints(QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);

        verticalLayout->addWidget(image_);

        sliderLayout = new QHBoxLayout();
        sliderLayout->setObjectName(QString::fromUtf8("sliderLayout"));
        slider = new QSlider(ImageViewer);
        slider->setObjectName(QString::fromUtf8("slider"));
        slider->setFocusPolicy(Qt::WheelFocus);
        slider->setOrientation(Qt::Horizontal);
        slider->setTickPosition(QSlider::TicksAbove);

        sliderLayout->addWidget(slider);

        label_2 = new QLabel(ImageViewer);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setMinimumSize(QSize(0, 0));

        sliderLayout->addWidget(label_2);

        frame_nb = new QLabel(ImageViewer);
        frame_nb->setObjectName(QString::fromUtf8("frame_nb"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_nb->sizePolicy().hasHeightForWidth());
        frame_nb->setSizePolicy(sizePolicy2);
        frame_nb->setMinimumSize(QSize(60, 0));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        frame_nb->setFont(font);

        sliderLayout->addWidget(frame_nb);


        verticalLayout->addLayout(sliderLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ImageViewer);

        QMetaObject::connectSlotsByName(ImageViewer);
    } // setupUi

    void retranslateUi(QWidget *ImageViewer)
    {
        ImageViewer->setWindowTitle(QApplication::translate("ImageViewer", "Form", 0, QApplication::UnicodeUTF8));
        zoomLabel->setText(QApplication::translate("ImageViewer", "Zoom:", 0, QApplication::UnicodeUTF8));
        zoomIn->setText(QString());
        zoomIn->setShortcut(QApplication::translate("ImageViewer", "+", 0, QApplication::UnicodeUTF8));
        zoomOut->setText(QString());
        zoomOut->setShortcut(QApplication::translate("ImageViewer", "-", 0, QApplication::UnicodeUTF8));
        zoomFixed->setText(QApplication::translate("ImageViewer", "&Fit to screen", 0, QApplication::UnicodeUTF8));
        zoomFixed->setShortcut(QApplication::translate("ImageViewer", "F", 0, QApplication::UnicodeUTF8));
        zoomOriginal->setText(QApplication::translate("ImageViewer", "&Original size", 0, QApplication::UnicodeUTF8));
        zoomOriginal->setShortcut(QApplication::translate("ImageViewer", "O", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ImageViewer", "Current Frame :", 0, QApplication::UnicodeUTF8));
        frame_nb->setText(QApplication::translate("ImageViewer", "0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImageViewer: public Ui_ImageViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGE_VIEWER_H
