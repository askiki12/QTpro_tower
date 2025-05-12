#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
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
    playerbg->play();
    //some
    ui->setupUi(this);
    this->setWindowTitle("喵星保卫战");
    this->setWindowIcon(QIcon("://image/Start/GameIcon.png"));
    this->setFixedSize(1000,700);
    QPushButton* StartButton = new QPushButton(this);
    QPixmap StartButtonPix;
    bool ret = StartButtonPix.load("://image/Start/StartButton.png");
    if(!ret) qDebug()<<"error open startButton";
    StartButton->setFixedSize(StartButtonPix.width(),StartButtonPix.height());
    StartButton->setStyleSheet("QPushButton{border:0px;border-image:url(://image/Start/StartButton.png)} "
                                "QPushButton:hover{border:0px;border-image:url(://image/Start/StartButton1.png)} "
                                "QPushButton:pressed{border:0px;border-image:url(://image/Start/StartButton2.png)}");

    StartButton->move(this->width()/2-StartButton->width()/2,this->height()*0.5);
    //StartButton->setToolTip("点击开始");
    StartButton->setText("  ");
    chooseScene = new ChooseWidget;
    connect(chooseScene,&ChooseWidget::choosesceneback,this,[&](){
        chooseScene->hide();
        this->show();
        playerbg->play();
    });
    connect(StartButton,&QPushButton::clicked,this,[&](){
        this->playerbutton->play();
        playerbg->stop();
        this->hide();
        //qDebug()<<"click";
        chooseScene->show();
        chooseScene->musicplay();
    });
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painterstart(this);
    QPixmap StartScreen,StartSign;
    bool ret1 = StartScreen.load("://image/Start/StartScreen.png");
    bool ret2 = StartSign.load("://image/Start/StartSign.png");
    if(!(ret1&&ret2)) qDebug()<<"error open start";
    painterstart.drawPixmap(0,0,this->width(),this->height(),StartScreen);
    painterstart.drawPixmap(this->width()/2-StartSign.width()*1.15/2,this->height()*0.15,StartSign.width()*1.15,StartSign.height()*1.05,StartSign);
}

Widget::~Widget()
{
    delete ui;
}


