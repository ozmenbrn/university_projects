#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QtCore/QCoreApplication>
#include <QTextBrowser>
#include <QAbstractButton>
#include <QDebug>
#include <QFrame>
#include <QTime>
#include <QVector>


QString tempString;			//this variable is for parsing int to string.
QTime timer;				//keeps the time. Clock
int temp;					//temporary variable
int openCard =0;			//keeps how many cards open. so when 2 cards open it can be known and turn them again into "?"
bool isCard;
bool player1Turn = true;    //whose turn is it.
int totalCardOpened=0;      //do something when all of the cards opened
int totalTime =0;
int player1time =0;
int player2time =0;
int gameNo = 0;
int highS =0;
int gameNoCopy=0;           // helps to utilize functions prevR and nextR
bool newGameButtonPressed = true;       //helps to make prev and next result button.
QVector<int> score1copy;                //1st player score goes here when new game or prev game button clicked
QVector<int> score2copy;                //2nd player score goes here when new game or prev game button clicked
QVector<int> time1copy;                 //1st player time goes here when new game or prev game button clicked
QVector<int> time2copy;                 //2nd player time goes here when new game or prev game button clicked
QButtonGroup *buttonGroup = new QButtonGroup();     //buttongroup
QPushButton *button[30] ;                           //buttons location
int score1;                             //1st player score
int score2;                             //2nd player score
QString cards[15] = {"Antelope","Bat","Bear","Bird","Chicken","Dinosaur","Dolphin","Elephant","Fox","Giraffe","Horse","Human","Koala","Mosquito","Shark"};
QString contentOfTheCards[30] = {};     //cards location


//main window 
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //EVERYTHING IN THIS FUNCTION IS FOR DESİGN AND START.
    //AND ALSO CONNECT SIGNALS BETWEEN BUTTON AND LABELS
    NewGame = new QPushButton("New Game",this);
    NewGame -> setGeometry(QRect(QPoint(750,0),QSize(150,75)));
    connect(NewGame,SIGNAL(released()),this,SLOT(newG()));

    PrevResult = new QPushButton("Prev. Result",this);
    PrevResult -> setGeometry(QRect(QPoint(750,75),QSize(150,75)));
    connect(PrevResult,SIGNAL(released()),this,SLOT(prevR()));

    NextResult = new QPushButton("Next Result",this);
    NextResult -> setGeometry(QRect(QPoint(750,150),QSize(150,75)));
    connect(NextResult,SIGNAL(released()),this,SLOT(nextR()));

    QFrame* frame= new QFrame(this);
    frame->setLineWidth(2);
    frame->setFrameShape(QFrame::VLine);
    frame->setGeometry(QRect(QPoint(100,40),QSize(100,135)));

    QFrame* frame1= new QFrame(this);
    frame1->setLineWidth(2);
    frame1->setFrameShape(QFrame::VLine);
    frame1->setGeometry(QRect(QPoint(225,40),QSize(100,135)));

    QFrame* frame2= new QFrame(this);
    frame2->setLineWidth(2);
    frame2->setFrameShape(QFrame::VLine);
    frame2->setGeometry(QRect(QPoint(350,40),QSize(100,135)));

    QFrame* frame3= new QFrame(this);
    frame3->setLineWidth(2);
    frame3->setFrameShape(QFrame::VLine);
    frame3->setGeometry(QRect(QPoint(0,40),QSize(50,135)));

    QFrame* frame4= new QFrame(this);
    frame4->setLineWidth(2);
    frame4->setFrameShape(QFrame::HLine);
    frame4->setGeometry(QRect(QPoint(25,50),QSize(375,150)));

    QFrame* frame5= new QFrame(this);
    frame5->setLineWidth(2);
    frame5->setFrameShape(QFrame::HLine);
    frame5->setGeometry(QRect(QPoint(25,0),QSize(375,150)));

    QFrame* frame6= new QFrame(this);
    frame6->setLineWidth(2);
    frame6->setFrameShape(QFrame::HLine);
    frame6->setGeometry(QRect(QPoint(25,0),QSize(375,80)));

    QFrame* frame7= new QFrame(this);
    frame7->setLineWidth(2);
    frame7->setFrameShape(QFrame::HLine);
    frame7->setGeometry(QRect(QPoint(25,100),QSize(375,150)));

    score_label = new QLabel("Score",this);
    score_label->setGeometry(QRect(QPoint(50,50),QSize(100,100)));

    time_label = new QLabel("Time(secs)",this);
    time_label->setGeometry(QRect(QPoint(50,100),QSize(100,100)));

    player1_label = new QLabel("Player1",this);
    player1_label->setGeometry(QRect(QPoint(185,10),QSize(100,100)));

    player2_label = new QLabel("Player2",this);
    player2_label->setGeometry(QRect(QPoint(310,10),QSize(100,100)));

    gamenum_label = new QLabel("Game No:",this);
    gamenum_label->setGeometry(QRect(QPoint(500,50),QSize(100,100)));

    gameNumber = new QLabel(tempString.setNum(gameNo),this);
    gameNumber->setGeometry(QRect(QPoint(600,50),QSize(100,100)));

    highscore_label = new QLabel("Highest Score:",this);
    highscore_label->setGeometry(QRect(QPoint(500,100),QSize(100,100)));

    highestScore = new QLabel(tempString.setNum(highS) ,this);
    highestScore->setGeometry(QRect(QPoint(600,100),QSize(100,100)));

    score1_label = new QLabel(QString::number(score1),this);
    score1_label->setGeometry(QRect(QPoint(205,50),QSize(100,100)));

    score2_label = new QLabel(QString::number(score2),this);
    score2_label->setGeometry(QRect(QPoint(335,50),QSize(100,100)));

    player1_time = new QLabel(QString::number(player1time),this);
    player1_time->setGeometry(QRect(QPoint(205,100),QSize(100,100)));

    player2_time = new QLabel(QString::number(player2time),this);
    player2_time->setGeometry(QRect(QPoint(335,100),QSize(100,100)));

    //BUTTONS LOCATION DECIDED
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){

            button[i*6+j] = new QPushButton("",this);
            button[i*6+j]-> setGeometry(QRect(QPoint(150*j,300+75*i),QSize(150,75)));
            buttonGroup->addButton(button[i*6+j],i*6+j);
            button[i*6+j]->setEnabled(false);
        }
    }

    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(turnTheCard(int)));    //connect all signals and sends to the turnTheCard function

}

