#include "tower.h"
#include "enemy.h"
#include "gamescene.h"
#include "bullet.h"



void Tower::setButtonPos()
{
    //qDebug()<<"setButtonPos()";
    QPoint pp(this->pos.x() * 100 + 50,this->pos.y() * 100 + 50);
    if(pp.x() > 100 && pp.x() < 1100){
        this->unGradeButton->move(pp.x() - 100,pp.y() - 25);
        this->removeButton->move(pp.x() + 50,pp.y() - 25);
    }
    else if(pp.x() < 100 && pp.y() < 700){
        this->unGradeButton->move(pp.x() - 25,pp.y() + 50);
        this->removeButton->move(pp.x() + 50,pp.y() - 25);
    }
    else if(pp.x() < 100 && pp.y() > 700){
        this->unGradeButton->move(pp.x() - 25,pp.y() - 100);
        this->removeButton->move(pp.x() + 50,pp.y() - 25);
    }
    else if(pp.x() > 1100 && pp.y() < 700){
        this->unGradeButton->move(pp.x() - 100,pp.y() - 25);
        this->removeButton->move(pp.x() - 25,pp.y() + 50);
    }
    else if(pp.x() > 1100 && pp.y() > 700){
        this->unGradeButton->move(pp.x() - 100,pp.y() - 25);
        this->removeButton->move(pp.x() - 25,pp.y() - 100);
    }
}

void Tower::setButtonStyle()
{
    if(this->level < 3 && mygame->getGold() >= this->upGradeMoney){
        unGradeButton->setStyleSheet("QPushButton{border:0px;"
                                    "border-image:url(://image/TowerButton/CanUp1.png);}"
                                    "QPushButton:hover{border:0px;border-image:url(://image/TowerButton/CanUp2.png)} "
                                    "QPushButton:pressed{border:0px;border-image:url(://image/TowerButton/CanUp3.png)}");
        unGradeButton->setEnabled(true);
        //qDebug()<<"setButtonStyle()";
    }
    else {
        unGradeButton->setStyleSheet("QPushButton{border:0px;"
                                     "border-image:url(://image/TowerButton/CannotUp.png);}");
        unGradeButton->setEnabled(false);
    }
    removeButton->setStyleSheet("QPushButton{border:0px;"
                                "border-image:url(://image/TowerButton/Remove1.png);}"
                                "QPushButton:hover{border:0px;border-image:url(://image/TowerButton/Remove2.png)} "
                                "QPushButton:pressed{border:0px;border-image:url(://image/TowerButton/Remove3.png)}");
}

void Tower::showButton()
{
    this->unGradeButton->show();
    this->removeButton->show();
}

void Tower::hideButton()
{
    this->unGradeButton->hide();
    this->removeButton->hide();
}

void Tower::gotSold()
{
    mygame->awardMoney(this->selfVal / 2);
    this->hideButton();
    if(mygame->getMyTowers().contains(this)){
        mygame->removeTower(this);
    }
}

TowerPostion *&Tower::getPostion()
{
    return this->mypostion;
}

void Tower::shootWeapon()
{
    if(this->towerkind == 5) return;
    QPoint posti(pos.x() * 100 + 50, pos.y() * 100 + 50);
    Bullet *bullet;
    bool ap = false;
    if(towerkind == 1) bullet = new Tower1Bullet(posti, myenemy->centerPos(), attackval, myenemy, mygame), ap = true;
    else if(towerkind == 2) bullet = new Tower2Bullet(posti, myenemy->centerPos(), attackval, myenemy, mygame), ap = true;
    else if(towerkind == 3) bullet = new Tower3Bullet(posti, myenemy->centerPos(), attackval, myenemy, mygame, fireVal), ap = true;
    else if(towerkind == 4) bullet = new Tower4Bullet(posti, myenemy->centerPos(), attackval, myenemy, mygame), ap = true;
    else return;
    //bullet = new Bullet(posti, myenemy->centerPos(), attackval, myenemy, mygame), ap = true;
    if(ap) bullet->move();
    mygame->addBullet(bullet);
    //qDebug()<<"shootWeapon()";
}

