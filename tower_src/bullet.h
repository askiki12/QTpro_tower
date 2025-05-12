#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QMediaPlayer>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include "gamescene.h"
class GameScene;

class Bullet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint mycurrentPos READ getCurrentPos WRITE setCurrentPos)
    //Q_PROPERTY(QPoint mycurrentPos READ getCurrentPos WRITE setCurrentPos NOTIFY positionChanged)

public:
    Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal = 0);
    void drawself(QPainter *painter) const;
    void move();
    void setCurrentPos(const QPoint& pos);
    virtual void ownAbility();
    virtual void setPixpath();
    QPoint getCurrentPos() const;
    //virtual QString getPixPath();
    friend class Enemy;

protected:
    QPoint mystartPos;
    QPoint mytargetPos;
    QPoint mycurrentPos;
    Enemy* mytarget;
    GameScene* mygame;
    int	mydamage;
    int bulletKind;
    int fireAttack;
    double slowSpeed;
    QString pixpath;
    //QSoundEffect *playerhurt;

protected slots:
    void hitTarget();

signals:
};


class Tower1Bullet: public Bullet{
    Q_OBJECT
public:

    Tower1Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal = 0);
    virtual void ownAbility();
};

class Tower2Bullet: public Bullet{
    Q_OBJECT
public:

    Tower2Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal = 0);
    virtual void ownAbility();

};

class Tower3Bullet: public Bullet{
    Q_OBJECT
public:

    Tower3Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal = 0);
    virtual void ownAbility();

};

class Tower4Bullet: public Bullet{
    Q_OBJECT
public:

    Tower4Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal = 0);
    virtual void ownAbility();

};

#endif // BULLET_H
