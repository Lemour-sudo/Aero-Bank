#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowTitle("Welcome to BB");
    ui->setupUi(this);

    ui->w1_pushLogin->setToolTip("<font color='gray'>Logging in</font>");
    ui->w1_pushRegister->setToolTip("<font color='gray'>Sign up</font>");

    ui->w1_labelBankName->setText("<p style='font-family:Brush Script MT'><font color='#fff'>Welcome to AeroBank</p>");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_w1_pushLogin_clicked()
{
    // Login

    ptrLogin = new LoginPage(this);
    ptrLogin->setWindowTitle("Login");
    ptrLogin->show();
}

void MainWindow::on_w1_pushRegister_clicked()
{
    // Register

    ptrRegister = new RegisterPage(this);
    ptrRegister->setWindowTitle("Sign up");
    ptrRegister->show();
}
