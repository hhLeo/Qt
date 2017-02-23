//客户端先走
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
using namespace std;
SOCKET ServerSocket;
char hostname[100];
struct hostent *phostinfo;
WORD wVersionRequested;
WSADATA wsaData;
int err;
WSADATA Ws;
SOCKET CientSocket;
struct sockaddr_in LocalAddr, ClientAddr;
int Ret = 0;
int AddrLen = 0;
HANDLE hThread = NULL;
DWORD WINAPI ClientThread(LPVOID lpParameter);
struct sockaddr_in ServerAddr;

char *RecvBuffer;
char *SendBuffer;
ShuJu *shuJu;//

char IP_ADDRESS[10]="127.0.0.1";

void Thread::run()
{
    qDebug("run----");

    char RecvBuffer[5000];
    qDebug("----recv");
    while(1)
    {
        memset(RecvBuffer, 0x00, 4000);
        qDebug("43");
        Ret = recv(CientSocket, RecvBuffer, 4000, 0);
        qDebug("45");
        if ( Ret == 0 || Ret == SOCKET_ERROR )
        {
            cout<<"exit"<<endl;
        }


        shuJu = (ShuJu*)RecvBuffer;
        //-----
//        qDebug("ZhuanHuan");
        temp = new ShuJu;
        memcpy(temp, shuJu, sizeof(ShuJu));

        emit tempChanged();
    }

    //-----
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), w(48), h(20),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //
//    qDebug() << "sizeof(ShuJu):" << sizeof(ShuJu) << "~~~";
    //
    RecvBuffer = new char[5000];
//    qDebug("thread");
    thread = new Thread;
    connect(thread, SIGNAL(tempChanged()), this, SLOT(readDraw()));
    init();
}

MainWindow::~MainWindow()
{
    closesocket(ServerSocket);
    closesocket(CientSocket);
    WSACleanup();
    delete ui;
}

void MainWindow::init()
{
    qDebug("init----");
    haveStart = false;
    choose = 0;
    dir = true;
    int i, j;
    for(i = 0; i < 5; i++)
        for(j = 0; j < 6; j++)
        {
            your[i][j].x = i;
            your[i][j].y = j;
            your[i][j].alive = true;
            your[i][j].rank = orig[i][j];
            my[i][j].x = i;
            my[i][j].y = j + 6;
            my[i][j].alive = true;
            my[i][j].rank = orig[i][5-j];
        }
    your[1][2].alive = false; your[3][2].alive = false;  your[2][3].alive = false;   your[1][4].alive = false;    your[3][4].alive = false;
    my[1][1].alive = false;   my[3][1].alive = false;  my[2][2].alive = false; my[1][3].alive = false;    my[3][3].alive = false;

    for(int i = 0; i < 5; i++)
        for (int j = 0; j < 6; j++)
        {
            yourRect[i][j].setRect(a[your[i][j].x], b[your[i][j].y], w, h);
            myRect[i][j].setRect(a[my[i][j].x], b[my[i][j].y], w, h);
        }
}

void MainWindow::readDraw()
{
    qDebug("temp~~");
    dir = 1 - thread->temp->dir;

    Pieces tem[5][6];
    QRectF tp[5][6];
//    memset(tem, NULL, sizeof(tem));
//    memset(tp, NULL, sizeof(tp));

//------------------------------------------------------------------------------
//    qDebug("print1------------------------------");
//    printData();
//    qDebug("print1End------------------------------");

    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 6; j++)
        {
//            tp[i][j] = thread->temp->myRect[i][j];
//            myRect[i][j] = thread->temp->yourRect[i][j];
//            yourRect[i][j] = tp[i][j];
            tem[i][j] = thread->temp->my[i][j];
            my[i][j] = thread->temp->your[i][j];
            my[i][j].x = 4 - my[i][j].x;
            my[i][j].y = 11 - my[i][j].y;
            your[i][j] = tem[i][j];
            your[i][j].x = 4 - your[i][j].x;
            your[i][j].y = 11 - your[i][j].y;

            /*
            tem[i][j].x = thread->temp->my[i][j].x;
            tem[i][j].y = thread->temp->my[i][j].y;
            qDebug("xy");
//                qDebug()<<thread->temp->my[i][j].rank;
            tem[i][j].rank = thread->temp->my[i][j].rank;
            qDebug("rank");
            tem[i][j].alive = thread->temp->my[i][j].alive;
            qDebug("tem");
            my[i][j].x = 4 - thread->temp->your[i][j].x;
            my[i][j].y = 11 - thread->temp->your[i][j].y;
            my[i][j].rank = thread->temp->your[i][j].rank;
            my[i][j].alive = thread->temp->your[i][j].alive;
            qDebug("your");
            your[i][j].x = 4 - tem[i][j].x;
            your[i][j].y = 11 - tem[i][j].y;
            your[i][j].rank = tem[i][j].rank;
            your[i][j].alive = tem[i][j].alive;
            qDebug("...");
            */
        }
