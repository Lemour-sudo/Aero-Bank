#include "loginpage.h"
#include "ui_loginpage.h"

LoginPage::LoginPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);

    ui->w2_lineName->setPlaceholderText("Enter your email");
    ui->w2_linePass->setPlaceholderText("case sensitive");
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::on_w2_pushLogin_clicked()
{
    // Login

    QString email, password, accNumber, clientName;
    QVector<QString> userDetails;

    email = ui->w2_lineName->text();

    password = ui->w2_linePass->text();

    ptrDB = new DBQueries();
    userDetails = ptrDB->verifyClient(email, password);
    accNumber = userDetails[0];
    clientName = userDetails[1];
    if(accNumber != "")
    {
        QMessageBox::information(this, "Login Feed", "Login Success");

        ptrBank = new BankingPage(this, clientName, accNumber);
        ptrBank->setWindowTitle("AeroBank");
        ptrBank->show();

        close();
    }
    else
    {
        QMessageBox::information(this, "Login Feed", "Wrong Details");
        ui->w2_lineName->clear();
        ui->w2_linePass->clear();
    }

}
