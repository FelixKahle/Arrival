// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_SELECTEDHEADERSTEMPLATELIST_H
#define ARRIVAL_SELECTEDHEADERSTEMPLATELIST_H

#include <QObject>

#include "data/selectedheaderstemplate.h"

namespace Arrival::App
{
    class SelectedHeadersTemplateList : public QObject
    {
        Q_OBJECT
    public:
        static constexpr const char* templateFileName = "templates.json";

    public:
        explicit SelectedHeadersTemplateList(QObject* parent = nullptr);

        Q_INVOKABLE void saveToJson(const QString& file) const;
        Q_INVOKABLE void loadFromJson(const QString& file);
        Q_INVOKABLE void addNewTemplate(const QString& formatIdentifier, const QString& templateName, const QList<int>& indices);
        Q_INVOKABLE QList<int> getTemplateIndices(int index) const;

        int templatesCount() const
        {
            return m_templates.count();
        }

        const QList<SelectedHeadersTemplate>& templates() const
        {
            return m_templates;
        }

    signals:
        void preItemAppend();
        void postItemAppend();

        void preItemRemove(int index);
        void postItemRemove();

        void preListReset();
        void postListReset();

    public slots:
        void appendItem(const SelectedHeadersTemplate& itemTemplate);
        void removeItem(const SelectedHeadersTemplate& itemTemplate);
        void removeItemAt(int index);
        void clearList();

    private:

        /*!
         * \brief m_templates List with all templates.
         */
        QList<SelectedHeadersTemplate> m_templates;
    };
}

#endif // ARRIVAL_SELECTEDHEADERSTEMPLATELIST_H