//    delete thread->temp;


    for(int i = 0; i < 5; i++)
        for (int j = 0; j < 6; j++)
        {
            yourRect[i][j].setRect(a[your[i][j].x], b[your[i][j].y], w, h);
            myRect[i][j].setRect(a[my[i][j].x], b[my[i][j].y], w, h);
        }

//------------------------------------------------------------------------------
//    qDebug("print2------------------------------");
//    printData();
//    qDebug("print2End------------------------------");

    update();
}

void MainWindow::printData()
{
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 6; j++)
        {
//            printf("myRect[%d][%d] = %d, yourRect[%d][%d] = %d\n",
//                   i, j, myRect[i][j], i, j, yourRect[i][j]);
            printf("my[%d][%d]:x = %d, y = %d, rank = %d, ",
                   i, j, my[i][j].x, my[i][j].y, my[i][j].rank);
            cout << "alive = " << my[i][j].alive << endl;
            printf("your[%d][%d]:x = %d, y = %d, rank = %d, ",
                   i, j, your[i][j].x, your[i][j].y, your[i][j].rank);
            cout << "alive = " << your[i][j].alive << endl;
        }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
//    qDebug("paintEvent----");//
      QPainter p(this);
    //画背景图片
    p.drawImage(50,50,QImage(":/new/pic/1.jpg"));
//    QFont font("宋体", 20);//no use??
//    p.setFont(font);
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
            have[i][j] = false;
    //画对方棋子
//    if(dir)
//    {
        p.setPen(QPen(Qt::black, 2));
        p.setBrush(Qt::darkBlue);
//    }
//    else
//    {
//        p.setPen(QPen(Qt::darkYellow, 2));
//        p.setBrush(Qt::darkYellow);
//    }//一闪一闪的orz...
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 6; j++)
            if(your[i][j].alive)
            {
                p.drawRect(yourRect[i][j]);
                p.setPen(QPen(Qt::white, 2));
                p.drawText(yourRect[i][j], Qt::AlignCenter, r[your[i][j].rank]);//display your pieces---------
                have[i][j] = true;
            }

    //画我方棋子
//    if(dir)
//    {
        p.setPen(QPen(Qt::darkYellow, 2));
        p.setBrush(Qt::darkYellow);
//    }
//    else
//    {
//        p.setPen(QPen(Qt::black, 2));
//        p.setBrush(Qt::darkBlue);
//    }
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 6; j++)
            if(my[i][j].alive)
            {
                p.drawRect(myRect[i][j]);
                p.setPen(QPen(Qt::white, 2));
                p.drawText(myRect[i][j], Qt::AlignCenter, r[my[i][j].rank]);
                have[i][j+6] = true;
            }
}

