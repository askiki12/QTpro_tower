#include "choosewidget.h"
//bool Choose[7] = {false};
//bool Finish[7] = {false};

ChooseWidget::ChooseWidget(QWidget *parent)
    : QWidget{parent}
{
    //music
    playerbg = new QMediaPlayer(this);
    playerbutton = new QSoundEffect(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);
    playerbg->setAudioOutput(audioOutput);
    playerbg->setSource(QUrl("qrc:/Music/begin.wav"));
    playerbutton->setSource(QUrl("qrc:/Music/click.wav"));
    playerbutton->setVolume(0.8);
    audioOutput->setVolume(0.5);
    playerbg->setLoops(QMediaPlayer::Infinite);
    //playerbg->play();
    //button
    loadLevelData();
    QPushButton* RebackButtou = new QPushButton(this);
    RebackButtou->setFixedSize(120,60);
    RebackButtou->move(800,600);

    RebackButtou->setStyleSheet("QPushButton{border:0px;"
                                "border-image:url(://image/GoBack.png);}"
                                "QPushButton:hover{border:0px;border-image:url(://image/GoBack1.png)} "
                                "QPushButton:pressed{border:0px;border-image:url(://image/GoBack2.png)}");
    connect(RebackButtou,&QPushButton::clicked,this,[&](){
        this->playerbutton->play();
        playerbg->stop();
        emit this->choosesceneback();
    });
    this->setWindowTitle("喵星保卫战 选择关卡");
    this->setWindowIcon(QIcon("://image/Start/GameIcon.png"));
    this->setFixedSize(1000,700);
    //Choose[0] = true;
    for(int i = 0; i < 6; i++){
        ChooseButton[i] = new QPushButton(this);
    }
    choosepaint();
    for(int i = 0; i < 6; i++){
        connect(ChooseButton[i],&QPushButton::clicked,this,[&, i](){
            this->playerbg->stop();
            this->playerbutton->play();
            this->hide();
            gameScene = new GameScene(i + 1);
            gameScene->show();
            connect(gameScene,&GameScene::gameback,this,[&](){
                this->playerbg->play();
                gameScene->hide();
                if(gameScene->getWin()){
                    Choose[gameScene->getleveNum()] = true;
                    this->saveLevelData();
                }
                this->StyleChange();
                this->show();
            });
            connect(gameScene,&GameScene::replaythisgame,this,[&](){
                gameScene->hide();
                //gameScene = new GameScene(gameScene->getleveNum());
                this->ChooseButton[gameScene->getleveNum() - 1]->click();
                //gameScene->show();
            });
        });
    }

}

void ChooseWidget::paintEvent(QPaintEvent *)
{
    QPainter painterchoose(this);
    QPixmap ChooseBackgroud;
    bool ret = ChooseBackgroud.load("://image/ChooseWi/ChooseBackgroud.png");
    if(!ret) qDebug()<<"error open choose";
    painterchoose.drawPixmap(0,0,this->width(),this->height(),ChooseBackgroud);
}

void ChooseWidget::choosepaint()
{
    for(int i = 0; i < 6; i++){
        ChooseButton[i]->setFixedSize(150,80);
    }
    ChooseButton[0]->move(550/4,200);
    ChooseButton[1]->move(1100/4+150,200);
    ChooseButton[2]->move(1550/4+300,200);
    ChooseButton[3]->move(550/4,400);
    ChooseButton[4]->move(1100/4+150,400);
    ChooseButton[5]->move(1550/4+300,400);
    StyleChange();
    for(int i = 0; i < 6; i++){
        ChooseButton[i]->setText(QString::number(i + 1));
    }
}

void ChooseWidget::StyleChange()
{
    for(int i = 0; i < 6; i++){
        if(Choose[i]){
            ChooseButton[i]->setStyleSheet("QPushButton{border:0px;"
                                           "border-image:url(://image/ChooseWi/openchoose.png);"
                                           "font-size: 36px;"
                                           "font-style: italic;} "
                                           "QPushButton:hover{border:0px;border-image:url(://image/ChooseWi/openchoose1.png)} "
                                           "QPushButton:pressed{border:0px;border-image:url(://image/ChooseWi/openchoose2.png)}");
            ChooseButton[i]->setEnabled(true);
        }
        else{
            ChooseButton[i]->setStyleSheet("QPushButton{border:0px;"
                                           "border-image:url(://image/ChooseWi/closechoose.png);"
                                           "font-size: 36px;"
                                           "font-style: italic;}");
            ChooseButton[i]->setEnabled(false);
        }
    }
}

void ChooseWidget::musicplay()
{
    this->playerbg->play();
}

void ChooseWidget::loadLevelData()
{
    QFile file("://Map/levels.txt");
    if (!file.exists()) {
        qDebug() << "File not found!";
        return;
    }
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        QStringList levelNumbers = line.split(" ");
        for (const QString &level : levelNumbers) {
            bool ok;
            int levelNum = level.toInt(&ok);
            if (ok && levelNum >= 1 && levelNum <= 6) {
                Choose[levelNum - 1] = true;
            }
        }
        file.close();
    } else {
        qDebug() << "Failed to open file!";
    }
}

void ChooseWidget::saveLevelData() {
    QFile file("://Map/levels.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < 6; ++i) {
            if (Choose[i]) {
                out << (i + 1) << " ";
            }
        }
        file.close();
    }
}

