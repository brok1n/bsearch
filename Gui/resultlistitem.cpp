#include "resultlistitem.h"
#include "ui_resultlistitem.h"

ResultListItem::ResultListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultListItem)
{
    ui->setupUi(this);
}

ResultListItem::~ResultListItem()
{
    delete ui;
}
