#include "towerpostion.h"

TowerPostion::TowerPostion() {
    towerExsist = false;
}

TowerPostion::TowerPostion(QPoint pos)
{
    this->towerPos = pos;
    towerExsist = false;
}

QPoint TowerPostion::getPos()
{
    return this->towerPos;
}

void TowerPostion::setPos(QPoint pos)
{
    this->towerPos = pos;
}

QPoint TowerPostion::getcenterpos()
{
    return QPoint(this->towerPos.x() + 50,this->towerPos.y() + 50);
}

bool TowerPostion::getTowerExsistence()
{
    return this->towerExsist;
}

bool TowerPostion::containPos(QPoint pos)
{
    bool bo1 = pos.x()>this->towerPos.x() * 100 && pos.x()<this->towerPos.x() * 100+100;
    bool bo2 = pos.y()>this->towerPos.y() * 100 && pos.y()<this->towerPos.y() * 100+100;
    return bo1&&bo2;
}

void TowerPostion::setExsistence(bool exsist)
{
    this->towerExsist = exsist;
}


