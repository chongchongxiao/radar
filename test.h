#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include "QPainter"
class Test : public QWidget
{
    Q_OBJECT
public:
    explicit Test(QWidget *parent = nullptr);
    void start();
protected:
    void paintEvent(QPaintEvent *e);

signals:

public slots:

private:

};

#endif // TEST_H
