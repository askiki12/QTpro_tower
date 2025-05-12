#include "gamescene.h"
#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include <QRandomGenerator>

void GameScene::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (focusWidget()) {
        focusWidget()->clearFocus();
    }
    QPoint clickPos = event->pos();
    for(auto& postion: mapData.towerpos){
        if(!postion.getTowerExsistence() && postion.containPos(clickPos) && choosewhich && !isPause){
            Tower* thetower;
            bool ap = false;
            if(choosewhich == 1 && this->gold >= 100) thetower = new GunCat(&postion,this), this->gold -= 100, ap = true;
            else if(choosewhich == 2 && this->gold >= 150) thetower = new MageCat(&postion,this), this->gold -= 150, ap = true;
            else if(choosewhich == 3 && this->gold >= 150) thetower = new FireCat(&postion,this), this->gold -= 150, ap = true;
            else if(choosewhich == 4 && this->gold >= 200) thetower = new CannonCat(&postion,this), this->gold -= 200, ap = true;
            goldnum->display(this->gold);
            if(ap){
                this->towers.append(thetower);
                //playertowerplace->play();
                postion.setExsistence(true);
                choosewhich = 0;
            }
            update();
            break;
        }
    }
    bool hascli = false;
    Tower* temptow = NULL;
    for(auto& tow: towers){
        if(!isPause && tow->containPos(clickPos)){
            temptow = tow;
            tow->showButton();
            hascli = true;
            break;
        }
    }
    if(!hascli){
        for(auto& tow: towers){
            tow->hideButton();
        }
    }
    else{
        for(auto& tow: towers){
            if(tow != temptow) tow->hideButton();
        }
    }
}

GameScene::GameScene(int _leveNum)
{
    playerbg = new QMediaPlayer(this);
    playerbutton = new QSoundEffect(this);
    playertowerplace = new QSoundEffect(this);
    playerhit = new QSoundEffect(this);
    playerhomehurt = new QSoundEffect(this);
    playervic = new QSoundEffect(this);
    playerdef = new QSoundEffect(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);
    playerbg->setAudioOutput(audioOutput);
    playerbg->setSource(QUrl("qrc:/Music/game.wav"));
    playerbutton->setSource(QUrl("qrc:/Music/click.wav"));
    playertowerplace->setSource(QUrl("qrc:/Music/towerreplace.wav"));
    playerhit->setSource(QUrl("qrc:/Music/hit.wav"));
    playerhomehurt->setSource(QUrl("qrc:/Music/homehurt.wav"));
    playervic->setSource(QUrl("qrc:/Music/vic.wav"));
    playerdef->setSource(QUrl("qrc:/Music/def.wav"));
    playervic->setVolume(0.8);
    playerdef->setVolume(0.8);
    playerhomehurt->setVolume(0.8);
    playerhit->setVolume(0.8);
    playertowerplace->setVolume(0.8);
    playerbutton->setVolume(0.8);
    audioOutput->setVolume(0.5);
    playerbg->setLoops(QMediaPlayer::Infinite);
    playerbg->play();
    //
    this->homeHp = 10;
    this->gameWave = 0;
    this->isWin = false;
    this->isLose = false;
    this->isPause = false;
    choosewhich = 1;
    gold = 500;
    homehealthnum = new QLCDNumber(this);
    goldnum = new QLCDNumber(this);
    goldnum->setDigitCount(4);
    homehealthnum->setDigitCount(4);
    QFont font("Arial", 20, QFont::Bold);
    goldnum->setFont(font);
    homehealthnum->setFont(font);
    goldnum->setStyleSheet("QLCDNumber {border: none;"
                           "selection-background-color: transparent;"
                           "selection-color: transparent;}");
    homehealthnum->setStyleSheet("QLCDNumber {border: none;"
                           "selection-background-color: transparent;"
                           "selection-color: transparent;}");
    goldnum->move(1230,150);
    homehealthnum->move(1215,100);
    goldnum->setFixedSize(70,50);
    homehealthnum->setFixedSize(70,50);
    goldnum->display(this->gold);
    homehealthnum->display(this->homeHp);
    this->setWindowIcon(QIcon("://image/Start/GameIcon.png"));
    QString str = QString("喵星保卫战 第 %1 关").arg(_leveNum);
    this->setWindowTitle(str);
    this->setFixedSize(1300,800);
    this->leveNum = _leveNum;
    towerchoosepaint();
    loadthismap();
    MAPInit();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateMap()));
    timer->start(30);
    QTimer::singleShot(3000,this,SLOT(gameStart()));
}

