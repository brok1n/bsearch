#ifndef RESULTLISTITEM_H
#define RESULTLISTITEM_H

#include <QWidget>

namespace Ui {
class ResultListItem;
}

class ResultListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ResultListItem(QWidget *parent = nullptr);
    ~ResultListItem();

private:
    Ui::ResultListItem *ui;
};

#endif // RESULTLISTITEM_H
