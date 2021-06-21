#include "transactionspage.h"
#include "ui_transactionspage.h"

TransactionsPage::TransactionsPage(QWidget *parent, QString user_id) :
    QDialog(parent),
    ui(new Ui::TransactionsPage)
{
    ui->setupUi(this);

    QSqlTableModel *tableModel = new QSqlTableModel(this);
    tableModel->setTable("dbo.Transactions");
    tableModel->setFilter("user_id = " + user_id);
    tableModel->select();

    ui->w5_tableView->setModel(tableModel);
    ui->w5_tableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->w5_tableView->hideColumn(0);
}

TransactionsPage::~TransactionsPage()
{
    delete ui;
}
