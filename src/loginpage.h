#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCore>
#include "dbqueries.h"
#include "bankingpage.h"
#include "simplecrypt.h"

namespace Ui {
class LoginPage;
}

/**
 * Handles the elements displayed on the user login window.
 * Communicates with DBQueries to verify login details from the database and grants access for correct credentials.
 */
class LoginPage : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor: displays window and components from the ui.
     * @param parent
     */
    explicit LoginPage(QWidget *parent = nullptr);

    /**
      * @brief Destructor: deletes the ui pointer.
      */
    ~LoginPage();

private slots:
    /**
     * @brief Sends, when triggered, entered login details to DBQueries which in turn checks the details in databse ...
     * and returns feedback. Opens the banking window if user is granted access, otherwise reports incorrect details entered.
     */
    void on_w2_pushLogin_clicked();

private:
    Ui::LoginPage *ui;
    DBQueries *ptrDB;
    BankingPage *ptrBank;
    SimpleCrypt *ptrCrypt;

};

#endif // LOGINPAGE_H
