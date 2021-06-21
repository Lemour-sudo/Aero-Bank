#ifndef TRANSACTIONSPAGE_H
#define TRANSACTIONSPAGE_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class TransactionsPage;
}

/**
 * Displays the transactions window where the logged-in user can see all their transactions in a table.
 */
class TransactionsPage : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor: displays window and components from the ui.
     * @param parent
     * @param user_id
     */
    explicit TransactionsPage(QWidget *parent = nullptr, QString user_id = "");

    /**
      * @brief Destructor: deletes the ui pointer.
      */
    ~TransactionsPage();

private:
    Ui::TransactionsPage *ui;

};

#endif // TRANSACTIONSPAGE_H