void MainWindow::mousePressEvent ( QMouseEvent * e )//鼠标单击事件响应
{
    qDebug("mousePressEvent----");
//    QString str="("+QString::number(e->x())+","+QString::number(e->y())+")";
//    if(e->button()==Qt::LeftButton)
//    {
//         qDebug() << tr("Mouse Left Button Pressed:") << str;
//    }
    tempX = e->x();
    tempY = e->y();
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
        {
            if((tempX >= a[i]) && (tempX <= a[i]+48) && (tempY >= b[j]) && (tempY <= b[j]+20))
            {
                choose++;
                chooseX = i;
                chooseY = j;
//                qDebug("chooseX:%d", chooseX);
//                qDebug("chooseY:%d", chooseY);
                //...
                if(haveStart)
                {
                    qDebug("haveStart--");
                    getXY();
                    rank_();
                    qDebug("choose:%d",choose);
                    if(choose == 2)
                    {
                        if(have[real0_x][real0_y])
                        {
                            qDebug("HAVE");
                            result(result_0());
//                            qDebug("~~3~~");
                            update();//
//                            qDebug("~~4~~");
                        }
                        else
                        {
                            qDebug("NOT HAVE");
                            my[real1_x][real1_y].x = chooseX;
                            my[real1_x][real1_y].y = chooseY;
                            qDebug("my[real1_x][real1_y].x:%d",my[real1_x][real1_y].x);
                            qDebug("my[real1_x][real1_y].y:%d",my[real1_x][real1_y].y);
                            for(int i = 0; i < 5; i++)
                                for (int j = 0; j < 6; j++)
                                {
                                    yourRect[i][j].setRect(a[your[i][j].x], b[your[i][j].y], w, h);
                                    myRect[i][j].setRect(a[my[i][j].x], b[my[i][j].y], w, h);
                                }
                            qDebug("my[real1_x][real1_y].x:%d",my[real1_x][real1_y].x);
                            qDebug("my[real1_x][real1_y].y:%d",my[real1_x][real1_y].y);

                            shuJu = new ShuJu;//
                            shuJu->dir = dir;
                            for(int i = 0; i < 5; i++)
                                for(int j = 0; j < 6; j++)
                                {
                                    shuJu->my[i][j] = my[i][j];
                                    shuJu->your[i][j] = your[i][j];
                                }
                            qDebug("send----");
                            Ret = send(CientSocket, (char*)shuJu, (int)sizeof(ShuJu), 0);
                            qDebug() << sizeof(ShuJu) << "~~~";
                            if ( Ret == SOCKET_ERROR )
                            {
                                cout<<"Send Info Error::"<<GetLastError()<<endl;
                            }

                            update();
                        }
                        choose = 0;
                        dir = true;
                    }

                    //...
                    if(choose == 1) dir = false;
                    return;
                }
                else
                {
                    qDebug("haveNotStart--");
                    swap(i,j);
                }
            }
        }
    qDebug("-1");
    chooseX = -1;
    chooseY = -1;
}

void MainWindow::getXY()
{
    qDebug("getXY()----");
    if(chooseX != -1 && chooseY != -1)
    {
        if(!dir)//对方,dir = 0
        {
            for(int i = 0; i < 5; i++)
                for(int j = 0; j < 6; j++)
                    if(your[i][j].x == chooseX && your[i][j].y == chooseY)
                    {
                        qDebug("dir = 0--");
                        real0_x = i;
                        real0_y = j;
                        return;
                    }
        }
        else//我方,dir = 1
        {
            for(int i = 0; i < 5; i++)
                for(int j = 0; j < 6; j++)
                    if(my[i][j].x == chooseX && my[i][j].y == chooseY)
                    {
                        qDebug("dir = 1--");
                        real1_x = i;
                        real1_y = j;
                        return;
                    }
        }
    }
}

void MainWindow::rank_()
{
    qDebug("rank_()----");
    if(!dir)
    {
        for(int i = 0; i < 12; i++)
            if(your[real0_x][real0_y].rank == i)
            {
                yourRank = i;
            }
    }
    else
    {
        for(int i = 0; i < 12; i++)
            if(my[real1_x][real1_y].rank == i)
            {
                myRank = i;
            }
    }
}

int MainWindow::result_0()
{
    qDebug("result_0()----");
    if(yourRank == 10)
    {
        win();//
    }
    if(myRank == 10)
    {
        lose();//
    }
    if(yourRank == 11 || myRank == 11)  return 1;
    if(yourRank == 0 && myRank == 9)    return 0;
    if(yourRank == 9 && myRank == 0)    return 2;
    if(yourRank == 9)   return 0;
    if(myRank == 9)     return 2;
    if(yourRank == myRank)  return 1;
    if(yourRank > myRank)   return 0;
    if(yourRank < myRank)   return 2;
}

