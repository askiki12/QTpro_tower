#ifndef TOWER_H
#define TOWER_H

#include <QObject>
#include <QTimer>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QPushButton>

class Enemy;
class GameScene;
class TowerPostion;

class Tower : public QObject
{
    Q_OBJECT
protected:
    TowerPostion* mypostion;
    int selfVal;
    int upGradeMoney;
    int towerkind;
    int level;
    int health;
    int attackval;
    int attackrate;
    int fireVal;
    QPoint pos;
    Enemy* myenemy = NULL;
    int attackrange;
    QSoundEffect *playertowerreplace;
    QSoundEffect *playerupgrade;
    //QString kipath;
    GameScene *mygame = NULL;
    QTimer* fireTimer = NULL;
    QPushButton* unGradeButton;
    QPushButton* removeButton;
    void setButtonPos();
    void setButtonStyle();
    void gotSold();
private slots:
    virtual void shootWeapon();

public:
    //Tower();
    Tower(TowerPostion* postion, GameScene* game);
    ~Tower();
    TowerPostion*& getPostion();
    virtual void drawself(QPainter *painter);
    virtual QPoint centerPos();
    virtual QString getPixPath();
    virtual void upgrade();
    virtual void getDamaged(int damage);
    virtual void getAttacked();
    virtual void checkEnemyInRange();
    virtual void attackEnemy();
    virtual void chooseEnemyForAttack(Enemy* thene);
    virtual void lostSightOfEnemy();
    virtual int getAttackVal();
    void showButton();
    void hideButton();
    void targetKilled();
    bool enInRange(QPoint enepos);
    bool containPos(QPoint enepos);


signals:
};

class CannonCat: public Tower{

public:
    CannonCat(TowerPostion* postion, GameScene* game);
    virtual QString getPixPath();
    virtual void upgrade();

};

class FireCat: public Tower{

public:
    FireCat(TowerPostion* postion, GameScene* game);
    virtual QString getPixPath();
    virtual void upgrade();

};

class GunCat: public Tower{

public:
    GunCat(TowerPostion* postion, GameScene* game);
    virtual QString getPixPath();
    virtual void upgrade();

};

class MageCat: public Tower{

public:
    MageCat(TowerPostion* postion, GameScene* game);
    virtual QString getPixPath();
    virtual void upgrade();

};

#endif // TOWER_H