void GameScene::paintEvent(QPaintEvent *)
{
    //MapDraw
    QPainter paintbg(this);
    QPixmap mainbg,road,tower,enst,ened;
    bool ret1 = mainbg.load("://image/mainbg.png");
    bool ret2 = road.load("://image/map_road.png");
    bool ret3 = tower.load("://image/Cat/CatAllowPos.png");
    bool ret4 = enst.load("://image/Cat/enemystart.png");
    bool ret5 = ened.load("://image/Cat/enemyend.png");
    if(!(ret1&&ret2&&ret3&&ret4&&ret5)) qDebug()<<"bd load error";
    paintbg.drawPixmap(0,0,this->width()-100,this->height(),mainbg);
    for(int i = 0;i < 12; i++){
        for(int j = 0; j < 8; j++){
            if(Map[i][j]==ROAD){
                paintbg.drawPixmap(i * 100, j * 100, 100, 100, road);
            }
            else if(Map[i][j]==TOWER){
                paintbg.drawPixmap(i * 100, j * 100, 100, 100, tower);
            }
        }
    }
    for(auto& tpath: mapData.paths){
        int s_x = tpath.start.x();
        int s_y = tpath.start.y();
        int e_x = tpath.end.x();
        int e_y = tpath.end.y();
        paintbg.drawPixmap(s_x * 100 + 35, s_y * 100 + 25, 30, 50, enst);
        paintbg.drawPixmap(e_x * 100 + 35, e_y * 100 + 25, 30, 50, ened);
    }
    QPixmap goldpix,toolchoosepix,heartpix;
    ret1 = goldpix.load("://image/GoldCoin.png");
    ret2 = toolchoosepix.load("://image/toolchosse.jpg");
    ret3 = heartpix.load("://image/heart.png");
    if(!(ret1&&ret2&&ret3)) qDebug()<<"error gold";
    paintbg.drawPixmap(1200,0,100,800,toolchoosepix);
    paintbg.drawPixmap(1200,155,35,40,goldpix);
    paintbg.drawPixmap(1210,105,40,40,heartpix);
    for(int i = 0;i <= 3;i ++){
        paintbg.drawLine(1200,50 + i * 50, 1300, 50 + i * 50);
    }
    for(int i = 0; i < 4; i++){
        paintbg.drawLine(1200,200 + i * 150, 1300, 200 + i * 150);
        paintbg.drawLine(1200,300 + i * 150, 1300, 300 + i * 150);
    }
    paintbg.drawLine(1200,200 + 4 * 150, 1300, 200 + 4 * 150);
    //wave
    QFont fontthis("宋体",27,QFont::Bold);
    paintbg.setFont(fontthis);
    QString wavePath="0";
    wavePath += QString::number(gameWave);
    wavePath += "/06";
    paintbg.drawText(1205,90,wavePath);
    //tower
    for(auto& kktower: this->towers){
        kktower->drawself(&paintbg);
    }
    for(auto& en: this->enemies){
        en->drawself(&paintbg);
    }
    for(auto& bu: this->mybulletList){
        bu->drawself(&paintbg);
    }
    //
    if(isWin){
        QPixmap win1("://image/gameend/vic.png");
        paintbg.drawPixmap(500,150,300,300,win1);
    }
    else if(isLose){
        QPixmap lose1("://image/gameend/def.png");
        paintbg.drawPixmap(500,150,300,300,lose1);
    }
}

