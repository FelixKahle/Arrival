// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include "ui/selectedheaderstemplatemodel.h"

namespace Arrival::App
{
    SelectedHeadersTemplateModel::SelectedHeadersTemplateModel(QObject* parent)
        : QAbstractListModel(parent)
        , m_templateList(nullptr)
    {}

    int SelectedHeadersTemplateModel::rowCount(const QModelIndex& parent) const
    {
        if (m_templateList)
        {
            return m_templateList->templatesCount();
        }
        return 0;
    }

    QVariant SelectedHeadersTemplateModel::data(const QModelIndex& index, int role) const
    {
        // Guard.
        if (!m_templateList || !index.isValid() || index.row() < 0 || index.row() >= rowCount())
        {
            return QVariant();
        }

        const SelectedHeadersTemplate& selectedTemplate = m_templateList->templates().at(index.row());
        const bool enabled = selectedTemplate.headerIdentifier() == m_showIdentifier;

        switch(role)
        {
        case SelectedHeadersTemplateModel::RoleNames::TemplateName: return selectedTemplate.templateName();
        case SelectedHeadersTemplateModel::RoleNames::Enabled: return enabled;
        default: return QVariant();
        }
    }

    QHash<int, QByteArray> SelectedHeadersTemplateModel::roleNames() const
    {
        QHash<int, QByteArray> names;
        names[TemplateName] = "templateName";
        names[Enabled] = "enabled";
        return names;
    }

    void SelectedHeadersTemplateModel::setShowIdentifier(const QString& identifier)
    {
        // Guard.
        if (identifier == m_showIdentifier)
        {
            return;
        }

        m_showIdentifier = identifier;
        emit showIdentifierChanged(identifier);

        // Enabled changed.
        const QModelIndex topLeft = createIndex(0, 0);
        const QModelIndex bottomRight = createIndex(rowCount() - 1, 0);
        emit dataChanged(topLeft, bottomRight, QVector<int>() << SelectedHeadersTemplateModel::Enabled);
    }

    void SelectedHeadersTemplateModel::setTemplateList(SelectedHeadersTemplateList* list)
    {
        // Guard.
        if (!list || list == m_templateList)
        {
            return;
        }

        beginResetModel();
        if (m_templateList)
        {
            m_templateList->disconnect();
        }
        m_templateList = list;

        connect(m_templateList, &SelectedHeadersTemplateList::preItemAppend, this, [=] {
            const int index = rowCount();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_templateList, &SelectedHeadersTemplateList::postItemAppend, this, [=] {
            endInsertRows();
        });

        connect(m_templateList, &SelectedHeadersTemplateList::preItemRemove, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_templateList, &SelectedHeadersTemplateList::postItemRemove, this, [=] {
            endRemoveRows();
        });

        connect(m_templateList, &SelectedHeadersTemplateList::preListReset, this, [=] {
            beginResetModel();
        });

        connect(m_templateList, &SelectedHeadersTemplateList::postListReset, this, [=] {
            endResetModel();
        });

        endResetModel();
    }
}
