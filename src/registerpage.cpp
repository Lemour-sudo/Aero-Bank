#include "registerpage.h"
#include "ui_registerpage.h"

RegisterPage::RegisterPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterPage)
{
    ui->setupUi(this);

    ui->w3_lineName->setPlaceholderText("Your first name");
    ui->w3_lineSurname->setPlaceholderText("Your family name");
    ui->w3_lineEmail->setPlaceholderText("Personal email address");
    ui->w3_linePass1->setPlaceholderText("case sensitive");
    ui->w3_linePass2->setPlaceholderText("case sensitive");
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

bool RegisterPage::on_w3_pushRegister_clicked()
{
    QString name, surname, email, password1, password2, password;
    QVector<QString> dbOut;

    name = ui->w3_lineName->text();
    surname = ui->w3_lineSurname->text();
    email = ui->w3_lineEmail->text();
    password1 = ui->w3_linePass1->text();
    password2 = ui->w3_linePass2->text();

    QVector<QString> strFields = {name, surname, email, password1, password2};

    QVector<QString>::const_iterator Iter =
            std::find(strFields.begin(), strFields.end(), "");

    if(Iter != strFields.end())
    {
        QMessageBox::warning(this, "Empty Field", "No empty fields allowed.");
        return FAILURE;
    }

    if(password1 != password2)
    {
        QMessageBox::warning(this, "Invalid Passwords", "Passwords do not match.");
        ui->w3_linePass1->clear();
        ui->w3_linePass2->clear();

        return FAILURE;
    }

    ptrCrypt = new SimpleCrypt();
    ptrCrypt->setKey(0x0c2ad4a4acb9f023);
    password = ptrCrypt->encryptToString(password1);

    qDebug() << "\nname:\t\t" << name <<
                "\nsurname:\t" << surname <<
                "\nemail:\t\t" << email <<
                "\npassword:\t" << password <<
                "\n";

    ptrDB = new DBQueries();
    dbOut = ptrDB->addClient(name, surname, email, password);
    if(dbOut[0] == "success")
    {
        QMessageBox::information(this, "Success", "Successfully registered :)");
    }
    else
    {
        QMessageBox::warning(this, "Failure", dbOut[1]);
        return FAILURE;
    }

    close();

    return SUCCESS;

}

void RegisterPage::on_w3_pushBack_clicked()
{
    close();
}