void GameScene::loadthismap()
{
    QString path;
    if(this->leveNum == 1) path = "://Map/map1.txt";
    else if(this->leveNum == 2) path = "://Map/map2.txt";
    else if(this->leveNum == 3) path = "://Map/map3.txt";
    else if(this->leveNum == 4) path = "://Map/map4.txt";
    else if(this->leveNum == 5) path = "://Map/map5.txt";
    else if(this->leveNum == 6) path = "://Map/map6.txt";
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" ");
            if (parts.isEmpty()) continue;
            if (parts[0] == "PATH") {
                //qDebug() << "PATH\n";
                GamePath tempPath;
                line = in.readLine();
                parts = line.split(" ");
                Q_ASSERT(!parts.isEmpty() && parts[0] == "START");
                //qDebug() << "START\n";
                tempPath.start = QPoint(parts[1].toInt() - 1, parts[2].toInt() - 1);
                //qDebug() << tempPath.start;
                (tempPath.path).append(tempPath.start);
                line = in.readLine();
                parts = line.split(" ");
                Q_ASSERT(!parts.isEmpty() && parts[0] == "INFPO");
                //qDebug() << "INFPO\n";
                for (int i = 1; i < parts.size(); i += 3) {
                    tempPath.path.append(QPoint(parts[i].toInt() - 1,  parts[i + 1].toInt() - 1));
                    //qDebug() << QPoint(parts[i].toInt() - 1,  parts[i + 1].toInt() - 1);
                }
                line = in.readLine();
                parts = line.split(" ");
                Q_ASSERT(!parts.isEmpty() && parts[0] == "END");
                tempPath.end = QPoint(parts[1].toInt() - 1, parts[2].toInt() - 1);
                (tempPath.path).append(tempPath.end);
                mapData.paths.append(tempPath);
            }
            else if(parts[0] == "TOWERPOS"){
                //qDebug() << "TOWERPOS\n";
                for (int i = 1; i < parts.size(); i += 3) {
                    mapData.towerpos.append(TowerPostion(QPoint(parts[i].toInt() - 1, parts[i + 1].toInt() - 1)));
                    //qDebug() << QPoint(parts[i].toInt() - 1, parts[i + 1].toInt() - 1);
                }
            }
        }
        file.close();
    }
}

void GameScene::MAPInit()
{
    Map.resize(12,QVector<PosType>(8,GRASS));
    //ROAD
    for(auto& tpath: mapData.paths){
        for(int i = 0; i < tpath.path.size() - 1; i++){
            if(tpath.path[i].x() == tpath.path[i + 1].x()){
                int p_x = tpath.path[i].x();
                if(tpath.path[i].y() < tpath.path[i + 1].y()){
                    for(int j = tpath.path[i].y(); j <= tpath.path[i + 1].y(); j++){
                        Map[p_x][j] = ROAD;
                    }
                }
                else{
                    for(int j = tpath.path[i + 1].y(); j <= tpath.path[i].y(); j++){
                        Map[p_x][j] = ROAD;
                    }
                }
            }
            else if(tpath.path[i].y() == tpath.path[i + 1].y()){
                int p_y = tpath.path[i].y();
                if(tpath.path[i].x() < tpath.path[i + 1].x()){
                    for(int j = tpath.path[i].x(); j <= tpath.path[i + 1].x(); j++){
                        Map[j][p_y] = ROAD;
                    }
                }
                else{
                    for(int j = tpath.path[i + 1].x(); j <= tpath.path[i].x(); j++){
                        Map[j][p_y] = ROAD;
                    }
                }
            }
        }
    }
    //TOWERPOS
    for(int i = 0; i < mapData.towerpos.size(); i++){
        QPoint po = mapData.towerpos[i].getPos();
        Map[po.x()][po.y()] = TOWER;
    }
}

