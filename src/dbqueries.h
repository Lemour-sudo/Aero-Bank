#ifndef DBQUERIES_H
#define DBQUERIES_H

#include <QObject>
#include <QtSql>
#include <QtWidgets>
#include <QtCore>
#include "simplecrypt.h"

#define SUCCESS 1
#define FAILURE 0

/**
 * DBQueries comprises tools that apply queries directly to the SQL database.
 *
 * It opens the database when instantiated and closes the database when the its instance is destroyed.
 *
 * Includes the functionalities:
 *      a) verifyting a user's login details,
 *      b) adding a new user to database,
 *      c) apply queried transactions to the database to the specified account,
 *      d) export queried use's transactions from database to output text file to specified folder.
 */
class DBQueries : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor: sets up and opens the BankOfSpain database.
     * @param parent
     */
    explicit DBQueries(QObject *parent = nullptr);

    /**
     * @brief Closes database connection.
     * @return 0
     */
    bool closeDB();


    /**
     * @brief Adds new client information to database along with unique user-id and Savings and Cheque accounts.
     * @param name
     * @param surname
     * @param email
     * @param password
     * @return feedback whether operation was successful or unsuccessful.
     */
    QVector<QString> addClient(QString name, QString surname, QString email, QString password);

    /**
     * @brief Check whether queried user exists and whether password is correct at user login.
     * @param username
     * @param password
     * @return user_id: return user_id if credentials are correct otherwise return empty string.
     */
    QString verifyClient(QString username, QString password);

    /**
     * @brief Applies the queried transaction to the database.
     * @param action: transaction type; deposit or withdraw
     * @param accNumber
     * @param accType: account type; Cheque account or Savings account
     * @param amount: assumed to be a positive float
     * @return {chequeBal, savingsBal}: vector of Cheque Balance and Savings Balance
     */
    QVector<float> transact(QString action, QString accNumber, int accType, float amount);

    /**
     * @brief Write queried user's transactions into text file.
     * @param accNumber
     * @param filename: string path directory
     * @return
     */
    bool exportTransac(QString accNumber, QString filename);

    /**
     * @brief Destructor: closes database.
     */
    ~DBQueries();


private:
    QSqlDatabase db;

    /**
     * @brief Applies queried transaction to database.
     * @param balance
     * @param update
     * @param action
     * @return
     */
    float execTrasac(float balance, float update, QString action);

    SimpleCrypt *ptrCrypt;

};

#endif // DBQUERIES_H
