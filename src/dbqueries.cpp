#include "dbqueries.h"

DBQueries::DBQueries(QObject *parent) : QObject(parent)
{
    QSettings settings(QString(":/files/configs/config.ini"), QSettings::IniFormat);
    QString hostName = settings.value("database/host_name").toString();
    QString userName = settings.value("database/user_name").toString();
    QString password = settings.value("database/password").toString();

    QString dbname = "aerobank_db";

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostName);
    db.setDatabaseName(dbname);
    db.setUserName(userName);
    db.setPassword(password);

    if(db.open())
    {
        qDebug() << "\nAeroBank DB Opened!\n";
    }
    else
    {
        qDebug() << "Error = " << db.lastError().text();
    }
}

DBQueries::~DBQueries()
{
    db.close();
    qDebug() << "\nAeroBank DB Closed.\n";
}

bool DBQueries::addClient(QString name, QString surname, QString email, QString password)
{
    qint64 accNumber;
    QDateTime now = QDateTime::currentDateTime();
    accNumber = now.toSecsSinceEpoch();

    QString clientLine = "INSERT INTO client "
                         "(account_number, name, surname, email, password) "
                         "VALUES (:accNumber, :name, :surname, :email, :password)";
    QSqlQuery clientQuery;
    clientQuery.prepare(clientLine);
    clientQuery.bindValue(":accNumber", accNumber);
    clientQuery.bindValue(":name", name);
    clientQuery.bindValue(":surname", surname);
    clientQuery.bindValue(":email", email);
    clientQuery.bindValue(":password", password);

    if(clientQuery.exec())
    {
        qDebug() << "\nClient added to the AeroBank DB.\n";
    }
    else
    {
        qDebug() << clientQuery.executedQuery();

        QSqlError queryError = clientQuery.lastError();
        qDebug() << queryError;
        if(queryError.nativeErrorCode() == "1062")
        {
            return "User email already registered. \nHow about a different email address?";
        }

        return FAILURE;
    }


    QString accLine = "INSERT INTO account "
                      "(account_number, cheque_balance, savings_balance) "
                      "VALUES (:accNumber, :chequeBalance, :savingsBalance)";
    QSqlQuery accQuery;
    accQuery.prepare(accLine);
    accQuery.bindValue(":accNumber", accNumber);
    accQuery.bindValue(":chequeBalance", 1.3);
    accQuery.bindValue(":savingsBalance", 0.0);

    if(accQuery.exec())
    {
        qDebug() << "\nClient account created and added to the AeroBank DB.\n";
    }
    else
    {
        qDebug() << accQuery.executedQuery();
        qDebug() << accQuery.lastError();
        return FAILURE;
    }

    return SUCCESS;
}

QString DBQueries::verifyClient(QString username, QString password)
{
    QString clientName;
    QString accNumber;

    QSqlQueryModel *queryModel = new QSqlQueryModel;

    QString queryLine = QString("SELECT * FROM client "
                                "WHERE Name = '%1' "
                                "LIMIT 1").arg(username);

    queryModel->setQuery(queryLine); //select the row of where the Name == username

    ptrCrypt = new SimpleCrypt();
    ptrCrypt->setKey(0x0c2ad4a4acb9f023);
    if(ptrCrypt->decryptToString(queryModel->record(0).value(4).toString()) == password)
    {
        accNumber = queryModel->record(0).value(0).toString();
        return accNumber;
    }
    return "";
}

