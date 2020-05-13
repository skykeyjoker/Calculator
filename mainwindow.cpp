#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 将等于号按钮设为默认，捕获数据 */
    ui->Btn_S->setDefault(true);

    /* 绑定菜单事件 */
    connect(ui->ACTION_ABOUT,&QAction::triggered,[=](){
        QMessageBox::about(this,"About","<h1>基于栈结构的复杂计算器演示程序</h1><p>作者:薛廉澄 2019170904023</p><p>特别提醒:</p><p>根号√计算需要<strong>先输入被开方数</strong>，另外，根号√也被<strong>转义为<i>@</i></strong></p>");
    }); //关于按钮
    connect(ui->Action_EXIT,&QAction::triggered,this,&QWidget::close); //退出按钮

    /* 绑定按钮C、CE、ESC键事件 */
    //C键LineEdit全部清空
    connect(ui->Btn_C,&QPushButton::clicked,[=](){
        ui->lineEdit->setText("");
    });
    //ESC键清空掉一个字符
    connect(ui->Btn_ESC,&QPushButton::clicked,[=](){
        QString str=ui->lineEdit->text();

        if(!str.isEmpty()) //如果LineEdit不为空
        {
            str.remove(str.length()-1,1);
            ui->lineEdit->setText(str);
        }
    });
    //CE键清掉一个数字
    connect(ui->Btn_CE, &QPushButton::clicked, [=]() {
        QString str = ui->lineEdit->text();

        if(!str.isEmpty())
        {
            int count = str.length() - 1;

            while(count>=0)
            {
                if ((str[count] >= '0' && str[count] <= '9') || (str[count] == '.'))
                {
                    str.remove(count, 1);
                    count--;
                }
                else
                {
                    break;
                }
            }

            if (count>=0)
            {
                if (str[count] == '-' && (str[count - 1] == '-' || str[count - 1] == '('))
                {
                    str.remove(count, 1);
                }
            }
            ui->lineEdit->setText(str);
        }
    });

    /* 绑定数字按键 */
    connect(ui->Btn_0,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'0');
    });
    connect(ui->Btn_1,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'1');
    });
    connect(ui->Btn_2,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'2');
    });
    connect(ui->Btn_3,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'3');
    });
    connect(ui->Btn_4,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'4');
    });
    connect(ui->Btn_5,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'5');
    });
    connect(ui->Btn_6,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'6');
    });
    connect(ui->Btn_7,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'7');
    });
    connect(ui->Btn_8,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'8');
    });
    connect(ui->Btn_9,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'9');
    });

    /* 绑定运算符按钮 */
    connect(ui->Btn_POW2,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+"^2");
    });
    connect(ui->Btn_powM1,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+"^(-1)");
    });
    connect(ui->Btn_ADDORMINUS,&QPushButton::clicked,[=](){
        QString str=ui->lineEdit->text();

        if(!str.isEmpty()) // +/-符号需要提前判断一下是否为空
        {
            if(str[str.length()-1]=='-')
            {
                str.replace(str.length()-1,1,'+');
            }
            else{
                str.replace(str.length()-1,1,'-');
            }
            ui->lineEdit->setText(str);
        }
    });
    connect(ui->Btn_P,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'.');
    });
    connect(ui->Btn_L,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'(');
    });
    connect(ui->Btn_R,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+')');
    });
    connect(ui->Btn_ADD,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'+');
    });
    connect(ui->Btn_MIN,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'-');
    });
    connect(ui->Btn_MUL,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'*');
    });
    connect(ui->Btn_DIV,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'/');
    });
    connect(ui->Btn_MOD,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'%');
    });
    connect(ui->Btn_POW,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'^');
    });
    connect(ui->Btn_SQRT,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+"@2");
    });
    connect(ui->Btn_SQRTS,&QPushButton::clicked,[=](){
        ui->lineEdit->setText(ui->lineEdit->text()+'@');
    });
    connect(ui->Btn_S,&QPushButton::clicked,[=](){
        if(ui->lineEdit->text()=="")
        {
            QMessageBox::critical(this,"错误","请输入一个算术表达式！");
        }
        else
        {
            char *str;
            QByteArray ba=ui->lineEdit->text().toUtf8();

            str=ba.data();

            double ans=calc(str);

            QString qstr=QString("%1").arg(ans);

            ui->lineEdit->setText(qstr);
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 重载 void KeyPressEvent(QKeyEvent *) 函数，捕获按键事件 */
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key()==Qt::Key_Enter||ev->key()==Qt::Key_Return)
    {
        if(ui->lineEdit->text()=="")
        {
            QMessageBox::critical(this,"错误","请输入一个算术表达式！");
        }
        else
        {
            char *str;
            QByteArray ba=ui->lineEdit->text().toUtf8();

            str=ba.data();

            double ans=calc(str);

            QString qstr=QString("%1").arg(ans);

            ui->lineEdit->setText(qstr);
        }
    }
    else
        if(ev->key()==Qt::Key_Backspace)
        {
            QString str=ui->lineEdit->text();

            if(!str.isEmpty())
            {
                str.remove(str.length()-1,1);
                ui->lineEdit->setText(str);
            }
        }
        else
            ui->lineEdit->setText(ui->lineEdit->text()+ev->text());
}

