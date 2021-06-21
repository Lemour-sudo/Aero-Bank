#include "dbqueries.h"

DBQueries::DBQueries(QObject *parent) : QObject(parent)
{
    QString servername = "";    // Enter your severname
    QString dbname = "BankOfSpain";

    db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BankDSN");

    if(db.open())
    {
        qDebug() << "\nBankOfSpain DB Opened!\n";
    }
    else
    {
        qDebug() << "Error = " << db.lastError().text();
    }
}

DBQueries::~DBQueries()
{
    db.close();
    qDebug() << "\nBankOfSpain DB Closed.\n";
}

bool DBQueries::addClient(QString name, QString surname, int age, QString email, QString password)
{
    qint64 user_id;
    QDateTime now = QDateTime::currentDateTime();
    user_id = now.toMSecsSinceEpoch();

    QString clientLine = "INSERT INTO [BankOfSpain].[dbo].[Clients] "
                         "([Name], [Surname], [Age], [Email], [Password], [user_id]) "
                         "VALUES (:name, :surname, :age,:email, :password, :user_id)";
    QSqlQuery clientQuery;
    clientQuery.prepare(clientLine);
    clientQuery.bindValue(":name", name);
    clientQuery.bindValue(":surname", surname);
    clientQuery.bindValue(":age", age);
    clientQuery.bindValue(":email", email);
    clientQuery.bindValue(":password", password);
    clientQuery.bindValue(":user_id", user_id);

    QString accLine = "INSERT INTO [BankOfSpain].[dbo].[Accounts] "
                      "([user_id], [AccountNumber], [Cheque], [Savings]) "
                      "VALUES (:user_id, :accNumber, :cheque, :savings)";
    QSqlQuery accQuery;
    accQuery.prepare(accLine);
    accQuery.bindValue(":user_id", user_id);
    accQuery.bindValue(":accNumber", user_id);
    accQuery.bindValue(":cheque", 1.3);
    accQuery.bindValue(":savings", 0.0);

    if(clientQuery.exec() && accQuery.exec())
    {
        qDebug() << "\nClient added to the Bank of Spain DB.\n";
    }
    else
    {
        qDebug() << "Error = " << db.lastError().text();
        return FAILURE;
    }

    return SUCCESS;
}

QString DBQueries::verifyClient(QString username, QString password)
{
    QString user_id;

    QSqlQueryModel *queryModel = new QSqlQueryModel;

    QString queryLine = QString("SELECT * FROM [BankOfSpain].[dbo].[Clients] "
                                "WHERE Name = '%1'").arg(username);

    queryModel->setQuery(queryLine); //select the row of where the Name == username

    ptrCrypt = new SimpleCrypt();
    ptrCrypt->setKey(0x0c2ad4a4acb9f023);
    if(ptrCrypt->decryptToString(queryModel->record(0).value(4).toString()) == password)
    {
        user_id = queryModel->record(0).value(5).toString();
        return user_id;
    }

    return "";
}

QVector<float> DBQueries::transact(QString action, QString user_id, int accType, float amount)
{
    float chequeBal = 0.0;
    float savingsBal = 0.0;
    float minimumBal = 0.0;

    float chequeDeposit = 0.0, chequeWithdrawal = 0.0;
    float savingsDeposit = 0.0, savingsWithdrawal = 0.0;

    QDateTime timeNow = QDateTime::currentDateTime();
    QString strTimeNow = timeNow.toString("dd.MM.yyyy hh:mm:ss");

    QSqlQueryModel *queryModel = new QSqlQueryModel;
    QString balQuery = QString("SELECT * FROM [BankOfSpain].[dbo].Accounts "
                               "WHERE user_id = '%1'").arg(user_id);
    queryModel->setQuery(balQuery);

    chequeBal = queryModel->record(0).value(2).toFloat();
    savingsBal = queryModel->record(0).value(3).toFloat();


    QString updateQuery;
    QString saveQuery;


    if(accType == 0)
    {
        /* Update Cheque Account */

        // Calculate and check balance
        chequeBal = execTrasac(chequeBal, amount, action);
        if(chequeBal < minimumBal)
        {
            return {chequeBal, savingsBal};
        }

        // Set Transaction data
        if(action == "deposit")
        {
            chequeDeposit = amount;
        }
        if(action == "withdraw")
        {
            chequeWithdrawal = amount;
        }

        // Set query to Accounts table
        updateQuery = QString("UPDATE [BankOfSpain].[dbo].[Accounts] "
                              "SET [Cheque] = %1 WHERE user_id = %2").arg(
                    QString::number(chequeBal), user_id);
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



        // Set query to Accounts table
        updateQuery = QString("UPDATE [BankOfSpain].[dbo].[Accounts] "
                              "SET [Savings] = %1 WHERE user_id = %2").arg(
                    QString::number(savingsBal), user_id);
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

        // Set Transaction data
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

        // Set query to Accounts table
        updateQuery = QString("UPDATE [BankOfSpain].[dbo].[Accounts] "
                              "SET [Cheque] = %1 , [Savings] = %2 WHERE user_id = %3").arg(
                    QString::number(chequeBal), QString::number(savingsBal), user_id);
    }

    // Set query to Transactions table
    saveQuery = QString("INSERT INTO [BankOfSpain].[dbo].[Transactions] "
                "([user_id], [ChequeDeposit], [ChequeWithdrawal], [ChequeBalance], "
                "[SavingsDeposit], [SavingsWithdrawal], [SavingsBalance], [DateTime]) "
                "VALUES ( %1, %2, %3, %4, %5, %6, %7, '%8' )").arg(
                user_id, QString::number(chequeDeposit),
                QString::number(chequeWithdrawal), QString::number(chequeBal),
                QString::number(savingsDeposit), QString::number(savingsWithdrawal),
                QString::number(savingsBal), strTimeNow);

    queryModel->setQuery(updateQuery); // update Accounts table
    queryModel->setQuery(saveQuery); // update Transactions table

    return {chequeBal, savingsBal};
}

bool DBQueries::exportTransac(QString user_id, QString filename)
{
    QSqlQueryModel *model = new QSqlQueryModel;

    QString myQuery = "SELECT * FROM [BankOfSpain].[dbo].[Transactions] WHERE user_id = " + user_id;

    model->setQuery(myQuery);

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


