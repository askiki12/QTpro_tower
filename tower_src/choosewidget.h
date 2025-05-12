#ifndef CHOOSEWIDGET_H
#define CHOOSEWIDGET_H
#include "GameScene.h"
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

class ChooseWidget : public QWidget
{
    Q_OBJECT
    QMediaPlayer *playerbg;
    QSoundEffect *playerbutton;
    QPushButton* ChooseButton[6];
public:
    explicit ChooseWidget(QWidget *parent = nullptr);
    bool Choose[6] = {false};
    void paintEvent(QPaintEvent *);
    void choosepaint();
    void StyleChange();
    GameScene* gameScene = NULL;
    void musicplay();
    void loadLevelData();
    void saveLevelData();

signals:
    void choosesceneback();
};

#endif // CHOOSEWIDGET_H
