#ifndef RESULTLISTITEM_H
#define RESULTLISTITEM_H

#include <QWidget>
#include "common.h"

namespace Ui {
class ResultListItem;
}

class ResultListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ResultListItem(Node*, QWidget *parent = nullptr);
    ~ResultListItem();

private:
    Ui::ResultListItem *ui;
    Node *mNode;
};

#endif // RESULTLISTITEM_H
