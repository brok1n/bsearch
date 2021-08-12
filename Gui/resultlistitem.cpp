#include "resultlistitem.h"
#include "ui_resultlistitem.h"

#include <QFileIconProvider>
#include <QIcon>
#include <QDebug>

ResultListItem::ResultListItem(Node *node, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ResultListItem)
    , mNode(node)
{
    ui->setupUi(this);


//    QFileIconProvider fileIcon;
//    QIcon icon = fileIcon.icon(mNode->fileInfo());


    ui->itemName->setText(mNode->name);
    ui->itemSize->setText(Common::formatFileSize(mNode->fileSize()));
    ui->itemType->setText(Common::fileTypeToName(mNode->fileType()));
    ui->itemPath->setText(mNode->fileInfo().absolutePath());

}

ResultListItem::~ResultListItem()
{
    mNode = nullptr;
    delete ui;
}