//This is the function that do all the matching opening cards and keep the scores and times.
void MainWindow::turnTheCard(int i){
    //If 1 one card open only
    if(openCard%2 == 0){
        temp = i;
        button[i]->setText(contentOfTheCards[i]);
        openCard++;
    }
    //2nd card opened and look inside if they are equal
    if(openCard%2 == 1 && (temp != i)){
        totalTime = timer.elapsed() /1000;
        button[i]->setText(contentOfTheCards[i]);
        //check 2 card is same or not
        if(contentOfTheCards[temp] == contentOfTheCards[i]){
            button[i]->setEnabled(false);
            button[temp]->setEnabled(false);
            buttonGroup->blockSignals(true);
            delay();
            buttonGroup->blockSignals(false);
            openCard++;

            //checks whose turn is it
            if(player1Turn == true){
                score1++;
                score1_label->setText(tempString.setNum(score1));
                totalCardOpened +=2;
            }
            else{
                score2++;
                score2_label->setText(tempString.setNum(score2));
                totalCardOpened +=2;
            }
        }
        //opened 2 cards are not same
        else{
            buttonGroup->blockSignals(true);
            delay();
            button[i]->setText("?");
            button[temp]->setText("?");
            buttonGroup->blockSignals(false);
            openCard++;
        }

        //Choose whose turn it is
        if(player1Turn == true){
            player1time += totalTime -player2time -player1time;
            player1Turn = false;
            player1_time->setText(tempString.setNum(player1time));

        }
        else{
            player2time += totalTime - player1time -player2time;
            player1Turn = true;
            player2_time->setText(tempString.setNum(player2time));

        }

        if(totalCardOpened == 30){
            if(score1>score2 && score1 > highS){
                highS = score1;
                highestScore->setText(tempString.setNum(score1));
            }
            else if(score2>score1 && score2>highS){
                highS = score2;
                highestScore->setText(tempString.setNum(score2));
            }
        }
    }
}

//This is a button for start a new Game.
void MainWindow::newG(){

    if(newGameButtonPressed == true){
        score1copy.append(score1);
        score2copy.append(score2);
        time1copy.append(player1time);
        time2copy.append(player2time);
    }
    newGameButtonPressed =true;
    buttonGroup->blockSignals(false);

    //keeps all variables to 0 when new game start.
    score1 =0;
    score2 =0;
    player1Turn = true;
    player1time =0;
    player2time =0;
    totalTime =0;
    totalCardOpened =0;
    timer.start();
    gameNo++;
    gameNoCopy = gameNo;
    gameNumber->setText(tempString.setNum(gameNo));
    score1_label->setText(tempString.setNum(0));
    score2_label->setText(tempString.setNum(0));
    player1_time->setText(tempString.setNum(0));
    player2_time->setText(tempString.setNum(0));

    //keeps all card eneable to click
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            button[i*6+j]->setText("?");
            button[i*6+j]->setEnabled(true);
        }
    }
    for(int i=0;i<30;i++){
        contentOfTheCards[i]="";
    }

    //random card generator
    for(int i=0;i<15;i++){
        int counter = 0;
        while(counter<2){
            qsrand(QTime::currentTime().msec());
            int rand1 = rand()%30;
            if(contentOfTheCards[rand1].isEmpty()){
                contentOfTheCards[rand1] = cards[i];
                counter++;
            }
        }
    }
}

//This is a button for previous game
void MainWindow::prevR(){
    if(gameNoCopy>1){
        gameNoCopy--;
        player1_time->setText(tempString.setNum(time1copy[gameNoCopy]));
        player2_time->setText(tempString.setNum(time2copy[gameNoCopy]));
        score1_label->setText(tempString.setNum(score1copy[gameNoCopy]));
        score2_label->setText(tempString.setNum(score2copy[gameNoCopy]));
        gameNumber->setText(tempString.setNum(gameNoCopy));
        buttonGroup->blockSignals(true);
    }
    if(newGameButtonPressed == true){
        score1copy.append(score1);
        score2copy.append(score2);
        time1copy.append(player1time);
        time2copy.append(player2time);
        newGameButtonPressed = false;
    }
}

//This is a button for next Game
void MainWindow::nextR(){
    if(gameNoCopy<gameNo){
        gameNoCopy++;
        player1_time->setText(tempString.setNum(time1copy[gameNoCopy]));
        player2_time->setText(tempString.setNum(time2copy[gameNoCopy]));
        score1_label->setText(tempString.setNum(score1copy[gameNoCopy]));
        score2_label->setText(tempString.setNum(score2copy[gameNoCopy]));
        gameNumber->setText(tempString.setNum(gameNoCopy));

        //next result goes to current game, than continue
        if(gameNoCopy == gameNo){
            score1copy.pop_back();
            score2copy.pop_back();
            time1copy.pop_back();
            time2copy.pop_back();
            newGameButtonPressed = true;
            buttonGroup->blockSignals(false);
        }
    }
}

//delay function
void MainWindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);					//1 second delay.
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//Deconstructre
MainWindow::~MainWindow()
{
    delete ui;
}
