#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "loginpage.h"
#include "registerpage.h"
#include "dbqueries.h"
#include "wigglywidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * This is the start window with buttons that allow the user to login or register.
 * Also displays the Bank's name in nice html font and the painted logo.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor: displays login window with all the elements.
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);

    /**
      * @brief Destructor: deletes the ui pointer when instance is destroyed.
      */
    ~MainWindow();

private slots:
    /**
     * @brief Opens the login window when triggered.
     */
    void on_w1_pushLogin_clicked();

    /**
     * @brief Opens new user registration window when triggered.
     */
    void on_w1_pushRegister_clicked();

private:
    Ui::MainWindow *ui;
    LoginPage *ptrLogin;
    RegisterPage *ptrRegister;

};
#endif // MAINWINDOW_H
