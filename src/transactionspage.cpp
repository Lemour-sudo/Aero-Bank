#include "transactionspage.h"
#include "ui_transactionspage.h"

TransactionsPage::TransactionsPage(QWidget *parent, QString accNumber) :
    QDialog(parent),
    ui(new Ui::TransactionsPage)
{
    ui->setupUi(this);

    QSqlTableModel *tableModel = new QSqlTableModel(this);
    tableModel->setTable("transaction");
    tableModel->setFilter("account_number = " + accNumber);
    tableModel->select();

    ui->w5_tableView->setModel(tableModel);
    ui->w5_tableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->w5_tableView->hideColumn(0);
}

TransactionsPage::~TransactionsPage()
{
    delete ui;
}
