#ifndef WIDGET_H
#define WIDGET_H
#include "choosewidget.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    QMediaPlayer *playerbg;
    QSoundEffect *playerbutton;

public:
    Widget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    ~Widget();
    ChooseWidget* chooseScene = NULL;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
