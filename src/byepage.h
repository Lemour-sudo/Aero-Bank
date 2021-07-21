#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "dbqueries.h"

/**
 * This is the leave window displayed after the user logs out.
 */
class ByePage : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Display animated paint text on the leave window after logging out.
     * @param parent
     */
    explicit ByePage(QWidget *parent = 0);

private:
    DBQueries *ptrDB;
};

#endif