void GameScene::towerchoosepaint()
{
    QLabel* moneylabel[4];
    for(int i = 0; i < 4; i++){
        moneylabel[i] = new QLabel(this);
        towerchoose[i] = new QPushButton(this);
        moneylabel[i]->setFixedSize(100,50);
        towerchoose[i]->setFixedSize(100,100);
        moneylabel[i]->move(1220,300 + 150 * i);
        towerchoose[i]->move(1200, 50 * (i + 1) + i * 100 + 150);
        moneylabel[i]->setStyleSheet("QLabel { "
                                 "border: none; "
                                 "background: transparent; "
                                 "color: #FF00FF; "
                                 "font-family: 'Arial'; "
                                 "font-size: 35px; "
                                 "font-weight: bold; "
                                 "}");
    }
    towerchoose[0]->setStyleSheet("QPushButton{border-image:url(://image/Cat/GunCat_1.png);}"
                                  "QPushButton:focus{border-image:url(://image/Cat/GunCat_1focus.png);}");
    towerchoose[1]->setStyleSheet("QPushButton{border-image:url(://image/Cat/MageCat_1.png);}"
                                  "QPushButton:focus{border-image:url(://image/Cat/MageCat_1focus.png);}");
    towerchoose[2]->setStyleSheet("QPushButton{border-image:url(://image/Cat/FireCat_1.png);}"
                                  "QPushButton:focus{border-image:url(://image/Cat/FireCat_1focus.png);}");
    towerchoose[3]->setStyleSheet("QPushButton{border-image:url(://image/Cat/CannonCat_1.png);}"
                                  "QPushButton:focus{border-image:url(://image/Cat/CannonCat_1focus.png);}");
    moneylabel[0]->setText("100");
    moneylabel[1]->setText("150");
    moneylabel[2]->setText("150");
    moneylabel[3]->setText("200");
    for(int i = 0; i < 4; i++){
        connect(towerchoose[i],&QPushButton::clicked,this,[&, i](){
            this->playerhit->play();
            choosewhich = i + 1;
        });
    }
    stopandstart = new QPushButton(this);
    stopandstart->setFixedSize(46,46);
    stopandstart->move(1227, 0);
    stopandstart->setStyleSheet("QPushButton{border:0px;"
                                "border-image:url(://image/stop.png);}");
    connect(stopandstart,&QPushButton::clicked, this, [&](){
        this->playerbutton->play();
        this->pause();
    });
    this->restrat = new QPushButton(this);
    this->replay = new QPushButton(this);
    this->reback = new QPushButton(this);
    restrat->setStyleSheet("QPushButton{border:0px;"
                                "border-image:url(://image/hidebutton/backtogame.png);}"
                                "QPushButton:hover{border:0px;border-image:url(://image/hidebutton/backtogame1.png)} "
                                "QPushButton:pressed{border:0px;border-image:url(://image/hidebutton/backtogame2.png)}");
    replay->setStyleSheet("QPushButton{border:0px;"
                                "border-image:url(://image/hidebutton/replay.png);}"
                                "QPushButton:hover{border:0px;border-image:url(://image/hidebutton/replay1.png)} "
                                "QPushButton:pressed{border:0px;border-image:url(://image/hidebutton/replay2.png)}");
    reback->setStyleSheet("QPushButton{border:0px;"
                                "border-image:url(://image/hidebutton/engame.png);}"
                                "QPushButton:hover{border:0px;border-image:url(://image/hidebutton/engame1.png)} "
                                "QPushButton:pressed{border:0px;border-image:url(://image/hidebutton/engame2.png)}");
    restrat->setFixedSize(100,100);
    replay->setFixedSize(100,100);
    reback->setFixedSize(100,100);
    restrat->move(450,300);
    reback->move(575,300);
    replay->move(700,300);
    connect(restrat,&QPushButton::clicked, this, [&](){
        this->playerbutton->play();
        this->restratact();
        this->hidesomebutton();
    });
    connect(replay,&QPushButton::clicked, this, [&](){
        this->playerbutton->play();
        this->replayact();
        this->hidesomebutton();
    });
    connect(reback,&QPushButton::clicked, this, [&](){
        this->playerbutton->play();
        this->rebackact();
        this->hidesomebutton();
    });
    this->hidesomebutton();

}

void GameScene::homeDamaged()
{
    if(this->homeHp > 0){
        homeHp--;
        this->playerhomehurt->play();
        this->homehealthnum->display(homeHp);
    }
    if(this->homeHp == 0){
        this->isLose=true;
        gameEnd();
    }
}

void GameScene::awardMoney(int money)
{
    this->gold+=money;
    this->goldnum->display(gold);
}

void GameScene::removeEnemy(Enemy *enemy)
{
    Q_ASSERT(enemy);
    enemies.removeOne(enemy);
    delete enemy;

    if(enemies.empty())
    {
        ++gameWave;
        if(!loadWaves()){
            isWin=true;
            gameEnd();
        }
    }
}

