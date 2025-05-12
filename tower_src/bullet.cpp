#include "bullet.h"
#include "enemy.h"
#include "gamescene.h"
#include <QVector2D>
#include <QPropertyAnimation>

Bullet::Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal)
    :mystartPos(startPos)
    ,mytargetPos(targetPoint)
    ,mycurrentPos(startPos)
    ,mytarget(target)
    ,mygame(game)
    ,mydamage(damage)
    ,bulletKind(0)
    ,fireAttack(fireVal)
    ,slowSpeed(2)
{
}

void Bullet::drawself(QPainter *painter) const
{
    QPixmap bulletpix;
    bool ret = bulletpix.load(pixpath);
    if(!ret) qDebug()<<"load bulletpix error";
    painter->drawPixmap(mycurrentPos.x(), mycurrentPos.y(), 15, 15, bulletpix);
    //painter->drawPixmap(mycurrentPos, QString("://image/Ammo/GunCatAmmo.png"));
}

void Bullet::move(){
    //qDebug()<<"move()";
    static int duration = 200;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "mycurrentPos");
    animation->setDuration(duration);
    animation->setStartValue(mystartPos);
    animation->setEndValue(mytargetPos);
    connect(animation, SIGNAL(finished()), this, SLOT(hitTarget()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    //animation->start();
}


void Bullet::hitTarget(){
    //qDebug()<<"hitTarget()";
    //playerhurt->play();
    if (mygame->getMyEnemies().contains(mytarget)){
        this->ownAbility();
    }
    mygame->removedBullet(this);
}

void Bullet::setCurrentPos(const QPoint& pos){
    mycurrentPos = pos;
    //setPos(QPointF(pos));
}

void Bullet::ownAbility()
{
    if(mytarget) mytarget->getDamaged(this);
    return;
}

void Bullet::setPixpath()
{
    QString path;
    if(bulletKind == 1) path = "://image/Ammo/GunCatAmmo.png";
    else if(bulletKind == 2) path = "://image/Ammo/MageCatAmmo.png";
    else if(bulletKind == 3) path = "://image/Ammo/FireCatAmmo.png";
    else if(bulletKind == 4) path = "://image/Ammo/CannonCatAmmo.png";
    pixpath = path;
}

QPoint Bullet::getCurrentPos() const{
    return mycurrentPos;
}


Tower1Bullet::Tower1Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal)
    :Bullet(startPos,targetPoint,damage,target,game,fireVal)
{
    this->bulletKind = 1;
    setPixpath();
}

void Tower1Bullet::ownAbility()
{
    if(mytarget) mytarget->getDamaged(this);
    return;
}



Tower2Bullet::Tower2Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal)
    :Bullet(startPos,targetPoint,damage,target,game,fireVal)
{
    this->bulletKind = 2;
    setPixpath();
}

void Tower2Bullet::ownAbility()
{
    if(this->mytarget){
        this->mytarget->slowdown();
        mytarget->getDamaged(this);
    }
}


Tower3Bullet::Tower3Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal)
    :Bullet(startPos,targetPoint,damage,target,game,fireVal)
{
    this->bulletKind = 3;
    setPixpath();
}

void Tower3Bullet::ownAbility()
{
    if(this->mytarget) this->mytarget->burn(this->fireAttack);
}


Tower4Bullet::Tower4Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, GameScene *game, int fireVal)
    :Bullet(startPos,targetPoint,damage,target,game,fireVal)
{
    this->bulletKind = 4;
    setPixpath();
}

void Tower4Bullet::ownAbility()
{
    for(auto& en: this->mygame->getMyEnemies()){
        int xx = en->centerPos().x() - this->mytargetPos.x();
        int yy = en->centerPos().y() - this->mytargetPos.y();
        double dis = sqrt(xx * xx + yy * yy);
        if(dis < 100) en->getDamaged(this);
    }
}

