#ifndef POPUPNOTIFYWIDGET_H
#define POPUPNOTIFYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <QScreen>

class PopupNotifyWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;

public:
    explicit PopupNotifyWidget(QWidget *parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void setPopupText(const QString& text);
    void show();

private slots:
    void hideAnimation();
    void hide();

private:
    QLabel label;
    QGridLayout layout;
    QPropertyAnimation animation;
    float popupOpacity;
    QTimer *timer;
    QScreen *_screen = Q_NULLPTR;
};

#endif // POPUPNOTIFYWIDGET_H
