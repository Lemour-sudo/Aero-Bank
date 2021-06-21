#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCore>
#include "dbqueries.h"
#include "simplecrypt.h"

namespace Ui {
class RegisterPage;
}

/**
 * @brief Displays new user register window with the forms to fill in required information, and provides buttons to submit information to DBQueries to verify and add new user.
 */
class RegisterPage : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor: displays window and components from the ui.
     * @param parent
     */
    explicit RegisterPage(QWidget *parent = nullptr);

    /**
      * @brief Destructor: deletes the ui pointer.
      */
    ~RegisterPage();

private slots:
    /**
     * @brief Submits entered user register information to DBQueries which in turn returns report whether was the new user successfully added to the database or not.
     * @return feedback whether user register information was accepted or not.
     */
    bool on_w3_pushRegister_clicked();

    /**
     * @brief Closes the register window when triggered.
     */
    void on_w3_pushBack_clicked();

private:
    Ui::RegisterPage *ui;
    DBQueries *ptrDB;
    SimpleCrypt *ptrCrypt;

};

#endif // REGISTERPAGE_H
