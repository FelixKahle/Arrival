// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include <QElapsedTimer>

#include <algorithm>

#include "ui/jobtablemodel.h"

namespace Arrival::App
{
    JobTableModel::JobTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_table(nullptr)
    , m_columnsToShow()
    {}

    int JobTableModel::rowCount(const QModelIndex& parent) const
    {
        if (!m_table)
        {
            return 0;
        }
        return m_table->rowCount();
    }

    int JobTableModel::columnCount(const QModelIndex& parent) const
    {
        return columns();
    }

    QHash<int, QByteArray> JobTableModel::roleNames() const
    {
        return { {Qt::DisplayRole, "display"} };
    }

    int JobTableModel::mapColumnIndex(int index) const
    {
        return m_columnsToShow.at(index);
    }

    int JobTableModel::columns() const
    {
        return m_columnsToShow.count();
    }

    JobTableRowState::State JobTableModel::rowState(int index) const
    {
        // Guard.
        if (!m_table || !m_table->hasData() || index < 0 || index >= m_table->rowCount())
        {
            return JobTableRowState::Invalid;
        }
        return m_table->data()->rows().at(index).state;
    }

    QVariant JobTableModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        // Guard.
        if (!m_table || !m_table->hasData() || section < 0 || section >= m_table->columnCount() || m_columnsToShow.isEmpty())
        {
            return QVariant("");
        }

        if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            return m_table->data()->headerNames().at(mapColumnIndex(section));
        }
        return QVariant("");
    }

    QVariant JobTableModel::data(const QModelIndex& index, int role) const
    {
        // Guard.
        if(!m_table || !m_table->hasData() || !index.isValid() || index.row() < 0 || index.row() >= m_table->rowCount() ||
            index.column() < 0 || index.column() >= m_table->columnCount() || m_columnsToShow.empty())
        {
            return QVariant("");
        }

        if (role == Qt::DisplayRole)
        {
            return m_table->data()->rows().at(index.row()).columns.at(mapColumnIndex(index.column()));
        }
        return QVariant("");
    }

    JobTable* JobTableModel::jobTable() const
    {
        return m_table;
    }

    const QList<int>& JobTableModel::columnsToShow() const
    {
        return m_columnsToShow;
    }

    void JobTableModel::setJobTable(JobTable* jobTable)
    {
        Q_CHECK_PTR(jobTable);

        beginResetModel();

        if (m_table)
        {
            // Disconnet from the old list when assigning to a new list
            m_table->disconnect(this);
        }

        // Set the pointer.
        m_table = jobTable;

        // TODO: Test if this is good.
        m_columnsToShow.clear();

        // Setup signals and slots.

        connect(m_table, &JobTable::preTableReset, this, [=]() {
            beginResetModel();

            // Clear the columns.
            m_columnsToShow.clear();
            emit columnsToShowChanged(m_columnsToShow);
        });

        connect(m_table, &JobTable::postTableReset, this, [=]() {
            endResetModel();
        });

        endResetModel();

        emit jobTableChanged(jobTable);
    }

    void JobTableModel::setColumnsToShow(const QList<int>& columnsToShow)
    {
        // Guard.
        if (m_columnsToShow == columnsToShow)
        {
            return;
        }

        // Guard.
        if (m_table && m_table->hasData())
        {
            const int columnCount = m_table->columnCount();
            for (const int index : columnsToShow)
            {
                if (index >= columnCount || index < 0)
                {
                    return;
                }
            }
        }

        beginResetModel();

        m_columnsToShow = columnsToShow;
        // Keep the list sorted.
        //std::sort(m_columnsToShow.begin(), m_columnsToShow.end(), [](int a, int b) -> bool {
        //    return a < b;
        //});

        endResetModel();

        emit columnsToShowChanged(m_columnsToShow);
    }
}