QVector<float> DBQueries::transact(QString action, QString accNumber, int accType, float amount)
{
    float chequeBal = 0.0;
    float savingsBal = 0.0;
    float minimumBal = 0.0;

    float chequeDeposit = 0.0, chequeWithdrawal = 0.0;
    float savingsDeposit = 0.0, savingsWithdrawal = 0.0;

    QDateTime timeNow = QDateTime::currentDateTime();

    QSqlQueryModel *queryModel = new QSqlQueryModel;
    QString balQuery = QString("SELECT * FROM account "
                               "WHERE account_number = '%1'").arg(accNumber);
    queryModel->setQuery(balQuery);

    chequeBal = queryModel->record(0).value(1).toFloat();
    savingsBal = queryModel->record(0).value(2).toFloat();


    // Query to account table
    QSqlQuery updateAccountQuery;

    if(accType == 0)
    {
        /* Update Cheque Account */

        // Calculate and check balance
        chequeBal = execTrasac(chequeBal, amount, action);
        if(chequeBal < minimumBal)
        {
            return {chequeBal, savingsBal};
        }

        // Set transaction data
        if(action == "deposit")
        {
            chequeDeposit = amount;
        }
        if(action == "withdraw")
        {
            chequeWithdrawal = amount;
        }

        // Set query to update account table
        updateAccountQuery.prepare(
            "UPDATE account "
            "SET cheque_balance = :chequeBal "
            "WHERE account_number = :accNumber"
        );
        updateAccountQuery.bindValue(":chequeBal", chequeBal);
        updateAccountQuery.bindValue(":accNumber", accNumber);
    }
    else if(accType == 1)
    {
        /* Update Savings Account */

        // Calculate and check balance
        savingsBal = execTrasac(savingsBal, amount, action);
        if(savingsBal < minimumBal)
        {
            return {chequeBal, savingsBal};
        }

        // Set Transaction data
        if(action == "deposit")
        {
            savingsDeposit = amount;
        }
        if(action == "withdraw")
        {
            savingsWithdrawal = amount;
        }

        // Set query to update account table
        updateAccountQuery.prepare(
            "UPDATE account "
            "SET savings_balance = :savingsBal "
            "WHERE account_number = :accNumber"
        );
        updateAccountQuery.bindValue(":savingsBal", savingsBal);
        updateAccountQuery.bindValue(":accNumber", accNumber);
    }
    else if(accType == 2)
    {
        /* Update both Cheque and Savings Accounts */

        // Calculate and check balance
        chequeBal = execTrasac(chequeBal, amount, action);
        savingsBal = execTrasac(savingsBal, amount, action);
        if((chequeBal < minimumBal) || (savingsBal < minimumBal))
        {
            return {chequeBal, savingsBal};
        }

        // Set transaction data
        if(action == "deposit")
        {
            savingsDeposit = amount;
            chequeDeposit = amount;
        }
        if(action == "withdraw")
        {
            savingsWithdrawal = amount;
            chequeWithdrawal = amount;
        }

        // Set query to update account table
        updateAccountQuery.prepare(
            "UPDATE account "
            "SET cheque_balance = :chequeBal , savings_balance = :savingsBal "
            "WHERE account_number = :accNumber"
        );
        updateAccountQuery.bindValue(":chequeBal", chequeBal);
        updateAccountQuery.bindValue(":savingsBal", savingsBal);
        updateAccountQuery.bindValue(":accNumber", accNumber);
    }

    // Update account table
    if(!updateAccountQuery.exec())
    {
        qDebug() << updateAccountQuery.lastError();
    }

    
    // Set query to save into transaction table
    QSqlQuery saveTransacQuery;
    saveTransacQuery.prepare(
        "INSERT INTO transaction "
        "(account_number, cheque_deposit, cheque_withdrawal, cheque_balance, "
        "savings_deposit, savings_withdrawal, savings_balance, date_time) "
        "VALUES ( :accNumber, :chequeDeposit, :chequeWithdrawal, :chequeBal, "
        ":savingsDeposit, :savingsWithdrawal, :savingsBal, :strTimeNow )"
    );
    saveTransacQuery.bindValue(":accNumber", accNumber);
    saveTransacQuery.bindValue(":chequeDeposit", QString::number(chequeDeposit));
    saveTransacQuery.bindValue(":chequeWithdrawal", QString::number(chequeWithdrawal));
    saveTransacQuery.bindValue(":chequeBal", QString::number(chequeBal));
    saveTransacQuery.bindValue(":savingsDeposit", QString::number(savingsDeposit));
    saveTransacQuery.bindValue(":savingsWithdrawal", QString::number(savingsWithdrawal));
    saveTransacQuery.bindValue(":savingsBal", QString::number(savingsBal));
    saveTransacQuery.bindValue(":strTimeNow", timeNow);

    // Update transaction table
    if(!saveTransacQuery.exec())
    {
        qDebug() << saveTransacQuery.lastError();
    }

    return {chequeBal, savingsBal};
}

bool DBQueries::exportTransac(QString accNumber, QString filename)
{
    QSqlQueryModel *model = new QSqlQueryModel;

    QString transacQuery = "SELECT * FROM transaction WHERE account_number = " + accNumber;

    model->setQuery(transacQuery);

    if(!model->lastError().NoError)
    {
        qDebug() << model->lastError();
    }

    // Collect model data to QString
    QString textData;
    int rows = model->rowCount();
    int columns = model->columnCount();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

                textData += model->data(model->index(i,j)).toString();
                textData += ", ";      // for .csv file format
        }
        textData += "\n";
    }

    // Save to file
    QFile csvFile(filename);
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&csvFile);
        out << textData;

        csvFile.close();
    }
    else
    {
        return FAILURE;
    }

    return SUCCESS;
}


float DBQueries::execTrasac(float balance, float update, QString action)
{
    if(action == "deposit")
    {
        balance += update;
    }
    else if(action == "withdraw")
    {
        balance -= update;
    }

    return balance;
}


