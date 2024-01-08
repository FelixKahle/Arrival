// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include "ui/headerlistmodel.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace Arrival::App
{
    HeaderListModel::HeaderListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_table(nullptr)
    , m_selectedHeaders()
    {}

    int HeaderListModel::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid() || !m_table || !m_table->hasData())
        {
            return 0;
        }

        return m_table->data()->columnCount();
    }

    QVariant HeaderListModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || !m_table || !m_table->hasData() || m_selectedHeaders.count() <= 0)
        {
            return QVariant();
        }

        const QString& headerName = m_table->data()->headerNames().at(index.row());
        const bool selected = m_selectedHeaders.at(index.row());

        switch (role) {
        case SelectedRole:
            return QVariant(selected);
        case HeaderNameRole:
            return QVariant(headerName);
        }
        return QVariant();
    }

    QHash<int, QByteArray> HeaderListModel::roleNames() const
    {
        QHash<int, QByteArray> names;
        names[SelectedRole] = "selected";
        names[HeaderNameRole] = "headerName";
        return names;
    }

    Qt::ItemFlags HeaderListModel::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            return Qt::NoItemFlags;
        }

        return Qt::ItemIsEditable;
    }

    bool HeaderListModel::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (!index.isValid() || !m_table || !m_table->hasData() || m_selectedHeaders.count() <= 0)
        {
            return false;
        }

        switch (role)
        {
        case SelectedRole:
        {
            bool selected = value.toBool();
            if (selected != m_selectedHeaders.at(index.row()))
            {
                m_selectedHeaders[index.row()] = selected;

                // We made a chnage to the model and the underlying data.
                // Emit the corresponding signals.
                updateSelectedHeaderIndices();
                emit selectedHeadersChanged(m_selectedHeaders);
                emit dataChanged(index, index, QVector<int>() << role);

                // We set some data.
                emit selectedHeader(index.row());

                // We made a change. Return true in that case.
                return true;
            }
            return false;
        }
        case HeaderNameRole:
            return false;
        }
        return false;
    }

    void HeaderListModel::updateSelectedHeaderIndices()
    {
        for (int i = 0; i < m_selectedHeaders.count(); i++)
        {
            if (m_selectedHeaders.at(i) && !m_selectedHeaderIndices.contains(i))
            {
                m_selectedHeaderIndices.append(i);
            }
            else if (!m_selectedHeaders.at(i) && m_selectedHeaderIndices.contains(i))
            {
                m_selectedHeaderIndices.removeOne(i);
            }
        }

        emit selectedHeaderIndicesChanged(selectedHeaderIndices());
    }

    JobTable* HeaderListModel::jobTable() const
    {
        return m_table;
    }

    const QList<bool>& HeaderListModel::selectedHeaders() const
    {
        return m_selectedHeaders;
    }

    const QList<int>& HeaderListModel::selectedHeaderIndices() const
    {
        return m_selectedHeaderIndices;
    }

    void HeaderListModel::setJobTable(JobTable* table)
    {
        if (!table || table == m_table)
        {
            return;
        }

        beginResetModel();
        if (m_table)
        {
            m_table->disconnect();
        }
        m_table = table;

        connect(m_table, &JobTable::preTableReset, this, [=]() {
            beginResetModel();

            // Clear the stored states as these become invalid.
            m_selectedHeaders.clear();
            m_selectedHeaderIndices.clear();
        });

        connect(m_table, &JobTable::postTableReset, this, [=]() {
            // Repopulate the list with the states and a default value.
            const int columnCount = m_table->columnCount();
            m_selectedHeaders.reserve(columnCount);
            for (int i = 0; i < columnCount; i++)
            {
                // Default value. Change in header file.
                m_selectedHeaders.append(ARRIVAL_HEADERLISTMODEL_DEFAULT_SELECT_VALUE);
            }
            emit selectedHeadersChanged(m_selectedHeaders);
            updateSelectedHeaderIndices();

            endResetModel();
        });

        endResetModel();
        emit jobTableChanged(table);
    }

    void HeaderListModel::setSelectedHeaders(const QList<bool>& headers)
    {
        m_selectedHeaders = headers;

        // Update the indicies according to the new headers list.
        updateSelectedHeaderIndices();

        // Tell the model that the data has changed.
        const QModelIndex topLeft = createIndex(0, 0);
        const QModelIndex bottomRight = createIndex(rowCount() - 1, 0);
        emit dataChanged(topLeft, bottomRight, QVector<int>() << HeaderListModel::SelectedRole);
        emit selectedHeadersChanged(selectedHeaders());
    }

    void HeaderListModel::setSelectedHeaderIndices(const QList<int>& indices)
    {
        m_selectedHeaderIndices = indices;

        // First of all reset all fields to false.
        for (int i = 0; i < m_selectedHeaders.count(); i++)
        {
            m_selectedHeaders[i] = false;
        }

        // Now set the correct fields to true.
        for (const int i : indices)
        {
            m_selectedHeaders[i] = true;
        }

        // Tell the model that the data has changed.
        QModelIndex topLeft = createIndex(0, 0);
        QModelIndex bottomRight = createIndex(rowCount() - 1, 0);
        emit selectedHeadersChanged(selectedHeaders());
        emit selectedHeaderIndicesChanged(selectedHeaderIndices());
        emit dataChanged(topLeft, bottomRight, QVector<int>() << HeaderListModel::SelectedRole);
    }
}
