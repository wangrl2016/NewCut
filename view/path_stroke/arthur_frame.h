//
// Created by wangrl2016 on 2022/12/23.
//

#ifndef NEWCUT_ARTHUR_WIDGET_H
#define NEWCUT_ARTHUR_WIDGET_H

#include <QWidget>
#include <QTextDocument>

namespace nc {
    class ArthurFrame : public QWidget {
    Q_OBJECT
    public:
        ArthurFrame(QWidget* parent = nullptr);

        virtual void Paint(QPainter*) {

        }

        void PaintDescription(QPainter* p);

        // 加载说明文档文件
        void LoadDescription(const QString& filename);

        // 将加载的字符串设置到QTextDocument中
        void SetDescription(const QString& html_desc);

        bool prefer_image() const {
            return prefer_image_;
        }

    signals:

        void DescriptionEnabledChanged(bool enabled);

    public slots:

        void SetPreferImage(bool pi) {
            prefer_image_ = pi;
        };

        void SetDescriptionEnabled(bool enabled);

    protected:
        void paintEvent(QPaintEvent* event) override;

        void resizeEvent(QResizeEvent* event) override;

        QPixmap tile_;
        bool show_doc_ = false;
        bool prefer_image_ = false;
        QTextDocument* document_ = nullptr;
    };
}

#endif //NEWCUT_ARTHUR_WIDGET_H
