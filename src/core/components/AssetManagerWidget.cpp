#include "core/components/AssetManagerWidget.h"

AssetManagerWidget::AssetManagerWidget(QWidget *parent) 
    : QWidget(parent) 
{
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setObjectName("asset_manager_widget");
}