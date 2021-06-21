#include "loginpage.h"
#include "ui_loginpage.h"

LoginPage::LoginPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);

    ui->w2_lineName->setPlaceholderText("Enter your first name");
    ui->w2_linePass->setPlaceholderText("case sensitive");
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::on_w2_pushLogin_clicked()
{
    // Login

    QString username, password, user_id;

    username = ui->w2_lineName->text();

    password = ui->w2_linePass->text();

    ptrDB = new DBQueries();
    user_id = ptrDB->verifyClient(username, password);
    if(user_id != "")
    {
        QMessageBox::information(this, "Login Feed", "Login Success");

        ptrBank = new BankingPage(this, username, user_id);
        ptrBank->setWindowTitle("Bank of Spain");
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
