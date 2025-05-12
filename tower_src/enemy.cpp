#include "enemy.h"
#include <QVector2D>
#include "tower.h"
#include "bullet.h"

Enemy::Enemy(GamePath tpath, GameScene *game)
    :isActive(false)
    ,maxHealth(200)
    ,health(200)
    ,speed(4)
    ,attackval(0)
    ,attackrange(0)
    ,attackrate(0)
    ,is_alive(true)
    ,pixpath("")
    ,bloodpix("")
    ,index(0)
    ,eneVal(50)
{
    this->mypath = tpath;
    this->mygame = game;
    this->mypos = QPoint(tpath.start.x() * 100,tpath.start.y() * 100);
    if(tpath.path[1].x() > tpath.start.x()) mypos.setX(mypos.x() - 30);
    else if(tpath.path[1].x() < tpath.start.x()) mypos.setX(mypos.x() + 30);
    else if(tpath.path[1].y() < tpath.start.y()) mypos.setY(mypos.y() + 30);
    else if(tpath.path[1].y() > tpath.start.y()) mypos.setY(mypos.y() - 30);
    this->myDestination = QPoint(mypath.path[index].x() * 100,mypath.path[index].y() * 100);
}

Enemy::~Enemy()
{
    mygame = NULL;
}

void Enemy::drawself(QPainter *painter)
{
    if(!this->isActive) return;
    QPixmap selfpix;
    bool ret = selfpix.load(this->pixpath);
    if(!ret) qDebug()<<"enemy load error";
    painter->save();
    painter->drawPixmap(mypos.x(), mypos.y(), 100, 100, selfpix);
    double barWidth = 200 / 3;
    double barHeight = 5;
    double barX = this->mypos.x() + (100 - barWidth) / 2;
    double barY = this->mypos.y() - barHeight - 3;
    double healthRatio = (health) / (double)maxHealth;
    double fillWidth = barWidth * healthRatio;
    painter->setPen(QPen(Qt::black, 1));
    painter->drawRect(barX, barY, barWidth, barHeight);
    painter->setBrush(Qt::red);
    painter->setPen(Qt::NoPen);
    painter->drawRect(QRect(QPoint(barX, barY), QSize((double)fillWidth, barHeight)));
    painter->restore();
}

void Enemy::move()
{
    if(!isActive) return;
    if(isCrash()){
        index++;
        if(index < mypath.path.size()){
            this->myDestination = QPoint(mypath.path[index].x() * 100,mypath.path[index].y() * 100);
        }
        else{
            mygame->homeDamaged();
            this->getRemoved();
        }
    }
    else{
        QPoint targetPoint=myDestination;
        double movementSpeed=speed;
        QVector2D normailzed(targetPoint - mypos);
        normailzed.normalize();
        mypos = mypos + normailzed.toPoint() * movementSpeed;
    }
}

bool Enemy::isCrash()
{
    int xx = mypos.x() - myDestination.x();
    int yy = mypos.y() - myDestination.y();
    int dis=sqrt(xx*xx+yy*yy);
    // if (this->speed == 4 && dis < 3) return true;
    // else if(this->speed == 6 && dis < 5) return true;
    // else if(this->speed == 5 && dis == 0) return true;
    if(dis <= 10) return true;
    return false;
}

void Enemy::getDamaged(Bullet *bullet)
{
    this->blood();
    this->health -= bullet->mydamage;
    canRemove();
}

void Enemy::getDamaged(int damage)
{
    this->blood();
    this->health -= damage;
    canRemove();
}

void Enemy::getAttacked(Tower *tow)
{
    attacktowers.append(tow);
}

void Enemy::gotLostSight(Tower *tow)
{
    attacktowers.removeOne(tow);
}

void Enemy::bedead()
{
    is_alive = false;
}

bool Enemy::canRemove()
{
    if(this->health <= 0){
        mygame->awardMoney(eneVal);
        getRemoved();
        return true;
    }
    return false;
}

void Enemy::getRemoved()
{
    this->bedead();
    if (!this->attacktowers.empty())
        for(auto& attacker: attacktowers)
            attacker->targetKilled();
    mygame->removeEnemy(this);
}

void Enemy::blood()
{
    QString temppix = this->pixpath;
    this->pixpath = this->bloodpix;
    QTimer::singleShot(100, this,[&, temppix](){
        this->pixpath = temppix;
    });
}

void Enemy::slowdown()
{
    if(this->speed >= 2){
        this->speed -= 2;
        QTimer::singleShot(200, this,[&](){
            this->speed += 2;
        });
    }
}

void Enemy::burn(int firenum)
{
    for(int i = 1; i <= 5; i++){
        //QPointer<Enemy> self = this;
        QTimer::singleShot(1000 * i, this,[&,firenum](){
            this->getDamaged(firenum);
        });
    }
}

QPoint Enemy::centerPos()
{
    return QPoint(mypos.x() + 50, mypos.y() + 50);
}

bool Enemy::getisActive()
{
    return isActive;
}

void Enemy::beginActivate(bool is)
{
    if(is) this->isActive = true;
}



Enemy_1::Enemy_1(GamePath path, GameScene *game)
    : Enemy(path, game)
{
    this->pixpath = "://image/Enemy/enemy1.png";
    this->bloodpix = "://image/Enemy/enemy1blood.png";
}

Enemy_2::Enemy_2(GamePath path, GameScene *game)
    : Enemy(path, game)
{
    this->speed = 6;
    this->maxHealth = 150;
    this->health = 150;
    this->attackval = 30;
    this->pixpath = "://image/Enemy/enemy2.png";
    this->bloodpix = "://image/Enemy/enemy2blood.png";
}

Enemy_3::Enemy_3(GamePath path, GameScene *game)
    : Enemy(path, game)
{
    this->eneVal = 70;
    this->attackrate = 700;
    this->maxHealth = 300;
    this->health = 300;
    this->pixpath = "://image/Enemy/enemy3.png";
    this->bloodpix = "://image/Enemy/enemy3blood.png";
}

Enemy_4::Enemy_4(GamePath path, GameScene *game)
    : Enemy(path, game)
{
    this->eneVal = 80;
    this->maxHealth = 500;
    this->health = 500;
    this->speed = 5;
    this->attackval = 80;
    this->pixpath = "://image/Enemy/enemy4.png";
    this->bloodpix = "://image/Enemy/enemy4blood.png";
}

Enemy_5::Enemy_5(GamePath path, GameScene *game)
    : Enemy(path, game)
{
    this->eneVal = 100;
    this->maxHealth = 1000;
    this->health = 1000;
    this->speed = 5;
    this->attackval = 90;
    this->pixpath = "://image/Enemy/enemy5.png";
    this->bloodpix = "://image/Enemy/enemy5blood.png";
}
