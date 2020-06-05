#pragma once

#include <QVideoWidget>


namespace media {
    class VideoWidget : public QVideoWidget
    {
        Q_OBJECT

    public:
        explicit VideoWidget(QWidget* parent = nullptr);

    protected:
        void keyPressEvent(QKeyEvent* event) override;
        void mouseDoubleClickEvent(QMouseEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
    };
}