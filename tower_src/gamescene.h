#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>
#include <Qicon>
#include <Qpixmap>
#include <QTimer>
#include <Qpainter>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <Qpushbutton>
#include <QDebug>
#include <QMainWindow>
#include <QFile>
#include <QLCDNumber>
#include <QLabel>
#include <QList>
#include <QMouseEvent>
#include "towerpostion.h"
#include <cmath>


class Bullet;
class Tower1Bullet;
class Tower2Bullet;
class Tower3Bullet;
class Tower4Bullet;
class Tower;
class CannonCat;
class GunCat;
class MageCat;
class FireCat;
class BuffBall;
class Enemy;
class Enemy_1;
class Enemy_2;
class Enemy_3;
class Enemy_4;
class Enemy_5;
struct GamePath
{
    QVector<QPoint> path;
    QPoint start;
    QPoint end;
};

struct Mapdata{
    QVector<GamePath> paths;
    QVector<TowerPostion> towerpos;
};

enum PosType{GRASS, ROAD, TOWER};

class GameScene : public QMainWindow
{
    Q_OBJECT
protected:
    QTimer* timer;
    QList<Enemy*> enemies;
    QList<Bullet*> mybulletList;
    QList<Tower*> towers;
    int choosewhich;
    QLCDNumber* goldnum;
    QLCDNumber* homehealthnum;
    QPushButton* towerchoose[4];
    QPushButton* stopandstart;
    QPushButton* restrat;
    QPushButton* reback;
    QPushButton* replay;
    int gold;
    Mapdata mapData;
    QVector<QVector<PosType>> Map;
    QMediaPlayer *playerbg;
    QSoundEffect *playerbutton;
    QSoundEffect *playertowerplace;
    QSoundEffect *playerhit;
    QSoundEffect *playerhomehurt;
    QSoundEffect *playervic;
    QSoundEffect *playerdef;
public:
    //explicit GameScene(QMainWindow *parent = nullptr);
    explicit GameScene(int _leveNum);
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent *);
    void loadthismap();
    void MAPInit();
    void towerchoosepaint();
    void homeDamaged();
    void awardMoney(int money);
    void removeEnemy(Enemy* enemy);
    bool loadWaves();
    void removedBullet(Bullet *bullet);
    void addBullet(Bullet *bullet);
    void removeTower(Tower* tower);
    QList<Enemy*>& getMyEnemies();
    QList<Tower*>& getMyTowers();
    void hidebuttonshow();
    void hidesomebutton();
    void restratact();
    void rebackact();
    void replayact();
    void loseGold(int num);
    int getGold();
    void pause();
    void endpause();
    bool getWin();
    int getleveNum();
    Enemy* generateEnemies(int i, GamePath& path);
    int leveNum;
    bool isWin;
    bool isLose;
    int homeHp;
    int gameWave;
    bool isPause;
private slots:
    void updateMap();
    void gameStart();
    void gameEnd();

signals:
    void gameback();
    void replaythisgame();
};

#endif // GAMESCENE_H
