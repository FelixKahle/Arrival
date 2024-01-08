// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_SELECTEDHEADERSTEMPLATEMODEL_H
#define ARRIVAL_SELECTEDHEADERSTEMPLATEMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "data/selectedheaderstemplatelist.h"

namespace Arrival::App
{
    class SelectedHeadersTemplateModel : public QAbstractListModel
    {
        Q_OBJECT

        Q_PROPERTY(SelectedHeadersTemplateList* templateList READ templateList WRITE setTemplateList NOTIFY templateListChanged)
        Q_PROPERTY(QString showIdentifier READ showIdentifier WRITE setShowIdentifier NOTIFY showIdentifierChanged)
    public:
        enum RoleNames
        {
            TemplateName = Qt::UserRole + 1,
            Enabled
        };

    public:
        explicit SelectedHeadersTemplateModel(QObject* parent = nullptr);

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;

        SelectedHeadersTemplateList* templateList() const
        {
            return m_templateList;
        }

        const QString& showIdentifier() const
        {
            return m_showIdentifier;
        }

    signals:
        void templateListChanged(SelectedHeadersTemplateList* list);
        void showIdentifierChanged(const QString& identifier);

    public slots:
        void setTemplateList(SelectedHeadersTemplateList* list);
        void setShowIdentifier(const QString& identifier);

    private:
        /*!
         * \brief m_showIdentifier The current identifier.
         */
        QString m_showIdentifier;

        /*!
         * \brief m_templateList The actual data.
         */
        SelectedHeadersTemplateList* m_templateList;
    };
}

#endif // ARRIVAL_SELECTEDHEADERSTEMPLATEMODEL_H
