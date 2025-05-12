#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "gamescene.h"
class Enemy : public QObject
{
    Q_OBJECT
protected:
    bool isActive;
    GamePath mypath;
    QPoint myDestination;
    GameScene *mygame=NULL;
    int maxHealth;
    int health;
    int speed;
    int attackval;
    int attackrange;
    int attackrate;
    QList<Tower*> attacktowers;
    Tower* tower = NULL;
    QPoint mypos;
    bool is_alive;
    QString pixpath;
    QString bloodpix;
    int index;
    int eneVal;
public:
    Enemy(GamePath path, GameScene* game);
    ~Enemy();
    virtual void drawself(QPainter* painter);
    void move();
    bool isCrash();
    //QPoint getPos();
    void getDamaged(Bullet* bullet);
    void getDamaged(int damage);
    void getAttacked(Tower* tow);
    void gotLostSight(Tower* tow);
    void bedead();
    bool canRemove();
    void getRemoved();
    virtual void blood();
    virtual void slowdown();
    virtual void burn(int firenum);
    QPoint centerPos();
    bool getisActive();

public slots:
    void beginActivate(bool is = true);

signals:
};

class Enemy_1 : public Enemy{

public:
    Enemy_1(GamePath path, GameScene* game);
};

class Enemy_2 : public Enemy{

public:
    Enemy_2(GamePath path, GameScene* game);
};

class Enemy_3 : public Enemy{

public:
    Enemy_3(GamePath path, GameScene* game);
};

class Enemy_4 : public Enemy{

public:
    Enemy_4(GamePath path, GameScene* game);
};

class Enemy_5 : public Enemy{

public:
    Enemy_5(GamePath path, GameScene* game);
};


#endif // ENEMY_H
