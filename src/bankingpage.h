#ifndef BANKINGPAGE_H
#define BANKINGPAGE_H

#include <QMainWindow>
#include <QtWidgets>
#include "dbqueries.h"
#include <QtSql>
#include <QtCore>
#include "transactionspage.h"
#include "byepage.h"

#include <QCoreApplication>

namespace Ui {
class BankingPage;
}

/**
 * Displays the banking window after user successfully logs in.
 *
 * Provides the functionalities: \n
 *     a) Transact according the transaction type selected (deposit or withdraw) to the selected account type (Cheque or Savings account). \n
 *     b) Display user's transactions. \n
 *     c) Print out user's bank statement to a text file. \n
 *     d) Connect to server.
 */
class BankingPage : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor: stores user name and id; displays labels, status bar, and progress bar
     * @param parent
     * @param name: username
     * @param iden: user id
     */
    explicit BankingPage(QWidget *parent = nullptr, QString name="", QString iden="");

    /**
     * @brief Destructor: deletes the ui pointer.
     */
    ~BankingPage();

private slots:
    /**
     * @brief Determines the transaction type and account type; then retrieves balances after applying the queried the transaction to the database.
     * @return feedback: true for successful transaction and false for failed transaction.
     */
    bool on_w4_pushTransact_clicked();

    /**
     * @brief Opens up the Transactions window when triggered.
     * @return: None
     */
    void on_w4_pushCheck_clicked();

    /**
     * @brief Fetches user's transactions and writes them to output text file and provides feedback.
     */
    void on_actionPrint_Statement_triggered();

    /**
     * @brief Opens the leave page when pressed.
     */
    void on_w4_pushBye_clicked();

public slots:
    /**
     * @brief Increments progressbar everytime when called.
     */
    void updateProgressBar();

private:
    Ui::BankingPage *ui;

    QString username;

    QString accNumber;

    DBQueries *ptrDB;

    TransactionsPage *ptrTransac;

    QLabel *statLabel;

    QProgressBar *statProgress;

    ByePage *ptrByePage;

    /**
     * @brief Determines whether feedback is valid or not and displays feedback messages in messagebox and status bar.
     * @param balances: stores Cheque account balance at index=0, and Savings account balance at index=1.
     * @return None
     */
    void feedbackTransact(QVector<float> balances);


public:

};

#endif // BANKINGPAGE_H