void MainWindow::result(int res)
{
    qDebug("result()----");
    //我方撞了对方，我输了
    if(res == 0){my[real1_x][real1_y].alive = false;}
    //我方撞了对方，平局
    if(res == 1){your[real0_x][real0_y].alive = false;
                 my[real1_x][real1_y].alive = false;}
    //我方撞了对方，我赢了
    if(res == 2){your[real0_x][real0_y].alive = false;
                 my[real1_x][real1_y].x = your[real0_x][real0_y].x;
                 my[real1_x][real1_y].y = your[real0_x][real0_y].y;
                 qDebug("chooseX: %d  chooseY: %d", chooseX, chooseY);
                 qDebug("your[real0_x][real0_y].x: %d", your[real0_x][real0_y].x);
                 qDebug("your[real0_x][real0_y].y: %d", your[real0_x][real0_y].y);
                 for(int i = 0; i < 5; i++)
                     for (int j = 0; j < 6; j++)
                     {
                         yourRect[i][j].setRect(a[your[i][j].x], b[your[i][j].y], w, h);
                         myRect[i][j].setRect(a[my[i][j].x], b[my[i][j].y], w, h);
                     }
                 }

    update();
    //-------------------------------
    //SendBuffer = (char*)shuJu;
    //qDebug()<<SendBuffer << strlen(SendBuffer);----------
    shuJu = new ShuJu;//
    shuJu->dir = dir;
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 6; j++)
        {
            shuJu->my[i][j] = my[i][j];
            shuJu->your[i][j] = your[i][j];
        }
    qDebug("send----");
    Ret = send(CientSocket, (char*)shuJu, (int)sizeof(ShuJu), 0);
    qDebug() << sizeof(ShuJu) << "~~~";
    if ( Ret == SOCKET_ERROR )
    {
        cout<<"Send Info Error::"<<GetLastError()<<endl;
    }
}

void MainWindow::on_startButton_clicked()
{
    qDebug("startButtonClicked----");
    ui->startButton->setDisabled(true);
    haveStart = true;
}

void MainWindow::win()
{
    qDebug("win----");
    QMessageBox *m = new QMessageBox;
    m->setText("Win~~~");
    m->exec();
}

void MainWindow::lose()
{
    qDebug("lose----");
    QMessageBox *m = new QMessageBox;
    m->setText("Lose ::>_<::");
    m->exec();
}

void MainWindow::swap(int chooseX, int chooseY)
{
    qDebug("swap----");
    if(choose == 1)
    {
        for(int i = 0; i < 5; i++)
            for(int j = 0; j < 6; j++)
                if(my[i][j].x == chooseX && my[i][j].y == chooseY)
                {
                    qDebug("dir = 1--");
                    real0_x = i;
                    real0_y = j;
                    break;
                }
        for(int i = 0; i < 12; i++)
            if(my[real0_x][real0_y].rank == i)
            {
                myRank = i;
            }
    }
    if(choose == 2)
    {
        for(int i = 0; i < 5; i++)
            for(int j = 0; j < 6; j++)
                if(my[i][j].x == chooseX && my[i][j].y == chooseY)
                {
                    qDebug("dir = 1--");
                    real1_x = i;
                    real1_y = j;
                    break;
                }
        for(int i = 0; i < 12; i++)
            if(my[real1_x][real1_y].rank == i)
            {
                yourRank = i;
            }
        if(canNotMove())//
        {
            qDebug("canNotMove--");
            return;
        }
        else
        {
            qDebug("canMove--");
            my[real0_x][real0_y].rank = yourRank;
            my[real1_x][real1_y].rank = myRank;

            for(int i = 0; i < 5; i++)
                for (int j = 0; j < 6; j++)
                {
                    yourRect[i][j].setRect(a[your[i][j].x], b[your[i][j].y], w, h);
                    myRect[i][j].setRect(a[my[i][j].x], b[my[i][j].y], w, h);
                }
            qDebug("my[real1_x][real1_y].x:%d",my[real1_x][real1_y].x);
            qDebug("my[real1_x][real1_y].y:%d",my[real1_x][real1_y].y);

            shuJu = new ShuJu;//
            shuJu->dir = dir;
            for(int i = 0; i < 5; i++)
                for(int j = 0; j < 6; j++)
                {
                    shuJu->my[i][j] = my[i][j];
                    shuJu->your[i][j] = your[i][j];
                }
            qDebug("send----");
            Ret = send(CientSocket, (char*)shuJu, (int)sizeof(ShuJu), 0);
            qDebug() << sizeof(ShuJu) << "~~~";
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
            }

            update();
        }
        choose = 0;
        dir = true;
    }
}

