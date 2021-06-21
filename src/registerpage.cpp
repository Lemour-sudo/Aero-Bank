#include "registerpage.h"
#include "ui_registerpage.h"

RegisterPage::RegisterPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterPage)
{
    ui->setupUi(this);

    ui->w3_lineName->setPlaceholderText("Your first name");
    ui->w3_lineSurname->setPlaceholderText("Your family name");
    ui->w3_lineAge->setPlaceholderText("Enter your age");
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
    QString name, surname, age, email, password1, password2, password;
    bool status;

    name = ui->w3_lineName->text();
    surname = ui->w3_lineSurname->text();
    age = ui->w3_lineAge->text();
    email = ui->w3_lineEmail->text();
    password1 = ui->w3_linePass1->text();
    password2 = ui->w3_linePass2->text();

    QVector<QString> strFields = {name, surname, age, email, password1, password2};

    QVector<QString>::const_iterator Iter =
            std::find(strFields.begin(), strFields.end(), "");

    if(age.toInt() < 1)
    {
        QMessageBox::warning(this, "Invalid", "Invalid age entered.");
        return FAILURE;
    }

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
                "\nage:\t\t" << age <<
                "\nemail:\t\t" << email <<
                "\npassword:\t" << password <<
                "\n";

    ptrDB = new DBQueries();
    status = ptrDB->addClient(name, surname, age.toInt(), email, password);
    if(status)
    {
        QMessageBox::information(this, "Success", "Successfully registered :)");
    }
    else
    {
        QMessageBox::warning(this, "Failure", "Sorry, registration failed. Please try again later :(");
        return FAILURE;
    }

    close();

    return SUCCESS;

}

void RegisterPage::on_w3_pushBack_clicked()
{
    close();
}