bool GameScene::loadWaves()
{
    if(this->gameWave >= 6) return false;
    int roadNum = this->mapData.paths.size();
    //qDebug()<<"roadNum=="<<roadNum;
    int gameTime = 3000;

    if(gameWave == 0){
        while(gameTime <= 20000){
            int en = QRandomGenerator::global()->bounded(1,3);
            int road = QRandomGenerator::global()->bounded(0, roadNum);
            //qDebug()<<"en=="<<en<<"road=="<<road;
            Enemy* enemy = generateEnemies(en,this->mapData.paths[road]);
            this->enemies.append(enemy);
            QTimer::singleShot(gameTime,enemy,SLOT(beginActivate()));
            gameTime += QRandomGenerator::global()->bounded(800,3000);
        }
    }
    else if(gameWave == 1){
        qDebug()<<"Wave=="<<gameWave;
        gameTime += 3000;
        while(gameTime <= 40000){
            int en = QRandomGenerator::global()->bounded(1,4);
            int road = QRandomGenerator::global()->bounded(0, roadNum);
            //qDebug()<<"en=="<<en<<"road=="<<road;
            Enemy* enemy = generateEnemies(en,this->mapData.paths[road]);
            this->enemies.append(enemy);
            QTimer::singleShot(gameTime,enemy,SLOT(beginActivate()));
            gameTime += QRandomGenerator::global()->bounded(400,3000);
        }
    }
    else if(gameWave == 2){
        qDebug()<<"Wave=="<<gameWave;
        gameTime += 3000;
        while(gameTime <= 60000){
            int en = 1;
            int randomnum = QRandomGenerator::global()->bounded(1,11);
            if(randomnum < 3) en = 1;
            else if(randomnum < 6) en = 2;
            else en = 3;
            int road = QRandomGenerator::global()->bounded(0, roadNum);
            //qDebug()<<"en=="<<en<<"road=="<<road;
            Enemy* enemy = generateEnemies(en,this->mapData.paths[road]);
            this->enemies.append(enemy);
            QTimer::singleShot(gameTime,enemy,SLOT(beginActivate()));
            gameTime += QRandomGenerator::global()->bounded(400,3000);
        }
    }
    else if(gameWave == 3){
        qDebug()<<"Wave=="<<gameWave;
        gameTime += 3000;
        while(gameTime <= 80000){
            int en = 1;
            int randomnum = QRandomGenerator::global()->bounded(1,11);
            if(randomnum <= 2) en = 1;
            else if(randomnum <= 4) en = 2;
            else if(randomnum <= 7) en = 3;
            else en = 4;
            int road = QRandomGenerator::global()->bounded(0, roadNum);
            //qDebug()<<"en=="<<en<<"road=="<<road;
            Enemy* enemy = generateEnemies(en,this->mapData.paths[road]);
            this->enemies.append(enemy);
            QTimer::singleShot(gameTime,enemy,SLOT(beginActivate()));
            gameTime += QRandomGenerator::global()->bounded(100,3000);
        }
    }
    else if(gameWave == 4){
        qDebug()<<"Wave=="<<gameWave;
        gameTime += 3000;
        while(gameTime <= 100000){
            int en = 1;
            int randomnum = QRandomGenerator::global()->bounded(1,11);
            if(randomnum == 1) en = 1;
            else if(randomnum == 2) en = 2;
            else if(randomnum <= 5) en = 3;
            else if(randomnum <= 7) en = 4;
            else en = 5;
            int road = QRandomGenerator::global()->bounded(0, roadNum);
            //qDebug()<<"en=="<<en<<"road=="<<road;
            Enemy* enemy = generateEnemies(en,this->mapData.paths[road]);
            this->enemies.append(enemy);
            QTimer::singleShot(gameTime,enemy,SLOT(beginActivate()));
            gameTime += QRandomGenerator::global()->bounded(100,3000);
        }
    }
    else if(gameWave == 5){
        qDebug()<<"Wave=="<<gameWave;
        gameTime += 3000;
        while(gameTime <= 120000){
            int en = 1;
            int randomnum = QRandomGenerator::global()->bounded(1,11);
            if(randomnum == 1) en = 1;
            else if(randomnum == 2) en = 2;
            else if(randomnum <= 4) en = 3;
            else if(randomnum <= 6) en = 4;
            else en = 5;
            int road = QRandomGenerator::global()->bounded(0, roadNum);
            //qDebug()<<"en=="<<en<<"road=="<<road;
            Enemy* enemy = generateEnemies(en,this->mapData.paths[road]);
            this->enemies.append(enemy);
            QTimer::singleShot(gameTime,enemy,SLOT(beginActivate()));
            gameTime += QRandomGenerator::global()->bounded(100,3000);
        }
    }
    return true;
}

