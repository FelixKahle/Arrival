// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include "data/jobtable.h"

namespace Arrival::App
{
    JobTable::JobTable(QObject* parent)
        : QObject(parent)
        , m_data(nullptr)
    {
    }

    void JobTable::setTableData(QSharedPointer<CSVCombinedData> data)
    {
        // Guard
        if (data == m_data)
        {
            return;
        }
        if (data.isNull())
        {
            // Clear previous data and return.
            clearTable();
            return;
        }

        emit preTableReset();
        m_data = data;
        emit postTableReset();

        emit rowCountChanged(rowCount());
        emit columnCountChanged(columnCount());
        emit newAddedCountChanged(newAddedCount());
        emit removedCountChanged(removedCount());
        emit hasDataChanged(hasData());
        emit headerNamesChanged(headerNames());
        emit formatIdentifierChanged(formatIdentifier());
    }

    void JobTable::clearTable()
    {
        // Guard.
        if (!hasData())
        {
            return;
        }

        // Clear the actual list
        emit preTableReset();
        m_data->clear();
        m_data.clear();
        emit postTableReset();

        // As the list got cleared, these values have to be updatet as well.
        emit rowCountChanged(rowCount());
        emit columnCountChanged(columnCount());
        emit newAddedCountChanged(newAddedCount());
        emit removedCountChanged(removedCount());
        emit hasDataChanged(hasData());
        emit headerNamesChanged(headerNames());
        emit formatIdentifierChanged(formatIdentifier());
    }
}
