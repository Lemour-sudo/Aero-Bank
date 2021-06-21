#ifndef WIGGLYWIDGET_H
#define WIGGLYWIDGET_H

#include <QBasicTimer>
#include <QWidget>

/**
 * Displays animated text in leave window after the use logs out.
 */
class WigglyWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor: displays window and components from the ui.
     * @param parent
     */
    WigglyWidget(QWidget *parent = 0);

protected:
    /**
     * @brief Handles the painted displayed animated text.
     * @param event
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief Controls the timing for the animations.
     * @param event
     */
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    QBasicTimer timer;

    QString text = "Good Bye";

    int step;
};

#endif
