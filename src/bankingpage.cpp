#include "bankingpage.h"
#include "ui_bankingpage.h"


BankingPage::BankingPage(QWidget *parent, QString name, QString iden) :
    QMainWindow(parent),
    ui(new Ui::BankingPage)
{
    ui->setupUi(this);

    this->username = name;
    this->accNumber = iden;

    ui->w4_labelTitle_1->setText(QString("Hello <i><b>%1</b></i>").arg(username));
    ui->w4_labelTitle_2->setText(QString("ID: %1").arg(accNumber));

    ui->w4_lineAmount->setValidator(new QDoubleValidator(0, 100000000, 2, this));

    statLabel = new QLabel(this);
    statProgress = new QProgressBar(this);

    ui->statusbar->addPermanentWidget(statLabel);
    ui->statusbar->addPermanentWidget(statProgress);
    statProgress->setTextVisible(false);
    statProgress->hide();


}

BankingPage::~BankingPage()
{
    delete ui;
}

bool BankingPage::on_w4_pushTransact_clicked()
{
    QString action;
    int accType;
    float amount;
    QVector<float> balances;
    bool feedback = false;

    // Check transaction type
    if(ui->w4_radioDeposit->isChecked())
    {
        action = "deposit";
    }
    if(ui->w4_radioWithdraw->isChecked())
    {
        action = "withdraw";
    }

    // Check account type
    if(ui->w4_checkCheque->isChecked() && !ui->w4_checkSavings->isChecked())
    {
        accType = 0;
    }
    else if(!ui->w4_checkCheque->isChecked() && ui->w4_checkSavings->isChecked())
    {
        accType = 1;
    }
    else if(ui->w4_checkCheque->isChecked() && ui->w4_checkSavings->isChecked())
    {
        accType = 2;
    }
    else
    {
        accType = -1;
        return feedback;
    }

    amount = ui->w4_lineAmount->text().toFloat();

    balances = ptrDB->transact(action, accNumber, accType, amount);

    feedbackTransact(balances);

    feedback = true;

    return feedback;
}

void BankingPage::on_w4_pushCheck_clicked()
{
    ptrTransac = new TransactionsPage(this, accNumber);
    ptrTransac->setWindowTitle("Transactions");
    ptrTransac->show();
}

void BankingPage::feedbackTransact(QVector<float> balances)
{
    QString message, status, msgBox;

    statProgress->show();

    if((balances[0] < 0.0) || (balances[1] < 0.0))
    {
        message = "\nTransaction failed; Insufficient Funds.";
        msgBox = "Transaction failed.\nInsufficient Funds.";
        status = "Transaction failed.";

    }
    else
    {
        message = "\nNew Cheque Account Balance: " + QString::number(balances[0]) +
                " ; New Savings Account Balance: " + QString::number(balances[1]);
        status = "Transaction complete.";
        msgBox = status;
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
    timer->start(100);

    ui->w4_listWidget->addItem(message);
    ui->statusbar->showMessage(status);

    statProgress->setValue(0);
    QMessageBox::information(this, "Feedback", msgBox);
    statProgress->hide();

}

void BankingPage::on_actionPrint_Statement_triggered()
{
    QString filepath=QFileDialog::getExistingDirectory(this, "Get Any File");

    QDateTime now = QDateTime::currentDateTime();
    QString code = QString::number(now.toMSecsSinceEpoch() % 10000);
    QString filename = filepath + "/statement_" + code + ".csv";

    if(ptrDB->exportTransac(accNumber, filename))
    {
        QMessageBox::information(this, "Print Report", "Report successfully printed");
    }
    else
    {
        QMessageBox::information(this, "Print Report", "Print unsuccessful");
    }
}

void BankingPage::updateProgressBar()
{
    statProgress->setValue(statProgress->value() + 20);
}


void BankingPage::on_w4_pushBye_clicked()
{
    ptrByePage = new ByePage(this);
    ptrByePage->setWindowTitle("Until next time ;)");
    ptrByePage->show();
    close();
}

