#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void delay();
    void newG();
    void prevR();
    void nextR();
    void turnTheCard(int i);


private:
    Ui::MainWindow *ui;
    QLabel *label;
    QPushButton *NewGame;
    QPushButton *PrevResult;
    QPushButton *NextResult;
    QLabel *gameNumber;
    QLabel *highestScore;
    QLabel *player1_time;
    QLabel *player2_time;
    QLabel *score_label;
    QLabel *time_label;
    QLabel *player1_label;
    QLabel *player2_label;
    QLabel *score1_label;
    QLabel *score2_label;
    QLabel *gamenum_label;
    QLabel *highscore_label;
    QFrame *frame;
    QFrame *frame1;
    QFrame *frame2;
    QFrame *frame3;
    QFrame *frame4;
    QFrame *frame5;
    QFrame *frame6;
    QFrame *frame7;

};

#endif // MAINWINDOW_H