bool MainWindow::canNotMove()
{
    qDebug("canNotMove----");
    int c = 0;
    if(myRank == 10 && real1_y != 11)  return true;
    if(myRank == 10 && real1_y == 11 && (real1_x == 0 || real1_x == 2 || real1_x == 4)) return true;
    if(myRank == 9 && (real1_y != 10 || real1_y != 11)) return true;
    if(myRank == 11 && real1_y == 1)    return true;

    if(yourRank == 10 && real0_y != 11)  return true;
    if(yourRank == 10 && real0_y == 11 && (real0_x == 0 || real0_x == 2 || real0_x == 4)) return true;
    if(yourRank == 9 && (real0_y != 10 || real0_y != 11)) return true;
    if(yourRank == 11 && real0_y == 1)    return true;
    return false;
}

void MainWindow::on_buildButton_clicked()
{
    Dialog *dialog = new Dialog;
    if(!dialog->exec())
    {
        return;
    }

    qDebug("----build");
    ui->buildButton->setDisabled(true);
    ui->connectButton->setDisabled(true);
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if(err != 0)
    {
        cout << "winsock start up error!";

    }
    gethostname(hostname,strlen(hostname));
    phostinfo = gethostbyname(hostname);

    cout << inet_ntoa(*(in_addr *) phostinfo->h_addr)<<endl;

    //Init Windows Socket
    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
    {
        cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;

    }

    //Create Socket
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( ServerSocket == INVALID_SOCKET )
    {
        cout<<"Create Socket Failed::"<<GetLastError()<<endl;
    }

    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    LocalAddr.sin_port = htons(PORT);
    memset(LocalAddr.sin_zero, 0x00, 8);

    //Bind Socket
    Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
    if ( Ret != 0 )
    {
        cout<<"Bind Socket Failed::"<<GetLastError()<<endl;

    }
    Ret = listen(ServerSocket, 10);
    if ( Ret != 0 )
    {
        cout<<"listen Socket Failed::"<<GetLastError()<<endl;

    }
    cout<<"The server have set"<<endl;

    AddrLen = sizeof(ClientAddr);
    CientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);

    if (CientSocket == INVALID_SOCKET)
    {
        cout<<"Accept Failed::"<<GetLastError()<<endl;
    }
    cout<<"connect:"<<inet_ntoa(ClientAddr.sin_addr)<<":"<<ClientAddr.sin_port<<endl;

    thread->start();
    qDebug("----buildOver");
}

void MainWindow::on_connectButton_clicked()
{
    ClientDialog *dialog = new ClientDialog;
    if (!(dialog->exec() && (dialog->getText() == IP_ADDRESS)))
        return;

    qDebug("----connect");
    ui->buildButton->setDisabled(true);
    ui->connectButton->setDisabled(true);
    //Init Windows Socket
    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
    {
        cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;

    }

    //Create Socket
    CientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( CientSocket == INVALID_SOCKET )
    {
        cout<<"Create Socket Failed::"<<GetLastError()<<endl;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    ServerAddr.sin_port = htons(PORT);
    memset(ServerAddr.sin_zero, 0x00, 8);

    Ret = ::connect(CientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));//=============
    if ( Ret == SOCKET_ERROR )
    {
        cout << "Connect Error::" << GetLastError() << endl;
    }
    else
    {
        cout<<"connect!"<<endl;
    }

    shuJu = new ShuJu;//
    shuJu->dir = dir;
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 6; j++)
        {
            shuJu->my[i][j] = my[i][j];
            shuJu->your[i][j] = your[i][j];
        }
    qDebug("send----");
    Ret = send(CientSocket, (char*)shuJu, (int)sizeof(ShuJu), 0);
    qDebug() << sizeof(ShuJu) << "~~~";
    if ( Ret == SOCKET_ERROR )
    {
        cout<<"Send Info Error::"<<GetLastError()<<endl;
    }

    thread->start();
    qDebug("----connectOver");
}

void MainWindow::on_head1_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Alinesfile(*.*)"));
    QFile f(fileName);

    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qFatal("Could not open file");
    }
    ui->head1->setIcon(QPixmap(fileName));
}

void MainWindow::on_equalButton_clicked()
{
    QMessageBox *m = new QMessageBox;
    m->setText("求和？");
    m->exec();
}

void MainWindow::on_loseButton_clicked()
{
    QMessageBox *m = new QMessageBox;
    m->setText("认输？");
    m->exec();
}