void GameScene::removedBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);
    mybulletList.removeOne(bullet);
    delete bullet;
}

void GameScene::addBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);
    mybulletList.append(bullet);
}

void GameScene::removeTower(Tower *tower)
{
    tower->getPostion()->setExsistence(false);
    this->towers.removeOne(tower);

}

QList<Enemy *> &GameScene::getMyEnemies()
{
    return this->enemies;
}

QList<Tower *> &GameScene::getMyTowers()
{
    return this->towers;
}

void GameScene::hidebuttonshow()
{
    this->restrat->show();
    this->reback->show();
    this->replay->show();
}

void GameScene::hidesomebutton()
{
    this->restrat->hide();
    this->reback->hide();
    this->replay->hide();
}

void GameScene::restratact()
{
    this->playerbg->play();
    stopandstart->setStyleSheet("QPushButton{border:0px;"
                                "border-image:url(://image/stop.png);}");
    stopandstart->setEnabled(true);
    for(int i = 0; i < 4; i++){
        towerchoose[i]->setEnabled(true);
    }
    isPause = false;
    updateMap();
    this->timer->start(30);
}

void GameScene::rebackact()
{
    //qDebug()<<"rebackact";
    emit this->gameback();
    return;
}

void GameScene::replayact()
{
    qDebug()<<"replay";
    emit replaythisgame();
}

void GameScene::loseGold(int num)
{
    this->gold -= num;
    this->goldnum->display(gold);
}

int GameScene::getGold()
{
    return gold;
}

Enemy *GameScene::generateEnemies(int i, GamePath& path)
{

    if(i == 1) return new Enemy_1(path, this);
    else if(i == 2) return new Enemy_2(path, this);
    else if(i == 3) return new Enemy_3(path, this);
    else if(i == 4) return new Enemy_4(path, this);
    else if(i == 5) return new Enemy_5(path, this);
    return NULL;
}

void GameScene::updateMap()
{
    for(auto& en: this->enemies){
        en->move();
    }
    for(auto& tow: this->towers){
        tow->checkEnemyInRange();
    }
    // for(auto& bu: this->mybulletList){
    //     bu->move();
    // }
    update();
}

void GameScene::gameStart()
{
    loadWaves();
}

void GameScene::gameEnd()
{
    QTimer::singleShot(3000,this,[](){});
    if(isWin){
        qDebug()<<"win";
        playervic->play();
        //this->pause();
        endpause();
        this->reback->show();
        update();
    }
    else{
        qDebug()<<"lose";
        //this->pause();
        playerdef->play();
        endpause();
        this->reback->show();
        update();
    }
}

void GameScene::pause()
{
    this->playerbg->pause();
    isPause = true;
    stopandstart->setStyleSheet("QPushButton{border:0px;"
                                "border-image:url(://image/restart.png);}");
    for(int i = 0; i < 4; i++){
        towerchoose[i]->setEnabled(false);
    }
    for(auto& tow: towers){
        tow->hideButton();
    }
    stopandstart->setEnabled(false);
    updateMap();
    this->timer->stop();
    hidebuttonshow();
}

void GameScene::endpause()
{
    this->playerbg->stop();
    isPause = true;
    stopandstart->setStyleSheet("QPushButton{border:0px;"
                                "border-image:url(://image/restart.png);}");
    for(int i = 0; i < 4; i++){
        towerchoose[i]->setEnabled(false);
    }
    for(auto& tow: towers){
        tow->hideButton();
    }
    stopandstart->setEnabled(false);
    this->timer->stop();
    reback->move(575,470);
}

bool GameScene::getWin()
{
    return isWin;
}

int GameScene::getleveNum()
{
    return leveNum;
}