Tower::Tower(TowerPostion* postion, GameScene* game)
    :mypostion(postion)
    ,selfVal(100)
    ,upGradeMoney(150)
    ,towerkind(0)
    ,level(1)
    ,health(500)
    ,attackval(50)
    ,attackrate(500)
    ,fireVal(0)
    ,attackrange(200)
{
    playertowerreplace = new QSoundEffect(this);
    playerupgrade = new QSoundEffect(this);
    playertowerreplace->setSource(QUrl("qrc:/Music/towerreplace.wav"));
    playerupgrade->setSource(QUrl("qrc:/Music/upgrade.wav"));
    playertowerreplace->setVolume(0.8);
    playerupgrade->setVolume(0.8);
    this->myenemy = NULL;
    this->pos = postion->getPos();
    this->mygame = game;
    fireTimer = new QTimer(this);
    connect(fireTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    if(game) this->unGradeButton = new QPushButton(game);
    if(game) this->removeButton = new QPushButton(game);
    unGradeButton->setFixedSize(50,50);
    removeButton->setFixedSize(50,50);
    setButtonPos();
    setButtonStyle();
    //qDebug()<<"Tower()";
    connect(unGradeButton, &QPushButton::clicked,this,[&](){
        //qDebug()<<"unGradeButton";
        this->playerupgrade->play();
        this->upgrade();
    });
    connect(removeButton, &QPushButton::clicked,this,[&](){
        //qDebug()<<"removeButton";
        this->playertowerreplace->play();
        this->gotSold();
    });
    showButton();
}

Tower::~Tower()
{
    delete fireTimer;
    fireTimer = NULL;
    mygame = NULL;

}

void Tower::drawself(QPainter *painter)
{
    QPixmap topix;
    bool ret = topix.load(this->getPixPath());
    if(!ret) qDebug()<<"error towerpix";
    painter->save();
    painter->drawPixmap(pos.x() * 100, pos.y() * 100, 100, 100, topix);//画出防御塔的图片
    painter->setPen(Qt::blue);
    painter->drawEllipse(this->centerPos(),attackrange,attackrange);
    painter->restore();
    setButtonStyle();
}



QPoint Tower::centerPos()
{
    return QPoint(this->pos.x() * 100 + 50,this->pos.y() * 100 + 50);
}

QString Tower::getPixPath()
{
    return "";
}

void Tower::upgrade()
{
    if(this->level < 3){
        this->level++;
        this->upGradeMoney += 150;
    }
}

void Tower::getDamaged(int damage)
{
    this->health -= damage;
}

void Tower::getAttacked()
{

}

void Tower::checkEnemyInRange()
{
    if (this->myenemy){
        QPoint enepos = myenemy->centerPos();
        if (!enInRange(enepos)) lostSightOfEnemy();
    }
    else{
        for(auto& ene: this->mygame->getMyEnemies()){
            QPoint enepos = ene->centerPos();
            if(enInRange(enepos) && ene->getisActive()){
                chooseEnemyForAttack(ene);
                //qDebug()<<"in range";
                break;
            }
        }
    }
}

void Tower::attackEnemy()
{
    //qDebug()<<"attackEnemy()";
    fireTimer->start(attackrate);

}

void Tower::chooseEnemyForAttack(Enemy *thene)
{
    this->myenemy = thene;
    attackEnemy();
    this->myenemy->getAttacked(this);
}


void Tower::lostSightOfEnemy()
{
    this->myenemy->gotLostSight(this);
    if(this->myenemy) this->myenemy = NULL;
    fireTimer->stop();
}

int Tower::getAttackVal()
{
    return this->attackval;
}

void Tower::targetKilled()
{
    if (this->myenemy)
        myenemy = NULL;
    fireTimer->stop();
}

bool Tower::enInRange(QPoint enepos)
{
    int xx = enepos.x() - this->centerPos().x();
    int yy = enepos.y() - this->centerPos().y();
    double dis = sqrt(xx*xx+yy*yy);
    if(dis <= this->attackrange) return true;
    return false;
}

bool Tower::containPos(QPoint enepos)
{
    bool bo1 = enepos.x() > this->pos.x() * 100 && enepos.x() < this->pos.x() * 100 + 100;
    bool bo2 = enepos.y() > this->pos.y() * 100 && enepos.y() < this->pos.y() * 100 + 100;
    return bo1 && bo2;
}


CannonCat::CannonCat(TowerPostion* postion, GameScene *game)
    : Tower(postion, game)
{
    towerkind = 4;
    attackval = 100;
    this->attackrate = 1000;
    attackrange = 250;
    selfVal = 200;
    upGradeMoney = 200;
}

QString CannonCat::getPixPath()
{
    if(level == 1) return "://image/Cat/CannonCat_1.png";
    else if(level == 2) return "://image/Cat/CannonCat_2.png";
    else if(level == 3) return "://image/Cat/CannonCat_3.png";
    return "";
}

void CannonCat::upgrade()
{
    if(this->level >= 3) return;
    mygame->loseGold(upGradeMoney);
    this->level++;
    this->selfVal += upGradeMoney;
    this->upGradeMoney += 150;
    if(level == 2){
        attackrate = 800;
        attackval = 120;
    }
    else if(level == 3){
        attackrate = 700;
        attackval = 150;
    }
    setButtonStyle();
}

FireCat::FireCat(TowerPostion* postion, GameScene *game)
    : Tower(postion, game)
{
    this->attackrate = 1000;
    this->fireVal = 20;
    this->attackval = 0;
    towerkind = 3;
    selfVal = 150;
    upGradeMoney = 150;
}

QString FireCat::getPixPath()
{
    if(level == 1) return "://image/Cat/FireCat_1.png";
    else if(level == 2) return "://image/Cat/FireCat_2.png";
    else if(level == 3) return "://image/Cat/FireCat_3.png";
    return "";
}

void FireCat::upgrade()
{
    if(this->level >= 3) return;
    mygame->loseGold(upGradeMoney);
    this->level++;
    this->selfVal += upGradeMoney;
    this->upGradeMoney += 150;
    if(level == 2){
        attackrate = 800;
        this->fireVal = 30;
    }
    else if(level == 3){
        attackrange = 250;
        this->fireVal = 40;
    }
    setButtonStyle();
}

GunCat::GunCat(TowerPostion* postion, GameScene *game)
    : Tower(postion, game)
{
    towerkind = 1;
}

QString GunCat::getPixPath()
{
    if(level == 1) return "://image/Cat/GunCat_1.png";
    else if(level == 2) return "://image/Cat/GunCat_2.png";
    else if(level == 3) return "://image/Cat/GunCat_3.png";
    return "";
}

void GunCat::upgrade()
{
    if(this->level >= 3) return;
    mygame->loseGold(upGradeMoney);
    this->level++;
    this->selfVal += upGradeMoney;
    this->upGradeMoney += 150;
    if(level == 2){
        attackrate = 400;
        attackval = 60;
    }
    else if(level == 3){
        attackrange = 250;
        attackval = 70;
    }
    setButtonStyle();
}


MageCat::MageCat(TowerPostion* postion, GameScene *game)
    : Tower(postion, game)
{
    towerkind = 2;
    selfVal = 150;
    upGradeMoney = 150;
}

QString MageCat::getPixPath()
{
    if(level == 1) return "://image/Cat/MageCat_1.png";
    else if(level == 2) return "://image/Cat/MageCat_2.png";
    else if(level == 3) return "://image/Cat/MageCat_3.png";
    return "";
}

void MageCat::upgrade()
{
    if(this->level >= 3) return;
    mygame->loseGold(upGradeMoney);
    this->level++;
    this->selfVal += upGradeMoney;
    this->upGradeMoney += 150;
    if(level == 2){
        attackrate = 400;
        attackval = 60;
    }
    else if(level == 3){
        attackrange = 250;
        attackval = 70;
    }
    setButtonStyle();
}


